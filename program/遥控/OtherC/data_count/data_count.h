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

#define KEY_L_U		0	/* ×óÉÏ */
#define KEY_L_D		1	/* ×óÏÂ */
#define KEY_L_L		2	/* ×ó×ó */
#define KEY_L_R		3	/* ×óÓÒ */
                                
#define KEY_R_U		4	/* ÓÒÉÏ */
#define KEY_R_D		5	/* ÓÒÏÂ */
#define KEY_R_L		6	/* ÓÒ×ó */
#define KEY_R_R		7	/* ÓÒÓÒ */

#define KEY_L_U_PRESS		GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_4) == RESET	/* ×óÉÏ */
#define KEY_L_D_PRESS		GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_7) == RESET	/* ×óÏÂ */
#define KEY_L_L_PRESS		GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_5) == RESET	/* ×ó×ó */
#define KEY_L_R_PRESS		GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_6) == RESET	/* ×óÓÒ */
                                                                                        
#define KEY_R_U_PRESS		GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_9) == RESET	/* ÓÒÉÏ */
#define KEY_R_D_PRESS		GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_6) == RESET	/* ÓÒÏÂ */
#define KEY_R_L_PRESS		GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_8) == RESET	/* ÓÒ×ó */
#define KEY_R_R_PRESS		GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_7) == RESET	/* ÓÒÓÒ */


void Data_Prepare(void);
void Data_Get(void);


#endif


