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
	
	/* ��ȡ���Դ��������� */
	Icm20602_Read();
	
	cnt ++;
	cnt %= 20;
	
	if(cnt == 0)
	{
		/* ��ȡ���������� */
		AK8975_Read();
	}
}




void IMU_Update_Task(u16 T_ms)
{
	/* �����������ٶȻ����ں�����kpϵ�� */
	imu_state.gkp = 0.3f;
	
	/* �����������ٶȻ����ں�����kiϵ�� */
	imu_state.gki = 0.002f;
	 
	/* �������̻����ں�����kiϵ�� */ //==========================�����====
	imu_state.mkp = 0.2f;
	
	/* ����������ʹ�� */
	imu_state.Mag_fix_enable = flag.mag_ok;
	
	/* ���ݸ���,�ں� */
	IMU_update(T_ms*0.001, &imu_state, icm.Gyr_rad, icm.Acc_cmss, mag.handle_val, &imu_data);
}



/* end of DataCal.c */
