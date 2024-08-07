#ifndef __ADC_H
#define __ADC_H
#include "includes.h"


extern u16 ADC_Data[5];


#define ADC_CHANNEL_01		ADC_Channel_10
#define ADC_CHANNEL_02		ADC_Channel_11
#define ADC_CHANNEL_03		ADC_Channel_12
#define ADC_CHANNEL_04		ADC_Channel_13
#define ADC_CHANNEL_05		ADC_Channel_14


#define REG_ADDR	(u32)&ADC1->DR
#define RAM_ADDR	(u32)ADC_Data
#define RAM_SIZE	5


void ADC1_init(void);


#endif


