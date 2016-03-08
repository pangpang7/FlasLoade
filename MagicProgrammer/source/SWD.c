#include "hw_config.h"
#include "sw_lib.h"
#include "SWD.h"
#include "SI6600BL.h"
#include "target_struct.h"
#include "swd_host.h"
#include "target_flash.h"
#include <stdint.h>
#include "OfflineDownloadCtrl.h"
#include "SPI.h"

bool SWD_pwd_authed;
bool SWDIsConnected;

//static const TARGET_FLASH* flash;

bool SWDInit(void)
{
	SWD_pwd_authed=FALSE;
	SWDIsConnected=FALSE;
	return TRUE;
}

bool SWD_MassEraseCmd(void)
{
	if (!target_flash_init(0))
		return FALSE;
	if(!target_flash_erase_chip())
		return FALSE;
	return TRUE;
}

bool SWD_Connect(void)
{
	//setup pin to SPI pin
	STC_IO_Config_Used_for_STC();
	//test connection
	if(!SPIConnectionTest(TRUE))
	{
		STC_IO_Config_Unused();
		return FALSE;
	}
	else
	{
		//swd init
		SWDInit();
		SWDIsConnected=TRUE;
		return TRUE;
	}
}

bool SWD_DisConnect(void)
{
	SWDIsConnected=FALSE;
	STC_IO_Config_Unused();
	return TRUE;
}

bool SWD_Write(u32 addr, u16 lg, u8* data, bool MainOrInfo)
{
	bool Res;
	if(((addr>=0x00000000)&&(addr<0x20000000))||((addr>=0xA0000000)&&(addr<0xE0000000)))
		Res=(bool)target_flash_program_page(addr,data,lg);
	else
		Res=(bool)swd_write_memory(addr,data,lg);
	return Res;
}

bool SWD_Read(u32 addr, u16 lg, u8* data, bool MainOrInfo)
{
	if(!swd_read_memory(addr,data,lg))
		return FALSE;
	else
		return TRUE;
}

bool SWD_PWDPageEarse(void)
{
	u32 tmp[2];
	//pwd page earse
	if(!target_flash_erase_sector(INFO_PAGE_1_READDR))
		return FALSE;
	//write a5a5 flag to the last 8 bytes of the pwd page earse
	tmp[0]=0x5a5a5a5a;
	tmp[1]=0xa5a5a5a5;
	if(!target_flash_program_page(0x7f8,(u8 *)tmp,8))
		return FALSE;

	//verify a5a5 flag
	if(!swd_read_memory(0x7f8,(u8 *)tmp,8))
		return FALSE;

	if((tmp[0]==0x5a5a5a5a)&&(tmp[1]==0xa5a5a5a5))
		return TRUE;
	else 
		return FALSE;
}

bool SWD_Restore(void)
{
	if(!SWD_MassEraseCmd())
		return FALSE;
	if(!SWD_PWDPageEarse())
		return FALSE;
	SWD_pwd_authed=FALSE;
	return TRUE;
}

bool SWD_WritePWD(u8* data,bool RestoreOrNot)
{
	//byte 0 - 3: max_quth_times
	//byte 4 - 19: pwd
	//byte 20 - 51: flash lock bit
	//byte 52 - 67: flash bus key
	//byte 68 - 71: flash bus key enable
	u8 tmp[4];
	//pwd page earse
	if(!SWD_Restore())
		return FALSE;
	//check flash bus key , if all FFs, replace it with MCU chip id
	if(memcmpfixed(&data[52],0xFF,16))
	{
		SWD_GetChipID(&data[52]);
	}
	//write all kinds of password
	if(!target_flash_program_page(PWD_MAX_AUTH_OFFSET,&data[0],PWD_MAX_AUTH_LEN))
		return FALSE;

	if(!target_flash_program_page(PWD_INFO_OFFSET,&data[4],PWD_LEN))
		return FALSE;

	if(!target_flash_program_page(FLASH_LOCK_BIT_OFFSET,&data[20],FLASH_LOCK_BIT_LEN))
		return FALSE;

	if(!target_flash_program_page(FLASH_BUS_KEY_OFFSET,&data[52],FLASH_BUS_KEY_LEN))
		return FALSE;

	if(!target_flash_program_page(FLASH_BUS_KEY_EN_OFFSET,&data[68],FLASH_BUS_KEY_EN_LEN))
		return FALSE;
	
	//write pwd flag
	tmp[0]=FISH_MAN;
	if(!target_flash_program_page(PWD_FLAG_OFFSET,tmp,PWD_FLAG_LEN))
		return FALSE;

	SWD_pwd_authed=TRUE;
	return TRUE;
}

bool SWD_AUTHPWD(u8* pwd)
{
	if(!SPIPassword_Write(pwd))
		return FALSE;
	if(!swd_set_target_state(RESET_PROGRAM))
		return FALSE;
	SWD_pwd_authed=TRUE;
	return TRUE;
}

bool SWD_GetPwdAuthed(void)
{
	return SWD_pwd_authed;
}

bool SWD_CheckPWDValid(void)
{
	if(swd_set_target_state(RESET_PROGRAM))
	{
		SWD_pwd_authed=TRUE;
		return FALSE;
	}
	else
		return TRUE;
}

bool SWD_GetMAXAUTHTimes(u8* max_auth_times)
{
	//always equal zero in SWD mode
	*max_auth_times=0;
	return TRUE;
}

bool SWD_RemovePWD(void)
{
	if(!SWD_Restore())
		return FALSE;
	else
		return TRUE;
}

bool SWD_GetPID(u8* pid)
{
	u8 tmp[4];
	//read PID
	if(!swd_read_memory(MCU_PID_OFFSET,(u8 *)tmp,MCU_PID_LEN))
		return FALSE;

	pid[0]=tmp[0];
	pid[1]=tmp[1];
	return TRUE;
}

bool SWD_GetChipVersion(u8* version)
{
	u8 tmp[4];
	//read VERSION
	if(!swd_read_memory(MCU_REVISION_OFFSET,(u8 *)tmp,MCU_REVISION_LEN))
		return FALSE;

	*version=tmp[3];
	return TRUE;
}

bool SWD_GetChipID(u8* ChipID)
{
	//read chip id
	if(!swd_read_memory(MCU_CHIP_ID_OFFSET,(u8 *)ChipID,MCU_CHIP_ID_LEN))
		return FALSE;
	else
		return TRUE;
}

bool SWD_GetFlashLockBit(u8* FlashLock)
{
	//read flash lock bit
	if(!swd_read_memory(FLASH_LOCK_BIT_OFFSET,(u8 *)FlashLock,FLASH_LOCK_BIT_LEN))
		return FALSE;
	else
		return TRUE;
}

bool SWD_GoUserApp(u32 addr)
{
	return FALSE;
}
