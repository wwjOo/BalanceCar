#include "timer.h"

void TIM4_init(u16 per, u16 pre)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4 ,ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Period= per-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler= pre-1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	NVIC_SetPriority(TIM4_IRQn,NVIC_EncodePriority(2,1,0));
	NVIC_EnableIRQ(TIM4_IRQn);
	
	TIM_Cmd(TIM4,ENABLE);
}


u16 TIM4_Cnt = 0;
u16 LED_Cnt = 0;


void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
		
		TIM4_Cnt++;

		LED_Cnt++;
	}
}

