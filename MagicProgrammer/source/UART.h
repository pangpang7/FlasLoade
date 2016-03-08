#ifndef __UART_H
#define __UART_H
#include "usb_type.h"

bool UART_MassEraseCmd(void);
bool UART_Connect(void);
bool UART_Write(u32 addr, u16 lg, u8* data, bool MainOrInfo);
bool UART_Read(u32 addr, u16 lg, u8* data, bool MainOrInfo);
bool UART_Restore(void);
bool UART_WritePWD(u8* data,bool RestoreOrNot);
bool UART_AUTHPWD(u8* pwd);
bool UART_CheckPWDValid(void);
bool UART_GetMAXAUTHTimes(u8* max_auth_times);
bool UART_RemovePWD(void);
bool UART_GetPID(u8* pid);
bool UART_GetChipVersion(u8* version);
bool UART_GetChipID(u8* ChipID);
bool UART_GetFlashLockBit(u8* FlashLock);
bool UART_GoUserApp(u32 addr);

#endif
