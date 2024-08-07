/**
  *****************************************************************************
  * @file				oled.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/



#include "main.h"
#include "oled_font.h"

																											

/*��SSD1106д��һ���ֽ�*/	
void OLED_WR_Byte(u8 data,u8 cmd)
{	
	u8 i;			  
	if(cmd)	/*����*/
	{
		OLED_DC_H;
	}
	else	/*����*/
	{		
		OLED_DC_L;		  
	}
	
	OLED_CS_L;	/*Ƭѡ����*/
	
	for(i=0;i<8;i++)
	{			  
		OLED_SCL_L;
		if(data&0x80)
		{
			OLED_SDA_H;
		}
		else
		{			
			OLED_SDA_L;
		}
		OLED_SCL_H;
		data<<=1;   
	}
	
	OLED_CS_H;
	OLED_DC_H;  
}


/*���õ�ַ*/
void OLED_Set_Pos(u8 x, u8 y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);      		/*����ҳ��ַ(0~7)*/
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);  /*������ʾλ�á��иߵ�ַ*/   
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD);   	/*������ʾλ�á��е͵�ַ*/
}  


/*����OLED��ʾ*/    
void oled_on(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  /*SET DCDC����*/
	OLED_WR_Byte(0X14,OLED_CMD);  /*DCDC ON*/
	OLED_WR_Byte(0XAF,OLED_CMD);  /*DISPLAY ON*/
}


/*�ر�OLED��ʾ*/     
void oled_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  /*SET DCDC����*/
	OLED_WR_Byte(0X10,OLED_CMD);  /*DCDC OFF*/
	OLED_WR_Byte(0XAE,OLED_CMD);  /*DISPLAY OFF*/
}	


/*����*/
void oled_full(void)
{
	u8 i,j;
	for(i=0; i<8; i++)
	{
		OLED_Set_Pos(0,0+i);
		for(j=0;j<127;j++)	/*x��ַ����*/
		{ 
			OLED_WR_Byte(0xff,OLED_DATA); /*0xff�����������8��*/
			#if 0	
				delay_ms(500);	/*ͨ����ʱ�鿴��ʾ����*/
			#endif
		}
	}
}


/*����*/
void oled_clear(void)  
{ 
	u8 i,j;	
	for(i=0; i<8; i++)  
	{  
		OLED_WR_Byte(0xb0+i,OLED_CMD);	/*����ҳ��ַ(0~7)*/
		OLED_WR_Byte(0x00,OLED_CMD);  	/*������ʾλ�á��е͵�ַ*/
		OLED_WR_Byte(0x10,OLED_CMD);  	/*������ʾλ�á��иߵ�ַ*/ 
		for(j=0; j<128; j++)
		{
			OLED_WR_Byte(0,OLED_DATA); 	/*д0����*/
		}
	}
}


/*��ҳ*/
void oled_clear_page(u8 first_page, u8 num)
{
	u8 i,j;
	
	for(i=first_page; i<first_page+num; i++)
	{
		OLED_Set_Pos(0,i);
		for(j=0; j<128; j++)
		{
			OLED_WR_Byte(0,OLED_DATA);
		}
	}
}


/*��ֲ�*/
void oled_clear_part(u8 x1, u8 y1, u8 x2, u8 y2)
{
	u8 i,j,x0,y0,temp;
	
	if(x1>x2)
	{
		temp = x2;
		x2 = x1;
		x1 = temp;
	}
	if(y1>y2)
	{
		temp = y2;
		y2 = y1;
		y1 = temp;
	}
	
	x0=x2-x1;
	y0=y2-y1;
	
	for(i=0; i<y0; i++)
	{
		for(j=0; j<x0; j++)
		{
			OLED_Set_Pos(x1+j,y1+i);
			for(j=0; j<x0; j++)	/*x��ַ����*/
			{
				OLED_WR_Byte(0x00,OLED_DATA);
				#if 0	
					delay_ms(500);	/*ͨ����ʱ�鿴��ʾ����*/
				#endif
			}			
		}
	}
}


/*��ʾ�ַ�-16��8*/
void oled_show_char(u8 x,u8 y,u8 _char)
{
	u8 i,c;
	c=_char-' '; /*�õ�ASCII��*/
	
	OLED_Set_Pos(x,y); /*��һҳ*/
	for(i=0; i<8; i++)
	{
		OLED_WR_Byte(ASCII_16_8[c*16+i],OLED_DATA); /*ɨ��ǰ8λ*/
	}
	
	OLED_Set_Pos(x,y+1); /*�ڶ�ҳ*/
	for(i=0; i<8; i++)		
	{	
		OLED_WR_Byte(ASCII_16_8[c*16+i+8],OLED_DATA); /*ɨ���8λ*/
	}
}


/*��ʾ�ַ���-16��8*/
#define LINE_FEED	0 /* 1Ϊ�Զ�����,0Ϊ������ */
void oled_show_string(u8 x,u8 y,u8 *string)
{
	u8 j=0;
	while(string[j] != '\0') /*�ַ��������Ϊ������'\0'*/
	{
		oled_show_char(x,y,string[j]);
		x+=8; /*������*/
		j++;
		
		if(x>120)
		{ 
			if(!LINE_FEED)
				break;
			else
			{
				x=0;
				y+=2;
			}
		}
	}
}


/*������ʾ*/
void oled_show_num(u16 x, u16 y, long double num, u8 reserve)
{
	u8 temp[20]={0},i=0,temp_i,temp1[10]={0},j=0,temp_j,k;	
	long double temp_num;
	long znum,xnum,znum_temp;
	
	temp_num = num; /*��������*/
	if(num<0) /*����ȡ��*/
	{
		num = 0-num;
	}
	if(reserve>9)/*����λ������*/
	{
		reserve = 9;
	}
	znum = (u32)num; /*��ȡ��������*/
	znum_temp = znum;
	xnum = (num-znum)*pow(10,reserve); /*��ȡС������,��ʾ�����Դ�ڴ�*/
	do /*�������ִ�������*/
	{
		temp1[j++] = znum%10;
		znum = znum/10;
	}while(znum>0);
	
	if(temp_num<0) /*������Ӹ���*/
	{
		temp1[j++] = (char)('-'-48);
	}
	
	temp_j = j;
	j = j-1;
	
	do /*��ת����*/
	{
		temp[i++] = temp1[j--]+48; /*'0'��ASCIIΪ48*/
	}while(i<temp_j);
	
	if(reserve != 0)
	{
		j = 0; /*temp1�������*/

		if(xnum == 0)
		{
			for(k=0; k<reserve; k++)
			{
				temp1[j++] = 0;
			}
		}
		else
		{ 
			do /*С�����ִ�������*/
			{
				temp1[j++] = xnum%10;
				xnum = xnum/10;
			}while(xnum>0);
			
			k = 1;
			
			while(k)
			{
				if((u16)((num-znum_temp)*pow(10,k)) == 0)
				{
					k++;
					temp1[j++] = 0;
				}
				else
				{
					k = 0;
				}
			}
		}
		temp1[j++] = (char)('.'-48); /*����С����*/
		
		temp_j = j;
		temp_i = i+temp_j;
		j=j-1;
		
		do /*��ת����*/
		{
			temp[i++] = temp1[j--]+48;
		}while(i<temp_i);
	}

	oled_show_string(x,y,temp); /*��ʾ*/
}


/*����32*32*/
void oled_show_chinese_32(u8 x,u8 y,u8 numble)
{
	u8 i,j;
	for(i=0; i<4; i++)
	{
		OLED_Set_Pos(x,y+i);
		for(j=0; j<32; j++)
		{
			OLED_WR_Byte(CH_32[4*numble+i][j],OLED_DATA);
			#if 0
				delay_ms(50);
			#endif
		}
	}
}


/*ͼƬ-128*64*/
void oled_show_picture(u8 *picture)
{
	u8 i,j;
	for(i=0; i<8; i++)
	{
		OLED_Set_Pos(0,i);
		for(j=0; j<128; j++)
		{
			OLED_WR_Byte(*picture,OLED_DATA);
			picture++;
			#if 0
				delay_ms(10);
			#endif
		}
	}
}


/*oled��ʼ��*/
void oled_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin=OLED_CS|OLED_DC|OLED_RES|OLED_SDA|OLED_SCL;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	
	GPIO_SetBits(GPIOE,OLED_CS|OLED_DC|OLED_RES|OLED_SDA|OLED_SCL);
	
	OLED_RES_H;
	delay_ms(100);
	OLED_RES_L;
	delay_ms(100);
	OLED_RES_H;
	delay_ms(100);
	
	OLED_WR_Byte(0xAE,OLED_CMD);
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0x10,OLED_CMD);
	OLED_WR_Byte(0x40,OLED_CMD);
	OLED_WR_Byte(0x81,OLED_CMD);
	OLED_WR_Byte(0xCF,OLED_CMD);
	OLED_WR_Byte(0xA1,OLED_CMD); /*0xA0���ҷ��� 0xA1����*/
	OLED_WR_Byte(0xC8,OLED_CMD); /*0xC0���·��� 0xC8����*/
	OLED_WR_Byte(0xA6,OLED_CMD);
	OLED_WR_Byte(0xA8,OLED_CMD);
	OLED_WR_Byte(0x3f,OLED_CMD);
	OLED_WR_Byte(0xD3,OLED_CMD);
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0xd5,OLED_CMD);
	OLED_WR_Byte(0x80,OLED_CMD);
	OLED_WR_Byte(0xD9,OLED_CMD);
	OLED_WR_Byte(0xF1,OLED_CMD);
	OLED_WR_Byte(0xDA,OLED_CMD);
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);
	OLED_WR_Byte(0x40,OLED_CMD);
	OLED_WR_Byte(0x20,OLED_CMD);
	OLED_WR_Byte(0x02,OLED_CMD);
	OLED_WR_Byte(0x8D,OLED_CMD);
	OLED_WR_Byte(0x14,OLED_CMD);
	OLED_WR_Byte(0xA4,OLED_CMD);
	OLED_WR_Byte(0xA6,OLED_CMD);
	OLED_WR_Byte(0xAF,OLED_CMD);
	OLED_WR_Byte(0xAF,OLED_CMD);
	oled_clear();
	OLED_Set_Pos(0,0);
	
//	Oled_Init_Show();
}



void Oled_Init_Show(void)
{
	oled_show_string(30,3,(u8*)"H E L L O");
	delay_ms(2000);
	oled_clear();
}


void Oled_Refresh(void)
{
	static u8 finish=0;
	
	if(finish == 0)
	{
		oled_show_string(0,0,(u8*)"Bp:");
		oled_show_string(0,2,(u8*)"Bd:");
		oled_show_string(0,4,(u8*)"Sp:");
		oled_show_string(0,6,(u8*)"Si:");
		
		oled_show_string(72,0,(u8*)"P=");
		oled_show_string(72,4,(u8*)"V=");
		finish = 1;
	}
	
	
	/*==============ˢ��===============*/
	oled_clear_part(32,0,56,8);
	oled_show_num(24,0,balance.kp,1);
	oled_show_num(24,2,balance.kd,1);
	oled_show_num(24,4,speed.kp,1);
	oled_show_num(24,6,speed.ki,3);
	
	oled_show_num(88,0,imu_data.pit,4);
	
	oled_show_num(88,6,speed.integral,4);
	
	oled_show_num(88,4,Power.Voltage,4);
}


/*end of oled.c*/


