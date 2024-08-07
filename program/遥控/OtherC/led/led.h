#ifndef __LED_H
#define __LED_H
#include "includes.h"


void LED_init(void);

void LED_Flash(void);


#define LED_R_ON	GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define LED_R_OFF	GPIO_ResetBits(GPIOB, GPIO_Pin_6)

#define LED_G_ON	GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define LED_G_OFF	GPIO_ResetBits(GPIOB, GPIO_Pin_5)

#define LED_B_ON	GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define LED_B_OFF	GPIO_ResetBits(GPIOB, GPIO_Pin_7)


#endif


