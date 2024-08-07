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
	float Speed_Set;	/* ǰ��Ŀ���ٶ� */
	float Tspeed_Set;	/* ת��Ŀ���ٶ� */
}_Car;


extern _PID balance;
extern _PID speed;
extern _PID turn;


#endif

