/**
  *****************************************************************************
  * @file				usart1.h
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/





#ifndef __USART1_H
#define	__USART1_H
#include "sys.h"




void usart1_Init(u32 B);
void usart1_send(u8 *data, u8 num);


typedef struct
{
	char buf[3];
	
	float x;
	float y;
}_Rc;


extern _Rc RC;



#endif




/*end of usart1.h*/
