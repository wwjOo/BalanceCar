#include "includes.h"




int main(void)
{
	SystemInit();
	LED_init();
	KEY_init();
	USART1_init(115200);
	ADC1_init();
	TIM4_init(1000,36);
	
	while(1)
	{
		if(TIM4_Cnt >= 25)
		{
			TIM4_Cnt = 0;
			
			Data_Get();
			Data_Prepare();
			usart1_send(TX1_Buf,13);
		}
		
		LED_Flash();
	}
}


