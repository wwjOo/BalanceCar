/**
  *****************************************************************************
  * @file				MagProcess.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/




#include "MagProcess.h"
#include "ak8975.h"
#include "icm20602.h"
#include "imu.h"
#include "stmflash.h"




_MAG mag;
short mag_ref_val[XYZ];
short max_in[XYZ];
short min_in[XYZ];



/* 获取初始值 */
void Mag_Get(void)
{
	mag.Original[X] = ((((short)ak8975_buf[1]) << 8) | ak8975_buf[0]);
	mag.Original[Y] = ((((short)ak8975_buf[3]) << 8) | ak8975_buf[2]);
	mag.Original[Z] = ((((short)ak8975_buf[5]) << 8) | ak8975_buf[4]);
	
	/* 适应机体坐标 */
	mag_ref_val[X] = +mag.Original[X];       //===================改变方向==================待解决
	mag_ref_val[Y] = -mag.Original[Y];
	mag_ref_val[Z] = -mag.Original[Z];
}



/* 校准复位 */
void Mag_Calib_Reset(u8 mode)
{
	u8 i;
	
	if(mode == 1) /* 校准Z */
	{
		max_in[Z] = -500;
		min_in[Z] =  500;
	}
	
	else if(mode == 2) /* 校准XY */
	{
		for(i=0; i<2; i++)
		{
			max_in[i] = -500;
			min_in[i] =  500;
		}
	}
	
	else /* 校准XYZ */
	{
		for(i=0; i<3; i++)
		{
			max_in[i] = -500;
			min_in[i] =  500;
		}
	}
}



/* 校准XY */
void Mag_Calib_XY(void)
{
	u8 i;
	
	for(i=0; i<2; i++)
	{
		max_in[i] = MAX(max_in[i],mag_ref_val[i]);
		min_in[i] = MIN(min_in[i],mag_ref_val[i]);
	}
}



/* 校准Z */
void Mag_Calib_Z(void)
{
	max_in[Z] = MAX(max_in[Z],mag_ref_val[Z]);
	min_in[Z] = MIN(min_in[Z],mag_ref_val[Z]);
}



#define ERROR1 10
#define ERROR2 11

/* 数据处理 */
void Mag_Data_Deal_Task(u16 T_ms,float gyr_deg_x,float gyr_deg_z)
{	
	u8 i;
	static u16 timer; /* 超时记录 */
	static float mag_cum_angle[2]; /* 积分角度 */
	
	for(i=0; i<3; i++)
	{
		mag.handle_val[i] = (mag_ref_val[i] - calib.mag_offset[i]);
	}

	if(mag.CALIB) /* 校准标志检测 */
	{	
		switch(mag.Calib_Step)
		{
			/* 第一步,水平旋转 */
			case 0: 
				
				Mag_Calib_XY();
			
				if(imu_data.z_vector[Z] < 0.985f) /* 地理坐标系的重力向量转到机体坐标系中z轴的分量 (+-10°) 偏角太大 */
				{
					mag.Calib_Step = ERROR1; /* 错误1 */
				}
				else
				{
					mag_cum_angle[0] += 0.001 * T_ms * gyr_deg_z; /* 角速度积分 */
					
					if(ABS(mag_cum_angle[0]) > 360) /* 至少旋转360度 */
					{
						mag_cum_angle[0] = 0;
						mag.Calib_Step = 1;
						timer = 0;
					}
				}
				break;
			
			/* 第二步,竖直旋转 */
			case 1: 
				
				Mag_Calib_Z();
			
				mag_cum_angle[1] += 0.001f * T_ms * gyr_deg_x; /* 角速度积分 */
			
				if(ABS(mag_cum_angle[1]) > 360) /* 至少旋转360度 */
				{
					mag_cum_angle[1] = 0;
					mag.Calib_Step = 2;
				}
				break;
			
			case 2:
				for(i=0; i<3; i++)
				{
					calib.mag_offset[i] = (max_in[i] + min_in[i]) * 0.5f; /* 取均值 */
				}
				
				Flash_Save_Mag(&calib);/* 保存mag校准数据 */
				
				Mag_Calib_Reset(3); /* 校准数据复位 */
				mag_cum_angle[0] = 0;
				mag_cum_angle[1] = 0;
				
				mag.Calib_Step = 0;
				mag.CALIB = 0; /* 完成校准 */
				timer = 0;
				break;
			
			case ERROR1:
				
				Mag_Calib_Reset(2); /* 复位XY,重新开始 */
				mag_cum_angle[0] = 0;
				mag.Calib_Step = 0;
				break;
			
			case ERROR2:
				
				Mag_Calib_Reset(3); /* 超时,复位XYZ,退出校准 */
				mag_cum_angle[0] = 0;
				mag_cum_angle[1] = 0;
			
				mag.Calib_Step = 0;
				mag.CALIB = 0;
				timer = 0;
				break;
			
			default:break;
		}
		
		if(mag.Calib_Step == 0 || mag.Calib_Step == 1)
		{
			/* 计时 */
			if(timer < 15000)
			{
				timer += T_ms;
			}
			else /* 超时 */
			{
				mag.Calib_Step = ERROR2;
			}
		}
		else
		{
			timer = 0;
		}
	}
	else
	{
		mag.Calib_Step = 0;
	}
}


void Mag_Update_Task(u16 T_ms)
{	
	Mag_Get();
	
	Mag_Data_Deal_Task(T_ms,icm.Gyr_deg[X],icm.Gyr_deg[Z]);
}




/* end of MagProcess.c */


