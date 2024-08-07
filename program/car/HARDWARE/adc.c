/**
  *****************************************************************************
  * @file				adc.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/





#include "adc.h"

u16 AdcValue = 0;


/* 初始化DMA */
static void Init_AdcDMA(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
	while(DMA_GetCmdStatus(DMA2_Stream0)!=DISABLE); /* 等待DMA可以配置 */

	DMA_InitStructure.DMA_Channel = DMA_Channel_0; /* DMA通道0 */
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)ADC1_BASE+0x4C; /* ADC1 偏移 : 0x00-0x4C(通道15) */
	DMA_InitStructure.DMA_Memory0BaseAddr    = (u32)&AdcValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; /* 数据长度半字 */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; /* 数据长度半字 */
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; /* 循环模式 */
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; /* 禁止FIFO */
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; /* 单次传输 */
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; /* 单次传输 */
	DMA_Init(DMA2_Stream0,&DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0,ENABLE); /* 开启DMA传输 */
}



/* 初始化ADC */
void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	Init_AdcDMA();

	/* PC5 */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; /* APB2的4分频 即84/4=21M */
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; /* 两个采样阶段的延时5个时钟 */
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution =  ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; /* 禁止触发检测 使用软件触发 */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1,ADC_Channel_15,1,ADC_SampleTime_480Cycles);
	
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE); /*源数据变化时开启DMA传输 */
	
	ADC_Cmd(ADC1,ENABLE); /* 开启转换 */
	ADC_DMACmd(ADC1,ENABLE); /* 使能ADC传输 */
	
	ADC_SoftwareStartConv(ADC1);/* 启动软件转换 */
}




/* end of adc.c*/


