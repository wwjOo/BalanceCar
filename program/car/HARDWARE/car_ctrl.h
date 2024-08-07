#ifndef __CAR_CTRL_H
#define __CAR_CTRL_H
#include "sys.h"





typedef struct
{
	float kp;
	float ki;
	float kd;
	
	float err;
	
	float integral;
	
	float dout;
	
	float out;
	
}_PID;


void PID_Init(void);
void Car_Ctrl(void);


typedef struct
{
	float Speed_Set;	/* 前进目标速度 */
	float Tspeed_Set;	/* 转向目标速度 */
}_Car;


extern _PID balance;
extern _PID speed;
extern _PID turn;


#endif

