/**
  *****************************************************************************
  * @file				oled.h
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/



#ifndef __OLED_H
#define __OLED_H
#include "sys.h"



/*��SSD1106д��һ���ֽ�*/	
void OLED_WR_Byte(u8 data,u8 cmd);
/*���õ�ַ*/
void OLED_Set_Pos(u8 x, u8 y);
/*����OLED��ʾ*/    
void oled_on(void);
/*�ر�OLED��ʾ*/     
void oled_Off(void);
/*����*/
void oled_full(void);
/*����*/
void oled_clear(void); 
/*��ҳ*/
void oled_clear_page(u8 first_page, u8 num);
/*��ֲ�*/
void oled_clear_part(u8 x1, u8 y1, u8 x2, u8 y2);
/*��ʾ�ַ�-16��8*/
void oled_show_char(u8 x,u8 y,u8 _char);
/*��ʾ�ַ���-16��8*/
void oled_show_string(u8 x,u8 y,u8 *string);
/*������ʾ*/
void oled_show_num(u16 x, u16 y, long double num, u8 reserve);
/*����32*32*/
void oled_show_chinese_32(u8 x,u8 y,u8 numble);
/*ͼƬ-128*64*/
void oled_show_picture(u8 *picture);
/*oled��ʼ��*/
void oled_Init(void);
void Oled_Init_Show(void);
void Oled_Refresh(void);



#define 	OLED_CS			GPIO_Pin_6
#define 	OLED_DC			GPIO_Pin_5
#define 	OLED_RES		GPIO_Pin_4
#define 	OLED_SDA		GPIO_Pin_3
#define 	OLED_SCL		GPIO_Pin_2

#define 	OLED_CS_L 		GPIO_ResetBits(GPIOE,OLED_CS)
#define 	OLED_CS_H 		GPIO_SetBits(GPIOE,OLED_CS)

#define 	OLED_DC_L 		GPIO_ResetBits(GPIOE,OLED_DC)
#define 	OLED_DC_H 		GPIO_SetBits(GPIOE,OLED_DC)

#define 	OLED_RES_L 		GPIO_ResetBits(GPIOE,OLED_RES)
#define 	OLED_RES_H 		GPIO_SetBits(GPIOE,OLED_RES)

#define 	OLED_SDA_L 		GPIO_ResetBits(GPIOE,OLED_SDA)
#define 	OLED_SDA_H 		GPIO_SetBits(GPIOE,OLED_SDA)

#define 	OLED_SCL_L 		GPIO_ResetBits(GPIOE,OLED_SCL)
#define 	OLED_SCL_H 		GPIO_SetBits(GPIOE,OLED_SCL)

#define 	OLED_CMD 		0
#define 	OLED_DATA 		1


#endif



/*end of oled.h*/







