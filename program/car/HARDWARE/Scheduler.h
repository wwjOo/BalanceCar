/**
  *****************************************************************************
  * @file				Scheduler.h
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/



#ifndef __SCHEDULER_H
#define __SCHEDULER_H
#include "sys.h"



typedef struct
{
	void(*task_func)(void);
	
	u16 rate_hz;
	u16 interval_ticks;
	u32 last_run;
	
}_SCHEDULER_TASK;



void Scheduler_Setup(void);
void Scheduler_Run(void);


#endif



/* end of Scheduler.h */


