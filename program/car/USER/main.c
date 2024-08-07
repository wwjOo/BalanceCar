/**
  *****************************************************************************
  * @file				main.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/

#include "main.h"


void start_up(void)
{
	systick_Init();
	led_Init();
	usart1_Init(115200);
	SPI2_init();
	Adc_Init();
	encode_Init();
	motor_Init(MOTOR_ARR,MOTOR_PRE);
	PID_Init();
	
	Icm20602_CSPin_Init();
	AK8975_CSPin_Init();
	  flag.gyr_ok
	= flag.acc_ok
	= Icm20602_Reg_Init();
	
	Flash_Read();

	oled_Init();
	Scheduler_Setup();
}


int main(void)
{	
	start_up();
	
	while(1)
	{
		Scheduler_Run();
	}
}



#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
	while(1)
	{
		/* printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	}
}
#endif



/* end of main.c */


