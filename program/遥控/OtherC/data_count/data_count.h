#ifndef __DATA_COUNT_H
#define __DATA_COUNT_H
#include "includes.h"


typedef struct
{
	u16 AD[4];
	
	u8 KEY[8];

}_RC;


extern _RC RC;


#define ABS(X)	(X<0 ? -X : X)

#define KEY_L_U		0	/* ���� */
#define KEY_L_D		1	/* ���� */
#define KEY_L_L		2	/* ���� */
#define KEY_L_R		3	/* ���� */
                                
#define KEY_R_U		4	/* ���� */
#define KEY_R_D		5	/* ���� */
#define KEY_R_L		6	/* ���� */
#define KEY_R_R		7	/* ���� */

#define KEY_L_U_PRESS		GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_4) == RESET	/* ���� */
#define KEY_L_D_PRESS		GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_7) == RESET	/* ���� */
#define KEY_L_L_PRESS		GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_5) == RESET	/* ���� */
#define KEY_L_R_PRESS		GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_6) == RESET	/* ���� */
                                                                                        
#define KEY_R_U_PRESS		GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_9) == RESET	/* ���� */
#define KEY_R_D_PRESS		GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_6) == RESET	/* ���� */
#define KEY_R_L_PRESS		GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_8) == RESET	/* ���� */
#define KEY_R_R_PRESS		GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_7) == RESET	/* ���� */


void Data_Prepare(void);
void Data_Get(void);


#endif


