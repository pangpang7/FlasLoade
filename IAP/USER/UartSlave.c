//#define RAM_TEST
#include "UartSlave.h"
#include "sw_lib.h"
#include "SI6600BL.h"

typedef void (*pFunction)(void);

#define UART_ACK 0x79
#define UART_NACK 0x1F
#define UART_MAX_PACKET_SIZE 2048

#define NO_CMD (0x7E)

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

#ifndef RAM_TEST
#define MAIN_ADDR_OFFSET 0xA0000000
#define INFO_ADDR_OFFSET 0
#else
#define MAIN_ADDR_OFFSET 0x20001800
#define INFO_ADDR_OFFSET 0x20001000
#endif

#define FISH_MAN 0x55

u8 uart_buf[UART_MAX_PACKET_SIZE];


bool EnterBL;
bool IsAuthed;

/*__asm void MSR_MSP(u32 addr) 
{

 MSR MSP, r0//set Main Stack value

 BX r14

}

__asm void CloseGlobalInt(void)
{
 MOV R0, #1           //; 
 MSR FAULTMASK, R0    //; close global interrupter
}*/

void UART1_port_init(PORT_TypeDef * PORTx)	
{
////---------------------------------//P16+P17  
	PORTx->Pn_SEL1 &= ~(0x3f<<18);	             
	PORTx->Pn_SEL1 |=  (PORT_CFG_UART<<21) + (PORT_CFG_UART<<18);
	
	PORTx->Pn_DIR  &= ~(0x03<<6);		                            
	PORTx->Pn_DIR  |=  (0x01<<6);		

////---------------------------------P36+P35
//	PORTx->Pn_SEL1 &= ~(0x3f<<15);	                            //clr 0
//	PORTx->Pn_SEL1 |=  (PORT_CFG_UART<<18) + (PORT_CFG_UART<<15);
//	
//	PORTx->Pn_DIR  &= ~(0x03<<5);		                            
//	PORTx->Pn_DIR  |=  (0x02<<5);		   //2'b10	   note:xxxxxxxxxxxxxx	

}
void UART_Init(UART_TypeDef* UARTx)
{
  //config MCR
	UARTx->MCR=0x00;
	//set baudrate
	UARTx->LCR = 0x80;	
	//UARTx->DLL = 0x11;		//57600 @16M 
	UARTx->DLL = 0x0D;		//76800 @16M 
	UARTx->DLH = 0x00;
	UARTx->LCR = 0;

	UARTx->LCR = 0x03;   //8bit data,  1bit stop, even parity
	UARTx->FCR = 0x01;   // FIFO disable
	 
	//UARTx->IER = UART_IER_ERBFI;	  //enable rx Intr	 
}

void UART_x_Tx(UART_TypeDef* UARTx,u8* data, u32 lg)
{
	 volatile int i;
	 volatile u8 tmp;
	 for(i=0;i<lg;i++)
	 {
		UARTx->THR=data[i];
		while(!(UARTx->LSR&UART_LSR_THREmpty));
		while(!(UARTx->LSR&UART_LSR_TxEmpty));
	 }	
   tmp=UARTx->RBR;	 
}

void UART_x_Rx(UART_TypeDef* UARTx,u8* data, u32 lg)
{
	volatile int i;
	for(i=0;i<lg;i++)
	{
		while(!(UARTx->LSR&UART_LSR_RecvDataRdy));
		data[i]=UARTx->RBR;
	}		
}

void FlashRestoreLockBit(void)
{
#ifndef RAM_TEST
	FLASH->MAIN_PL0 = *((volatile unsigned int *)(FLASH_LOCK_BIT_OFFSET+0x0));
	FLASH->MAIN_PL1 = *((volatile unsigned int *)(FLASH_LOCK_BIT_OFFSET+0x4));
	FLASH->MAIN_PL2 = *((volatile unsigned int *)(FLASH_LOCK_BIT_OFFSET+0x8));
	FLASH->MAIN_PL3 = *((volatile unsigned int *)(FLASH_LOCK_BIT_OFFSET+0xc));
	FLASH->MAIN_PL4 = *((volatile unsigned int *)(FLASH_LOCK_BIT_OFFSET+0x10));
	FLASH->MAIN_PL5 = *((volatile unsigned int *)(FLASH_LOCK_BIT_OFFSET+0x14));
	FLASH->MAIN_PL6 = *((volatile unsigned int *)(FLASH_LOCK_BIT_OFFSET+0x18));
	FLASH->MAIN_PL7 = *((volatile unsigned int *)(FLASH_LOCK_BIT_OFFSET+0x1c));
#endif	
}

void FlashClearLockBit(void)
{
#ifndef RAM_TEST
	u32 i;
	for(i=0;i<8;i++)
	{
		FLASH->PL_SEQ0=0x5a5a5a5a;
		FLASH->PL_SEQ0=0xa5a5a5a5;
		*((volatile unsigned int  *)(EFLASH_CTRL_BASEADDR+0x38+i*4))=0xFFFFFFFF;
	}
#endif
}

bool FlashProgramme(u8* data, u32 lg, u32 addr)
{
#ifndef RAM_TEST
	u32 i;
	
	if((addr>=MAIN_ADDR_OFFSET+MAIN_NUM_OF_PAGES_ISP*PAGE_SIZES)||(addr+lg-1>=MAIN_ADDR_OFFSET+MAIN_NUM_OF_PAGES_ISP*PAGE_SIZES))
		return FALSE;
	
	//program
	FLASH->FLASH_CTRL = 0x01;
	
	for(i=0;i<lg/4;i++)
	{
		(*(volatile u32 *)(addr+i*4)) = (((u32)data[i*4+3])<<24)
																	| (((u32)data[i*4+2])<<16)
																	| (((u32)data[i*4+1])<<8)
																	| (((u32)data[i*4+0])<<0);
	}
	
	return TRUE;
#else
	u32 i;
	
	for(i=0;i<lg/4;i++)
	{
		(*(volatile u32 *)(addr+i*4)) = (((u32)data[i*4+3])<<24)
																	| (((u32)data[i*4+2])<<16)
																	| (((u32)data[i*4+1])<<8)
																	| (((u32)data[i*4+0])<<0);
	}
#endif
}

void FlashMassErase(void)
{
#ifndef RAM_TEST
	u32 i;
	//erase 254 pages
	
	//page erase
	FLASH->FLASH_CTRL= 0x02;
	
	for(i=0;i<MAIN_NUM_OF_PAGES_ISP;i++)
	{
		(*(volatile u32 *)(MAIN_ADDR_OFFSET+i*PAGE_SIZES))=0;
	}
#else
	
	memset((u8*)MAIN_ADDR_OFFSET,0xff,20*1024);
#endif
}

bool FlashPWDPageEarse(void)
{
#ifndef RAM_TEST
	u32 tmp;
	//pwd page earse
	
	//page erase
	FLASH->FLASH_CTRL= 0x02;
	
	(*(volatile u32 *)(INFO_PAGE_1_ADDR))=0;
	
	//write a5a5 flag to the last 8 bytes of the pwd page earse
	
	//program
	FLASH->FLASH_CTRL = 0x01;
	
	tmp=0x5a5a5a5a;
	(*(volatile u32 *)(0x7f8))=tmp;
	tmp=0xa5a5a5a5;
	(*(volatile u32 *)(0x7fc))=tmp;
	
	if(((*(volatile u32 *)(0x7f8))==0x5a5a5a5a)&&((*(volatile u32 *)(0x7fc))==0xa5a5a5a5))
		return TRUE;
	else
		return FALSE;
#else
	u32 tmp;
	memset((u8*)INFO_PAGE_1_ADDR,0xff,1024);
	tmp=0x5a5a5a5a;
	(*(volatile u32 *)(INFO_ADDR_OFFSET+0x7f8))=tmp;
	tmp=0xa5a5a5a5;
	(*(volatile u32 *)(INFO_ADDR_OFFSET+0x7fc))=tmp;
	
	if(((*(volatile u32 *)(INFO_ADDR_OFFSET+0x7f8))==0x5a5a5a5a)&&((*(volatile u32 *)(INFO_ADDR_OFFSET+0x7fc))==0xa5a5a5a5))
		return TRUE;
	else
		return FALSE;
#endif
}

bool FlashRestore(void)
{
	bool Res;
	FlashMassErase();
	
	Res=FlashPWDPageEarse();
	
	IsAuthed=FALSE;
	
	return Res;
}


__asm void FlashGoUserApp(void)
{
#ifndef RAM_TEST
				;read user MSP
				LDR     R0,  =0xa0000000  
				LDR     R1,  [R0, #0]
				;save user MSP in R13
				MOV     R13, R1           
			   
			  ;read user code reset address
				LDR     R0, =0xa0000004
				LDR     R1, [R0, #0]

        ;set sfr_lock = 1
        ;LDR     R4, =0x40013500
				
				;set remap register = 1
			  LDR     R2, =0x40020060
        LDR     R3, =0x00000001				
        STR     R3, [R2]		
				
				;set sfr_lock = 1							
        ;STR     R3, [R4]
				
        BX      R1
				
				nop
				nop
				nop				
#else
#endif
}

void WritePWD(u8* data)
{
	//byte 0 - 3: max_auth_times
	//byte 4 - 19: pwd
	//byte 20 - 51: flash lock bit
	//byte 52 - 67: flash bus key
	//byte 68 - 71: flash bus key enable
	u8 tmp[4];
	
	//check flash bus key , if all FFs, replace it with MCU chip id
	if(memcmpfixed(&data[52],0xFF,16))
	{
		memcpy(&data[52],(u8*)MCU_CHIP_ID_OFFSET,MCU_CHIP_ID_LEN);
	}
	//write all kinds of password
	FlashProgramme(&data[0],PWD_MAX_AUTH_LEN,PWD_MAX_AUTH_OFFSET);
	FlashProgramme(&data[4],PWD_LEN,PWD_INFO_OFFSET);
	FlashProgramme(&data[20],FLASH_LOCK_BIT_LEN,FLASH_LOCK_BIT_OFFSET);
	FlashProgramme(&data[52],FLASH_BUS_KEY_LEN,FLASH_BUS_KEY_OFFSET);
	FlashProgramme(&data[68],FLASH_BUS_KEY_EN_LEN,FLASH_BUS_KEY_EN_OFFSET);
	
	//write pwd flag
	tmp[0]=FISH_MAN;
	FlashProgramme(tmp,PWD_FLAG_LEN,PWD_FLAG_OFFSET);

	IsAuthed=TRUE;
}

bool AUTHPWD(u8* pwd)
{
	u8 tmp[PWD_LEN];
	u32 tmp32;
	u32 i;
	
	//read password
	memcpy(tmp,(u8*)PWD_INFO_OFFSET,PWD_LEN);
	
	if(memcmp(tmp,pwd,PWD_LEN)!=0)
	{
		//read max_auth_times
		memcpy(tmp,(u8*)PWD_MAX_AUTH_OFFSET,PWD_MAX_AUTH_LEN);
		
		if((tmp[0]!=0)&&(tmp[0]<32))//0 means 00
		{
			memcpy((u8 *)&tmp32,(u8*)CUR_PWD_OFFSET,CUR_PWD_LEN);
			
			for(i=0;i<32;i++)
			{
				if((tmp32&(1<<i)))
					break;
			}

			if(i>=tmp[0])
			{
				FlashRestore();
			}
			else
			{
				tmp32 &= ~(1<<i);
				FlashProgramme((u8 *)&tmp32,CUR_PWD_LEN,CUR_PWD_OFFSET);
			}
		}
		else if(tmp[0]>=32)
		{
			//invalid value
			FlashRestore();
		}
		
		return FALSE;
	}
	
	IsAuthed=TRUE;
	
	return TRUE;
}

u8 GetMAXAUTHTimes(void)
{
	u32 tmp32,i;
	u8 tmp[4];

	//read cur_auth_times
	memcpy((u8 *)&tmp32,(u8*)CUR_PWD_OFFSET,CUR_PWD_LEN);

	for(i=0;i<32;i++)
	{
		if((tmp32&(1<<i)))
			break;
	}

	//read max_auth_times
	memcpy(tmp,(u8*)PWD_MAX_AUTH_OFFSET,PWD_MAX_AUTH_LEN);

	return tmp[0]-i;
}

void UartSlaveInit(void)
{
#ifdef RAM_TEST
	*((u8*)MCU_PID_OFFSET)=0x56;
	*((u8*)MCU_PID_OFFSET+1)=0x78;
	
	*((u8*)MCU_REVISION_OFFSET)=0x33;
#endif
	EnterBL=FALSE;
	IsAuthed=FALSE;
}

void UartSendACK(void)
{
	uart_buf[0]=UART_ACK;
	UART_x_Tx(UART_1,uart_buf,1);
}

void UartSendNACK(void)
{
	uart_buf[0]=UART_NACK;
	UART_x_Tx(UART_1,uart_buf,1);
}

void UartCMDProc(void)
{
	u32 addr;
	u32 lg;
	bool MainOrInfo;
	bool LockBitIsCleared=FALSE;
	
	UART_x_Rx(UART_1,uart_buf,1);
	if(uart_buf[0]==0x7f)
	{
		UART_x_Tx(UART_1,uart_buf,1);
		return;
	}
	else
		UART_x_Rx(UART_1,&uart_buf[1],9);
	if(!checksum(uart_buf,10))
	{
		//checksum OK!
		if((!IsAuthed)&&(*((u8*)PWD_FLAG_OFFSET)==FISH_MAN))
		{
			if((uart_buf[0]!=UART_AUTH_PWD_CMD)
				&&(uart_buf[0]!=UART_REMOVE_PWD_CMD)
				&&(uart_buf[0]!=UART_CHK_PWD_VALID_CMD)
				)
				uart_buf[0]=NO_CMD;
		}
		else
		{
			if(!LockBitIsCleared)
			{
				LockBitIsCleared=TRUE;
				FlashClearLockBit();
			}
		}
		switch(uart_buf[0])
		{
			case UART_GET_ID_CMD:
				memcpy(uart_buf,(u8*)MCU_PID_OFFSET,2);
			  UART_x_Tx(UART_1,uart_buf,2);
				break;
			case UART_WRITE_CMD:
				//first packet
				addr = ((u32)(uart_buf[5])<<24)
										| ((u32)(uart_buf[4])<<16)
										| ((u32)(uart_buf[3])<<8)
										| ((u32)(uart_buf[2])<<0);
				lg = ((u16)(uart_buf[7])<<8)
										| ((u16)(uart_buf[6])<<0);
				MainOrInfo = (bool)uart_buf[8];
				if(MainOrInfo)
					addr += MAIN_ADDR_OFFSET;
				else
					addr += INFO_ADDR_OFFSET;
				UartSendACK();
				
				//second packet
				UART_x_Rx(UART_1,uart_buf,lg);
				if(FlashProgramme(uart_buf,lg,addr))
					UartSendACK();
				else
					UartSendNACK();
				break;
			case UART_READ_CMD:
				//first packet
			  addr = ((u32)(uart_buf[5])<<24)
										| ((u32)(uart_buf[4])<<16)
										| ((u32)(uart_buf[3])<<8)
										| ((u32)(uart_buf[2])<<0);
				lg = ((u16)(uart_buf[7])<<8)
										| ((u16)(uart_buf[6])<<0);
				MainOrInfo = (bool)uart_buf[8];
				if(MainOrInfo)
					addr += MAIN_ADDR_OFFSET;
				else
					addr += INFO_ADDR_OFFSET;
				UART_x_Tx(UART_1,(u8*)addr,lg);
				break;
			case UART_REMOVE_PWD_CMD:
				if(FlashRestore())
					UartSendACK();
				else
					UartSendNACK();
				break;
			case UART_SET_PWD_CMD:
				//first packet
				if(uart_buf[2]) //RestoreOrNot
					FlashRestore();
			  UartSendACK();
			
				//second packet
				UART_x_Rx(UART_1,uart_buf,72);
				WritePWD(uart_buf);
				UartSendACK();
				break;
			case UART_AUTH_PWD_CMD:
				//first packet
			  UartSendACK();
			
				//second packet
				UART_x_Rx(UART_1,uart_buf,16);
				if(AUTHPWD(uart_buf))
					UartSendACK();
				else
					UartSendNACK();
				break;
			case UART_CHK_PWD_VALID_CMD:
				if(*((u8*)PWD_FLAG_OFFSET)==FISH_MAN)
					UartSendACK();
				else
					UartSendNACK();
				break;
			case UART_MASSERASE_CMD:
				FlashMassErase();
				UartSendACK();
				break;
		  case UART_GET_MAX_AUTH_CMD:
				uart_buf[0]=GetMAXAUTHTimes();
				UART_x_Tx(UART_1,uart_buf,1);
				break;
			case UART_GET_VERSION_CMD:
				UART_x_Tx(UART_1,(u8*)(MCU_REVISION_OFFSET+3),1);
				break;
			case UART_GET_CHIP_ID:
				UART_x_Tx(UART_1,(u8*)MCU_CHIP_ID_OFFSET,8);
				break;
			case UART_GET_FLASHLOCK:
				UART_x_Tx(UART_1,(u8*)FLASH_LOCK_BIT_OFFSET,32);
				break;
			case UART_GO_CMD:
				FlashRestoreLockBit();
				BOOT0|=BIT16;
				FlashGoUserApp();
				break;
			default:
				UartSendNACK();
				break;
		}
	}
	else
	{
		UartSendNACK();
	}
}

void UartSlaveProtocolProc(void)
{
	if(!EnterBL)
	{
		UART_x_Rx(UART_1,uart_buf,1);
		if(uart_buf[0]==0x7F)
		{
			UART_x_Tx(UART_1,uart_buf,1);
			EnterBL=TRUE;
		}
	}
	else
	{
		UartCMDProc();
	}
}
