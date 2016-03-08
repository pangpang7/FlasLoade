#include "hw_config.h"
#include "sw_lib.h"
#include "UART.h"
#include "SI6600BL.h"

#define UART_ACK 0x79
#define UART_NACK 0x1F
#define UART_MAX_PACKET_SIZE 32

#define UART_GET_ID_CMD 0x02
#define UART_WRITE_CMD 0x31
#define UART_READ_CMD 0X11
#define UART_REMOVE_PWD_CMD 0xB2
#define UART_SET_PWD_CMD 0xB0
#define UART_AUTH_PWD_CMD 0xB1
#define UART_CHK_PWD_VALID_CMD 0xB3
#define UART_MASSERASE_CMD 0xC0
#define UART_GET_MAX_AUTH_CMD 0xC1
#define UART_GET_VERSION_CMD 0xC2
#define UART_GET_CHIP_ID 0xC3
#define UART_GET_FLASHLOCK 0xC4
#define UART_GO_CMD 0x21

u8 uart_tx_buf[UART_MAX_PACKET_SIZE];
u8 uart_rx_buf[UART_MAX_PACKET_SIZE];

bool UART_MassEraseCmd(void)
{
	//first packet
	//s: cmd ~cmd 0 0 0 0 0 0 0 checksum
	//r: ack 
	uart_tx_buf[0]=UART_MASSERASE_CMD;
	uart_tx_buf[1]=~uart_tx_buf[0];
	uart_tx_buf[2]=0;
	uart_tx_buf[3]=0;
	uart_tx_buf[4]=0;
	uart_tx_buf[5]=0;
	uart_tx_buf[6]=0;
	uart_tx_buf[7]=0;
	uart_tx_buf[8]=0;
	uart_tx_buf[9]=checksum(&uart_tx_buf[0],9);
	if(UART_send(10,uart_tx_buf)!=10)
		return FALSE;
	UART_SetTimeOut(7500);
	if(UART_rev(1,uart_rx_buf)!=1)
	{
		UART_SetTimeOut(3300);
		return FALSE;
	}
	UART_SetTimeOut(3300);
	if(uart_rx_buf[0]!=UART_ACK)
		return FALSE;
	return TRUE;	
}

bool UART_Connect(void)
{
	//fisrt packet
	//s: 0x7F
	//r: 0x7F
	UART_SetTimeOut(3300);
	uart_tx_buf[0]=0x7F;
	if(UART_send(1,uart_tx_buf)!=1)
		return FALSE;
	if(UART_rev(1,uart_rx_buf)!=1)
		return FALSE;
	if(uart_rx_buf[0]!=uart_tx_buf[0])
		return FALSE;
	return TRUE;
}

bool UART_Write(u32 addr, u16 lg, u8* data, bool MainOrInfo)
{
	//fisrt packet
	//s: cmd ~cmd addr0 addr1 addr2 addr3 lg0 lg1 MainOrInfo checksum
	//r: ack
	uart_tx_buf[0]=UART_WRITE_CMD;
	uart_tx_buf[1]=~uart_tx_buf[0];
	uart_tx_buf[2]=addr&0xff;
	uart_tx_buf[3]=(addr>>8)&0xff;
	uart_tx_buf[4]=(addr>>16)&0xff;
	uart_tx_buf[5]=(addr>>24)&0xff;
	uart_tx_buf[6]=lg&0xff;
	uart_tx_buf[7]=(lg>>8)&0xff;
	uart_tx_buf[8]=MainOrInfo;
	uart_tx_buf[9]=checksum(&uart_tx_buf[0],9);
	if(UART_send(10,uart_tx_buf)!=10)
		return FALSE;
	if(UART_rev(1,uart_rx_buf)!=1)
		return FALSE;
	if(uart_rx_buf[0]!=UART_ACK)
		return FALSE;

	//second packet
	//s: data0 data1 ... datan
	//r: ack
	if(UART_send(lg,data)!=lg)
		return FALSE;
	if(UART_rev(1,uart_rx_buf)!=1)
		return FALSE;
	if(uart_rx_buf[0]!=UART_ACK)
		return FALSE;

	return TRUE;
}

bool UART_Read(u32 addr, u16 lg, u8* data, bool MainOrInfo)
{
	//first packet
	//s: cmd ~cmd addr0 addr1 addr2 addr3 lg0 lg1 MainOrInfo checksum
	//r: data0 data1 ... datan
	uart_tx_buf[0]=UART_READ_CMD;
	uart_tx_buf[1]=~uart_tx_buf[0];
	uart_tx_buf[2]=addr&0xff;
	uart_tx_buf[3]=(addr>>8)&0xff;
	uart_tx_buf[4]=(addr>>16)&0xff;
	uart_tx_buf[5]=(addr>>24)&0xff;
	uart_tx_buf[6]=lg&0xff;
	uart_tx_buf[7]=(lg>>8)&0xff;
	uart_tx_buf[8]=MainOrInfo;
	uart_tx_buf[9]=checksum(&uart_tx_buf[0],9);
	if(UART_send(10,uart_tx_buf)!=10)
		return FALSE;
	if(UART_rev(lg,data)!=lg)
		return FALSE;

	return TRUE;
}

bool UART_Restore(void)
{
	//first packet
	//s: cmd ~cmd 0 0 0 0 0 0 0 checksum
	//r: ack 
	uart_tx_buf[0]=UART_REMOVE_PWD_CMD;
	uart_tx_buf[1]=~uart_tx_buf[0];
	uart_tx_buf[2]=0;
	uart_tx_buf[3]=0;
	uart_tx_buf[4]=0;
	uart_tx_buf[5]=0;
	uart_tx_buf[6]=0;
	uart_tx_buf[7]=0;
	uart_tx_buf[8]=0;
	uart_tx_buf[9]=checksum(&uart_tx_buf[0],9);
	if(UART_send(10,uart_tx_buf)!=10)
		return FALSE;
	UART_SetTimeOut(7500);
	if(UART_rev(1,uart_rx_buf)!=1)
	{
		UART_SetTimeOut(3300);
		return FALSE;
	}
	UART_SetTimeOut(3300);
	if(uart_rx_buf[0]!=UART_ACK)
		return FALSE;
	return TRUE;
}

bool UART_WritePWD(u8* data,bool RestoreOrNot)
{
	//first packet
	//s: cmd ~cmd RestoreOrNot 0 0 0 0 0 0 checksum
	//r: ack 
	uart_tx_buf[0]=UART_SET_PWD_CMD;
	uart_tx_buf[1]=~uart_tx_buf[0];
	uart_tx_buf[2]=RestoreOrNot;
	uart_tx_buf[3]=0;
	uart_tx_buf[4]=0;
	uart_tx_buf[5]=0;
	uart_tx_buf[6]=0;
	uart_tx_buf[7]=0;
	uart_tx_buf[8]=0;
	uart_tx_buf[9]=checksum(&uart_tx_buf[0],9);
	if(UART_send(10,uart_tx_buf)!=10)
		return FALSE;
	UART_SetTimeOut(7500);
	if(UART_rev(1,uart_rx_buf)!=1)
	{
		UART_SetTimeOut(3300);
		return FALSE;
	}
	UART_SetTimeOut(3300);
	if(uart_rx_buf[0]!=UART_ACK)
		return FALSE;

	//second packet
	//s: max_auth_times[0]...[3] pwd0 ... pwd15 flashlock0 ... flashlock31 flashbuskey0 ... flashbuskey15 flashbuskeyen[0]...[3]
	//r: ack
	if(UART_send(72,data)!=72)
		return FALSE;
	
	if(UART_rev(1,uart_rx_buf)!=1)
		return FALSE;
	
	if(uart_rx_buf[0]!=UART_ACK)
		return FALSE;
	return TRUE;  
}

bool UART_AUTHPWD(u8* pwd)
{
	//first packet
	//s: cmd ~cmd 0 0 0 0 0 0 0 checksum
	//r: ack 
	uart_tx_buf[0]=UART_AUTH_PWD_CMD;
	uart_tx_buf[1]=~uart_tx_buf[0];
	uart_tx_buf[2]=0;
	uart_tx_buf[3]=0;
	uart_tx_buf[4]=0;
	uart_tx_buf[5]=0;
	uart_tx_buf[6]=0;
	uart_tx_buf[7]=0;
	uart_tx_buf[8]=0;
	uart_tx_buf[9]=checksum(&uart_tx_buf[0],9);
	if(UART_send(10,uart_tx_buf)!=10)
		return FALSE;
	if(UART_rev(1,uart_rx_buf)!=1)
		return FALSE;
	if(uart_rx_buf[0]!=UART_ACK)
		return FALSE;

	//second packet
	//s: pwd0 ... pwd15
	//r: ack(AUTH SUCCESS)/nack(AUTH FAILED)
	if(UART_send(16,pwd)!=16)
		return FALSE;
	if(UART_rev(1,uart_rx_buf)!=1)
		return FALSE;
	if(uart_rx_buf[0]!=UART_ACK)
		return FALSE;
	return TRUE;
}

bool UART_CheckPWDValid(void)
{
	//first packet
	//s: cmd ~cmd 0 0 0 0 0 0 0 checksum
	//r: ack(PWD Valid)/nack(PWD Invalid)
	uart_tx_buf[0]=UART_CHK_PWD_VALID_CMD;
	uart_tx_buf[1]=~uart_tx_buf[0];
	uart_tx_buf[2]=0;
	uart_tx_buf[3]=0;
	uart_tx_buf[4]=0;
	uart_tx_buf[5]=0;
	uart_tx_buf[6]=0;
	uart_tx_buf[7]=0;
	uart_tx_buf[8]=0;
	uart_tx_buf[9]=checksum(&uart_tx_buf[0],9);
	if(UART_send(10,uart_tx_buf)!=10)
		return FALSE;
	if(UART_rev(1,uart_rx_buf)!=1)
		return FALSE;
	if(uart_rx_buf[0]!=UART_ACK)
		return FALSE;
	return TRUE;
}

bool UART_GetMAXAUTHTimes(u8* max_auth_times)
{
	//first packet
	//s: cmd ~cmd 0 0 0 0 0 0 0 checksum
	//r: max_auth_times
	uart_tx_buf[0]=UART_GET_MAX_AUTH_CMD;
	uart_tx_buf[1]=~uart_tx_buf[0];
	uart_tx_buf[2]=0;
	uart_tx_buf[3]=0;
	uart_tx_buf[4]=0;
	uart_tx_buf[5]=0;
	uart_tx_buf[6]=0;
	uart_tx_buf[7]=0;
	uart_tx_buf[8]=0;
	uart_tx_buf[9]=checksum(&uart_tx_buf[0],9);
	if(UART_send(10,uart_tx_buf)!=10)
		return FALSE;
	if(UART_rev(1,max_auth_times)!=1)
		return FALSE;
	return TRUE;
	
}

bool UART_RemovePWD(void)
{
	//first packet
	//s: cmd ~cmd 0 0 0 0 0 0 0 checksum
	//r: ack 
	uart_tx_buf[0]=UART_REMOVE_PWD_CMD;
	uart_tx_buf[1]=~uart_tx_buf[0];
	uart_tx_buf[2]=0;
	uart_tx_buf[3]=0;
	uart_tx_buf[4]=0;
	uart_tx_buf[5]=0;
	uart_tx_buf[6]=0;
	uart_tx_buf[7]=0;
	uart_tx_buf[8]=0;
	uart_tx_buf[9]=checksum(&uart_tx_buf[0],9);
	if(UART_send(10,uart_tx_buf)!=10)
		return FALSE;
	UART_SetTimeOut(7500);
	if(UART_rev(1,uart_rx_buf)!=1)
	{
		UART_SetTimeOut(3300);
		return FALSE;
	}
	UART_SetTimeOut(3300);
	if(uart_rx_buf[0]!=UART_ACK)
		return FALSE;
	return TRUE;
}

bool UART_GetPID(u8* pid)
{
	//first packet
	//s: cmd ~cmd 0 0 0 0 0 0 0 checksum
	//r: pid0 pid1 
	uart_tx_buf[0]=UART_GET_ID_CMD;
	uart_tx_buf[1]=~uart_tx_buf[0];
	uart_tx_buf[2]=0;
	uart_tx_buf[3]=0;
	uart_tx_buf[4]=0;
	uart_tx_buf[5]=0;
	uart_tx_buf[6]=0;
	uart_tx_buf[7]=0;
	uart_tx_buf[8]=0;
	uart_tx_buf[9]=checksum(&uart_tx_buf[0],9);
	if(UART_send(10,uart_tx_buf)!=10)
		return FALSE;
	if(UART_rev(2,pid)!=2)
		return FALSE;	
	return TRUE;	
}

bool UART_GetChipVersion(u8* version)
{
	//first packet
	//s: cmd ~cmd 0 0 0 0 0 0 0 checksum
	//r: version 
	uart_tx_buf[0]=UART_GET_VERSION_CMD;
	uart_tx_buf[1]=~uart_tx_buf[0];
	uart_tx_buf[2]=0;
	uart_tx_buf[3]=0;
	uart_tx_buf[4]=0;
	uart_tx_buf[5]=0;
	uart_tx_buf[6]=0;
	uart_tx_buf[7]=0;
	uart_tx_buf[8]=0;
	uart_tx_buf[9]=checksum(&uart_tx_buf[0],9);
	if(UART_send(10,uart_tx_buf)!=10)
		return FALSE;
	if(UART_rev(1,version)!=1)
		return FALSE;	
	return TRUE;
}

bool UART_GetChipID(u8* ChipID)
{
	//first packet
	//s: cmd ~cmd 0 0 0 0 0 0 0 checksum
	//r: chipID0 ... chipID7 
	uart_tx_buf[0]=UART_GET_CHIP_ID;
	uart_tx_buf[1]=~uart_tx_buf[0];
	uart_tx_buf[2]=0;
	uart_tx_buf[3]=0;
	uart_tx_buf[4]=0;
	uart_tx_buf[5]=0;
	uart_tx_buf[6]=0;
	uart_tx_buf[7]=0;
	uart_tx_buf[8]=0;
	uart_tx_buf[9]=checksum(&uart_tx_buf[0],9);
	if(UART_send(10,uart_tx_buf)!=10)
		return FALSE;
	if(UART_rev(8,ChipID)!=8)
		return FALSE;	
	return TRUE;
}

bool UART_GetFlashLockBit(u8* FlashLock)
{
	//first packet
	//s: cmd ~cmd 0 0 0 0 0 0 0 checksum
	//r: FlashLockBit0 ... FlashLockBit31 
	uart_tx_buf[0]=UART_GET_FLASHLOCK;
	uart_tx_buf[1]=~uart_tx_buf[0];
	uart_tx_buf[2]=0;
	uart_tx_buf[3]=0;
	uart_tx_buf[4]=0;
	uart_tx_buf[5]=0;
	uart_tx_buf[6]=0;
	uart_tx_buf[7]=0;
	uart_tx_buf[8]=0;
	uart_tx_buf[9]=checksum(&uart_tx_buf[0],9);
	if(UART_send(10,uart_tx_buf)!=10)
		return FALSE;
	if(UART_rev(32,FlashLock)!=32)
		return FALSE;	
	return TRUE;
}

bool UART_GoUserApp(u32 addr)
{
	//first packet
	//s: cmd ~cmd addr0 addr1 addr2 addr3 0 0 0 checksum
	//no rev!!!
	uart_tx_buf[0]=UART_GO_CMD;
	uart_tx_buf[1]=~uart_tx_buf[0];
	uart_tx_buf[2]=addr&0xff;
	uart_tx_buf[3]=(addr>>8)&0xff;
	uart_tx_buf[4]=(addr>>16)&0xff;
	uart_tx_buf[5]=(addr>>24)&0xff;
	uart_tx_buf[6]=0;
	uart_tx_buf[7]=0;
	uart_tx_buf[8]=0;
	uart_tx_buf[9]=checksum(&uart_tx_buf[0],9);
	if(UART_send(10,uart_tx_buf)!=10)
		return FALSE;	
	return TRUE;
}
