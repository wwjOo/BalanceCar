#ifndef __USART_H
#define __USART_H

#include "includes.h"


extern u8 TX1_Buf[256];


void USART1_init(u32 B);
void usart1_send(u8 *data, u8 num);


#endif


