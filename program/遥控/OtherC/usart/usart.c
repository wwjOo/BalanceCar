#include "usart.h"


void USART1_init(u32 B)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* PA9,10 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(2,1,1));
	NVIC_EnableIRQ(USART1_IRQn);

	USART_InitStructure.USART_BaudRate = B;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;

	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
}



u8 RX1_Buf[50]  = {0};
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



void USART1_IRQHandler(void)
{
	static u8 count = 0;
	
	if(USART_GetITStatus(USART1,USART_IT_TXE))
	{
		USART1->DR = TX1_Buf[count++];
		
		if(count == TX1_Count)
		{
			count = 0;
			TX1_Count = 0;
			USART1->CR1 &= ~USART_CR1_TXEIE;
		}
	}
}


