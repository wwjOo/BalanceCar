/**
  *****************************************************************************
  * @file				motor.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/





#include "main.h"



/* 电机状态控制 */
void motor_state_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	/* motor1(E10.11) - 低电平停止 */
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	/* STBY1(E15) - 低电平时电机全部停转 */
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	/* motor2(E12.13) - 低电平停止 */
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	MOTOR_ALL_ON;
}


/* 电机说明:最佳频率10KHz,PWM为0停转,越大转速越快 */
/* 电机pwm控制 TIM1_1,4 - PE9,14  */
void motor_Init(u16 arr,u16 pre)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	TIM_TimeBaseInitStruct.TIM_Period=arr-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=pre-1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=0;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	TIM_Cmd(TIM1,ENABLE);

	motor_state_Init();
}






_Motor Motor;

void Motor_Ctrl(void)
{	
	Motor.out1 = balance.out + speed.out + turn.out;
	Motor.out2 = balance.out + speed.out - turn.out;
	
	if(ABS(imu_data.pit) < STOP_ANGLE && Motor.Switch == 1)
	{
		MOTOR_ALL_ON;
		
		MOTOR1_SPEED = ABS(Motor.out1);
		MOTOR2_SPEED = ABS(Motor.out2);
	
		if(Motor.out1 > 0)
		{
			MOTOR1_R;
		}
		else
		{
			MOTOR1_L;
		}
		
		if(Motor.out2 > 0)
		{
			MOTOR2_L;
		}
		else
		{
			MOTOR2_R;
		}
	}
	else
	{
		MOTOR_ALL_OFF;
	}
}




/* end of motor.c*/

