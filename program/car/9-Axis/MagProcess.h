/**
  *****************************************************************************
  * @file				MagProcess.h
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/




#ifndef __MAGPROCESS_H
#define __MAGPROCESS_H
#include "sys.h"
#include "structural.h"



typedef struct
{
	u8 CALIB;
	
	u8 Calib_Step;
	
	short Original[XYZ];	/* 初始数据 */
	short handle_val[XYZ];	/* 处理后数据 */

}_MAG;



void Mag_Calib_Reset(u8 mode);
void Mag_Data_Deal_Task(u16 T_ms,float gyr_deg_x,float gyr_deg_z);

void Mag_Update_Task(u16 T_ms);


extern _MAG mag;



#endif





/* end of MagProcess.h */

