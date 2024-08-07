#include "data_count.h"


_RC RC;
short RC_DATA[5];
float Voltage;


void Data_Get(void)
{
	RC.AD[0] = ADC_Data[0];
	RC.AD[1] = ADC_Data[1];
	RC.AD[2] = ADC_Data[2];
	RC.AD[3] = ADC_Data[3];
	
	if(KEY_L_U_PRESS)
		RC.KEY[KEY_L_U] = 1;
	else
		RC.KEY[KEY_L_U] = 0;
	
	if(KEY_L_D_PRESS)
		RC.KEY[KEY_L_D] = 1;
	else
		RC.KEY[KEY_L_D] = 0;
	
	if(KEY_L_L_PRESS)
		RC.KEY[KEY_L_L] = 1;
	else
		RC.KEY[KEY_L_L] = 0;
	
	if(KEY_L_R_PRESS)
		RC.KEY[KEY_L_R] = 1;
	else
		RC.KEY[KEY_L_R] = 0;
	/**********************/
	if(KEY_R_U_PRESS)
		RC.KEY[KEY_R_U] = 1;
	else
		RC.KEY[KEY_R_U] = 0;
	
	if(KEY_R_D_PRESS)
		RC.KEY[KEY_R_D] = 1;
	else
		RC.KEY[KEY_R_D] = 0;
	
	if(KEY_R_L_PRESS)
		RC.KEY[KEY_R_L] = 1;
	else
		RC.KEY[KEY_R_L] = 0;
	
	if(KEY_R_R_PRESS)
		RC.KEY[KEY_R_R] = 1;
	else
		RC.KEY[KEY_R_R] = 0;
	
	Voltage = ADC_Data[4]/4096.0f * 3.3f * 2.0f;
}




void Data_Prepare(void)
{
	u8 i;
	
	RC_DATA[0] = -(RC.AD[0]/2.025f - 1000) * 0.555f;
	RC_DATA[1] = -(RC.AD[1]/2.025f - 1000) * 0.555f;
	RC_DATA[2] = -(RC.AD[2]/2.025f - 1000) * 0.555f;
	RC_DATA[3] = -(RC.AD[3]/2.025f - 1000) * 0.555f;
	
	for(i=0; i<4; i++)
	{
		if(ABS(RC_DATA[i]) < 50)
		{
			RC_DATA[i] = 0;
		}
		else
		{
			RC_DATA[i] -= 50;
		}
		if(ABS(RC_DATA[i]) > 500)
		{
			RC_DATA[i] = RC_DATA[i]/ABS(RC_DATA[i]) * 500;
		}
	}
	RC_DATA[4] = RC.KEY[0] | RC.KEY[1]<<1 | RC.KEY[2]<<2 | RC.KEY[3]<<3 | RC.KEY[4]<<4 | RC.KEY[5]<<5 | RC.KEY[6]<<6 | RC.KEY[7]<<7;
	/********************/
	TX1_Buf[0] = 0xAA;
	TX1_Buf[1] = 0xAF;
	TX1_Buf[2] = 0x0C;
	
	memcpy(&TX1_Buf[3],&RC_DATA,10);
}





void Data_Count(void)
{
	
}




