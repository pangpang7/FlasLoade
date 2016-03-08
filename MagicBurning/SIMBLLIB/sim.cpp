#include <stdio.h>
#include <string.h>
#include "stdafx.h"

#include "sim.h"

#define ACK 0x79
#define DATA 0x55
#define NACK 0x1F

#define BL_VERSION 0x10
#define BL_PID1 0x33
#define BL_PID2 0x55

#define RAM_BASE (0x20000000)
#define RAM_SIZE (0x10000)
#define FLASH_BASE (0x8000000)
#define FLASH_SIZE (0x80000)


CSIM::CSIM()
{
	isConnected = FALSE;
	VirRam= (BYTE *)VirtualAlloc(NULL,1*1024*1024l,MEM_COMMIT,PAGE_READWRITE);
	VirFlash= (BYTE *)VirtualAlloc(NULL,1*1024*1024l,MEM_COMMIT,PAGE_READWRITE);

	//HID_Init();
}
CSIM::~CSIM()
{
	VirtualFree(VirRam,0,MEM_RELEASE);
	VirtualFree(VirFlash,0,MEM_RELEASE);
	//HID_UnInit();
}

BYTE CSIM::checksum(BYTE *pd, DWORD cnt)
{
	DWORD i;
	BYTE res=pd[0];
	for(i=1;i<cnt;i++)		
		res^=pd[i];
	return res;
}

bool CSIM::ChkMemValid(DWORD addr)
{
	if(((addr>=RAM_BASE)&&(addr<=RAM_BASE+RAM_SIZE))||((addr>=FLASH_BASE)&&(addr<=FLASH_BASE+FLASH_SIZE)))
		return TRUE;
	else
		return FALSE;
}

void CSIM::SetComSettings(int _numPort, long _speedInBaud, int _nbBit, int _parity, float _nbStopBit)
{

}

bool CSIM::open(string devname)
{
  return isConnected;
}

bool CSIM::open()
{
	isConnected=TRUE;
	return TRUE;
}

void CSIM::closeCom()
{
	isConnected=FALSE;
}

bool CSIM::setTimeOut(DWORD ms)
{
	return TRUE;
}

bool CSIM::setSpeed(DWORD baudrate)
{
	return TRUE;
}

void CSIM::SetStatus(STATS sts)
{
	BL_Request.cur_status=sts;
	if(sts==IDLE)
		BL_Request.cur_cmd=INIT_CON;
}

void CSIM::SetTXreq(void)
{
	BL_Request.rx_done=FALSE;
	BL_Request.tx_req=TRUE;
}

void CSIM::ChkNACKreq(void)
{
	if(!BL_Request.tx_req)
	{
		BL_Request.tx_count=1;
		BL_Request.tx_buf[0]=(BYTE)NACK;
		SetTXreq();	
		SetStatus(IDLE);
	}
}

bool CSIM::Virmemrd(BYTE* data, DWORD Vmembase, DWORD lg)
{
	if(ChkMemValid(Vmembase)&&(ChkMemValid(Vmembase+lg)))
	{
		if((Vmembase>RAM_BASE)&&(Vmembase>RAM_BASE+RAM_SIZE))
		{
			memcpy(data,VirRam+(Vmembase-RAM_BASE),lg);
		}
		else
		{
			memcpy(data,VirFlash+(Vmembase-FLASH_BASE),lg);
		}
		return TRUE;
	}
	else
		return FALSE;
}

bool CSIM::Virmemwr(BYTE* data, DWORD Vmembase, DWORD lg)
{
	if(ChkMemValid(Vmembase)&&(ChkMemValid(Vmembase+lg)))
	{
		if((Vmembase>RAM_BASE)&&(Vmembase>RAM_BASE+RAM_SIZE))
		{
			memcpy(VirRam+(Vmembase-RAM_BASE),data,lg);
		}
		else
		{
			memcpy(VirFlash+(Vmembase-FLASH_BASE),data,lg);
		}
		return TRUE;
	}
	else
		return FALSE;
}

void CSIM::VirFlashEarse(DWORD sec)
{
	if(sec=0xffff)
		memset(VirFlash,0,1024*1024);
}

void CSIM::RQ_CMDProc(void)
{
	DWORD i;
	switch(BL_Request.cur_cmd)
	{
		case INIT_CON:
			BL_Request.tx_count=1;
			BL_Request.tx_buf[0]=INIT_CON;
			SetTXreq();
			SetStatus(IDLE);
			break;
		case GET_CMD:
			BL_Request.tx_count=15;
			BL_Request.tx_buf[0]=(BYTE)ACK;
		    BL_Request.tx_buf[1]=11;
			BL_Request.tx_buf[2]=BL_VERSION;
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
			BL_Request.tx_buf[14]=(BYTE)ACK;
			SetTXreq();
			SetStatus(IDLE);
			break;
		case GET_VER_ROPS_CMD:
			BL_Request.tx_count=5;
			BL_Request.tx_buf[0]=(BYTE)ACK;
			BL_Request.tx_buf[1]=BL_VERSION;
			BL_Request.tx_buf[2]=0x00;
			BL_Request.tx_buf[3]=0x00;
			BL_Request.tx_buf[4]=(BYTE)ACK;
			SetTXreq();
			SetStatus(IDLE);
			break;
		case GET_ID_CMD:
			BL_Request.tx_count=5;
			BL_Request.tx_buf[0]=(BYTE)ACK;
			BL_Request.tx_buf[1]=2;
			BL_Request.tx_buf[2]=BL_PID1;
			BL_Request.tx_buf[3]=BL_PID2;
			BL_Request.tx_buf[4]=(BYTE)ACK;
			SetTXreq();
			SetStatus(IDLE);
			break;
		case READ_CMD:
			if(BL_Request.cur_status==CMD)
			{
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(BYTE)ACK;
				SetTXreq();	
				SetStatus(SUB1);
			}
			else if(BL_Request.cur_status==SUB1)
			{
				if((BL_Request.rx_count==5)&&!checksum(&BL_Request.rx_buf[0],5))
				{
					BL_Config.addr = ((DWORD)(BL_Request.rx_buf[0])<<24)
										| ((DWORD)(BL_Request.rx_buf[1])<<16)
										| ((DWORD)(BL_Request.rx_buf[2])<<8)
										| ((DWORD)(BL_Request.rx_buf[3])<<0);
					BL_Request.tx_count=1;
					BL_Request.tx_buf[0]=(BYTE)ACK;
					SetTXreq();	
					SetStatus(SUB2);
				}
			}
			else if(BL_Request.cur_status==SUB2)
			{
				if((BL_Request.rx_count==2)&&!checksum(&BL_Request.rx_buf[0],2))
				{
					BL_Config.cnt=BL_Request.rx_buf[0]+1;
					BL_Request.tx_count= BL_Config.cnt+1;
					BL_Request.tx_buf[0]=(BYTE)ACK;

					if(!Virmemrd(&BL_Request.tx_buf[1],BL_Config.addr,BL_Config.cnt))
						return;

					SetTXreq();	
					SetStatus(IDLE);	
				}
			}	
			break;
		/*case GO_CMD:
			if(BL_Request.cur_status==CMD)
			{
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(BYTE)ACK;
				SetTXreq();	
				SetStatus(SUB1);
			}
			else if(BL_Request.cur_status==SUB1)
			{
				if((BL_Request.rx_count==5)&&!checksum(&BL_Request.rx_buf[0],5))
				{
					BL_Config.addr = ((DWORD)(BL_Request.rx_buf[0])<<24)
										| ((DWORD)(BL_Request.rx_buf[1])<<16)
										| ((DWORD)(BL_Request.rx_buf[2])<<8)
										| ((DWORD)(BL_Request.rx_buf[3])<<0);
					//((void *())(BL_Config.addr+4))(); ///why add 4???
				}
			}
			break;*/
		case WRITE_CMD:
			if(BL_Request.cur_status==CMD)
			{
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(BYTE)ACK;
				SetTXreq();	
				SetStatus(SUB1);
			}
			else if(BL_Request.cur_status==SUB1)
			{
				if((BL_Request.rx_count==5)&&!checksum(&BL_Request.rx_buf[0],5))
				{
					BL_Config.addr = ((DWORD)(BL_Request.rx_buf[0])<<24)
										| ((DWORD)(BL_Request.rx_buf[1])<<16)
										| ((DWORD)(BL_Request.rx_buf[2])<<8)
										| ((DWORD)(BL_Request.rx_buf[3])<<0);
					BL_Request.tx_count=1;
					BL_Request.tx_buf[0]=(BYTE)ACK;
					SetTXreq();	
					SetStatus(SUB2);
				}
			}
			else if(BL_Request.cur_status==SUB2)
			{
				if((BL_Request.rx_count==BL_Request.rx_buf[0]+1)
					&&!checksum(&BL_Request.rx_buf[0],BL_Request.rx_buf[0]+1))
				{
					BL_Config.cnt=BL_Request.rx_buf[0]+1;

					if(ChkMemValid(BL_Config.addr)&&ChkMemValid(BL_Config.addr+BL_Config.cnt))
					{
						// need adjust in ram or flash???
						if(!Virmemwr(&BL_Request.rx_buf[1],BL_Config.addr,BL_Config.cnt))
							return;
						BL_Request.tx_count=1;
						BL_Request.tx_buf[0]=(BYTE)ACK;
						SetTXreq();	
						SetStatus(IDLE);
					}

				}
			}
			break;
		case ERASE_EXT_CMD:
			if(BL_Request.cur_status==CMD)
			{
				BL_Request.tx_count=1;
				BL_Request.tx_buf[0]=(BYTE)ACK;
				SetTXreq();	
				SetStatus(SUB1);
			}
			else if(BL_Request.cur_status==SUB1)
			{
				if((BL_Request.rx_buf[0]!=0xFF) // special pages earse
					&& (!checksum(&BL_Request.rx_buf[0],3))
					)
				{
					BL_Config.excnt=((WORD)BL_Request.rx_buf[0]<<8)
									|((WORD)BL_Request.rx_buf[1]<<0);
					if(BL_Request.rx_count==BL_Config.excnt+3)
					{
						for(i=0;i<BL_Request.rx_count;i++)
						{
							if(1)//check page valid/invalid / earse specail page
								break;
						}
						if(i==BL_Request.rx_count)// earse success
						{
							BL_Request.tx_count=1;
							BL_Request.tx_buf[0]=(BYTE)ACK;
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
					VirFlashEarse(0xffff);
					BL_Request.tx_count=1;
					BL_Request.tx_buf[0]=(BYTE)ACK;
					SetTXreq();	
					SetStatus(IDLE);
					// mass earse
					// check earse result
					// send ack
				}
			}
			break;
		default:
			break;
	}
}

int CSIM::sendData(DWORD lg, LPBYTE data)
{
	BL_Request.rx_count=lg;
	memcpy(BL_Request.rx_buf,data,BL_Request.rx_count);
	rx_queue.c.clear();
	if(BL_Request.cur_status==IDLE)
	{
		if((BL_Request.rx_count==2)&&(BL_Request.rx_buf[0]==~BL_Request.rx_buf[1]))
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
	for(int i=0;i<BL_Request.tx_count;i++)
		rx_queue.push(BL_Request.tx_buf[i]);
	return lg;
}

int CSIM::sendData(string* data)
{
	if( data == NULL )
		return false;

	return sendData((DWORD)data->size(), (LPBYTE)data->data());
}

int CSIM::receiveData(DWORD lg, LPBYTE data)
{
	if(rx_queue.size()<lg)
	{
		rx_queue.c.clear();
		return 0;
	}
	else
	{
		for(int i=0;i<lg;i++)
		{
			data[i]=rx_queue.front();
			rx_queue.pop();
		}
		return lg;
	}
}

int CSIM::receiveData(string* data)
{
	char buffer[1025];
	int nbChar=0;


	if( data==NULL)
		return false;

	nbChar = receiveData(1024, (LPBYTE)buffer);
	buffer[nbChar] = 0;
	data->assign(buffer);
	return nbChar;
}

bool CSIM::setRts(bool val)
{
	return TRUE;
}

bool CSIM::setDtr(bool val)
{
	return TRUE;
}

bool CSIM::setTxd(bool val)
{
	return TRUE;
}

bool CSIM::getCts()
{
	return FALSE;
}

bool CSIM::getDtr()
{
	return FALSE;
}

bool CSIM::getRi()
{
	return FALSE;
}

bool CSIM::getCd()
{
	return FALSE;
}

string CSIM::getErrorMsg()
{
	return "NO error";
}


