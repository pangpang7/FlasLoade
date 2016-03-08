#ifndef __SPI_H
#define __SPI_H
#include "usb_type.h"

bool SPIPassword_Write(u8* pwd);
void SPI_TxRx(u8 addr,u16 TxPassword,u16 *RxPassword1);
bool SPIConnectionTest(bool ReleaseRST);

#endif 
