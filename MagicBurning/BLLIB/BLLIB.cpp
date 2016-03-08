#include <malloc.h>
#include "stdafx.h"
#include "BLLIB.h"


/************************************************************************************/
/* Generic BL API types
/*
/*
/************************************************************************************/

typedef BYTE virt_GetProgress(LPBYTE progress);  
typedef BYTE virt_GetActivityTime(LPDWORD time);  
typedef BYTE virt_SetActivityTime(DWORD time); 
typedef BYTE virt_Send_RQ(LPBL_Request pRQ);
typedef BYTE virt_SetBLMode(BYTE DLmode, BYTE MCUcom);
typedef BYTE virt_SetCOMSettings(int numPort, long speedInBaud, int nbBit, int parity, float nbStopBit);
typedef BYTE virt_COM_Open();
typedef BYTE virt_COM_Close();
typedef BYTE virt_BL_SetSpeed(DWORD speed);
typedef BYTE virt_BL_Init_BL();
typedef BYTE virt_BL_GET(LPBYTE Version, LPCommands pCmds);                      
typedef BYTE virt_BL_GET_VER_ROPS(LPBYTE Version, LPBYTE ROPEnabled, LPBYTE ROPDisabled);             
typedef BYTE virt_BL_GET_ID(LPBYTE size, LPBYTE pID);                   
typedef BYTE virt_BL_READ(DWORD Address, BYTE Size, LPBYTE pData);                     
typedef BYTE virt_BL_GO(DWORD Address);                       
typedef BYTE virt_BL_WRITE(DWORD address, BYTE size, LPBYTE pData);                    
typedef BYTE virt_BL_ERASE(WORD NbSectors, LPBYTE pSectors);                      
typedef BYTE virt_BL_WRITE_PROTECT(BYTE NbSectors, LPBYTE pSectors);            
typedef BYTE virt_BL_WRITE_TEMP_UNPROTECT();     
typedef BYTE virt_BL_WRITE_PERM_UNPROTECT();     
typedef BYTE virt_BL_READOUT_PROTECT();     
typedef BYTE virt_BL_READOUT_TEMP_UNPROTECT();   
typedef BYTE virt_BL_READOUT_PERM_UNPROTECT();   
typedef BYTE virt_BL_UPLOAD(DWORD Address, LPBYTE pData, DWORD Length);  
typedef BYTE virt_BL_VERIFY(DWORD Address, LPBYTE pData, DWORD Length,BOOL bTruncateLeadFFForDnLoad); 
typedef BYTE virt_BL_DNLOAD(DWORD Address, LPBYTE pData, DWORD Length,BOOL bTruncateLeadFFForDnLoad);   
typedef BYTE virt_SetPaketSize(BYTE size);
typedef BYTE virt_GetPaketSize(LPBYTE size);
typedef ACKS virt_GetAckValue();
typedef BOOL virt_COM_is_Open();
typedef BYTE virt_SetTimeOut(DWORD vms);
typedef BYTE virt_BL_SetRts(bool Val);
typedef BYTE virt_BL_SetDtr(bool Val);
typedef BYTE virt_BL_setTxd(bool val);	
typedef BYTE virt_BL_getCts(bool* pval);					
typedef BYTE virt_BL_getDtr(bool* pval);	
typedef BYTE virt_BL_getRi(bool* pval);					
typedef BYTE virt_BL_getCd(bool* pval);
typedef BYTE virt_BL_SetEcho(int val);
typedef BYTE virt_BL_SetFlowControl(int Val);  


typedef BYTE virt_TARGET_GetFlashSize(DWORD Addr, LPWORD val);
typedef BYTE virt_TARGET_GetMemoryAddress(DWORD Addr, LPBYTE val);
typedef BYTE virt_TARGET_GetRDPOptionByte(LPBYTE RDP);
typedef BYTE virt_TARGET_GetWRPOptionBytes(LPBYTE WRP0, LPBYTE WRP1, LPBYTE WRP2, LPBYTE WRP3); 
typedef BYTE virt_TARGET_GetUserOptionByte(LPBYTE User);
typedef BYTE virt_TARGET_GetDataOptionByte(LPBYTE Data0, LPBYTE Data1);
typedef BYTE virt_TARGET_SetOptionBytes(DWORD Addr,BYTE User, BYTE RDP, BYTE Data0, BYTE Data1, BYTE WRP0, BYTE WRP1, BYTE WRP2, BYTE WRP3);
typedef BYTE virt_TARGET_GetOptionBytes(DWORD Addr,LPBYTE User, LPBYTE RDP, LPBYTE Data0, LPBYTE Data1, LPBYTE WRP0, LPBYTE WRP1, LPBYTE WRP2, LPBYTE WRP3); 


/************************************************************************************/
/* Generic BL API references
/*
/*
/************************************************************************************/

virt_GetProgress					*pt_GetProgress;						//(LPBYTE progress);  
virt_GetActivityTime				*pt_GetActivityTime;					//(LPDWORD time);  
virt_SetActivityTime				*pt_SetActivityTime;					//(DWORD time); 
virt_TARGET_GetFlashSize			*pt_TARGET_GetFlashSize;				//(DWORD Addr, LPWORD val);
virt_TARGET_GetMemoryAddress		*pt_TARGET_GetMemoryAddress;			//(DWORD Addr, LPWORD val);
virt_TARGET_GetRDPOptionByte		*pt_TARGET_GetRDPOptionByte;			//(LPBYTE RDP);
virt_TARGET_GetWRPOptionBytes		*pt_TARGET_GetWRPOptionBytes;		//(LPBYTE WRP0, LPBYTE WRP1, LPBYTE WRP2, LPBYTE WRP3);
virt_Send_RQ						*pt_Send_RQ;							//(LPBL_Request pRQ);
virt_SetBLMode						*pt_SetBLMode;
virt_SetCOMSettings					*pt_SetCOMSettings;					//(int numPort, long speedInBaud, int nbBit, int parity, float nbStopBit);
virt_COM_Open						*pt_COM_Open;						//();
virt_COM_Close						*pt_COM_Close;						//();
virt_BL_SetSpeed					*pt_BL_SetSpeed;					//(DWORD speed);
virt_BL_Init_BL					*pt_BL_Init_BL;					//();
virt_BL_GET						*pt_BL_GET;						//(LPBYTE Version, LPCommands pCmds);                      
virt_BL_GET_VER_ROPS				*pt_BL_GET_VER_ROPS;				//(LPBYTE Version, LPBYTE ROPEnabled, LPBYTE ROPDisabled);             
virt_BL_GET_ID					*pt_BL_GET_ID;						//(LPBYTE size, LPBYTE pID);                   
virt_BL_READ						*pt_BL_READ;						//(DWORD Address, BYTE Size, LPBYTE pData);                     
virt_BL_GO						*pt_BL_GO;							//(DWORD Address);                       
virt_BL_WRITE						*pt_BL_WRITE;						//(DWORD address, BYTE size, LPBYTE pData);                    
virt_BL_ERASE						*pt_BL_ERASE;						//(BYTE NbSectors, LPBYTE pSectors);                     
virt_BL_WRITE_PROTECT				*pt_BL_WRITE_PROTECT;				//(BYTE NbSectors, LPBYTE pSectors);            
virt_BL_WRITE_TEMP_UNPROTECT		*pt_BL_WRITE_TEMP_UNPROTECT;		//();     
virt_BL_WRITE_PERM_UNPROTECT		*pt_BL_WRITE_PERM_UNPROTECT;		//();     
virt_BL_READOUT_PROTECT			*pt_BL_READOUT_PROTECT;			//();     
virt_BL_READOUT_TEMP_UNPROTECT	*pt_BL_READOUT_TEMP_UNPROTECT;		//();   
virt_BL_READOUT_PERM_UNPROTECT	*pt_BL_READOUT_PERM_UNPROTECT;		//();   
virt_BL_UPLOAD					*pt_BL_UPLOAD;						//(DWORD Address, LPBYTE pData, DWORD Length);  
virt_BL_VERIFY					*pt_BL_VERIFY;						//(DWORD Address, LPBYTE pData, DWORD Length,BOOL bTruncateLeadFFForDnLoad); 
virt_BL_DNLOAD					*pt_BL_DNLOAD;						//(DWORD Address, LPBYTE pData, DWORD Length,BOOL bTruncateLeadFFForDnLoad);   
virt_SetPaketSize					*pt_SetPaketSize;					//(BYTE size);
virt_GetPaketSize					*pt_GetPaketSize;					//(LPBYTE size);
virt_GetAckValue					*pt_GetAckValue;						//();
virt_COM_is_Open					*pt_COM_is_Open;						//();
virt_SetTimeOut						*pt_SetTimeOut;						//(DWORD vms);
virt_TARGET_GetUserOptionByte		*pt_TARGET_GetUserOptionByte;		//(LPBYTE User);
virt_TARGET_GetDataOptionByte		*pt_TARGET_GetDataOptionByte;		//(LPBYTE Data0, LPBYTE Data1);
virt_TARGET_SetOptionBytes			*pt_TARGET_SetOptionBytes;				//(BYTE User, BYTE RDP, BYTE Data0, BYTE Data1, BYTE WRP0, BYTE WRP1, BYTE WRP2, BYTE WRP3);
virt_TARGET_GetOptionBytes			*pt_TARGET_GetOptionBytes;				//(LPBYTE User, LPBYTE RDP, LPBYTE Data0, LPBYTE Data1, LPBYTE WRP0, LPBYTE WRP1, LPBYTE WRP2, LPBYTE WRP3);
virt_BL_SetRts					*pt_BL_SetRts;						//(BOOL val);
virt_BL_SetDtr					*pt_BL_SetDtr;						//(BOOL val);
virt_BL_setTxd					*pt_BL_setTxd;						//(BOOL val);	
virt_BL_getCts					*pt_BL_getCts;						//(BOOL* pval);					
virt_BL_getDtr					*pt_BL_getDtr;						//(BOOL* pval);	
virt_BL_getRi						*pt_BL_getRi;						//(BOOL* pval);					
virt_BL_getCd						*pt_BL_getCd;						//(BOOL* pval);
virt_BL_SetEcho                   *pt_BL_SetEcho ;                  //(int val);
virt_BL_SetFlowControl            *pt_BL_SetFlowControl ;                  //(bool val);




DWORD   MAX_DATA_SIZE       = 0xFF;   // Packet size(in byte)
BYTE    ACK                 = 0x79;
BYTE    NACK                = 0x1F;

ACKS ACK_VALUE          = ST79;
LPTARGET_DESCRIPTOR Target;
//CRS232 Cur_COM;
ACKS   McuTarget;
DWORD  Progress;
DWORD  ActivityTime ;

HINSTANCE BL_Lib_Hdle = NULL;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH :{
			                        McuTarget = UNDEFINED;
								    Target = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));
								    
									Target->Version                    = 0x00;
									Target->CmdCount                   = 0x00;
									Target->PIDLen                     = 0x00;
									Target->PID                        = (LPBYTE)malloc(1);
									Target->ROPE                       = 0x00;
									Target->ROPD                       = 0x00; 

									Target->GET_CMD                    = FALSE    ; //Get the version and the allowed commands supported by the current version of the boot loader
									Target->GET_VER_ROPS_CMD           = FALSE    ; //Get the BL version and the Read Protection status of the NVM
									Target->GET_ID_CMD                 = FALSE    ; //Get the chip ID
									Target->READ_CMD                   = FALSE    ; //Read up to 256 bytes of memory starting from an address specified by the user
									Target->GO_CMD                     = FALSE    ; //Jump to an address specified by the user to execute (a loaded) code
									Target->WRITE_CMD                  = FALSE    ; //Write maximum 256 bytes to the RAM or the NVM starting from an address specified by the user
									Target->ERASE_CMD                  = FALSE    ; //Erase from one to all the NVM sectors
									Target->ERASE_EXT_CMD              = FALSE    ; //Erase from one to all the NVM sectors
									Target->WRITE_PROTECT_CMD          = FALSE    ; //Enable the write protection in a permanent way for some sectors
									Target->WRITE_TEMP_UNPROTECT_CMD   = FALSE    ; //Disable the write protection in a temporary way for all NVM sectors
									Target->WRITE_PERM_UNPROTECT_CMD   = FALSE    ; //Disable the write protection in a permanent way for all NVM sectors
									Target->READOUT_PERM_PROTECT_CMD   = FALSE    ; //Enable the readout protection in a permanent way
									Target->READOUT_TEMP_UNPROTECT_CMD = FALSE    ; //Disable the readout protection in a temporary way
									Target->READOUT_PERM_UNPROTECT_CMD = FALSE    ; //Disable the readout protection in a permanent way
								 }break;
		case DLL_THREAD_ATTACH  :{ 
			                        /*McuTarget = UNDEFINED;
								    Target = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

								   	Target->Version                    = 0x00;
									Target->CmdCount                   = 0x00;
									Target->PIDLen                     = 0x00;
									Target->PID                        = (LPBYTE)malloc(1);
									Target->ROPE                       = 0x00;
									Target->ROPD                       = 0x00; 

									Target->GET_CMD                    = FALSE    ; //Get the version and the allowed commands supported by the current version of the boot loader
									Target->GET_VER_ROPS_CMD           = FALSE    ; //Get the BL version and the Read Protection status of the NVM
									Target->GET_ID_CMD                 = FALSE    ; //Get the chip ID
									Target->READ_CMD                   = FALSE    ; //Read up to 256 bytes of memory starting from an address specified by the user
									Target->GO_CMD                     = FALSE    ; //Jump to an address specified by the user to execute (a loaded) code
									Target->WRITE_CMD                  = FALSE    ; //Write maximum 256 bytes to the RAM or the NVM starting from an address specified by the user
									Target->ERASE_CMD                  = FALSE    ; //Erase from one to all the NVM sectors
									Target->WRITE_PROTECT_CMD          = FALSE    ; //Enable the write protection in a permanent way for some sectors
									Target->WRITE_TEMP_UNPROTECT_CMD   = FALSE    ; //Disable the write protection in a temporary way for all NVM sectors
									Target->WRITE_PERM_UNPROTECT_CMD   = FALSE    ; //Disable the write protection in a permanent way for all NVM sectors
									Target->READOUT_PERM_PROTECT_CMD   = FALSE    ; //Enable the readout protection in a permanent way
									Target->READOUT_TEMP_UNPROTECT_CMD = FALSE    ; //Disable the readout protection in a temporary way
									Target->READOUT_PERM_UNPROTECT_CMD = FALSE    ; //Disable the readout protection in a permanent way
								 */}break;
		case DLL_THREAD_DETACH  :{}break;
		case DLL_PROCESS_DETACH :{}break;
    }
    return TRUE;
}

/************************************************************************************/
/* SET COMMUNICATION INTERFACE TYPE
/* UART - CAN - ...
/*
/************************************************************************************/
BLLIB_API BYTE TARGET_SetComIntType(BYTE com_int_type)
{
	//com_int_type = 0; //  This is reserved for Future

	switch (com_int_type)
	{
		case 0:{
					BL_Lib_Hdle = GetModuleHandle("SIMBLLIB");
					if ( BL_Lib_Hdle == NULL )
					{
						BL_Lib_Hdle = LoadLibrary("SIMBLLIB") ;
					}
		   }break;
		case 1:{
					BL_Lib_Hdle = GetModuleHandle("USBHIDBLLIB");
					if ( BL_Lib_Hdle == NULL )
					{
						BL_Lib_Hdle = LoadLibrary("USBHIDBLLIB") ;
					}
		   }break;
	}


	pt_GetProgress					= (virt_GetProgress*)GetProcAddress(BL_Lib_Hdle,"GetProgress");
	pt_GetActivityTime 				= (virt_GetActivityTime*)GetProcAddress(BL_Lib_Hdle,"GetActivityTime");			//(LPDWORD time);  
	pt_SetActivityTime				= (virt_SetActivityTime*)GetProcAddress(BL_Lib_Hdle,"SetActivityTime");				//(DWORD time); 
	pt_TARGET_GetFlashSize			= (virt_TARGET_GetFlashSize*)GetProcAddress(BL_Lib_Hdle,"TARGET_GetFlashSize");
	pt_TARGET_GetMemoryAddress		= (virt_TARGET_GetMemoryAddress*)GetProcAddress(BL_Lib_Hdle,"TARGET_GetMemoryAddress");
	pt_TARGET_GetRDPOptionByte		= (virt_TARGET_GetRDPOptionByte*)GetProcAddress(BL_Lib_Hdle,"TARGET_GetRDPOptionByte");
	pt_TARGET_GetWRPOptionBytes		= (virt_TARGET_GetWRPOptionBytes*)GetProcAddress(BL_Lib_Hdle,"TARGET_GetWRPOptionBytes");
	pt_Send_RQ						= (virt_Send_RQ*)GetProcAddress(BL_Lib_Hdle,"Send_RQ");
	pt_SetBLMode					= (virt_SetBLMode*)GetProcAddress(BL_Lib_Hdle,"SetBLMode");
	pt_SetCOMSettings				= (virt_SetCOMSettings*)GetProcAddress(BL_Lib_Hdle,"SetCOMSettings");
	pt_COM_Open						= (virt_COM_Open*)GetProcAddress(BL_Lib_Hdle,"COM_Open");
	pt_COM_Close					= (virt_COM_Close*)GetProcAddress(BL_Lib_Hdle,"COM_Close");
	pt_BL_SetSpeed				= (virt_BL_SetSpeed*)GetProcAddress(BL_Lib_Hdle,"BL_SetSpeed");
	pt_BL_Init_BL					= (virt_BL_Init_BL*)GetProcAddress(BL_Lib_Hdle,"BL_Init_BL");
	pt_BL_GET						= (virt_BL_GET*)GetProcAddress(BL_Lib_Hdle,"BL_GET");                   
	pt_BL_GET_VER_ROPS			= (virt_BL_GET_VER_ROPS*)GetProcAddress(BL_Lib_Hdle,"BL_GET_VER_ROPS");          
	pt_BL_GET_ID					= (virt_BL_GET_ID*)GetProcAddress(BL_Lib_Hdle,"BL_GET_ID");              
	pt_BL_READ					= (virt_BL_READ*)GetProcAddress(BL_Lib_Hdle,"BL_READ");                   
	pt_BL_GO						= (virt_BL_GO*)GetProcAddress(BL_Lib_Hdle,"BL_GO");                    
	pt_BL_WRITE					= (virt_BL_WRITE*)GetProcAddress(BL_Lib_Hdle,"BL_WRITE");                  
	pt_BL_ERASE					= (virt_BL_ERASE*)GetProcAddress(BL_Lib_Hdle,"BL_ERASE");     
	pt_BL_WRITE_PROTECT			= (virt_BL_WRITE_PROTECT*)GetProcAddress(BL_Lib_Hdle,"BL_WRITE_PROTECT");          
	pt_BL_WRITE_TEMP_UNPROTECT	= (virt_BL_WRITE_TEMP_UNPROTECT*)GetProcAddress(BL_Lib_Hdle,"BL_WRITE_TEMP_UNPROTECT"); 
	pt_BL_WRITE_PERM_UNPROTECT	= (virt_BL_WRITE_PERM_UNPROTECT*)GetProcAddress(BL_Lib_Hdle,"BL_WRITE_PERM_UNPROTECT"); 
	pt_BL_READOUT_PROTECT			= (virt_BL_READOUT_PROTECT*)GetProcAddress(BL_Lib_Hdle,"BL_READOUT_PROTECT");     
	pt_BL_READOUT_TEMP_UNPROTECT	= (virt_BL_READOUT_TEMP_UNPROTECT*)GetProcAddress(BL_Lib_Hdle,"BL_READOUT_TEMP_UNPROTECT"); 
	pt_BL_READOUT_PERM_UNPROTECT	= (virt_BL_READOUT_PERM_UNPROTECT*)GetProcAddress(BL_Lib_Hdle,"BL_READOUT_PERM_UNPROTECT");
	pt_BL_UPLOAD					= (virt_BL_UPLOAD*)GetProcAddress(BL_Lib_Hdle,"BL_UPLOAD");
	pt_BL_VERIFY					= (virt_BL_VERIFY*)GetProcAddress(BL_Lib_Hdle,"BL_VERIFY");
	pt_BL_DNLOAD					= (virt_BL_DNLOAD*)GetProcAddress(BL_Lib_Hdle,"BL_DNLOAD"); 
	pt_SetPaketSize					= (virt_SetPaketSize*)GetProcAddress(BL_Lib_Hdle,"SetPaketSize");
	pt_GetPaketSize					= (virt_GetPaketSize*)GetProcAddress(BL_Lib_Hdle,"GetPaketSize");
	pt_GetAckValue					= (virt_GetAckValue*)GetProcAddress(BL_Lib_Hdle,"GetAckValue");
	pt_COM_is_Open					= (virt_COM_is_Open*)GetProcAddress(BL_Lib_Hdle,"COM_is_Open");
	pt_SetTimeOut					= (virt_SetTimeOut*)GetProcAddress(BL_Lib_Hdle,"SetTimeOut");
	pt_TARGET_GetUserOptionByte		= (virt_TARGET_GetUserOptionByte*)GetProcAddress(BL_Lib_Hdle,"TARGET_GetUserOptionByte");
	pt_TARGET_GetDataOptionByte		= (virt_TARGET_GetDataOptionByte*)GetProcAddress(BL_Lib_Hdle,"TARGET_GetDataOptionByte");
	pt_TARGET_SetOptionBytes		= (virt_TARGET_SetOptionBytes*)GetProcAddress(BL_Lib_Hdle,"TARGET_SetOptionBytes");
	pt_TARGET_GetOptionBytes		= (virt_TARGET_GetOptionBytes*)GetProcAddress(BL_Lib_Hdle,"TARGET_GetOptionBytes");
	pt_BL_SetRts					= (virt_BL_SetRts*)GetProcAddress(BL_Lib_Hdle,"BL_SetRts");
	pt_BL_SetDtr					= (virt_BL_SetDtr*)GetProcAddress(BL_Lib_Hdle,"BL_SetDtr");
	pt_BL_setTxd					= (virt_BL_setTxd*)GetProcAddress(BL_Lib_Hdle,"BL_setTxd");
	pt_BL_getCts					= (virt_BL_getCts*)GetProcAddress(BL_Lib_Hdle,"BL_getCts");				
	pt_BL_getDtr					= (virt_BL_getDtr*)GetProcAddress(BL_Lib_Hdle,"BL_getDtr");
	pt_BL_getRi					= (virt_BL_getRi*)GetProcAddress(BL_Lib_Hdle,"BL_getRi");				
	pt_BL_getCd					= (virt_BL_getCd*)GetProcAddress(BL_Lib_Hdle,"BL_getCd");
    pt_BL_SetEcho					= (virt_BL_SetEcho*)GetProcAddress(BL_Lib_Hdle,"BL_SetEcho");
	pt_BL_SetFlowControl			= (virt_BL_SetFlowControl*)GetProcAddress(BL_Lib_Hdle,"BL_SetFlowControl");
	return 0;
}

BLLIB_API BYTE SetBLMode(BYTE DLmode, BYTE MCUcom)
{
	EXTEND_CFG cfg;
	if(pt_SetCOMSettings)
	{
		cfg.MODE_DLmode=DLmode;
		cfg.MODE_MCUcom=MCUcom;
		cfg.MODE_reserved1=0;
		cfg.MODE_reserved2=0;
		return BL_WRITE(MODE_CFG_BASE,MODE_CFG_SIZE,(LPBYTE)&cfg);
	}
	else
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/*Set the communication settings for UART, CAN, ...
/* UART - numPort, speedInBaud, nbBit,  parity, nbStopBit
/* CAN  - only : speedInBaud
/************************************************************************************/
BLLIB_API BYTE SetCOMSettings(int numPort, long speedInBaud, int nbBit,
	                            int parity, float nbStopBit)
{
	BYTE result;
	EXTEND_CFG cfg;
	if(pt_SetCOMSettings)
	{
		result=pt_SetCOMSettings(numPort, speedInBaud, nbBit, parity, nbStopBit);
		if(result==SUCCESS)
		{
			cfg.UART_speedInBaud=speedInBaud;
			cfg.UART_nbBit=nbBit;
			cfg.UART_parity=parity;
			cfg.UART_nbStopBit=nbStopBit;
			cfg.UART_reserved1=0;
			result=BL_WRITE(UART_CFG_BASE,UART_CFG_SIZE,(LPBYTE)&cfg.UART_speedInBaud);
		}
		return result;
	}
	else 
		return LIB_LOADING_ERROR;
}

BLLIB_API BYTE Send_RQ(LPBL_Request pRQ)
{
	if(pt_Send_RQ)
		return pt_Send_RQ(pRQ);
	else 
		return LIB_LOADING_ERROR;	
}

BLLIB_API BYTE COM_Open()
{
	if(pt_COM_Open)
		return pt_COM_Open();
	else 
		return LIB_LOADING_ERROR;
}

BLLIB_API BYTE COM_Close()
{
	if(pt_COM_Close)
		return pt_COM_Close();
	else 
		return LIB_LOADING_ERROR;
}

BLLIB_API BYTE BL_SetSpeed(DWORD speed)
{
	if(pt_BL_SetSpeed)
		return pt_BL_SetSpeed(speed);
	else 
		return LIB_LOADING_ERROR;
}

BLLIB_API BYTE BL_Init_BL()
{
	if(pt_BL_Init_BL)
		return pt_BL_Init_BL();
	else 
		return LIB_LOADING_ERROR;
}
/******************************************************************************************/
/* Boot Loader commands implementation                     
/******************************************************************************************/                                                                                   
BLLIB_API BYTE BL_GET(LPBYTE Version, LPCommands pCmds)
{
	if(pt_BL_GET)
		return pt_BL_GET(Version, pCmds);
	else 
		return LIB_LOADING_ERROR;
}
                                                                                    
BLLIB_API BYTE BL_GET_VER_ROPS(LPBYTE Version, LPBYTE ROPEnabled, LPBYTE ROPDisabled)
{
	if(pt_BL_GET_VER_ROPS)
		return pt_BL_GET_VER_ROPS(Version, ROPEnabled, ROPDisabled);
	else 
		return LIB_LOADING_ERROR;
}              
                                                                                    
BLLIB_API BYTE BL_GET_ID(LPBYTE size, LPBYTE pID) 
{
	if(pt_BL_GET_ID)
		return pt_BL_GET_ID(size, pID);
	else 
		return LIB_LOADING_ERROR;
}                   
                                                                                     
BLLIB_API BYTE BL_READ(DWORD Address, BYTE Size, LPBYTE pData) 
{
	if(pt_BL_READ)
		return pt_BL_READ(Address, Size, pData); 
	else 
		return LIB_LOADING_ERROR;
}                     
                                                                                    
BLLIB_API BYTE BL_GO(DWORD Address) 
{
	if(pt_BL_GO)
		return pt_BL_GO(Address); 
	else 
		return LIB_LOADING_ERROR;
}                       
                                                                                     
BLLIB_API BYTE BL_WRITE(DWORD address, BYTE size, LPBYTE pData)
{
	if(pt_BL_WRITE)
		return pt_BL_WRITE(address, size, pData);
	else 
		return LIB_LOADING_ERROR;
}                     
                                                                                    
BLLIB_API BYTE BL_ERASE(WORD NbSectors, LPBYTE pSectors)
{
	if(pt_BL_ERASE)
		return pt_BL_ERASE(NbSectors, pSectors);
	else 
		return LIB_LOADING_ERROR;
}    
                  
                                                                                    
BLLIB_API BYTE BL_WRITE_PROTECT(BYTE NbSectors, LPBYTE pSectors)
{
	if(pt_BL_WRITE_PROTECT)
		return pt_BL_WRITE_PROTECT(NbSectors, pSectors);
	else 
		return LIB_LOADING_ERROR;
}             
                                                                                    
BLLIB_API BYTE BL_WRITE_TEMP_UNPROTECT() 
{
	if(pt_BL_WRITE_TEMP_UNPROTECT)
		return pt_BL_WRITE_TEMP_UNPROTECT(); 
	else 
		return LIB_LOADING_ERROR;
}     
                                                                                    
BLLIB_API BYTE BL_WRITE_PERM_UNPROTECT() 
{
	if(pt_BL_WRITE_PERM_UNPROTECT)
		return pt_BL_WRITE_PERM_UNPROTECT(); 
	else 
		return LIB_LOADING_ERROR;
}     
                                                                                    
BLLIB_API BYTE BL_READOUT_PROTECT() 
{
	if(pt_BL_READOUT_PROTECT)
		return pt_BL_READOUT_PROTECT(); 
	else 
		return LIB_LOADING_ERROR;
}     
                                                                                    
BLLIB_API BYTE BL_READOUT_TEMP_UNPROTECT()  
{
	if(pt_BL_READOUT_TEMP_UNPROTECT)
		return pt_BL_READOUT_TEMP_UNPROTECT();  
	else 
		return LIB_LOADING_ERROR;
}  
    
/************************************************************************************/
/* READOUT_PERM_UNPROTECT
/*
/*
/************************************************************************************/                                                                                 
BLLIB_API BYTE BL_READOUT_PERM_UNPROTECT()  
{
    if(pt_BL_READOUT_PERM_UNPROTECT)
		return pt_BL_READOUT_PERM_UNPROTECT();  
	else 
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/* UPLOAD
/*
/*
/************************************************************************************/
BLLIB_API BYTE BL_UPLOAD(DWORD Address, LPBYTE pData, DWORD Length)
{
	if(pt_BL_UPLOAD)
		return pt_BL_UPLOAD(Address, pData, Length);
	else 
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/* VERIFY
/*
/*
/************************************************************************************/
BLLIB_API BYTE BL_VERIFY(DWORD Address, LPBYTE pData, DWORD Length,BOOL bTruncateLeadFFForDnLoad)
{
	if(pt_BL_VERIFY)
		return pt_BL_VERIFY(Address, pData, Length,bTruncateLeadFFForDnLoad);
	else 
		return LIB_LOADING_ERROR;
}
/************************************************************************************/
/* DNLOAD - this command uses the BL_WRITE function to download a big block of data
/*
/*
/************************************************************************************/
BLLIB_API BYTE BL_DNLOAD(DWORD Address, LPBYTE pData, DWORD Length,BOOL bTruncateLeadFFForDnLoad)
{
	if(pt_BL_DNLOAD)
		return pt_BL_DNLOAD(Address, pData, Length,bTruncateLeadFFForDnLoad);
	else 
		return LIB_LOADING_ERROR;
}
/************************************************************************************/
/* SET PACKET SIZE
/*
/*
/************************************************************************************/
BLLIB_API BYTE SetPaketSize(BYTE size)
{
	if(pt_SetPaketSize)
		return pt_SetPaketSize(size);
	else 
		return LIB_LOADING_ERROR;
}
 
/************************************************************************************/
/* GET PACKET SIZE
/*
/*
/************************************************************************************/
BLLIB_API BYTE GetPaketSize(LPBYTE size)
{
	if(pt_GetPaketSize)
		return pt_GetPaketSize(size);
	else 
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/* GetAckValue
/*
/*
/************************************************************************************/
BLLIB_API ACKS GetAckValue()
{
  	if(pt_GetAckValue)
		return pt_GetAckValue();
	else 
		return UNDEFINED;	
}

/************************************************************************************/
/* IsConnected
/*
/*
/************************************************************************************/
BLLIB_API BOOL COM_is_Open()
{
	if(pt_COM_is_Open)
		return pt_COM_is_Open();
	else 
		return FALSE;
}

/************************************************************************************/
/* SetTimeOut
/*
/*
/************************************************************************************/
BLLIB_API BYTE SetTimeOut(DWORD vms)
{
    if(pt_SetTimeOut)
		return pt_SetTimeOut(vms);
	else 
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/* GetFlashSize
/*
/*
/************************************************************************************/
BLLIB_API BYTE TARGET_GetFlashSize(DWORD Addr, LPWORD val)
{
	if(pt_TARGET_GetFlashSize)
		return pt_TARGET_GetFlashSize(Addr, val);
	else 
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/* GetMemoryAddress
/*
/*
/************************************************************************************/
BLLIB_API BYTE TARGET_GetMemoryAddress(DWORD Addr, LPBYTE val)
{
	if(pt_TARGET_GetMemoryAddress)
		return pt_TARGET_GetMemoryAddress(Addr, val);
	else 
		return LIB_LOADING_ERROR;
}


/************************************************************************************/
/* GetRDPOptionByte
/*
/*
/************************************************************************************/
BLLIB_API BYTE TARGET_GetRDPOptionByte(LPBYTE RDP)
{
	if(pt_TARGET_GetRDPOptionByte)
		return pt_TARGET_GetRDPOptionByte(RDP);
	else 
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/* GetWRPOptionBytes
/*
/*
/************************************************************************************/
BLLIB_API BYTE TARGET_GetWRPOptionBytes(LPBYTE WRP0, LPBYTE WRP1, LPBYTE WRP2, LPBYTE WRP3)
{
	if(pt_TARGET_GetWRPOptionBytes)
		return pt_TARGET_GetWRPOptionBytes(WRP0, WRP1, WRP2, WRP3);
	else 
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/* GetUserOptionByte
/*
/*
/************************************************************************************/
BLLIB_API BYTE TARGET_GetUserOptionByte(LPBYTE User)
{
	if(pt_TARGET_GetUserOptionByte)
		return pt_TARGET_GetUserOptionByte(User);
	else 
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/* GetDataOptionByte
/*
/*
/************************************************************************************/
BLLIB_API BYTE TARGET_GetDataOptionByte(LPBYTE Data0, LPBYTE Data1)
{
	if(pt_TARGET_GetDataOptionByte)
		return pt_TARGET_GetDataOptionByte(Data0, Data1);
	else 
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/* SetOptionBytes
/*
/*
/************************************************************************************/
BLLIB_API BYTE TARGET_SetOptionBytes(DWORD Addr,BYTE User, BYTE RDP, BYTE Data0, BYTE Data1, 
								   BYTE WRP0, BYTE WRP1, BYTE WRP2, BYTE WRP3)
{
	if(pt_TARGET_SetOptionBytes)
		return pt_TARGET_SetOptionBytes(Addr,User, RDP, Data0, Data1, WRP0, WRP1, WRP2, WRP3);
	else 
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/* GetOptionBytes
/*
/*
/************************************************************************************/
BLLIB_API BYTE TARGET_GetOptionBytes(DWORD Addr,LPBYTE User, LPBYTE RDP, LPBYTE Data0, LPBYTE Data1, 
								   LPBYTE WRP0, LPBYTE WRP1, LPBYTE WRP2, LPBYTE WRP3)
{
	if(pt_TARGET_GetOptionBytes)
		return pt_TARGET_GetOptionBytes(Addr,User, RDP, Data0, Data1, WRP0, WRP1, WRP2, WRP3);
	else 
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/* GET PROGRESS STATE
/*
/*
/************************************************************************************/
BLLIB_API BYTE GetProgress(LPBYTE progress)
{
   	if(pt_GetProgress)
		return pt_GetProgress(progress);
	else 
		return LIB_LOADING_ERROR;
}
/************************************************************************************/
/* Get activity time
/*
/*
/************************************************************************************/
BLLIB_API BYTE GetActivityTime(LPDWORD time)
{
  	if(pt_GetActivityTime)
		return pt_GetActivityTime(time);
	else 
		return LIB_LOADING_ERROR;
}  

/************************************************************************************/
/* Set activity time
/*
/*
/************************************************************************************/
BLLIB_API BYTE SetActivityTime(DWORD time)
{
   	if(pt_SetActivityTime)
		return pt_SetActivityTime(time);
	else 
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/*   Set Rts Line State
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
BLLIB_API BYTE BL_SetRts(bool Val)
{
	if(pt_BL_SetRts)
		return pt_BL_SetRts(Val);
	else 
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/*   Set Dtr Line State
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
BLLIB_API BYTE BL_SetDtr(bool Val)
{
    if(pt_Send_RQ)
		return pt_BL_SetDtr(Val);
	else 
		return LIB_LOADING_ERROR;
}

/************************************************************************************/
/*   Set the state of TXD. Return: true if success.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
BLLIB_API BYTE BL_setTxd(bool val)
{
	if(pt_BL_setTxd)
		return pt_BL_setTxd(val);
	else 
		return LIB_LOADING_ERROR;
}
		
/************************************************************************************/
/*   Return: The state of CTS.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
BLLIB_API BYTE BL_getCts(bool* pval)	
{
   	if(pt_BL_getCts)
		return pt_BL_getCts(pval);
	else 
		return LIB_LOADING_ERROR;
}				
			
/************************************************************************************/
/*   Return: The state of DTR.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
BLLIB_API BYTE BL_getDtr(bool* pval)	
{
  	if(pt_BL_getDtr)
		return pt_BL_getDtr(pval);
	else 
		return LIB_LOADING_ERROR;
}
					
/************************************************************************************/
/*   Return: The state of RI.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
BLLIB_API BYTE BL_getRi(bool* pval)
{
   	if(pt_BL_getRi)
		return pt_BL_getRi(pval);
	else 
		return LIB_LOADING_ERROR;
}					
	
/************************************************************************************/
/*   Return: The state of DTR.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
BLLIB_API BYTE BL_getCd(bool* pval)	
{
   	if(pt_BL_getCd)
		return pt_BL_getCd(pval);
	else 
		return LIB_LOADING_ERROR;
}



/************************************************************************************/
/* Set Echo back Mode
/*   0   =  Echo Disabled
/*   1   =  Echo Back Enabled
/*   2   =  Listen Echo Enabled
/************************************************************************************/
BLLIB_API BYTE BL_SetEcho(int val)
{

	if(pt_BL_SetEcho)
		return pt_BL_SetEcho(val);
	else 
		return LIB_LOADING_ERROR;
}


/************************************************************************************/
/* SetFlowControl : Enable/Disable Flow Control of DTR and RTS
/* FALSE   =   Disabled
/* TRUE    =   Enabled
/************************************************************************************/
BLLIB_API BYTE BL_SetFlowControl(bool val)
{

	if(pt_BL_SetFlowControl)
		return pt_BL_SetFlowControl(val);
	else 
		return LIB_LOADING_ERROR;
}

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE******/


