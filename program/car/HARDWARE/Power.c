/**
  *****************************************************************************
  * @file				Power.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/20
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/





#include "Power.h"
#include "adc.h"

#define ABS(X) ((X > 0) ? X : -X)

_Power Power = {20,0};


/* 1v~11v~ADC(1241) 11/1241=0.00886382f(理论系数) */
void Power_UpdateTask(u8 T_ms)
{
	float voltage_get;
	static float coaf = 1.0f;
	
	static u8 cnt;
	static float v_last;
	
	cnt++;
	
	voltage_get = AdcValue *0.00879f; /* 实际系数 */
	
	Power.Voltage += coaf * (voltage_get - Power.Voltage);
	
	if(cnt%10)
	{
		if(ABS(Power.Voltage - v_last)<0.1f)
		{
			coaf = 0.01f;
			
			if(Power.Voltage < WARN_V)	/* 低电量 */
			{
				Power.Warn = 1;
			}
			
			else
			{
				Power.Warn = 0;
			}
		}
	}
	
	v_last = Power.Voltage;
}



/* end of Power.h*/


