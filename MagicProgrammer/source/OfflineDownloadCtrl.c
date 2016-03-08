#include "hw_config.h"
#include "OfflineDownloadCtrl.h"
#include "usb_type.h"
#include "BLSlave.h"

#include "sw_lib.h"
#include "STC.h"
#include "SI6600BL.h"
#include "UART.h"
#include "SWD.h"

const MCU_CFG* pmcu_cfg;
MCU_CFG mcu_scfg;
u8 RQ_Buffer[384];
u8 RD_Buffer[2048];
bool pwd_authed;

LPMCU_CFG GetMCUCFG(void)
{
	return &mcu_scfg;
}

void EarseAndResetAll(void)
{
	u32 i;
	memset((u8 *)&mcu_scfg,0,sizeof(MCU_CFG));
	for(i=0;i<32;i++)
		mcu_scfg.FlashLock[i]=0xFF;
	mcu_scfg.flag=FISH_MAN;
	mcu_scfg.checksum=checksum((u8 *)&mcu_scfg,sizeof(MCU_CFG)-1);
	OfflineEarseMCURAM();
	OfflineEarseMCUFLASH();
	FLASH_Earse(MCU_CFG_BASE,MCU_CFG_BASE+sizeof(mcu_scfg)-1);
	FLASH_Program(MCU_CFG_BASE,(u8 *)&mcu_scfg,sizeof(mcu_scfg));
}

void OfflineInit(void)
{
	
	pmcu_cfg=(const MCU_CFG*)MCU_CFG_BASE;
	if((checksum((u8 *)pmcu_cfg,sizeof(MCU_CFG)))||(pmcu_cfg->flag != FISH_MAN))
	{
		EarseAndResetAll();
	}
	else
	{
		memcpy((u8 *)&mcu_scfg,(const u8 *)pmcu_cfg,sizeof(MCU_CFG));
	}
	//always set false at POR or other reset
	pwd_authed=FALSE;
	//set offline download result LED off
	GPIO_SetBits(GPIOE, GPIO_Pin_5);
	//for test
	//mcu_scfg.flash_offset=0x8040000;
}

bool SetRamDone(bool done)
{ 
	if((done!=mcu_scfg.ram_done))
	{
		mcu_scfg.ram_done=done;
		mcu_scfg.checksum=checksum((u8 *)&mcu_scfg,sizeof(MCU_CFG)-1);
		if(!mcu_scfg.ram_done)
			OfflineEarseMCURAM();
		if(!FLASH_Earse(MCU_CFG_BASE,MCU_CFG_BASE+sizeof(mcu_scfg)-1))
			return FALSE;
		if(!FLASH_Program(MCU_CFG_BASE,(u8 *)&mcu_scfg,sizeof(mcu_scfg)))
			return FALSE;	
	}
	return TRUE;	
}

bool SetPwd(u8* data)
{
	memcpy(mcu_scfg.max_auth_num,data,ALL_PWD_LEN-PWD_FLAG_LEN);
	mcu_scfg.pwdflag[0]=FISH_MAN;
	mcu_scfg.checksum=checksum((u8 *)&mcu_scfg,sizeof(MCU_CFG)-1);
	if(!FLASH_Earse(MCU_CFG_BASE,MCU_CFG_BASE+sizeof(mcu_scfg)-1))
		return FALSE;
	if(!FLASH_Program(MCU_CFG_BASE,(u8 *)&mcu_scfg,sizeof(mcu_scfg)))
		return FALSE;
	pwd_authed=TRUE;
	return TRUE;	
}

bool AuthPwd(u8* pwd)
{
	int i;
	for(i=0;i<16;i++)
	{
		if(mcu_scfg.pwd[i]!=pwd[i])
		{
			if(mcu_scfg.max_auth_num[0]!=0)
			{
				mcu_scfg.max_auth_num[0]--;
				if(mcu_scfg.max_auth_num[0]!=0)
				{
					mcu_scfg.checksum=checksum((u8 *)&mcu_scfg,sizeof(MCU_CFG)-1);
					if(!FLASH_Earse(MCU_CFG_BASE,MCU_CFG_BASE+sizeof(mcu_scfg)-1))
						return FALSE;
					if(!FLASH_Program(MCU_CFG_BASE,(u8 *)&mcu_scfg,sizeof(mcu_scfg)))
						return FALSE;
				}
				else
					EarseAndResetAll();
			}
			return FALSE;
		}
	}
	pwd_authed=TRUE;
	return TRUE;
}

bool GetPwdAuthed(void)
{
	return pwd_authed;
}

bool GetPwdValidation(void)
{
	if(mcu_scfg.pwdflag[0]==FISH_MAN)
		return TRUE;
	else
		return FALSE;
}

u8 GetMaxAuthNum(void)
{
	return mcu_scfg.max_auth_num[0];
}

bool RemovePwd(void)
{
	EarseAndResetAll();
	return TRUE;
}

bool SetFlashDone(bool done)
{
	if((done!=mcu_scfg.flash_done))
	{
		mcu_scfg.flash_done=done;
		mcu_scfg.checksum=checksum((u8 *)&mcu_scfg,sizeof(MCU_CFG)-1);
		if(!mcu_scfg.flash_done)
			OfflineEarseMCUFLASH();
		if(!FLASH_Earse(MCU_CFG_BASE,MCU_CFG_BASE+sizeof(mcu_scfg)-1))
			return FALSE;
		if(!FLASH_Program(MCU_CFG_BASE,(u8 *)&mcu_scfg,sizeof(mcu_scfg)))
			return FALSE;	
	}
	return TRUE;	
}

bool OfflineEarseMCURAM(void)
{
	memset(mcu_scfg.ram_map,0,MCU_RAM_PAGES);
	return FLASH_Earse(MCU_RAM_BASE,MCU_RAM_BASE+MCU_RAM_SIZE-1);
}

bool OfflineEarseMCUFLASH(void)
{
	memset(mcu_scfg.flash_map,0,MCU_FLASH_PAGES);
	return FLASH_Earse(MCU_FLASH_BASE,MCU_FLASH_BASE+MCU_RAM_SIZE-1);	
}

bool OfflineProgramMCURAM(u32 StartAddr,u8 * data, u32 lg)
{
	u32 ReAddr=StartAddr+mcu_scfg.flash_offset;
	if(mcu_scfg.ram_done)
		return FALSE;
	if(((ReAddr<MCU_RAM_BASE)||(ReAddr>=MCU_RAM_BASE+MCU_RAM_SIZE))
		||((ReAddr+lg-1<MCU_RAM_BASE)||(ReAddr+lg-1>=MCU_RAM_BASE+MCU_RAM_SIZE)))
		return FALSE;
	if(!FLASH_Program(ReAddr,data,lg))
		return FALSE;
	mcu_scfg.ram_map[(ReAddr-MCU_RAM_BASE)/PRO_PAGE_SIZE]=TRUE;
	mcu_scfg.ram_map[(ReAddr+lg-1-MCU_RAM_BASE)/PRO_PAGE_SIZE]=TRUE;
	return TRUE;
}

bool OfflineProgramMCUFLASH(u32 StartAddr,u8 * data, u32 lg)
{
	u32 ReAddr=StartAddr+mcu_scfg.flash_offset;
	if(mcu_scfg.flash_done)
		return FALSE;
	if(((ReAddr<MCU_FLASH_BASE)||(ReAddr>=MCU_FLASH_BASE+MCU_FLASH_SIZE))
		||((ReAddr+lg-1<MCU_FLASH_BASE)||(ReAddr+lg-1>=MCU_FLASH_BASE+MCU_FLASH_SIZE)))
		return FALSE;
	if(!FLASH_Program(ReAddr,data,lg))
		return FALSE;
	mcu_scfg.flash_map[(ReAddr-MCU_FLASH_BASE)/PRO_PAGE_SIZE]=TRUE;
	mcu_scfg.flash_map[(ReAddr+lg-1-MCU_FLASH_BASE)/PRO_PAGE_SIZE]=TRUE;

	if (mcu_scfg.flash_map[127]==TRUE)
	{
		return TRUE;
	}
	return TRUE;
}

bool OfflineReadMCURAM(u32 StartAddr,u8 * data, u32 lg)
{
	memcpy(data,(u8 *)(StartAddr+mcu_scfg.ram_offset),lg);
	return TRUE;
}

bool OfflineReadMCUFLASH(u32 StartAddr,u8 * data, u32 lg)
{
	memcpy(data,(u8 *)(StartAddr+mcu_scfg.flash_offset),lg);
	return TRUE;
}

void OFFLINE_GetPID(u8* pid)
{
	pid[0]=mcu_scfg.MCUPID[0];
	pid[1]=mcu_scfg.MCUPID[1];
}

void OFFLINE_GetChipVersion(u8* version)
{
    *version=0xFF;	
}

void OFFLINE_GetChipID(u8* ChipID)
{
	memset(ChipID,0,8);
}

void OFFLINE_GetFlashLockBit(u8* FlashLock)
{
	memcpy(FlashLock,mcu_scfg.FlashLock,32);
}


bool OfflineDownloadwithUART(void)
{
	u32 i,j;
	//u8 tmp[2];
	if(!mcu_scfg.flash_done) // no user FW for MCU
		return FALSE;

	//connect
	if(!UART_Connect())
		return FALSE;
		
	//check pid
	/*if(!UART_GetPID(tmp))
		return FALSE;
	if(memcmp(tmp,mcu_scfg.MCUPID,2)!=0)
		return FALSE;*/
	
	//check boot loader version
	
	//still not real boot loader version

	//remove password
  	if(!UART_RemovePWD())
		return FALSE;
	//write password or not
	if(mcu_scfg.pwdflag[0]==FISH_MAN)
	{
		if(!UART_WritePWD(mcu_scfg.max_auth_num,TRUE))
			return FALSE;
	}

	//download user FW
	for(i=0;i<MCU_FLASH_PAGES;i++)
	{
		if(mcu_scfg.flash_map[i])
		{
			for(j=0;j<PRO_PAGE_SIZE/MAX_DATA_SIZE;j++)
			{
				if(!UART_Write(MCU_FLASH_BASE+i*PRO_PAGE_SIZE+j*MAX_DATA_SIZE-mcu_scfg.flash_offset, 
						MAX_DATA_SIZE, 
						(u8*)(MCU_FLASH_BASE+i*PRO_PAGE_SIZE+j*MAX_DATA_SIZE), 
						TRUE))
					return FALSE;
				DelayMs(10);	
			}
		}
	}
	//verify
	for(i=0;i<MCU_FLASH_PAGES;i++)
	{
		if(mcu_scfg.flash_map[i])
		{
			for(j=0;j<PRO_PAGE_SIZE/MAX_DATA_SIZE;j++)
			{
				if(!UART_Read(MCU_FLASH_BASE+i*PRO_PAGE_SIZE+j*MAX_DATA_SIZE-mcu_scfg.flash_offset, 
						MAX_DATA_SIZE, 
						RD_Buffer, 
						TRUE))
					return FALSE;
				if(memcmp(RD_Buffer,(u8*)(MCU_FLASH_BASE+i*PRO_PAGE_SIZE+j*MAX_DATA_SIZE),MAX_DATA_SIZE)!=0)
					return FALSE;
				DelayMs(5);		
			}
		}
	}

	

	//GO usr app
	if(!UART_GoUserApp(0))
		return FALSE;

	return TRUE;
}

bool OfflineDownloadwithSTC(void)
{
	STC_CONFIG tmpcfg;
	u32 i;
	//u8 tmp[2];
	if(!mcu_scfg.flash_done) // no user FW for MCU
		return FALSE;

	//connect 
    STC_Connect();
	//check pid
	/*STC_GetPID(tmp);
	if(memcmp(tmp,mcu_scfg.MCUPID,2)!=0)
		return FALSE;*/

	//write password or remove password
	if(mcu_scfg.pwdflag[0]==FISH_MAN)
	{
		STC_WritePWD(mcu_scfg.max_auth_num);
	}
	else
	{
		STC_RemovePWD();
	}

	//download user FW
	for(i=0;i<MCU_FLASH_PAGES;i++)
	{
		if(mcu_scfg.flash_map[i])
		{
			tmpcfg.MainOrInfo=TRUE;
			tmpcfg.addr=MCU_FLASH_BASE+i*PRO_PAGE_SIZE-mcu_scfg.flash_offset;
			tmpcfg.data=(u8*)(MCU_FLASH_BASE+i*PRO_PAGE_SIZE);
			tmpcfg.cnt=PRO_PAGE_SIZE;
			STC_Write(&tmpcfg);
		}
	}

	//verify
	for(i=0;i<MCU_FLASH_PAGES;i++)
	{
		if(mcu_scfg.flash_map[i])
		{
			tmpcfg.MainOrInfo=TRUE;
			tmpcfg.addr=MCU_FLASH_BASE+i*PRO_PAGE_SIZE-mcu_scfg.flash_offset;
			tmpcfg.data=RD_Buffer;
			tmpcfg.cnt=PRO_PAGE_SIZE;
			STC_Read(&tmpcfg);

			if(memcmp(RD_Buffer,(u8*)(MCU_FLASH_BASE+i*PRO_PAGE_SIZE),PRO_PAGE_SIZE)!=0)
				return FALSE;
		}
	}

	//GO usr app
	STC_DisConnect();

	return TRUE;
}

bool OfflineDownloadwithSWD(void)
{
	u32 i;
	//u8 tmp[2];
	if(!mcu_scfg.flash_done) // no user FW for MCU
		return FALSE;

	//connect 
    SWD_Connect();
	//check pid
	/*SWD_GetPID(tmp);
	if(memcmp(tmp,mcu_scfg.MCUPID,2)!=0)
		return FALSE;*/

	//write password or remove password
	if(mcu_scfg.pwdflag[0]==FISH_MAN)
	{
		SWD_WritePWD(mcu_scfg.max_auth_num,TRUE);
	}
	else
	{
		SWD_RemovePWD();
	}

	//download user FW
	for(i=0;i<MCU_FLASH_PAGES;i++)
	{
		if(mcu_scfg.flash_map[i])
		{
			if(!SWD_Write(MCU_FLASH_BASE+i*PRO_PAGE_SIZE-mcu_scfg.flash_offset, PRO_PAGE_SIZE, (u8*)(MCU_FLASH_BASE+i*PRO_PAGE_SIZE), TRUE))
				return FALSE;
		}
	}

	//verify
	for(i=0;i<MCU_FLASH_PAGES;i++)
	{
		if(mcu_scfg.flash_map[i])
		{
			if(!SWD_Read(MCU_FLASH_BASE+i*PRO_PAGE_SIZE-mcu_scfg.flash_offset,PRO_PAGE_SIZE,RD_Buffer,TRUE))
				return FALSE;

			if(memcmp(RD_Buffer,(u8*)(MCU_FLASH_BASE+i*PRO_PAGE_SIZE),PRO_PAGE_SIZE)!=0)
				return FALSE;
		}
	}

	//GO usr app
	if(!SWD_GoUserApp(0))
		return FALSE;

	return TRUE;
}

void OfflineDownload(void)
{
	if(GetKeyDown())
	{
		
		GPIO_SetBits(GPIOE, GPIO_Pin_5);
		if(GetComDogDead())
		{
			SetHappyLight(HAPPY_BLINK_2);
			if(mcu_scfg.com_type==0)
			{
				if(OfflineDownloadwithSTC())
					GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				else
					GPIO_SetBits(GPIOE, GPIO_Pin_5);
			}
			else if(mcu_scfg.com_type==1)
			{
				if(OfflineDownloadwithUART())
					GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				else
					GPIO_SetBits(GPIOE, GPIO_Pin_5);
			}
			else if(mcu_scfg.com_type==2)
			{
				if(OfflineDownloadwithSWD())
					GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				else
					GPIO_SetBits(GPIOE, GPIO_Pin_5);	
			}
			SetHappyLight(HAPPY_BLINK_1);
		}	
		ClearKeyDown();
			
	}
}

