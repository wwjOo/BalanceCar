/**
  *****************************************************************************
  * @file				encoder.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/19
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/






#include "encode.h"


/* TIM2/5 CH1,CH2(PA0,1) / TIM3 CH1,CH2 */
void encode_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);
	
	TIM_TimeBaseInitStruct.TIM_Period=0XFFFF;
	TIM_TimeBaseInitStruct.TIM_Prescaler=0;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	TIM2->CNT = 0;
	TIM3->CNT = 0;
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
	TIM_ICInitStruct.TIM_ICFilter=20; /* 滤波 */
	TIM_ICInit(TIM2, &TIM_ICInitStruct);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
	TIM_ICInitStruct.TIM_ICFilter=20; /* 滤波 */
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}



/* 瞬时角速度计算 - 20ms读取,否则该组PID参数将会失效*/
#include "motor.h"
void Encode_Task(u16 T_ms)
{	
	static float encode_lpf;
	
	encode_lpf = (short)TIM2->CNT - (short)TIM3->CNT;
	
	TIM2->CNT = 0;
	TIM3->CNT = 0;
	
	Motor.encode += 0.5f * (encode_lpf - Motor.encode);
}   




/* end of encoder.c*/

