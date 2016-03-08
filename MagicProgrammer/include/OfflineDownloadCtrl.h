#ifndef __OFFLINEDOWNLOADCTRL_H
#define __OFFLINEDOWNLOADCTRL_H

#include "usb_type.h"

#define MCU_FLASH_PAGES 128
#define MCU_RAM_PAGES 48
#define MCU_CFG_PAGES 1
#define PRO_PAGE_SIZE 0x800
#define MAX_DATA_SIZE 1024
#define FISH_MAN 0x55

#define MCU_FLASH_BASE 0x8040000
#define MCU_FLASH_SIZE (MCU_FLASH_PAGES*PRO_PAGE_SIZE)

#define MCU_RAM_SIZE (MCU_RAM_PAGES*PRO_PAGE_SIZE)
#define MCU_RAM_BASE (MCU_FLASH_BASE-MCU_RAM_SIZE)
#define MCU_CFG_SIZE (MCU_CFG_PAGES*PRO_PAGE_SIZE)
#define MCU_CFG_BASE (MCU_RAM_BASE-MCU_CFG_SIZE)

// first 128K for programmer code


#define PRO_VERSION 0x10
#define PRO_PID0 0x09
#define PRO_PID1 0x16

typedef struct 	//need DWORD align
{
	//bool flash_map[MCU_FLASH_PAGES];
	//bool ram_map[MCU_RAM_PAGES];
	bool flash_map[128];
	bool ram_map[48];
	bool ram_done;
	bool flash_done;
	u8 com_type;  // 0.STC 1.UART 2.SWD
	u8 MCU_page_size; // page_size / 256 
	u8 MCUPID[4]; //2 bytes reserved for DWORD align
	u32 flash_offset;
	u32 ram_offset;
	u8 max_auth_num[4];
	u8 pwd[16];
	u8 FlashLock[32];
	u8 FlashBusKey[16];
	u8 FlashBusKeyEn[4];
	u8 pwdflag[4];
	u8 reserved1;
	u8 reserved2;
	u8 flag; // always equal 0x55
	u8 checksum;		
}MCU_CFG, *LPMCU_CFG;

LPMCU_CFG GetMCUCFG(void);
void OfflineInit(void);
bool SetRamDone(bool done);
bool SetFlashDone(bool done);
void OfflineDownload(void);
bool OfflineEarseMCURAM(void);
bool OfflineEarseMCUFLASH(void);
bool OfflineProgramMCURAM(u32 StartAddr,u8 * data, u32 lg);
bool OfflineProgramMCUFLASH(u32 StartAddr,u8 * data, u32 lg);
bool OfflineReadMCURAM(u32 StartAddr,u8 * data, u32 lg);
bool OfflineReadMCUFLASH(u32 StartAddr,u8 * data, u32 lg);
bool SetPwd(u8* data);
bool AuthPwd(u8* pwd);
bool GetPwdAuthed(void);
bool GetPwdValidation(void);
u8 GetMaxAuthNum(void);
bool RemovePwd(void);

void OFFLINE_GetPID(u8* pid);
void OFFLINE_GetChipVersion(u8* version);
void OFFLINE_GetChipID(u8* ChipID);
void OFFLINE_GetFlashLockBit(u8* FlashLock);

bool OfflineDownloadwithSTC(void);

#endif
