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



/* ��ȡ��ַָ�������ֵ */
u32 STMFLASH_ReadWord(u32 addr)
{
	/* 32λ��ȡ */
	return *(u32*)addr;
}



/* ��ȡ��ַ�������� */
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




/* ��flash */
void STMFLASH_Read(u32 ReadAddr, u32 *Buffer, u32 NumToRead)
{
	u8 i;
	
	for(i=0; i<NumToRead; i++)
	{
		Buffer[i] = STMFLASH_ReadWord(ReadAddr);
		ReadAddr += 4; /* ��ַ����4byte */
	}
}




/* дflash */
void STMFLASH_Write(u32 WriteAddr, u32 *Buffer, u32 NumToWrite)
{ 
	/* FLASH Status  */
	FLASH_Status status = FLASH_COMPLETE;
	
	u32 addr_head=0;
	u32 addr_end =0;
	
	if(WriteAddr<STM32_FLASH_BASE || WriteAddr%4) /* 4�ı��� */
	{
		return;
	}
	
	/* FLASH_Unlock */
	FLASH_Unlock(); 
	
	/* �ر����ݻ��� */
	FLASH_DataCacheCmd(DISABLE); 
 		
	addr_head = WriteAddr;
	addr_end = WriteAddr + NumToWrite*4;
	
	if(addr_head < 0X1FFF0000) /* ֻ�����洢��,����Ҫִ�в�������!! */
	{
		while(addr_head < addr_end)
		{
			if(STMFLASH_ReadWord(addr_head) != 0XFFFFFFFF) /* �Է�0xFFFF FFFF�ĵط�,Ҫ�Ȳ��� */
			{   
				status = FLASH_EraseSector(STMFLASH_GetFlashSector(addr_head), VoltageRange_3);
				
				if(status != FLASH_COMPLETE)
				{
					break; /* �������� */
				}
			}
			else
			{
				addr_head += 4; /* ��һ������ */
			}
		}
	}
	
	if(status == FLASH_COMPLETE)
	{
		while(WriteAddr < addr_end) /* д������ */
		{
			if(FLASH_ProgramWord(WriteAddr, *Buffer) != FLASH_COMPLETE) /* д���쳣 */
			{ 
				break;
			}
			
			Buffer++;
			WriteAddr += 4;
		} 
	}
	
	/* FLASH�������,�������ݻ��� */
	FLASH_DataCacheCmd(ENABLE);
	
	/* ���� */
	FLASH_Lock();
} 



/* ��ȡУ׼���� */
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


/* GYR����У׼���� */
void Flash_Save_Gyr(_CALIB *save)
{
	STMFLASH_Read(FLASH_SAVE_ADDR, (u32*)flash_buf, ALL);
	
	flash_buf[GX] = save->gyr_offset[X];
	flash_buf[GY] = save->gyr_offset[Y];
	flash_buf[GZ] = save->gyr_offset[Z];
	
	STMFLASH_Write(FLASH_SAVE_ADDR, (u32*)flash_buf, ALL);
}


/* ACC����У׼���� */
void Flash_Save_Acc(_CALIB *save)
{
	STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)flash_buf,ALL);
	
	flash_buf[AX] = save->acc_offset[X];
	flash_buf[AY] = save->acc_offset[Y];
	flash_buf[AZ] = save->acc_offset[Z];
	
	STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)flash_buf,ALL);
}


/* MAG����У׼���� */
void Flash_Save_Mag(_CALIB *save)
{
	STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)flash_buf,ALL);
	
	flash_buf[MX] = save->mag_offset[X];
	flash_buf[MY] = save->mag_offset[Y];
	flash_buf[MZ] = save->mag_offset[Z];
	
	STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)flash_buf,ALL);
}





/* end of stmflash.c */



