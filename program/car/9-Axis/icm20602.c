/**
  *****************************************************************************
  * @file				icm20602.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/




#include "icm20602.h"
#include "systick.h"
#include "spi.h"
#include "stmflash.h"



void Icm20602_CSPin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOD, GPIO_Pin_8);
}


static void icm20602_enable(u8 Enable)
{
	if(Enable)	
		GPIO_ResetBits(GPIOD, GPIO_Pin_8);
	else		
		GPIO_SetBits(GPIOD, GPIO_Pin_8);
}


static void icm20602_readbuf(u8 reg, u8 length, u8 *data)
{
	icm20602_enable(1);
	
	SPI2_RW(reg | 0x80);
	SPI2_Receive(data, length);
	
	icm20602_enable(0);
}


static u8 icm20602_writebyte(u8 reg, u8 data)
{
	u8 status;
	
	icm20602_enable(1);
	
	status = SPI2_RW(reg);
	SPI2_RW(data);
	
	icm20602_enable(0);
	
	return status;
}





u8 Icm20602_Reg_Init(void)
{
	u8 temp;
	
	icm20602_writebyte(MPU_RA_PWR_MGMT_1,0x80); /* 重置内部寄存器并恢复默认设置,重置完成后该位自动清零 */
	delay_ms(10);
	icm20602_writebyte(MPU_RA_PWR_MGMT_1,0x01); /* 自动选择最佳可用时钟源-TTL(如果准备好),否则使用内部时钟源震荡器 */
	delay_ms(10);
	
	icm20602_readbuf(MPUREG_WHOAMI, 1, &temp); /* 返回从机ID */
	if(temp != MPU_WHOAMI_20602)	 return 0; /* 默认ID:0X12 */

	/* reset reg */
	icm20602_writebyte(MPU_RA_SIGNAL_PATH_RESET,0x03); /* 重置温度,加速度数字信号路径 */
	delay_ms(10);
    /* reset reg */
	icm20602_writebyte(MPU_RA_USER_CTRL,0x01); /* 重置陀螺仪数字信号路径 */	
	delay_ms(10);
	
	icm20602_writebyte(0x70,0x40);/* 禁用I2C从模块,仅将串行接口置于SPI模式 */
	delay_ms(10);
	icm20602_writebyte(MPU_RA_PWR_MGMT_2,0x00);
	delay_ms(10);
	icm20602_writebyte(MPU_RA_SMPLRT_DIV,0);
	delay_ms(10);
	icm20602_writebyte(MPU_RA_CONFIG,ICM20602_LPF_20HZ);
	delay_ms(10);
	icm20602_writebyte(MPU_RA_GYRO_CONFIG,(3 << 3)); /* +-2000°/s */
	delay_ms(10);
	icm20602_writebyte(MPU_RA_ACCEL_CONFIG,(2 << 3)); /* +-8g */
	delay_ms(10);
	icm20602_writebyte(0X1D,0x04);
	delay_ms(10);
	icm20602_writebyte(0X1E,0x00);/* 关闭低功耗 */
	delay_ms(10);
	icm20602_writebyte(0X23,0x00);/* 关闭FIFO */
	delay_ms(10);
	
	Center_offset_Set();
	
//	icm.Acc_Z_CALIB = 1; /* 开机自动校准acc_Z */
//	icm.Gyr_CALIB   = 2; /* 开机自动校准陀螺仪 */
	
	return 1; /* 初始化完成 */
}




/* 读取原始数据 */
u8 mpu_buffer[14];
void Icm20602_Read(void)
{
	icm20602_readbuf(MPUREG_ACCEL_XOUT_H,14,mpu_buffer);
}


//===================================================================


#define CALIB_SUM_TIME_SET 50 /* 求和次数设置 */

_ICM icm;
int icm_ref_val[6];
u16 acc_z_sum_time;  /*acc_z求和次数*/
int acc_z_sum_val[3];/*累加值*/



void Acc_Z_Calib(void)
{
	u8 i;
	static short acc_z_average[4];
	
	if(icm.Acc_Z_CALIB)
	{
		acc_z_sum_time++; /* 累加次数+1 */
		
		/* XY的参考值为减去重力加速度偏移量后的相对值 */
		acc_z_sum_val[0] += icm_ref_val[A_X];
		acc_z_sum_val[1] += icm_ref_val[A_Y];
		/* Z轴为原始数据 */
		acc_z_sum_val[2] += icm.Acc_Original[Z];
		
		if(acc_z_sum_time >= CALIB_SUM_TIME_SET) /* 达到校准次数 */
		{
			for(i=0; i<3; i++)
			{
				acc_z_average[i] = acc_z_sum_val[i] / CALIB_SUM_TIME_SET; /* 取平均值 */
				
				acc_z_sum_val[i] = 0;
			}
			
			acc_z_sum_time = 0;
			icm.Acc_Z_CALIB = 0;
			
			/* XY轴参数减去acc重力校准偏移量情况下Z轴理论加速度值 */
			acc_z_average[3] = Sqrt(4096*4096 - (POW(acc_z_average[0]) + POW(acc_z_average[1])));
			/* 校准值 = 实际值 - 理论值 */
			calib.acc_offset[Z] = acc_z_average[2] - acc_z_average[3];
		}
	}
}



/* 机身静止检测 */
void motionless_check(u8 T_ms)
{
	u8 i;

	for(i=0; i<3; i++)
	{
		/* 陀螺仪原始数据 > 50 */
		if(ABS(icm.Gyr_Original[i]) > 50)
		{
			flag.motionless = 0;
			return;
		}
	}
	
	flag.motionless = 1;
}




/* 传感器数据校准 */
void icm20602_Data_calib(u8 T_ms)
{
	u8 i;
	static u8 func_cycle_time = 0; /* 该函数执行次数记录 */
	static u16 acc_sum_time = 0, gyro_sum_time = 0; /* 累加次数记录 */
	static int sum_val[6]={0}; /* 累加值 */
	
	
	/* 机身静止检测 */
	motionless_check(T_ms);
	
	if(icm.Gyr_CALIB || icm.Acc_CALIB || icm.Acc_Z_CALIB) /* 检测校准标志 */
	{	
		/* 机身摇晃 或 过度倾斜 时 禁止校准 */
		if(flag.motionless == 0 || icm.Acc_Original[Z] < 2000)
		{
			/* 累加次数清零 */
			acc_sum_time = 0;
			gyro_sum_time = 0;
			acc_z_sum_time = 0;
			
			for(i=0; i<3; i++)
			{
				/* 累加值清零 */
				sum_val[G_X+i] = 0;
				sum_val[A_X+i] = 0;
				acc_z_sum_val[i] = 0;
			}
		}

		func_cycle_time++;
		
		if(func_cycle_time >= 10) /* 10个周期执行一次 */
		{
			func_cycle_time=0;

			if(icm.Gyr_CALIB)
			{   
				//==============================校准Gyro=============================
				gyro_sum_time++; /* 累加次数+1 */
				
				for(i=0; i<3; i++)
				{
					sum_val[G_X+i] += icm.Gyr_Original[i];
				}
				
				if(gyro_sum_time >= CALIB_SUM_TIME_SET) /* 达到累加次数 */
				{
					for(i=0; i<3; i++)
					{
						calib.gyr_offset[i] = (float)sum_val[G_X+i] / CALIB_SUM_TIME_SET; /* 取平均值 */
						
						sum_val[G_X+i] = 0;
					}
					
					gyro_sum_time = 0;
					
					if(icm.Gyr_CALIB == 1) /* icm.gyr_CALIBRATE == 1时保存数据 == 2时只校准 */
					{
						Flash_Save_Gyr(&calib);  /* 保存gyr校准数据 */
					}
					
					icm.Gyr_CALIB = 0; /* gyr 校准标志清零 */
				}
			}
			
			
			if(icm.Acc_CALIB == 1)
			{
				//==============================校准Acc==============================
				acc_sum_time++;
				
				sum_val[A_X] += icm.Acc_Original[A_X];
				sum_val[A_Y] += icm.Acc_Original[A_Y];
				sum_val[A_Z] += icm.Acc_Original[A_Z] - 4096; /* 65535/16 = 4096(+-8G) */

				if(acc_sum_time >= CALIB_SUM_TIME_SET) /* 达到累加次数 */
				{
					for(i=0 ;i<3; i++)
					{
						/* 此处Acc_Z的校准值为-----去除重力加速度下的Z轴累加平均值 */
						calib.acc_offset[i] = (float)sum_val[A_X+i] / CALIB_SUM_TIME_SET; /* 取平均值 */
						
						sum_val[A_X+i] = 0;
					}

					acc_sum_time =0;
					
					Flash_Save_Acc(&calib);	 /* 保存acc校准数据 */
					
					icm.Acc_CALIB = 0; /* Acc 校准标志清零 */
				}	
			}
		}
	}
}




_CENTER_POINT_OFFSET center_offset;
/* 设置传感器相对机身重心的偏移量 */
void Center_offset_Set(void)
{
	center_offset.distance[X] = 0.0f;
	center_offset.distance[Y] = 0.0f;
	center_offset.distance[Z] = 0.0f;
}




/* 数据准备 */
void Icm_Data_Prepare(u8 T_ms) /* Gyr,Acc校准,读原始数据 */
{	
	u8 i;
	float hz=0;
	static float gyr_filter_last[XYZ],acc_filter_last[XYZ];
	
	if(T_ms != 0)
	{
		hz = 1000/T_ms; /* 频率(hz) = 1/周期(s) */
	}

	icm20602_Data_calib(T_ms); /* Gyr,Acc校准 */

	/* 读取buffer原始数据 */
	icm.Acc_Original[X] = (short)((((u16)mpu_buffer[0]) << 8) | mpu_buffer[1]);
	icm.Acc_Original[Y] = (short)((((u16)mpu_buffer[2]) << 8) | mpu_buffer[3]);
	icm.Acc_Original[Z] = (short)((((u16)mpu_buffer[4]) << 8) | mpu_buffer[5]);
 
	icm.Gyr_Original[X] = (short)((((u16)mpu_buffer[ 8]) << 8) | mpu_buffer[ 9]);
	icm.Gyr_Original[Y] = (short)((((u16)mpu_buffer[10]) << 8) | mpu_buffer[11]);
	icm.Gyr_Original[Z] = (short)((((u16)mpu_buffer[12]) << 8) | mpu_buffer[13]);

	icm.Tempreature_Original = ((((short)mpu_buffer[6]) << 8) | mpu_buffer[7]);
	icm.Tempreature = icm.Tempreature_Original/326.8f + 25;
	
	/* 得出校准后的数据 */
	for(i=0; i<3; i++)
	{ 
		icm_ref_val[A_X+i] = icm.Acc_Original[i] - calib.acc_offset[i];
		icm_ref_val[G_X+i] = icm.Gyr_Original[i] - calib.gyr_offset[i];
	}
	
	/* 二次校准Acc_z轴 */
	Acc_Z_Calib();
	
	/* 
		对相对量进行低通滤波
		Y(本次) =  aX(本次) + (1-a)Y(上次) 
	=>	Y(本次)+= a[X(本次) -  Y(上次)
	*/
	for(i=0; i<3; i++)
	{	
		icm.Gyr_filter[X+i] += 0.12f * (icm_ref_val[G_X+i] - gyr_filter_last[i]);
		icm.Acc_filter[X+i] += 0.12f * (icm_ref_val[A_X+i] - acc_filter_last[i]);			
	}

	for(i=0; i<3; i++)
	{
		center_offset.gyr_rad_v[i] = icm.Gyr_filter[X+i] * PN2000_TO_RAD_P_S; /* 求GYR角速度 */
		center_offset.gyr_rad_a[i] = hz * (center_offset.gyr_rad_v[i] - center_offset.gyr_rad_v_old[i]); /* 求GYR角加速度 */	
		center_offset.gyr_rad_v_old[i] = center_offset.gyr_rad_v[i];
	}

	   /* 求x */		   /* 求y */		   /* 求z */																										
	/*  z   x	*/		/*  x   y	*/		/*  y   z	*/
    /*	|  /    */      /*	|  /    */      /*	|  /    */
    /*	| /     */      /*	| /     */      /*	| /     */
    /*	|/      */      /*	|/      */      /*	|/      */
    /* 	— — — y */      /* 	— — — z */      /* 	— — — x */
	/* 自旋时各轴向心加速度(当加速度计不在机体坐标零点时) */
	center_offset.centri_a[X] = center_offset.gyr_rad_a[Z] * center_offset.distance[Y]; /* 向心a = 角a * R */
	center_offset.centri_a[Y] = center_offset.gyr_rad_a[X] * center_offset.distance[Z];
	center_offset.centri_a[Z] = center_offset.gyr_rad_a[Y] * center_offset.distance[X];
	
	/* 赋值 */
	for(i=0; i<3; i++)
	{
		gyr_filter_last[X+i] = icm.Gyr_filter[X+i];
		acc_filter_last[X+i] = icm.Acc_filter[X+i] - center_offset.centri_a[i] / PN8G_TO_CM_P_SS; /* 若有自旋，则减去自旋产生的向心加速度干扰 */
	}
	
	/* 转换单位 */
	for(i=0; i<3; i++)
	{
		/* GYR转换到deg/s */
		icm.Gyr_deg[i] = icm.Gyr_filter[X+i] * PN2000_TO_DEG_P_S;
		/* GYR转换到rad/s */
		icm.Gyr_rad[i] = icm.Gyr_filter[X+i] * PN2000_TO_RAD_P_S;
		/* ACC转换到cm/s2 */
		icm.Acc_cmss[i]= icm.Acc_filter[X+i] * PN8G_TO_CM_P_SS;
	}
}





/* end of icm20602.c */


