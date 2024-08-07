#include "led.h"


void Low_Power(void);
void Mid_Power(void);
void Full_Power(void);


void LED_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void LED_Flash(void)
{
	if(LED_Cnt <= 500)
	{
		LED_B_ON;
		LED_R_OFF;
		LED_G_OFF;
	}
	else if(LED_Cnt <= 1000)
	{
		LED_B_OFF;
		LED_R_ON;
		LED_G_OFF;
	}
	else if(LED_Cnt <= 1500)
	{
		LED_B_OFF;
		LED_R_OFF;
		LED_G_ON;
	}
	else
	{
		LED_Cnt = 0;
	}
}


