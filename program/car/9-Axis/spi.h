/**
  *****************************************************************************
  * @file				spi.h
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/



#ifndef __SPI_H
#define __SPI_H
#include "sys.h"


void SPI2_init(void);

u8 SPI2_RW(u8 data);

void SPI2_Transmit(u8 *Data, u16 size);
void SPI2_Receive(u8 *Data, u16 size);


#endif



/* end of spi.h */
