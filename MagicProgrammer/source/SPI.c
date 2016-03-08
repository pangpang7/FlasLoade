#include "hw_config.h"
#include "SPI.h"
#include "sw_lib.h"
//    SPI   STC
//PB12 NSS	TMS
//PB13 SCK  TCK
//PB14 MISO	TDO
//PB15 MOSI	TDI
//PB11 RST	RST

u8 ArrayITX[64];
u8 ArrayIRX[64];
u8 PasswordToX;

u8 CodeCountRecord[2] = {0};

u8 Offline_Code_address_index = 0;

//for test
u8 TimerOver;

#define PASSWORD_ADRESS1  0x30
#define PASSWORD_ADRESS2 (PASSWORD_ADRESS1+1)
#define PASSWORD_ADRESS3 (PASSWORD_ADRESS2+1)
#define PASSWORD_ADRESS4 (PASSWORD_ADRESS3+1)
#define PASSWORD_ADRESS5 (PASSWORD_ADRESS4+1)
#define PASSWORD_ADRESS6 (PASSWORD_ADRESS5+1)
#define PASSWORD_ADRESS7 (PASSWORD_ADRESS6+1)
#define PASSWORD_ADRESS8 (PASSWORD_ADRESS7+1)

#define CODE_COUNT_ADRESS_BYTE 20

u8 SaveData;
enum Offline_code_address
{
	Code_Adress1,
	Code_Adress2,
	Code_Adress3,
	Code_Adress4,
	Code_Adress5,
	Code_Adress6,
	Code_Adress7,
	Code_Adress8
};

void SPI_TxRx(u8 addr,u16 TxPassword,u16 *RxPassword1)
{
	u8 i;
	for(i=0;i<8;i++)
	ArrayITX[15-i]=(addr>>i)&1;
			 	  	     
	for(i=0;i<16;i++)
	ArrayITX[31-i]=(TxPassword>>i)&1;

	SO_ResetBits(NSS);  

	SO_SetClock();
	SO_ResetClock();

	SO_SetClock();
	SO_ResetClock();

	for(i=0;i<32;i++)
	{
		SO_WriteBits(MOSI,(BitAction)ArrayITX[i]);
		ArrayIRX[i]=SI_GetBits();
		SO_SetClock();
		SO_ResetClock();
	}
	SO_SetBits(MOSI);

	SO_SetClock();
	SO_ResetClock();

	SO_SetClock();
	SO_ResetClock();

	SO_SetClock();
	SO_ResetClock();

	SO_SetClock();
	SO_ResetClock();

	SO_SetBits(NSS);

	*RxPassword1=0;

	for(i=16;i<32;i++)
	{
		if(ArrayIRX[i])
			*RxPassword1|=(0x01<<(31-i));
	}
}

void SPIPassword_Write_ArrayITX()
{
	    ArrayITX[0]=0;
		ArrayITX[1]=0;
		ArrayITX[2]=0;
		ArrayITX[3]=1;
		ArrayITX[4]=0;
		ArrayITX[5]=0;
		ArrayITX[6]=0;
		ArrayITX[7]=0;		
}

void SPIPassword_Read_ArrayITX()
{
		ArrayITX[0]=0;
		ArrayITX[1]=0;
		ArrayITX[2]=0;
		ArrayITX[3]=1;
		ArrayITX[4]=0;
		ArrayITX[5]=0;
		ArrayITX[6]=0;
		ArrayITX[7]=1;	
}

bool SPIConnectionTest(bool ReleaseRST)
{
	u16 testPassword1 = 0,testPassword2 = 0;
	u16 RxPassword=0;

	SO_ResetClock();
	SO_SetBits(NSS);

	SO_SetBits(RST);
	SPIPassword_Write_ArrayITX();

	SO_ResetBits(RST);
	DelayMs(10);

	SPI_TxRx(0,0x5a5a,&RxPassword);	
	SPIPassword_Read_ArrayITX();
	SPI_TxRx(0,0x00,&testPassword1);

	SPIPassword_Write_ArrayITX();
	SPI_TxRx(4,0xa5a5,&RxPassword);	
	SPIPassword_Read_ArrayITX();
	SPI_TxRx(4,0x00,&testPassword2);
	
	if(((0x5a5a != testPassword1)&&(0xa5a5 != testPassword2))&&(0x03!=SaveData))
	{
	    //Connection error
		if(ReleaseRST)
		{
			SO_SetBits(RST);
			DelayMs(10);
		}	
		return FALSE;
	}
	else
	{
		if(ReleaseRST)
		{
			SO_SetBits(RST);
			DelayMs(10);
		}
		return TRUE;
	}
}

bool SPIPassword_Write(u8* pwd)
{
	u16 RxPassword=0;
	u16 TxPassword=0;
	u8 i = 0;

	if(!SPIConnectionTest(FALSE))
		return FALSE;
			
	SPIPassword_Write_ArrayITX();
	for(i=0;i<16;i+=2)   //128 bit password
	{
		TxPassword=((u16)pwd[i])|(((u16)pwd[i]+1)<<8);
		SPI_TxRx(i/2,TxPassword,&RxPassword);			 			 
	}

	SO_SetBits(RST);

	DelayMs(10);

	return TRUE;	
}
