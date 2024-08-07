/**
  *****************************************************************************
  * @file				ak8975.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/



#include "ak8975.h"
#include "MagProcess.h"
#include "spi.h"


void AK8975_CSPin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOD, GPIO_Pin_10);
	
	flag.mag_ok = 1;
	
	Mag_Calib_Reset(3); /* 校准数据复位 */
}



static void ak8975_enable(u8 Enable)
{
	if(Enable)	
		GPIO_ResetBits(GPIOD, GPIO_Pin_10);
	else		
		GPIO_SetBits(GPIOD, GPIO_Pin_10);
}



static void ak8975_Trig(void)
{
	ak8975_enable(1);
	
	SPI2_RW(AK8975_CNTL_REG);
	SPI2_RW(0x01);
	
	ak8975_enable(0);
}



u8 ak8975_buf[6];
void AK8975_Read(void)
{		
	u8 i;
	
	ak8975_enable(1); /* SPI片选选中 */
	
	SPI2_RW(AK8975_HXL_REG | 0x80);
	for(i=0; i<6; i++)
	{
		ak8975_buf[i] = SPI2_RW(0xff);
	}
	
	ak8975_enable(0);
	
	ak8975_Trig();
}





/* end of ak8975.c */
