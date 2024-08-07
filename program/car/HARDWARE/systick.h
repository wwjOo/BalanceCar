/**
  *****************************************************************************
  * @file				systick.h
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/


#ifndef __SYSTICK_H
#define	__SYSTICK_H
#include "sys.h"


void systick_Init(void);
u32 getsystick_us(void);
u32 getsystick_ms(void);
void delay_us(u32 us);
void delay_ms(u32 ms);


#endif


/* end of systick.h */

