/**
  *****************************************************************************
  * @file				spi.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/



#include "spi.h"



void SPI2_init(void)
{
	SPI_InitTypeDef SPI_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2); /* SPI2_SCK */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2); /* SPI2_MISO */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2); /* SPI2_MOSI */

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    /* SPI SCK */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* SPI MISO */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* SPI MOSI */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
    SPI_InitStructure.SPI_Mode 		= SPI_Mode_Master; 
    SPI_InitStructure.SPI_DataSize 	= SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL 		= SPI_CPOL_High; 
    SPI_InitStructure.SPI_CPHA 		= SPI_CPHA_2Edge; 
    SPI_InitStructure.SPI_NSS 		= SPI_NSS_Soft; 
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; 
    SPI_InitStructure.SPI_FirstBit 	= SPI_FirstBit_MSB; 
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);

    SPI_Cmd(SPI2, ENABLE);
}


u8 SPI2_RW(u8 data)
{
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	
    SPI_I2S_SendData(SPI2, data);
	
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	
    return SPI_I2S_ReceiveData(SPI2);
}


/* ·¢ËÍ */
void SPI2_Transmit(u8 *Data, u16 size)
{
	u16 i;
	
    for(i=0; i<size; i++)
    {
        SPI2_RW(Data[i]);
    }
}


/* ½ÓÊÕ */
void SPI2_Receive(u8 *Data, u16 size)
{
	u16 i;
	
    for(i=0; i<size; i++)
    {
        Data[i] = SPI2_RW(0);
    }
}



/* end of spi.h */


