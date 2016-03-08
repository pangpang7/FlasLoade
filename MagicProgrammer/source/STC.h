#ifndef __STC_H
#define __STC_H
#include "usb_type.h"

#define STC_TEST

typedef struct
{
	bool MainOrInfo;  // true->Mass, false->info
	bool NormalOrBurst; // true->Normal, false->Burst
	u32 addr;
	u8* data;
	u32 cnt; //dword count
}STC_CONFIG, *LPSTC_CONFIG;

void STC_MassEraseCmdAll(void);
void STC_MassEraseCmd(void);
void STC_Connect(void);
void STC_ReferenceCellEraseCmd(void);
void STC_DisConnect(void);
void STC_Write(LPSTC_CONFIG pcfg);
void STC_Read(LPSTC_CONFIG pcfg);
void STC_OP(u8 Symbol,u32 addr,u32 dataa,u8 count);
bool STC_Restore(void);
void STC_WritePWD(u8* data);
bool STC_AUTHPWD(u8* pwd);
bool STC_CheckPWDValid(void);
u8 STC_GetMAXAUTHTimes(void);
bool STC_RemovePWD(void);
void STC_GetPID(u8* pid);
void STC_GetChipVersion(u8* version);
void STC_GetChipID(u8* ChipID);
void STC_GetFlashLockBit(u8* FlashLock);
void STC_BL_Init(void);
bool STC_GetPwdAuthed(void);
#ifdef STC_TEST
void STC_Test(void);
#endif
#endif
