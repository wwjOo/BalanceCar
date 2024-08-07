/**
  *****************************************************************************
  * @file				imu.h
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/




#ifndef __IMU_H
#define __IMU_H
#include "sys.h"
#include "structural.h"


typedef struct
{	
	float w;	/* q0 */
	float x;	/* q1 */
	float y;	/* q2 */
	float z;	/* q3 */
	
	/* ��̬�� */
	float rol;	
	float pit;
	float yaw;

	float x_vector[XYZ];	/* ��������ϵX�����������ϵ����ķ����������� */
	float y_vector[XYZ];	/* ��������ϵY�����������ϵ����ķ����������� */
	float z_vector[XYZ];	/* ��������ϵZ�����������ϵ����ķ����������� */
	
	float h_mag[XYZ];

}_IMU;




typedef struct
{
	/* pid����ϵ�� */
	float gkp;
	float gki;
	float mkp;
	
	u8 Gyr_reset;
	u8 Mag_reset;
	u8 Mag_fix_enable;
	
}_IMU_STATE;



void IMU_update(float T_ms,_IMU_STATE *state,float gyr[XYZ], int acc[XYZ],short mag[XYZ],_IMU *imu);
void Angle_calculate(void);


extern _IMU imu_data;
extern _IMU_STATE imu_state;


#endif




/* end of imu.h */


