/**
  *****************************************************************************
  * @file				usart1.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/






#include "main.h"



/* init */
void usart1_Init(u32 B)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	USART_InitStruct.USART_BaudRate   = B;
	USART_InitStruct.USART_Parity     = USART_Parity_No;
	USART_InitStruct.USART_StopBits   = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStruct);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1,USART_IT_TXE,DISABLE); /* 关闭发送中断 */
	
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(2,1,1));
	NVIC_EnableIRQ(USART1_IRQn);
	
	USART_Cmd(USART1,ENABLE);
}



/* Send */
u8 TX1_Buf[256] = {0};
u8 TX1_Count = 0;
void usart1_send(u8 *data, u8 num)
{
	u8 i;
	
	if(!(USART1->CR1 & USART_CR1_TXEIE))
	{
		for(i=0; i< num; i++)
		{
			TX1_Buf[TX1_Count++] = *(data + i);
		}
		
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}
}




_Rc RC;


/* receive */
u8 RX1_Buf[50]  = {0};
void Data_Receive_Prepare(u8 data)
{
	static u8 state = 0;
	
	if(state==0 && data=='*')/* 头 */
	{
		state=1;
	}
	else if(state == 1 && (data == 'D' || data == 'T' || data == 'S')) /* 功能字 */
	{
		switch(data)
		{
			case 'D':
				state = 11;
				break;
			case 'T':
				state = 21;
				break;
			case 'S':
				state = 31;
				break;
		}
	}
	else if(state > 10 && state <= 20) //*D - 摇杆
	{
		if(state==11 && data=='X')
		{
			state=12;
		}
		else if(state==12)	
		{
			state=13;
			RC.buf[0]=data;
		}
		else if(state==13)	
		{
			state=14;
			RC.buf[1]=data;
			RC.y = atoi(RC.buf) - 50;
		}
		else if(state == 14 && data=='Y')
		{
			state = 15;
		}
		else if(state==15)	
		{
			state=16;
			RC.buf[0]=data;
		}
		else if(state == 16)
		{
			state = 0;
			RC.buf[1]=data;
			RC.x = 50 - atoi(RC.buf);
		}
	}
	else if(state > 20 && state <=30) //*T - PID调参
	{
		if(state==21)
		{
			if(data == 'B')
				state = 22;
			else if(data == 'S')
				state = 23;
		}
		else if(state == 22) //balance
		{
			switch(data)
			{
				case '1':
					balance.kp+=2.5;
					break;
				case '3':
					balance.kp-=2.5;
					break;
				case '2':
					balance.kd+=0.1;
					break;
				case '4':
					balance.kd-=0.1;
					break;
			}
			state = 0;
		}
		else if(state == 23) //speed
		{
			switch(data)
			{
				case '1':
					speed.kp+=2.0;
					break;
				case '3':
					speed.kp-=2.0;
					break;
				case '2':
					speed.ki+=0.025;
					break;
				case '4':
					speed.ki-=0.025;
					break;
			}
			state = 0;
		}
	}
	else if(state > 30 && state <=40) //*S 电机开关
	{
		if(data == 'C')
			Motor.Switch = 1;
		else if(data == 'c')
			Motor.Switch = 0;
		
		state = 0;
	}
	else
	{
		state = 0;
	}
}



void USART1_IRQHandler(void)
{
	u8 data;
	static u8 count = 0;
	
	if(USART_GetITStatus(USART1,USART_IT_TXE))
	{
		USART1->DR = TX1_Buf[count++];
		
		if(count == TX1_Count)
		{
			USART1->CR1 &= ~USART_CR1_TXEIE;
		}
	}
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		
		data = USART_ReceiveData(USART1);
		
		Data_Receive_Prepare(data);
	}
}


/*end of usart1.c*/

