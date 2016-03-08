
#ifndef __SIM_H__
#define __SIM_H__

#include<queue>

#include <string>
using namespace std;
#include "SIMBLLIB.h"

#define RAM_LMT_L (0x20000000)
#define RAM_LMT_U (0x20000000+0x10000-1)
#define FLASH_LMT_L (0x8000000)
#define FLASH_LMT_U (0x8000000+0x80000-1)

typedef enum  {IDLE=0x00,CMD,SUB1,SUB2,SUB3,SUB4} STATS;

typedef struct 
{
	BYTE rx_buf[1024];
	BYTE tx_buf[1024];
	DWORD rx_count;
	DWORD tx_count;
	BOOL rx_done;
	BOOL tx_req;
	DWORD cur_cmd;
	STATS cur_status;
}BL_REQ, * LPBL_REQ;

typedef struct
{
	DWORD addr;
	BYTE cnt;
	WORD excnt;
}BL_CONFIG, * LPBL_CONFIG;


class CSIM  
{
public:
	void SetParity(int _parity);
	bool isConnected;

	int numPort;
	long speedInBaud;
	int nbBit;
	int parity;
	float nbStopBit;
	int  bEcho ;             /* Echo back for LIN emulation */
	                         /* 0 : Disabled , 1 : Echo Back , 2 : Listen back */
    bool FlowControl;

	//------ CONSTRUCTOR ------
	CSIM();									
	virtual ~CSIM();

	//------ OPEN AND CONFIGURE ------
	void SetComSettings(int _numPort, long _speedInBaud, int _nbBit, int _parity, float _nbStopBit);


	bool open();	// Open the serial port COM "numPort" at the speed "speedInBaud".
			        // bauds with and this adjustement : "nbBit" bit / "nbStopBit" stop bit / "parity").
	bool open(string devname);
														// Return: true if success.
	void closeCom();			                //Close the serial port.
	bool setTimeOut(DWORD ms);					//Set the time-out for receive data. Return: true if success.
	bool setSpeed(DWORD baudrate);				//Set the speed in bauds. Return: true if success.

	//------ SEND AND RECEIVE DATA ------
	int sendData(DWORD lg, LPBYTE data);		//Send table "data" of "lg" bytes.  Return: number of bytes really sent.
	int sendData(string* data);					//Send string "data".  Return: number of bytes really sent.
	int receiveData(DWORD lg, LPBYTE data);		//Receive table "data" who is limit at "lg" bytes.  Return: number of bytes received.
	int receiveData(string* data);				//Receive string "data". Return: number of bytes received.

	//------ READ AND WRITE THE STATE OF THE CONTROL LINE ------

	bool setRts(bool val);			// Set the state of RTS. Return: true if success.
	bool setDtr(bool val);			// Set the state of DTR. Return: true if success.
	bool setTxd(bool val);			// Set the state of TXD. Return: true if success.
	bool getCts();					// Return: The state of CTS.			
	bool getDtr();					// Return: The state of DTR.	
	bool getRi();					// Return: The state of RI.	
	bool getCd();					// Return: The state of CD.	
	
	string getErrorMsg();			// Return: The error message generated by the last function.

private:

	void SetStatus(STATS sts);
	void SetTXreq(void);
	void RQ_CMDProc(void);
	BYTE checksum(BYTE *pd, DWORD cnt);
	bool ChkMemValid(DWORD addr);
	void ChkNACKreq(void);
	bool Virmemrd(BYTE* data, DWORD Vmembase, DWORD lg);
	bool Virmemwr(BYTE* data, DWORD Vmembase, DWORD lg);
	void VirFlashEarse(DWORD sec);

	BYTE tx_pack[64];
	BYTE rx_pack[64];
	std::queue<BYTE> rx_queue;
	BYTE tx_pcnt;
	BYTE rx_pcnt;
	WORD m_tolsize;
	WORD m_offset;
	BYTE m_cnt;
	BOOL m_rx_flag;
	BOOL m_tx_flag;

	BL_REQ BL_Request;
	BL_CONFIG BL_Config;
	BYTE* VirRam;
	BYTE* VirFlash;
};

#endif /* __HID_H__ */