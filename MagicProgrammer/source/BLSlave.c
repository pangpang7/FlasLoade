#include "hw_config.h"
#include "usb_type.h"
#include "usb_lib.h"
#include "BLSlave.h"
#include "sw_lib.h"
#include "OfflineDownloadCtrl.h"
#include "STC.h"
#include "UART.h"
#include "SWD.h"

typedef void (*pFunction)(void);


#define BL_VERSION (0xFF)
#define BL_PID1 (0xFF)
#define BL_PID2 (0xFF)

#define FLASH_L (0x8008000)
#define FLASH_U (MCU_CFG_BASE-1)
#define RAM_L   (0x20000000)
#define RAM_U   (0x20008000-1)


#define MAX_SIZE_TX (1200)
#define MAX_SIZE_RX (1200)

#define MAX_SIZE_PACKAGE (64)
#define VALID_DATA_SIZE (55)

typedef struct 
{
	u8 rx_buf[MAX_SIZE_RX];
	u8 tx_buf[MAX_SIZE_RX];
	u32 rx_count;
	u32 tx_count;
	bool rx_done;
	bool tx_req;
	u32 cur_cmd;
	STATS cur_status;
}BL_REQ, * LPBL_REQ;

typedef struct
{
	BL_MODE bl_mode;
	COM_TYPE com_type;
	DOWNLAOD_TYPE download_type;
	u32 addr;
	u32 cnt;
	u16 excnt;
}BL_CONFIG, * LPBL_CONFIG;

typedef enum  {COM_IDLE=0x00,COM_IN_PROC} USB_STATS;

typedef struct
{
	u8 tx_pack[MAX_SIZE_PACKAGE];
	u8 rx_pack[MAX_SIZE_PACKAGE];
	u16 tolsize;
	u16 offset;
	u8 cnt;
	u16 tolsendsize;
	USB_STATS cur_rxstats;
	volatile ACKS acks;
	bool sack_rq;
}USBCOM, * LPUSBCOM;



BL_REQ BL_Request;
BL_CONFIG BL_Config; 

USBCOM usbcom;

__asm void MSR_MSP(u32 addr) 

{

 MSR MSP, r0//set Main Stack value

 BX r14

}

// byte 0: report ID
// byte 1: acks
// byte 2-3: total size
// byte 4-5: offset in this packet
// byte 6: count in this packet
// byte 7-8: size need receive(use in packet from host side)
// byte 9-63: valid data
// ...MSB in first byte

void USBInit(void)
{
	usbcom.cur_rxstats=COM_IDLE;
	usbcom.sack_rq=FALSE;
	usbcom.tx_pack[0]=0x07; // tx report ID
}

void USBOutCB(void)
{
	USBCOM tmp;
	PMAToUserBufferCopy(usbcom.rx_pack, ENDP1_RXADDR, MAX_SIZE_PACKAGE);
	SetEPRxStatus(ENDP1, EP_RX_VALID);

	//UART_send(MAX_SIZE_PACKAGE,usbcom.rx_pack);
	if(usbcom.rx_pack[1]!=DATA)
	{
		if(usbcom.rx_pack[1]==ACK)
			usbcom.acks=ACK;
		else
			usbcom.acks=NACK;
		return;
	}

	
	tmp.tolsize=((u16)usbcom.rx_pack[2]<<8)
					|((u16)usbcom.rx_pack[3]<<0);
	tmp.offset=((u16)usbcom.rx_pack[4]<<8)
					|((u16)usbcom.rx_pack[5]<<0);
	tmp.cnt=usbcom.rx_pack[6];
	tmp.tolsendsize=((u16)usbcom.rx_pack[7]<<8)
					|((u16)usbcom.rx_pack[8]<<0);
	if(usbcom.cur_rxstats!=COM_IDLE)
	{
		if((usbcom.tolsize!=tmp.tolsize)//tolsize not correct
			||(tmp.offset!=usbcom.offset+usbcom.cnt)// offset not correct
			||(tmp.offset+tmp.cnt>tmp.tolsize)//count not correct
			)
		{
			usbcom.cur_rxstats=COM_IDLE;
			return;
		}
	}
	else
	{
		if(tmp.offset!=0)
			return;
		usbcom.cur_rxstats=COM_IN_PROC;
	}	

	usbcom.tolsize=tmp.tolsize;
	usbcom.offset=tmp.offset;
	usbcom.cnt=tmp.cnt;
	memcpy(&BL_Request.rx_buf[usbcom.offset],&usbcom.rx_pack[9],usbcom.cnt);
	if(tmp.offset+tmp.cnt==tmp.tolsize)
	{
		BL_Request.rx_count=tmp.tolsize;
		usbcom.cur_rxstats=COM_IDLE;
		BL_Request.rx_done=TRUE;
		/*if((BL_Request.rx_count==2)
			&&(BL_Request.rx_buf[0]==~BL_Request.rx_buf[1])
			&&(BL_Request.rx_buf[0]==GET_CMD)
		)
		{
			//like a com reset
			usbcom.acks=NACK;
		} */
	}
	else
	{
		/*usbcom.sack_rq=TRUE;*/
	}
}

void USBIn(void)
{
	USBCOM tmp;
	if(BL_Request.tx_req)
	{
		
		ComDogFeed();
		tmp.tolsize=BL_Request.tx_count;
		tmp.offset=0;
		usbcom.tx_pack[1]=DATA;
		while(BL_Request.tx_count>=VALID_DATA_SIZE)
		{
			tmp.cnt=VALID_DATA_SIZE;
			BL_Request.tx_count-=tmp.cnt;
			usbcom.tx_pack[3]=tmp.tolsize & 0xff;
			usbcom.tx_pack[2]=tmp.tolsize >> 8;
			usbcom.tx_pack[5]=tmp.offset & 0xff;
			usbcom.tx_pack[4]=tmp.offset >> 8;
			usbcom.tx_pack[6]=tmp.cnt;
			memcpy(&usbcom.tx_pack[9],&BL_Request.tx_buf[tmp.offset],tmp.cnt);
			tmp.offset+=tmp.cnt;
			UserToPMABufferCopy(usbcom.tx_pack, ENDP1_TXADDR, MAX_SIZE_PACKAGE);
	     	SetEPTxCount(ENDP1, MAX_SIZE_PACKAGE);
	     	SetEPTxValid(ENDP1);
			while(GetEPTxStatus(ENDP1) != EP_TX_NAK);
			/*if(BL_Request.tx_count>0)
			{
				usbcom.acks=UNDEFINED;
				while(usbcom.acks==UNDEFINED);	//add timeout
				if(usbcom.acks!=ACK)
				{
					BL_Request.tx_req=FALSE;
					return;	
				}
			}*/
		}
		if(BL_Request.tx_count>0)
		{
			tmp.cnt=BL_Request.tx_count;
			usbcom.tx_pack[3]=tmp.tolsize & 0xff;
			usbcom.tx_pack[2]=tmp.tolsize >> 8;
			usbcom.tx_pack[5]=tmp.offset & 0xff;
			usbcom.tx_pack[4]=tmp.offset >> 8;
			usbcom.tx_pack[6]=tmp.cnt;
			memcpy(&usbcom.tx_pack[9],&BL_Request.tx_buf[tmp.offset],tmp.cnt);
			tmp.offset+=tmp.cnt;
			UserToPMABufferCopy(usbcom.tx_pack, ENDP1_TXADDR, MAX_SIZE_PACKAGE);
	     	SetEPTxCount(ENDP1, MAX_SIZE_PACKAGE);
	     	SetEPTxValid(ENDP1);
			while(GetEPTxStatus(ENDP1) != EP_TX_NAK);	
		}
		BL_Request.tx_req=FALSE;
	}
	else if(usbcom.sack_rq)
	{
		ComDogFeed();
		usbcom.tx_pack[1]=ACK;
		UserToPMABufferCopy(usbcom.tx_pack, ENDP1_TXADDR, MAX_SIZE_PACKAGE);
	    SetEPTxCount(ENDP1, MAX_SIZE_PACKAGE);
	    SetEPTxValid(ENDP1);
		usbcom.sack_rq=FALSE;
	}
}


void RQ_init(void)
{
	BL_Request.rx_count=0;
	BL_Request.tx_count=0;
	BL_Request.cur_cmd=NO_CMD;
	BL_Request.cur_status=IDLE;
	BL_Request.rx_done=FALSE;
	BL_Request.tx_req=FALSE;

	BL_Config.bl_mode=OFF_LINE;
	BL_Config.com_type=TYPE_UART;
	BL_Config.download_type=DTYPE_MCU_FLASH;

	//for test
	//BL_Config.download_type=DTYPE_MCU_FLASH;
}

void SetStatus(STATS sts)
{
	BL_Request.cur_status=sts;
	if(sts==IDLE)
		BL_Request.cur_cmd=NO_CMD;
}

void SetTXreq(void)
{
	BL_Request.rx_done=FALSE;
	BL_Request.tx_req=TRUE;
}

void ChkNACKreq(void)
{
	if(!BL_Request.tx_req)
	{
		BL_Request.tx_count=1;
		BL_Request.tx_buf[0]=(u8)NACK;
		SetTXreq();	
		SetStatus(IDLE);
	}
}

void RQ_CMDProc(void)
{
	u32 i;
	u16 cnt;
	static LPMCU_CFG ptmpcfg;
	static STC_CONFIG stccfg;

	//shield some cmd if no auth in offline mode
	if(!(BL_Config.bl_mode==ON_LINE))
	{
		//offline mode
		if(!GetPwdAuthed()&&GetPwdValidation())
		{
			if((BL_Request.cur_cmd!=GET_PRO_CMD)
				&&(BL_Request.cur_cmd!=SET_PRO_CMD)
				&&(BL_Request.cur_cmd!=AUTH_PWD)
				&&(BL_Request.cur_cmd!=REMOVE_PWD)
				&&(BL_Request.cur_cmd!=CHK_PWD_VALID)
				&&(BL_Request.cur_cmd!=INIT_CON)
				)
				BL_Request.cur_cmd=NO_CMD;
		}
	}
	else
	{
		if(BL_Config.com_type==TYPE_STC)
		{
			if(!STC_GetPwdAuthed()&&STC_CheckPWDValid())
			{
				if((BL_Request.cur_cmd!=GET_PRO_CMD)
					&&(BL_Request.cur_cmd!=SET_PRO_CMD)
					&&(BL_Request.cur_cmd!=AUTH_PWD)
					&&(BL_Request.cur_cmd!=REMOVE_PWD)
					&&(BL_Request.cur_cmd!=CHK_PWD_VALID)
					&&(BL_Request.cur_cmd!=INIT_CON)
					&&(BL_Request.cur_cmd!=GO_CMD)// for restore operation
					)
					BL_Request.cur_cmd=NO_CMD;
			}	
		}
		else if(BL_Config.com_type==TYPE_SWD)
		{
			if(!SWD_GetPwdAuthed())
			{
				if((BL_Request.cur_cmd!=GET_PRO_CMD)
					&&(BL_Request.cur_cmd!=SET_PRO_CMD)
					&&(BL_Request.cur_cmd!=AUTH_PWD)
					&&(BL_Request.cur_cmd!=REMOVE_PWD)
					&&(BL_Request.cur_cmd!=CHK_PWD_VALID)
					&&(BL_Request.cur_cmd!=INIT_CON)
					&&(BL_Request.cur_cmd!=GO_CMD)// for restore operation
					)
					BL_Request.cur_cmd=NO_CMD;
			}
		}
	}

	switch(BL_Request.cur_cmd)
	{
		case INIT_CON:
			if(BL_Config.bl_mode==ON_LINE)
			{
				if(BL_Config.com_type==TYPE_STC)
				{
					STC_Connect();
				}
				else if(BL_Config.com_type==TYPE_SWD)
				{
					if(!SWD_Connect())
						break;
				}
				else
				{
					if(!UART_Connect())
						break;
				}
			}
			BL_Request.tx_count=1;
			BL_Request.tx_buf[0]=ACK;
			
			SetTXreq();
			SetStatus(IDLE);
			break;
		case GET_PRO_CMD: // only for programmer
			ptmpcfg=GetMCUCFG();
			BL_Request.tx_count=28;
			BL_Request.tx_buf[0]=(u8)ACK;
		  BL_Request.tx_buf[1]=PRO_VERSION;
			BL_Request.tx_buf[2]=PRO_PID0;
		  BL_Request.tx_buf[3]=PRO_PID1;
		  BL_Request.tx_buf[4]=(MCU_RAM_BASE>>24)&0xff;
			BL_Request.tx_buf[5]=(MCU_RAM_BASE>>16)&0xff;
		  BL_Request.tx_buf[6]=(MCU_RAM_BASE>>8)&0xff;
		  BL_Request.tx_buf[7]=(MCU_RAM_BASE>>0)&0xff;
		  BL_Request.tx_buf[8]=(MCU_FLASH_BASE>>24)&0xff;
			BL_Request.tx_buf[9]=(MCU_FLASH_BASE>>16)&0xff;
		  BL_Request.tx_buf[10]=(MCU_FLASH_BASE>>8)&0xff;
		  BL_Request.tx_buf[11]=(MCU_FLASH_BASE>>0)&0xff;
		  BL_Request.tx_buf[12]=ptmpcfg->MCUPID[0];
		  BL_Request.tx_buf[13]=ptmpcfg->MCUPID[1];
		  BL_Request.tx_buf[14]=BL_Config.bl_mode;
		  BL_Request.tx_buf[15]=BL_Config.com_type;
		  BL_Request.tx_buf[16]=ptmpcfg->MCU_page_size;
		  BL_Request.tx_buf[17]=(ptmpcfg->ram_offset>>24)&0xff;
			BL_Request.tx_buf[18]=(ptmpcfg->ram_offset>>16)&0xff;
		  BL_Request.tx_buf[19]=(ptmpcfg->ram_offset>>8)&0xff;
		  BL_Request.tx_buf[20]=(ptmpcfg->ram_offset>>0)&0xff;
			BL_Request.tx_buf[21]=(ptmpcfg->flash_offset>>24)&0xff;
			BL_Request.tx_buf[22]=(ptmpcfg->flash_offset>>16)&0xff;
		  BL_Request.tx_buf[23]=(ptmpcfg->flash_offset>>8)&0xff;
		  BL_Request.tx_buf[24]=(ptmpcfg->flash_offset>>0)&0xff;
			BL_Request.tx_buf[25]=ptmpcfg->ram_done;
			BL_Request.tx_buf[26]=ptmpcfg->flash_done;
			BL_Request.tx_buf[27]=(u8)ACK;
			SetTXreq();
			SetStatus(IDLE);
			break;
		case SET_PRO_CMD: //only for programmer
			if(BL_Request.cur_status==CMD)
			{
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(u8)ACK;
				SetTXreq();	
				SetStatus(SUB1);
			}
			else if(BL_Request.cur_status==SUB1)
			{
				if(BL_Request.rx_buf[0]!=12)
					break;
				ptmpcfg=GetMCUCFG();
				ptmpcfg->MCUPID[0]=BL_Request.rx_buf[1];
				ptmpcfg->MCUPID[1]=BL_Request.rx_buf[2];
				BL_Config.bl_mode=(BL_MODE)BL_Request.rx_buf[3];
				BL_Config.com_type=(COM_TYPE)BL_Request.rx_buf[4];
				ptmpcfg->com_type=BL_Request.rx_buf[4];
				ptmpcfg->MCU_page_size=BL_Request.rx_buf[5];
				ptmpcfg->ram_offset=((u32)(BL_Request.rx_buf[6])<<24)
										| ((u32)(BL_Request.rx_buf[7])<<16)
										| ((u32)(BL_Request.rx_buf[8])<<8)
										| ((u32)(BL_Request.rx_buf[9])<<0);
				ptmpcfg->flash_offset=((u32)(BL_Request.rx_buf[10])<<24)
										| ((u32)(BL_Request.rx_buf[11])<<16)
										| ((u32)(BL_Request.rx_buf[12])<<8)
										| ((u32)(BL_Request.rx_buf[13])<<0);
				
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(u8)ACK;
				SetTXreq();	
				SetStatus(IDLE);
			}
			break;
		case SET_OFFLINE_CMD:
			if(BL_Request.cur_status==CMD)
			{
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(u8)ACK;
				SetTXreq();	
				SetStatus(SUB1);
			}
			else if(BL_Request.cur_status==SUB1)
			{
				if(BL_Request.rx_buf[0]!=2)
					break;
				ptmpcfg=GetMCUCFG();
				BL_Config.download_type=(DOWNLAOD_TYPE)BL_Request.rx_buf[3];
			    if(BL_Config.download_type==DTYPE_MCU_RAM)
					SetRamDone((bool)BL_Request.rx_buf[1]);
				else if(BL_Config.download_type==DTYPE_MCU_FLASH)
					SetFlashDone((bool)BL_Request.rx_buf[2]);
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(u8)ACK;
				SetTXreq();	
				SetStatus(IDLE);
			}
			break;
		case SET_PWD:
			if(BL_Request.cur_status==CMD)
			{
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(u8)ACK;
				SetTXreq();	
				SetStatus(SUB1);
			}
			else if(BL_Request.cur_status==SUB1)
			{
				if(BL_Request.rx_buf[0]!=4+16+32+16+4-1)
					break;
				if(BL_Config.bl_mode==ON_LINE)
				{
					if(BL_Config.com_type==TYPE_STC)
					{
						//write password and max_auth_num to MCU via STC
						STC_WritePWD(&BL_Request.rx_buf[1]);
					}
					else if(BL_Config.com_type==TYPE_SWD)
					{
						if(!SWD_WritePWD(&BL_Request.rx_buf[1],TRUE))
							break;
					}
					else
					{
						if(!UART_WritePWD(&BL_Request.rx_buf[1],TRUE))
							break;
					}
				}
				else
				{
					//offline mode
				    if(!SetPwd(&BL_Request.rx_buf[1]))
						break;
				}
				
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(u8)ACK;
				SetTXreq();	
				SetStatus(IDLE);
			}
			break;
		case AUTH_PWD:
			if(BL_Request.cur_status==CMD)
			{
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(u8)ACK;
				SetTXreq();	
				SetStatus(SUB1);
			}
			else if(BL_Request.cur_status==SUB1)
			{
				if(BL_Request.rx_buf[0]!=16-1)
					break;
				if(BL_Config.bl_mode==ON_LINE)
				{
					if(BL_Config.com_type==TYPE_STC)
					{
						//read password and auth password via STC
						if(!STC_AUTHPWD(&BL_Request.rx_buf[1]))
							break;
					}
					else if(BL_Config.com_type==TYPE_SWD)
					{
						if(!SWD_AUTHPWD(&BL_Request.rx_buf[1]))
							break;
					}
					else
					{
						if(!UART_AUTHPWD(&BL_Request.rx_buf[1]))
							break;
					}
				}
				else
				{
					//offline mode
				    if(!AuthPwd(&BL_Request.rx_buf[1]))
						break;
				}
				
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(u8)ACK;
				SetTXreq();	
				SetStatus(IDLE);
			}
			break;
		case REMOVE_PWD:
			if(BL_Config.bl_mode==ON_LINE)
			{
				if(BL_Config.com_type==TYPE_STC)
				{
					//mass earse main and password via STC
					if(!STC_RemovePWD())
						break;
				}
				else if(BL_Config.com_type==TYPE_SWD)
				{
					if(!SWD_RemovePWD())
						break;
				}
				else
				{
					if(!UART_RemovePWD())
						break;
				}
			}
			else
			{ 
				//offline mode
				if(!RemovePwd())
					break;
			}
			BL_Request.tx_count=1;
			BL_Request.tx_buf[0]=(u8)ACK;
			SetTXreq();	
			SetStatus(IDLE);
			break;
		case CHK_PWD_VALID:
			if(BL_Request.cur_status==CMD)
			{
				if(BL_Config.bl_mode==ON_LINE)
				{
					if(BL_Config.com_type==TYPE_STC)
					{
						//get pwd validation
						if(!STC_CheckPWDValid())
							break;
					}
					else if(BL_Config.com_type==TYPE_SWD)
					{
						if(!SWD_CheckPWDValid())
							break;
					}
					else
					{
						if(!UART_CheckPWDValid())
							break;	
					}
				}
				else
				{
					if(!GetPwdValidation())
						break;
				}
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(u8)ACK;
				SetTXreq();	
				SetStatus(SUB1);
			}
			else if(BL_Request.cur_status==SUB1)
			{
				if(BL_Request.rx_buf[0]!=1)
					break;
				if(BL_Config.bl_mode==ON_LINE)
				{
					if(BL_Config.com_type==TYPE_STC)
					{
						//get max auth num
						BL_Request.tx_count=1;
						BL_Request.tx_buf[0]=STC_GetMAXAUTHTimes();
					}
					else if(BL_Config.com_type==TYPE_SWD)
					{
						if(!SWD_GetMAXAUTHTimes(&BL_Request.tx_buf[0]))
							break;
						BL_Request.tx_count=1;
					}
					else
					{
						if(!UART_GetMAXAUTHTimes(&BL_Request.tx_buf[0]))
							break;
						BL_Request.tx_count=1;
					}	
				}
				else
				{
					//offline mode
				  	BL_Request.tx_count=1;
					BL_Request.tx_buf[0]=GetMaxAuthNum();
				}
				SetTXreq();	
				SetStatus(IDLE);
				
			}
			break;
		case GET_CMD:
			if(BL_Config.bl_mode==ON_LINE)
			{
				if(BL_Config.com_type==TYPE_STC)
				{
					STC_GetChipVersion(&BL_Request.tx_buf[2]);
				}
				else if(BL_Config.com_type==TYPE_SWD)
				{
					if(!SWD_GetChipVersion(&BL_Request.tx_buf[2]))
						break;
				}
				else
				{
					if(!UART_GetChipVersion(&BL_Request.tx_buf[2]))
						break;
				}	
			}
			else
				BL_Request.tx_buf[2]=BL_VERSION;	
			BL_Request.tx_count=15;
			BL_Request.tx_buf[0]=(u8)ACK;
		    BL_Request.tx_buf[1]=11;
		    BL_Request.tx_buf[3]=0x00;
			BL_Request.tx_buf[4]=0x01;
			BL_Request.tx_buf[5]=0x02;
			BL_Request.tx_buf[6]=0x11;
			BL_Request.tx_buf[7]=0x21;
			BL_Request.tx_buf[8]=0x31;
			BL_Request.tx_buf[9]=0x44;
			BL_Request.tx_buf[10]=0x63;
			BL_Request.tx_buf[11]=0x73;
			BL_Request.tx_buf[12]=0x82;
			BL_Request.tx_buf[13]=0x92;
			BL_Request.tx_buf[14]=(u8)ACK;
			SetTXreq();
			SetStatus(IDLE);
			break;
		case GET_VER_ROPS_CMD:
			if(BL_Config.bl_mode==ON_LINE)
			{
				if(BL_Config.com_type==TYPE_STC)
				{
					STC_GetChipVersion(&BL_Request.tx_buf[1]);
				}
				else if(BL_Config.com_type==TYPE_SWD)
				{
					if(!SWD_GetChipVersion(&BL_Request.tx_buf[1]))
						break;
				}
				else
				{
					if(!UART_GetChipVersion(&BL_Request.tx_buf[1]))
						break;
				}	
			}
			else
				BL_Request.tx_buf[1]=BL_VERSION;
			BL_Request.tx_count=5;
			BL_Request.tx_buf[0]=(u8)ACK;
			BL_Request.tx_buf[2]=0x00;
			BL_Request.tx_buf[3]=0x00;
			BL_Request.tx_buf[4]=(u8)ACK;
			SetTXreq();
			SetStatus(IDLE);
			break;
		case GET_ID_CMD:
			if(BL_Config.bl_mode==ON_LINE)
			{
				if(BL_Config.com_type==TYPE_STC)
				{
					// GET PID 2 BYTES
					STC_GetPID(&BL_Request.tx_buf[2]);
					// GET CHIP VERSION 1 BYTE
					STC_GetChipVersion(&BL_Request.tx_buf[4]);
					// GET CHIP ID 8 BYTES
					STC_GetChipID(&BL_Request.tx_buf[5]);
					// GET FLASH LOCK BIT 32 BYTES
					STC_GetFlashLockBit(&BL_Request.tx_buf[13]);
				}
				else if(BL_Config.com_type==TYPE_SWD)
				{
					// GET PID 2 BYTES
					if(!SWD_GetPID(&BL_Request.tx_buf[2]))
						break;
					// GET CHIP VERSION 1 BYTE
					if(!SWD_GetChipVersion(&BL_Request.tx_buf[4]))
						break;
					// GET CHIP ID 8 BYTES
					if(!SWD_GetChipID(&BL_Request.tx_buf[5]))
						break;
					// GET FLASH LOCK BIT 32 BYTES
					if(!SWD_GetFlashLockBit(&BL_Request.tx_buf[13]))
						break;	
				}
				else
				{
					// GET PID 2 BYTES
					if(!UART_GetPID(&BL_Request.tx_buf[2]))
						break;
					// GET CHIP VERSION 1 BYTE
					if(!UART_GetChipVersion(&BL_Request.tx_buf[4]))
						break;
					// GET CHIP ID 8 BYTES
					if(!UART_GetChipID(&BL_Request.tx_buf[5]))
						break;
					// GET FLASH LOCK BIT 32 BYTES
					if(!UART_GetFlashLockBit(&BL_Request.tx_buf[13]))
						break;
				}
			}
			else
			{
				// offline mode
				// GET PID 2 BYTES
				OFFLINE_GetPID(&BL_Request.tx_buf[2]);
				// GET CHIP VERSION 1 BYTE
				OFFLINE_GetChipVersion(&BL_Request.tx_buf[4]);
				// GET CHIP ID 8 BYTES
				OFFLINE_GetChipID(&BL_Request.tx_buf[5]);
				// GET FLASH LOCK BIT 32 BYTES
				OFFLINE_GetFlashLockBit(&BL_Request.tx_buf[13]);
			}
			BL_Request.tx_count=46;
			BL_Request.tx_buf[0]=(u8)ACK;
			BL_Request.tx_buf[1]=1;

			BL_Request.tx_buf[45]=(u8)ACK;
			SetTXreq();
			SetStatus(IDLE);
			break;
		case READ_CMD:
			if(BL_Request.cur_status==CMD)
			{
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(u8)ACK;
				SetTXreq();	
				SetStatus(SUB1);
			}
			else if(BL_Request.cur_status==SUB1)
			{
				if((BL_Request.rx_count==5)&&!checksum(&BL_Request.rx_buf[0],5))
				{
					BL_Config.addr = ((u32)(BL_Request.rx_buf[0])<<24)
										| ((u32)(BL_Request.rx_buf[1])<<16)
										| ((u32)(BL_Request.rx_buf[2])<<8)
										| ((u32)(BL_Request.rx_buf[3])<<0);
					BL_Request.tx_count=1;
					BL_Request.tx_buf[0]=(u8)ACK;
					SetTXreq();	
					SetStatus(SUB2);
				}
			}
			else if(BL_Request.cur_status==SUB2)
			{
				if((BL_Request.rx_count==3)&&!checksum(&BL_Request.rx_buf[0],3))
				{
					cnt=(((u16)BL_Request.rx_buf[0])<<8)
					+(((u16)BL_Request.rx_buf[1])<<0);
					BL_Config.cnt=cnt+1;
					BL_Request.tx_count= BL_Config.cnt+1;
					BL_Request.tx_buf[0]=(u8)ACK;
					if(BL_Config.bl_mode==ON_LINE)
					{
						if(BL_Config.com_type==TYPE_STC)
						{
							//STC read
							stccfg.addr=BL_Config.addr;
							stccfg.data=&BL_Request.tx_buf[1];
							stccfg.cnt=BL_Config.cnt;
							stccfg.MainOrInfo=TRUE;
							STC_Read(&stccfg);
						}
						else if(BL_Config.com_type==TYPE_SWD)
						{
							if(!SWD_Read(BL_Config.addr,BL_Config.cnt,&BL_Request.tx_buf[1],TRUE))
								break;	
						}
						else
						{
							if(!UART_Read(BL_Config.addr,BL_Config.cnt,&BL_Request.tx_buf[1],TRUE))
								break;
						}
					}
					else
					{	//offline mode
						if(BL_Config.download_type==DTYPE_PRO)	
							memcpy(&BL_Request.tx_buf[1],(u8 *)BL_Config.addr,BL_Config.cnt);
						else if(BL_Config.download_type==DTYPE_MCU_RAM)
							OfflineReadMCURAM(BL_Config.addr,&BL_Request.tx_buf[1],BL_Config.cnt);
						else if(BL_Config.download_type==DTYPE_MCU_FLASH)
							OfflineReadMCUFLASH(BL_Config.addr,&BL_Request.tx_buf[1],BL_Config.cnt);
					}
					SetTXreq();	
					SetStatus(IDLE);	
				}
			}	
			break;
		case GO_CMD:
			if(BL_Request.cur_status==CMD)
			{
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(u8)ACK;
				SetTXreq();	
				SetStatus(SUB1);
			}
			else if(BL_Request.cur_status==SUB1)
			{
				if((BL_Request.rx_count==5)&&!checksum(&BL_Request.rx_buf[0],5))
				{
					if(BL_Config.bl_mode==ON_LINE)
					{
						if(BL_Config.com_type==TYPE_STC)
						{
							STC_DisConnect();
						}
						else if(BL_Config.com_type==TYPE_SWD)
						{
							BL_Config.addr = ((u32)(BL_Request.rx_buf[0])<<24)
											| ((u32)(BL_Request.rx_buf[1])<<16)
											| ((u32)(BL_Request.rx_buf[2])<<8)
											| ((u32)(BL_Request.rx_buf[3])<<0);
							SWD_GoUserApp(BL_Config.addr);
						}
						else
						{
							BL_Config.addr = ((u32)(BL_Request.rx_buf[0])<<24)
											| ((u32)(BL_Request.rx_buf[1])<<16)
											| ((u32)(BL_Request.rx_buf[2])<<8)
											| ((u32)(BL_Request.rx_buf[3])<<0);
							UART_GoUserApp(BL_Config.addr);
						}
					}
					else
					{
						BL_Config.addr = ((u32)(BL_Request.rx_buf[0])<<24)
											| ((u32)(BL_Request.rx_buf[1])<<16)
											| ((u32)(BL_Request.rx_buf[2])<<8)
											| ((u32)(BL_Request.rx_buf[3])<<0);
						//close global interrupter
						NVIC_SETFAULTMASK();
						//set stack point in user app
						MSR_MSP(*((u32*)BL_Config.addr));
						//jump to user app reset handler
						((pFunction)(*(u32*)(BL_Config.addr+4)))(); 
					}
				}
			}
			break;
		case WRITE_CMD:
			if(BL_Request.cur_status==CMD)
			{
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(u8)ACK;
				SetTXreq();	
				SetStatus(SUB1);
			}
			else if(BL_Request.cur_status==SUB1)
			{
				if((BL_Request.rx_count==5)&&!checksum(&BL_Request.rx_buf[0],5))
				{
					BL_Config.addr = ((u32)(BL_Request.rx_buf[0])<<24)
										| ((u32)(BL_Request.rx_buf[1])<<16)
										| ((u32)(BL_Request.rx_buf[2])<<8)
										| ((u32)(BL_Request.rx_buf[3])<<0);
					BL_Request.tx_count=1;
					BL_Request.tx_buf[0]=(u8)ACK;
					SetTXreq();	
					SetStatus(SUB2);
				}
			}
			else if(BL_Request.cur_status==SUB2)
			{
				cnt=(((u16)BL_Request.rx_buf[0])<<8)
					+(((u16)BL_Request.rx_buf[1])<<0);
				if((BL_Request.rx_count==cnt+1+3)
					&&!checksum(&BL_Request.rx_buf[0],cnt+1+3))
				{
					BL_Config.cnt=cnt+1;
					if(BL_Config.bl_mode==ON_LINE)
					{
						if(BL_Config.com_type==TYPE_STC)
						{
							//STC write
							stccfg.addr=BL_Config.addr;
							stccfg.data=&BL_Request.rx_buf[2];
							stccfg.cnt=BL_Config.cnt;
							stccfg.MainOrInfo=TRUE;
							STC_Write(&stccfg);
							BL_Request.tx_count=1;
							BL_Request.tx_buf[0]=(u8)ACK;
							SetTXreq();	
							SetStatus(IDLE);
						}
						else if(BL_Config.com_type==TYPE_SWD)
						{
							if(!SWD_Write(BL_Config.addr,BL_Config.cnt,&BL_Request.rx_buf[2],TRUE))
								break;
							BL_Request.tx_count=1;
							BL_Request.tx_buf[0]=(u8)ACK;
							SetTXreq();	
							SetStatus(IDLE);
						}
						else
						{
							if(!UART_Write(BL_Config.addr,BL_Config.cnt,&BL_Request.rx_buf[2],TRUE))
								break;
							BL_Request.tx_count=1;
							BL_Request.tx_buf[0]=(u8)ACK;
							SetTXreq();	
							SetStatus(IDLE);
						}
					}
					else
					{

						if(BL_Config.download_type==DTYPE_PRO)
						{
							if(((BL_Config.addr>FLASH_L)&&(BL_Config.addr<FLASH_U))
								&&((BL_Config.addr+BL_Config.cnt>FLASH_L)&&(BL_Config.addr+BL_Config.cnt<FLASH_U)))
							{
								//in writable flash
								if(FLASH_Program(BL_Config.addr,&BL_Request.rx_buf[2],BL_Config.cnt))
								{
									BL_Request.tx_count=1;
									BL_Request.tx_buf[0]=(u8)ACK;
									SetTXreq();	
									SetStatus(IDLE);	
								}
							}
							else if(((BL_Config.addr>RAM_L)&&(BL_Config.addr<RAM_U))
								&&((BL_Config.addr+BL_Config.cnt>RAM_L)&&(BL_Config.addr+BL_Config.cnt<RAM_U)))	
							{
								//in readable ram
								memcpy((u8 *)BL_Config.addr,&BL_Request.rx_buf[2],BL_Config.cnt);
								BL_Request.tx_count=1;
								BL_Request.tx_buf[0]=(u8)ACK;
								SetTXreq();	
								SetStatus(IDLE);
							}
							
						}
						else if(BL_Config.download_type==DTYPE_MCU_RAM)
						{
							if(OfflineProgramMCURAM(BL_Config.addr,&BL_Request.rx_buf[2],BL_Config.cnt))
							{
								BL_Request.tx_count=1;
								BL_Request.tx_buf[0]=(u8)ACK;
								SetTXreq();	
								SetStatus(IDLE);	
							}
						}
						else if(BL_Config.download_type==DTYPE_MCU_FLASH)
						{
							if(OfflineProgramMCUFLASH(BL_Config.addr,&BL_Request.rx_buf[2],BL_Config.cnt))
							{
								BL_Request.tx_count=1;
								BL_Request.tx_buf[0]=(u8)ACK;
								SetTXreq();	
								SetStatus(IDLE);
							}
							//for test
							/*BL_Request.tx_count=1;
							BL_Request.tx_buf[0]=(u8)ACK;
							SetTXreq();	
							SetStatus(IDLE);*/
						}	
					}
				}
			}
			break;
		case ERASE_EXT_CMD:
			if(BL_Request.cur_status==CMD)
			{
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(u8)ACK;
				SetTXreq();	
				SetStatus(SUB1);
			}
			else if(BL_Request.cur_status==SUB1)
			{
				if((BL_Request.rx_buf[0]!=0xFF) // special pages earse
					&& (!checksum(&BL_Request.rx_buf[0],3))
					)
				{
					BL_Config.excnt=((u16)BL_Request.rx_buf[0]<<8)
									|((u16)BL_Request.rx_buf[1]<<0);
					if(BL_Request.rx_count==BL_Config.excnt+3)
					{
						for(i=0;i<BL_Request.rx_count;i++)
						{
							if(BL_Config.bl_mode==ON_LINE)
							{
								if(BL_Config.com_type==TYPE_STC)
								{
									//STC special earse?? nothing!!!
									break;
								}
								else if(BL_Config.com_type==TYPE_SWD)
								{
									break;
								}
								else
								{
									break;
								}
							}
							else
							{
								//offline, nothing to do
								break;
							}
						}
						if(i==BL_Request.rx_count)// earse success
						{
							BL_Request.tx_count=1;
							BL_Request.tx_buf[0]=(u8)ACK;
							SetTXreq();	
							SetStatus(IDLE);
						}
					}
				}
				else if((BL_Request.rx_buf[0]==0xFF) // mass earse MCU FW storage area for offline download use
						&&(BL_Request.rx_buf[1]==0xFF)
						&&(BL_Request.rx_buf[2]==0x00)
						&&(BL_Request.rx_count==3)
					)
				{
					if(BL_Config.bl_mode==ON_LINE)
					{
						if(BL_Config.com_type==TYPE_STC)
						{
							//STC mass earse main
							STC_MassEraseCmd();
							BL_Request.tx_count=1;
							BL_Request.tx_buf[0]=(u8)ACK;
							SetTXreq();	
							SetStatus(IDLE);
						}
						else if(BL_Config.com_type==TYPE_SWD)
						{
							if(!SWD_MassEraseCmd())
								break;
							BL_Request.tx_count=1;
							BL_Request.tx_buf[0]=(u8)ACK;
							SetTXreq();	
							SetStatus(IDLE);
						}
						else
						{
							if(!UART_MassEraseCmd())
								break;
							BL_Request.tx_count=1;
							BL_Request.tx_buf[0]=(u8)ACK;
							SetTXreq();	
							SetStatus(IDLE);
						}
					}
					else
					{
						if(BL_Config.download_type==DTYPE_PRO)
						{
							if(FLASH_Earse(FLASH_L,FLASH_U))
							{
								BL_Request.tx_count=1;
								BL_Request.tx_buf[0]=(u8)ACK;
								SetTXreq();	
								SetStatus(IDLE);	
							}
						}
						else if(BL_Config.download_type==DTYPE_MCU_RAM)
						{
							if(OfflineEarseMCURAM())
							{
								BL_Request.tx_count=1;
								BL_Request.tx_buf[0]=(u8)ACK;
								SetTXreq();	
								SetStatus(IDLE);	
							}
						}
						else if(BL_Config.download_type==DTYPE_MCU_FLASH)
						{
							if(OfflineEarseMCUFLASH())
							{
								BL_Request.tx_count=1;
								BL_Request.tx_buf[0]=(u8)ACK;
								SetTXreq();	
								SetStatus(IDLE);	
							}
						}
					}
				}
			}
			break;
		default:
			break;
	}
}

void RQ_Proc(void)
{
	if(GetComDogDead())
	{
		STC_DisConnect();
		SWD_DisConnect();
		SetHappyLight(HAPPY_BLINK_1);
		BL_Config.bl_mode=OFF_LINE;
	}
	if((BL_Request.rx_done)&&(!BL_Request.tx_req))
	{
		ComDogFeed();
		SetHappyLight(HAPPY_BLINK_2);
		
		if(BL_Request.cur_status==IDLE)
		{

			if((BL_Request.rx_count==2)&&(BL_Request.rx_buf[0]+BL_Request.rx_buf[1]==0xff))
			{
				BL_Request.cur_cmd=BL_Request.rx_buf[0];
				BL_Request.cur_status=CMD;
			}
			else if((BL_Request.rx_count==1)&&(BL_Request.rx_buf[0]==INIT_CON))
			{
				BL_Request.cur_cmd=BL_Request.rx_buf[0];
				BL_Request.cur_status=CMD;
			}
		}

		RQ_CMDProc();
		ChkNACKreq();	
	}
	
}
