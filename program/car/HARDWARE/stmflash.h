/**
  *****************************************************************************
  * @file				stmflash.h
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/





#ifndef __STMFLASH_H
#define __STMFLASH_H
#include "sys.h"
#include "structural.h"



/* 主存储器基地址*/
#define STM32_FLASH_BASE 	0x08000000 

/* 设置要存储数据的基地址 - 扇区11基地址(0x080E 0000---0x080F FFFF   共128字节)*/
#define FLASH_SAVE_ADDR		0x080E0000

/* stm32f40x/f41x各扇区基地址 */					  
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) /* 16  Kbytes */ 
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) /* 16  Kbytes */ 
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) /* 16  Kbytes */ 
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) /* 16  Kbytes */ 
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) /* 64  Kbytes */ 
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) /* 128 Kbytes */ 
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) /* 128 Kbytes */ 
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) /* 128 Kbytes */ 
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) /* 128 Kbytes */ 
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) /* 128 Kbytes */ 
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) /* 128 Kbytes */ 
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) /* 128 Kbytes */ 




enum
{
	GX=0,GY, GZ,
	AX,	 AY, AZ,
	MX,	 MY, MZ,

	ALL,
};



void Flash_Read(void);
void Flash_Save_Gyr(_CALIB *save);
void Flash_Save_Acc(_CALIB *save);
void Flash_Save_Mag(_CALIB *save);


extern float flash_buf[ALL];


#endif



/* end of stmflash.h */


