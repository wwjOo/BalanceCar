/**
  *****************************************************************************
  * @file				motor.h
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/






#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"


#define MOTOR_ARR	8400 /* 10KHz */
#define MOTOR_PRE	1


#define MOTOR1_SPEED	TIM1->CCR1
#define MOTOR2_SPEED	TIM1->CCR4

#define MOTOR_ALL_ON		PEout(15)=1
#define MOTOR_ALL_OFF		PEout(15)=0

#define MOTOR1_OFF			PEout(10)=1;PEout(11)=1	
#define MOTOR1_R			PEout(10)=0;PEout(11)=1   	/* 顺时针 */
#define MOTOR1_L			PEout(10)=1;PEout(11)=0 
		
#define MOTOR2_OFF			PEout(12)=1;PEout(13)=1	
#define MOTOR2_R			PEout(12)=1;PEout(13)=0 	/* 顺时针 */
#define MOTOR2_L			PEout(12)=0;PEout(13)=1	

#define STOP_ANGLE			70.0f 	/*最大倾角,超过停转*/


typedef struct
{
	float out1;
	float out2;
	
	u8 Switch;
	
	float encode;
	
}_Motor;


void motor_Init(u16 arr,u16 pre);
void Motor_Ctrl(void);

extern _Motor Motor;

#endif



/* end of motor.h*/


