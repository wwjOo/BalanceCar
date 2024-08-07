/**
  *****************************************************************************
  * @file				DataCal.h
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/



#ifndef __DATACAL_H
#define __DATACAL_H
#include "sys.h"
#include "structural.h"




void Sensor_Get(void);
void IMU_Update_Task(u16 T_ms);






#endif




/* end of DataCal.h */



