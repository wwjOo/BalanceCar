/**
  *****************************************************************************
  * @file				stmflash.c
  * @author				WWJ
  * @version			v1.0
  * @date				2019/04/09
  * @environment		stm32f407
  * @brief   
  *****************************************************************************
**/





#include "stmflash.h"



/* 读取地址指向的区域值 */
u32 STMFLASH_ReadWord(u32 addr)
{
	/* 32位读取 */
	return *(u32*)addr;
}



/* 获取地址所在扇区 */
u16 STMFLASH_GetFlashSector(u32 addr)
{
		 if(addr < ADDR_FLASH_SECTOR_1)		return FLASH_Sector_0;
	else if(addr < ADDR_FLASH_SECTOR_2)		return FLASH_Sector_1;
	else if(addr < ADDR_FLASH_SECTOR_3)		return FLASH_Sector_2;
	else if(addr < ADDR_FLASH_SECTOR_4)		return FLASH_Sector_3;
	else if(addr < ADDR_FLASH_SECTOR_5)		return FLASH_Sector_4;
	else if(addr < ADDR_FLASH_SECTOR_6)		return FLASH_Sector_5;
	else if(addr < ADDR_FLASH_SECTOR_7)		return FLASH_Sector_6;
	else if(addr < ADDR_FLASH_SECTOR_8)		return FLASH_Sector_7;
	else if(addr < ADDR_FLASH_SECTOR_9)		return FLASH_Sector_8;
	else if(addr < ADDR_FLASH_SECTOR_10)	return FLASH_Sector_9;
	else if(addr < ADDR_FLASH_SECTOR_11)	return FLASH_Sector_10;
	else									return FLASH_Sector_11;	
}




/* 读flash */
void STMFLASH_Read(u32 ReadAddr, u32 *Buffer, u32 NumToRead)
{
	u8 i;
	
	for(i=0; i<NumToRead; i++)
	{
		Buffer[i] = STMFLASH_ReadWord(ReadAddr);
		ReadAddr += 4; /* 地址增加4byte */
	}
}




/* 写flash */
void STMFLASH_Write(u32 WriteAddr, u32 *Buffer, u32 NumToWrite)
{ 
	/* FLASH Status  */
	FLASH_Status status = FLASH_COMPLETE;
	
	u32 addr_head=0;
	u32 addr_end =0;
	
	if(WriteAddr<STM32_FLASH_BASE || WriteAddr%4) /* 4的倍数 */
	{
		return;
	}
	
	/* FLASH_Unlock */
	FLASH_Unlock(); 
	
	/* 关闭数据缓存 */
	FLASH_DataCacheCmd(DISABLE); 
 		
	addr_head = WriteAddr;
	addr_end = WriteAddr + NumToWrite*4;
	
	if(addr_head < 0X1FFF0000) /* 只有主存储区,才需要执行擦除操作!! */
	{
		while(addr_head < addr_end)
		{
			if(STMFLASH_ReadWord(addr_head) != 0XFFFFFFFF) /* 对非0xFFFF FFFF的地方,要先擦除 */
			{   
				status = FLASH_EraseSector(STMFLASH_GetFlashSector(addr_head), VoltageRange_3);
				
				if(status != FLASH_COMPLETE)
				{
					break; /* 擦除错误 */
				}
			}
			else
			{
				addr_head += 4; /* 下一个区域 */
			}
		}
	}
	
	if(status == FLASH_COMPLETE)
	{
		while(WriteAddr < addr_end) /* 写入数据 */
		{
			if(FLASH_ProgramWord(WriteAddr, *Buffer) != FLASH_COMPLETE) /* 写入异常 */
			{ 
				break;
			}
			
			Buffer++;
			WriteAddr += 4;
		} 
	}
	
	/* FLASH操作完毕,开启数据缓存 */
	FLASH_DataCacheCmd(ENABLE);
	
	/* 上锁 */
	FLASH_Lock();
} 



/* 读取校准数据 */
float flash_buf[ALL];
void Flash_Read(void)
{
	STMFLASH_Read(FLASH_SAVE_ADDR, (u32*)flash_buf, ALL);
	
	calib.acc_offset[X] = flash_buf[AX];
	calib.acc_offset[Y] = flash_buf[AY];
	calib.acc_offset[Z] = flash_buf[AZ];
	
	calib.gyr_offset[X] = flash_buf[GX];
	calib.gyr_offset[Y] = flash_buf[GY];
	calib.gyr_offset[Z] = flash_buf[GZ];
	
	calib.mag_offset[X] = flash_buf[MX];
	calib.mag_offset[Y] = flash_buf[MY];
	calib.mag_offset[Z] = flash_buf[MZ];
}


/* GYR保存校准数据 */
void Flash_Save_Gyr(_CALIB *save)
{
	STMFLASH_Read(FLASH_SAVE_ADDR, (u32*)flash_buf, ALL);
	
	flash_buf[GX] = save->gyr_offset[X];
	flash_buf[GY] = save->gyr_offset[Y];
	flash_buf[GZ] = save->gyr_offset[Z];
	
	STMFLASH_Write(FLASH_SAVE_ADDR, (u32*)flash_buf, ALL);
}


/* ACC保存校准数据 */
void Flash_Save_Acc(_CALIB *save)
{
	STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)flash_buf,ALL);
	
	flash_buf[AX] = save->acc_offset[X];
	flash_buf[AY] = save->acc_offset[Y];
	flash_buf[AZ] = save->acc_offset[Z];
	
	STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)flash_buf,ALL);
}


/* MAG保存校准数据 */
void Flash_Save_Mag(_CALIB *save)
{
	STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)flash_buf,ALL);
	
	flash_buf[MX] = save->mag_offset[X];
	flash_buf[MY] = save->mag_offset[Y];
	flash_buf[MZ] = save->mag_offset[Z];
	
	STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)flash_buf,ALL);
}





/* end of stmflash.c */



