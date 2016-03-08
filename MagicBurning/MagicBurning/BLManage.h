#ifndef __BLMANAGE_H__
#define __BLMANAGE_H__
#include "StdAfx.h"
#include <string>
#include <stdlib.h>
#include "../USBHIDBLLIB/USBHIDBLLIB.h"
#include "../Files/Files.h"
#define WM_UPDATEDATA WM_USER + 1999 //customized event

using namespace std;

enum CBLMANAGE_OPERATION {CBLMANAGE_SET_CONFIG,
	CBLMANAGE_SET_OFF_CONFIG,
	CBLMANAGE_GET_INFO,
	CBLMANAGE_EARSE,
	CBLMANAGE_DOWNLOAD,
	CBLMANAGE_UPLOAD,
	CBLMANAGE_SECURITY,
	CBLMANAGE_CONNECT,
	CBLMANAGE_DISCONNECT,
	CBLMANAGE_NULL,
	CBLMANAGE_TEST};

typedef struct 
{
	char  FWfile_download[256];
	char  FWfile_upload[256];
	BYTE FWfile_type; //0.MCU app, 1.MCU bootloader, 2.Programmer bootloader
	char  Programmer_name[256];
	char  MCU_name[256];
	CBLMANAGE_OPERATION op;
	DWORD op_id;
	DWORD start_addr;//use in bin file or GO operation
	bool verify;
	bool optimize;
	BYTE NeedEarse; // 0.No earse 1.partly earse 2. all earse
	bool jmptoapp; 
	BYTE pwd[16];
	BYTE MAX_AUTH_NUM;
	BYTE FlashLock[32];
	BYTE FlashBusKey[16];
	BYTE FlashBusEnable; // 0x55 means enable
	BYTE SecType; // 0.write pwd 1.auth pwd 2.remove pwd 3.check pwd validation 4 restore to factory settings
	PRO_DESCRIPTOR pro;
}CBLMANAGE_OP,*LPCBLMANAGE_OP;



typedef struct
{
	bool PIsConnected;
	bool MIsConnected;
	bool op_done;
	bool op_success;
	bool verify_ok;
	bool authed;
	BYTE CUR_AUTH_NUM;
	bool ValidChecked;
	BYTE progress; // 0-0%,100-100%
	string CurPhase;
	char MapFile[256];
	DWORD DEV_RAM_BASE;
	DWORD DEV_FLASH_BASE;
	PMAPPING pmMapping;
	TARGET_DESCRIPTOR target;
	PRO_DESCRIPTOR pro;
}CBLMANAGE_INFO,*LPCBLMANAGE_INFO;

class CBLMANAGE
{
public:
	CBLMANAGE();									
	virtual ~CBLMANAGE();
	void SetMainPwnd(CWnd *pWnd);
	static bool FileExist(char* filename);
	bool OP_Req_asycn(LPCBLMANAGE_OP lpop,BYTE order);
	bool OP_Req_sycn(LPCBLMANAGE_INFO lpinfo,LPCBLMANAGE_OP lpop);
	void COMreset(void);
	void ProgressProc(BYTE prog);
	void GetCurInfo(LPCBLMANAGE_INFO lpinfo);
	void GetCurOp(LPCBLMANAGE_INFO lpop);
	bool CheckOPBusy(void);
	void SetMCUBase(DWORD rambase,DWORD flashbase);
private:
	volatile bool OP_Busy;
	bool OP_sync;
	CBLMANAGE_OP current_op;
	CBLMANAGE_INFO current_info;
	HANDLE	hThreadMonitor;
	bool flagThreadMonitor;
	friend DWORD WINAPI ThreadMonitor (LPVOID param);
	CWnd *MainpWnd;
	std::queue<CBLMANAGE_OP> man_queue;
};
#endif
