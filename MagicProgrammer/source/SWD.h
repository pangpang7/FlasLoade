#ifndef __SWD_H
#define __SWD_H

#include "usb_type.h"

bool SWD_DisConnect(void);
bool SWD_MassEraseCmd(void);
bool SWD_Connect(void);
bool SWD_Write(u32 addr, u16 lg, u8* data, bool MainOrInfo);
bool SWD_Read(u32 addr, u16 lg, u8* data, bool MainOrInfo);
bool SWD_Restore(void);
bool SWD_WritePWD(u8* data,bool RestoreOrNot);
bool SWD_AUTHPWD(u8* pwd);
bool SWD_CheckPWDValid(void);
bool SWD_GetMAXAUTHTimes(u8* max_auth_times);
bool SWD_RemovePWD(void);
bool SWD_GetPID(u8* pid);
bool SWD_GetChipVersion(u8* version);
bool SWD_GetChipID(u8* ChipID);
bool SWD_GetFlashLockBit(u8* FlashLock);
bool SWD_GoUserApp(u32 addr);
bool SWD_GetPwdAuthed(void);

#endif
