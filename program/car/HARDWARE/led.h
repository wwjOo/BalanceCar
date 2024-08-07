/**
  *****************************************************************************
  * @file				led.h
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/




#ifndef __LED_H
#define __LED_H
#include "sys.h"


typedef struct
{
	u8 number; /* LED序号 */
	
	GPIO_TypeDef* GPIOx;
	u16 Pin;
	
}_LED;


/* 可修改部分 */
//===================================================================
enum //LED序号
{
	LED1=0,
	LED2,
	
	LED_NUM,
};
//====
#define LED1_CHANGE	PAout(4) = ~PAout(4)				/* 闪烁 */
#define LED2_CHANGE	PAout(5) = ~PAout(5)				/* 闪烁 */
//====
#define LED1_OFF	GPIO_SetBits(  led1.GPIOx,led1.Pin)	/* 亮(默认低电平为点亮) */
#define LED1_ON		GPIO_ResetBits(led1.GPIOx,led1.Pin)	/* 灭 */
#define LED2_OFF	GPIO_SetBits(  led2.GPIOx,led2.Pin)	/* 亮(默认低电平为点亮) */
#define LED2_ON		GPIO_ResetBits(led2.GPIOx,led2.Pin)	/* 灭 */
//====
#define LED_INIT	led_Config(&led1);led_Config(&led2);
//====
extern	_LED led1;
extern	_LED led2;
//===================================================================


void led_Config(_LED *L);
void led_Init(void);
void led_state_ctrl(void);

void led_task(float T);

void led_lightmode_check(void);
void led_reset(void);

void TIM6_Init(u16 arr,u16 pre);


#endif


/* end of led.h */

