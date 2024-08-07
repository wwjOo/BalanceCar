#ifndef __TIMER_H
#define __TIMER_H
#include "includes.h"



void TIM4_init(u16 per, u16 pre);


extern u16 TIM4_Cnt;
extern u16 LED_Cnt;



#endif
