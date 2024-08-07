/**
  *****************************************************************************
  * @file				DataCal.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/




#include "DataCal.h"
#include "icm20602.h"
#include "ak8975.h"
#include "imu.h"
#include "MagProcess.h"




void Sensor_Get(void) /* 1ms */
{
	static u8 cnt;
	
	/* 读取惯性传感器数据 */
	Icm20602_Read();
	
	cnt ++;
	cnt %= 20;
	
	if(cnt == 0)
	{
		/* 读取磁力计数据 */
		AK8975_Read();
	}
}




void IMU_Update_Task(u16 T_ms)
{
	/* 设置重力加速度互补融合修正kp系数 */
	imu_state.gkp = 0.3f;
	
	/* 设置重力加速度互补融合修正ki系数 */
	imu_state.gki = 0.002f;
	 
	/* 设置罗盘互补融合修正ki系数 */ //==========================待解决====
	imu_state.mkp = 0.2f;
	
	/* 磁力计修正使能 */
	imu_state.Mag_fix_enable = flag.mag_ok;
	
	/* 数据更新,融合 */
	IMU_update(T_ms*0.001, &imu_state, icm.Gyr_rad, icm.Acc_cmss, mag.handle_val, &imu_data);
}



/* end of DataCal.c */
