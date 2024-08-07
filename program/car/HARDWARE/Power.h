/**
  *****************************************************************************
  * @file				Power.h
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/20
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/





#ifndef __POWER_H
#define __POWER_H
#include "sys.h"



#define WARN_V		3.8f*3 	/* 11.4V */


typedef struct
{
	float Voltage;
	u8 Warn;
	
}_Power;


void Power_UpdateTask(u8 T_ms);


extern _Power Power;


#endif




/* end of Power.h*/

