/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : SIMBLLIB.h
* Author             : MCD Application Team
* Version            : v2.8.0
* Date               : 01-September-2015
* Description        : Defines the SIM  boot loader protocol interface
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
********************************************************************************
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE
* "MCD-ST Liberty SW License Agreement V2.pdf"
*******************************************************************************/

#ifndef DLIB_H
#define DLIB_H

#include "StdAfx.h"
#include "sim.h"

#ifdef SIMBLLIB_EXPORTS
#define SIMBLLIB_API __declspec(dllexport)
#else
#define SIMBLLIB_API __declspec(dllimport)
#endif

const BYTE INIT_CON                    = 0x7F;

const BYTE GET_CMD                     = 0x00; //Get the version and the allowed commands supported by the current version of the boot loader
const BYTE GET_VER_ROPS_CMD            = 0x01; //Get the BL version and the Read Protection status of the NVM
const BYTE GET_ID_CMD                  = 0x02; //Get the chip ID
const BYTE SET_SPEED_CMD               = 0x03; //set the new baudrate
const BYTE READ_CMD                    = 0x11; //Read up to 256 bytes of memory starting from an address specified by the user
const BYTE GO_CMD                      = 0x21; //Jump to an address specified by the user to execute (a loaded) code
const BYTE WRITE_CMD                   = 0x31; //Write maximum 256 bytes to the RAM or the NVM starting from an address specified by the user
const BYTE ERASE_CMD                   = 0x43; //Erase from one to all the NVM sectors
const BYTE ERASE_EXT_CMD               = 0x44; //Erase from one to all the NVM sectors
const BYTE WRITE_PROTECT_CMD           = 0x63; //Enable the write protection in a permanent way for some sectors
const BYTE WRITE_TEMP_UNPROTECT_CMD    = 0x71; //Disable the write protection in a temporary way for all NVM sectors
const BYTE WRITE_PERM_UNPROTECT_CMD    = 0x73; //Disable the write protection in a permanent way for all NVM sectors
const BYTE READOUT_PROTECT_CMD         = 0x82; //Enable the readout protection in a permanent way
const BYTE READOUT_TEMP_UNPROTECT_CMD  = 0x91; //Disable the readout protection in a temporary way
const BYTE READOUT_PERM_UNPROTECT_CMD  = 0x92; //Disable the readout protection in a permanent way


const BYTE SUCCESS                     = 0x00; // No error 
const BYTE ERROR_OFFSET                = 0x00; //error offset 

const BYTE COM_ERROR_OFFSET            = ERROR_OFFSET + 0x00;
const BYTE NO_CON_AVAILABLE            = COM_ERROR_OFFSET + 0x01;  // No serial port opened
const BYTE COM_ALREADY_OPENED          = COM_ERROR_OFFSET + 0x02;  // Serial port already opened
const BYTE CANT_OPEN_COM               = COM_ERROR_OFFSET + 0x03;  // Fail to open serial port
const BYTE SEND_FAIL                   = COM_ERROR_OFFSET + 0x04;  // send over serial port fail
const BYTE READ_FAIL                   = COM_ERROR_OFFSET + 0x05;  // Read from serial port fail

const BYTE SYS_MEM_ERROR_OFFSET        = ERROR_OFFSET + 0x10;
const BYTE CANT_INIT_BL                = SYS_MEM_ERROR_OFFSET + 0x01; // Fail to start system memory BL
const BYTE UNREOGNIZED_DEVICE          = SYS_MEM_ERROR_OFFSET + 0x02; // Unreconized device
const BYTE CMD_NOT_ALLOWED             = SYS_MEM_ERROR_OFFSET + 0x03; // Command not allowed
const BYTE CMD_FAIL                    = SYS_MEM_ERROR_OFFSET + 0x04; // command failed

const BYTE PROGRAM_ERROR_OFFSET        = ERROR_OFFSET + 0x20;
const BYTE INPUT_PARAMS_ERROR          = PROGRAM_ERROR_OFFSET + 0x01;
const BYTE INPUT_PARAMS_MEMORY_ALLOCATION_ERROR = PROGRAM_ERROR_OFFSET + 0x02;



enum  ACKS {UNDEFINED=0x00, ST75=0x75, ST79=0x79};
enum  INTERFACE_TYPE {SIM, CAN};

enum  EBaudRate { brCustom,br110, br300, br600, br1200, br2400, br4800, br9600, br14400, br19200, br38400,
                  br56000, br57600, br115200, br128000, br256000 };// Port Numbers ( custom or COM1..COM16 }
enum  EPortNumber { pnCustom,pnCOM1, pnCOM2, pnCOM3, pnCOM4, pnCOM5, pnCOM6, pnCOM7,pnCOM8, pnCOM9, pnCOM10, 
                    pnCOM11, pnCOM12, pnCOM13,pnCOM14, pnCOM15, pnCOM16 };// Data bits ( 5, 6, 7, 8 }
enum  EDataBits { db5BITS, db6BITS, db7BITS, db8BITS };
// Stop bits ( 1, 1.5, 2 }
enum  EStopBits { sb1BITS, sb1HALFBITS, sb2BITS };
// Parity ( None, odd, even, mark, space }
enum  EParity { ptNONE, ptODD, ptEVEN, ptMARK, ptSPACE };
// Hardware Flow Control ( None, None + RTS always on, RTS/CTS }
enum  EHwFlowControl { hfNONE, hfNONERTSON, hfRTSCTS };
// Software Flow Control ( None, XON/XOFF }
enum  ESwFlowControl { sfNONE, sfXONXOFF };
// What to do with incomplete (incoming} packets ( Discard, Pass }
enum  EPacketMode { pmDiscard, pmPass };

enum  OPERATION {NONE, ERASE, UPLOAD, DNLOAD, DIS_R_PROT, DIS_W_PROT, ENA_R_PROT, ENA_W_PROT};

typedef struct RESULT
{
	OPERATION operation;
	char*     filename;
    HANDLE    Image;
}* LPRESULT;

typedef struct Commands
{
	BOOL GET_CMD                    ; //Get the version and the allowed commands supported by the current version of the boot loader
    BOOL GET_VER_ROPS_CMD           ; //Get the BL version and the Read Protection status of the NVM
    BOOL GET_ID_CMD                 ; //Get the chip ID
	BOOL SET_SPEED_CMD                   ; //Change the CAN baudrate
    BOOL READ_CMD                   ; //Read up to 256 bytes of memory starting from an address specified by the user
    BOOL GO_CMD                     ; //Jump to an address specified by the user to execute (a loaded) code
    BOOL WRITE_CMD                  ; //Write maximum 256 bytes to the RAM or the NVM starting from an address specified by the user
    BOOL ERASE_CMD                  ; //Erase from one to all the NVM sectors
	BOOL ERASE_EXT_CMD              ; //Erase from one to all the NVM sectors
    BOOL WRITE_PROTECT_CMD          ; //Enable the write protection in a permanent way for some sectors
    BOOL WRITE_TEMP_UNPROTECT_CMD   ; //Disable the write protection in a temporary way for all NVM sectors
    BOOL WRITE_PERM_UNPROTECT_CMD   ; //Disable the write protection in a permanent way for all NVM sectors
    BOOL READOUT_PROTECT_CMD   ; //Enable the readout protection in a permanent way
    BOOL READOUT_TEMP_UNPROTECT_CMD ; //Disable the readout protection in a temporary way
    BOOL READOUT_PERM_UNPROTECT_CMD ; //Disable the readout protection in a permanent way
}* LPCommands;

typedef struct TARGET_DESCRIPTOR
{
	BYTE Version                    ;
	BYTE CmdCount                   ;
	BYTE PIDLen                     ;
	BYTE* PID                       ;

	BYTE ROPE                       ;
	BYTE ROPD                       ;

	BOOL GET_CMD                    ; //Get the version and the allowed commands supported by the current version of the boot loader
    BOOL GET_VER_ROPS_CMD           ; //Get the BL version and the Read Protection status of the NVM
    BOOL GET_ID_CMD                 ; //Get the chip ID
    BOOL SET_SPEED_CMD              ;
	BOOL READ_CMD                   ; //Read up to 256 bytes of memory starting from an address specified by the user
    BOOL GO_CMD                     ; //Jump to an address specified by the user to execute (a loaded) code
    BOOL WRITE_CMD                  ; //Write maximum 256 bytes to the RAM or the NVM starting from an address specified by the user
    BOOL ERASE_CMD                  ; //Erase from one to all the NVM sectors
	BOOL ERASE_EXT_CMD              ; //Erase from one to all the NVM sectors
    BOOL WRITE_PROTECT_CMD          ; //Enable the write protection in a permanent way for some sectors
    BOOL WRITE_TEMP_UNPROTECT_CMD   ; //Disable the write protection in a temporary way for all NVM sectors
    BOOL WRITE_PERM_UNPROTECT_CMD   ; //Disable the write protection in a permanent way for all NVM sectors
    BOOL READOUT_PERM_PROTECT_CMD   ; //Enable the readout protection in a permanent way
    BOOL READOUT_TEMP_UNPROTECT_CMD ; //Disable the readout protection in a temporary way
    BOOL READOUT_PERM_UNPROTECT_CMD ; //Disable the readout protection in a permanent way
}* LPTARGET_DESCRIPTOR;

typedef struct BL_Request
{
	BYTE   _cmd;
	DWORD  _address;
	WORD   _length;
	BYTE   _nbSectors;
    LPTARGET_DESCRIPTOR _target;
	LPBYTE _data;
	WORD   _wbSectors;
}* LPBL_Request; 


extern "C"
{

/************************************************************************************/
/* SET COMMUNICATION INTERFACE TYPE
/* SIM - ...
/*
/************************************************************************************/
SIMBLLIB_API BYTE TARGET_SetComIntType(BYTE com_int_type);

/************************************************************************************/
/* GET PROGRESS STATE
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE GetProgress(LPBYTE progress);  

/************************************************************************************/
/* GET ACTIVITY TIME
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE GetActivityTime(LPDWORD time);  

/************************************************************************************/
/* SET ACTIVITY TIME
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE SetActivityTime(DWORD time); 

/************************************************************************************/
/* GetFlashSize
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE TARGET_GetFlashSize(DWORD Addr, LPWORD val);
/************************************************************************************/
/* GetRAMSize
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE TARGET_GetMemoryAddress(DWORD Addr, LPBYTE val);
/************************************************************************************/
/* GetRDPOptionByte
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE TARGET_GetRDPOptionByte(LPBYTE RDP);
/************************************************************************************/
/* GetWRPOptionBytes
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE TARGET_GetWRPOptionBytes(LPBYTE WRP0, LPBYTE WRP1, LPBYTE WRP2, LPBYTE WRP3);
/************************************************************************************/
/* Basic function to send a request
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE Send_RQ(LPBL_Request pRQ);

/************************************************************************************/
/*
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE SetCOMSettings(int numPort, long speedInBaud, int nbBit,
	                            int parity, float nbStopBit);
/************************************************************************************/
/*
/*
/*
/************************************************************************************/
/*SIMBLLIB_API BYTE ESetCOMSettings(EPortNumber numPort, EBaudRate speedInBaud, EDataBits nbBit,
	                            EParity parity, EStopBits nbStopBit);*/
/************************************************************************************/
/*
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE COM_Open();
/************************************************************************************/
/*
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE COM_Close();
/************************************************************************************/
/*
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_SetSpeed(DWORD speed);
/************************************************************************************/
/*
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_Init_BL();
/************************************************************************************/
/* 0x00; //Get the version and the allowed commands supported by the current version of the boot loader
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_GET(LPBYTE Version, LPCommands pCmds);                      
/************************************************************************************/
/* 0x01; //Get the BL version and the Read Protection status of the NVM
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_GET_VER_ROPS(LPBYTE Version, LPBYTE ROPEnabled, LPBYTE ROPDisabled);             
/************************************************************************************/
/* 0x02; //Get the chip ID
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_GET_ID(LPBYTE size, LPBYTE pID);                   
/************************************************************************************/
/* 0x11; //Read up to 256 bytes of memory starting from an address specified by the user
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_READ(DWORD Address, BYTE Size, LPBYTE pData);                     
/************************************************************************************/
/* 0x21; //Jump to an address specified by the user to execute (a loaded) code
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_GO(DWORD Address);                       
/************************************************************************************/
/* 0x31; //Write maximum 256 bytes to the RAM or the NVM starting from an address specified by the user
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_WRITE(DWORD address, BYTE size, LPBYTE pData);                    
/************************************************************************************/
/* 0x43; //Erase from one to all the NVM sectors
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_ERASE(WORD NbSectors, LPBYTE pSectors);                     
/************************************************************************************/
/* 0x63; //Enable the write protection in a permanent way for some sectors
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_WRITE_PROTECT(BYTE NbSectors, LPBYTE pSectors);            
/************************************************************************************/
/* 0x71; //Disable the write protection in a temporary way for all NVM sectors
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_WRITE_TEMP_UNPROTECT();     
/************************************************************************************/
/* 0x73; //Disable the write protection in a permanent way for all NVM sectors
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_WRITE_PERM_UNPROTECT();     
/************************************************************************************/
/* 0x82; //Enable the readout protection in a permanent way
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_READOUT_PROTECT();     
/************************************************************************************/
/* 0x91; //Disable the readout protection in a temporary way
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_READOUT_TEMP_UNPROTECT();   
/************************************************************************************/
/* 0x92; //Disable the readout protection in a permanent way
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_READOUT_PERM_UNPROTECT();   
/************************************************************************************/
/* UPLOAD
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_UPLOAD(DWORD Address, LPBYTE pData, DWORD Length);  
/************************************************************************************/
/* VERIFY
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_VERIFY(DWORD Address, LPBYTE pData, DWORD Length,BOOL bTruncateLeadFFForDnLoad); 
/************************************************************************************/
/* DNLOAD
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE BL_DNLOAD(DWORD Address, LPBYTE pData, DWORD Length,BOOL bTruncateLeadFFForDnLoad);   

/************************************************************************************/
/* SET PACKET SIZE
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE SetPaketSize(BYTE size);
 
/************************************************************************************/
/* GET PACKET SIZE
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE GetPaketSize(LPBYTE size);

/************************************************************************************/
/* GetAckValue
/*
/*
/************************************************************************************/
SIMBLLIB_API ACKS GetAckValue();

/************************************************************************************/
/* IsConnected
/*
/*
/************************************************************************************/
SIMBLLIB_API BOOL COM_is_Open();

/************************************************************************************/
/* SetTimeOut
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE SetTimeOut(DWORD vms);
/************************************************************************************/
/* GetUserOptionByte
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE TARGET_GetUserOptionByte(LPBYTE User);
/************************************************************************************/
/* GetDataOptionByte
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE TARGET_GetDataOptionByte(LPBYTE Data0, LPBYTE Data1);

/************************************************************************************/
/* SetSIFData
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE TARGET_SetOptionBytes(DWORD Addr,BYTE User, BYTE RDP, BYTE Data0, BYTE Data1, 
								   BYTE WRP0, BYTE WRP1, BYTE WRP2, BYTE WRP3);

/************************************************************************************/
/* GetSIFData
/*
/*
/************************************************************************************/
SIMBLLIB_API BYTE TARGET_GetOptionBytes(DWORD Addr,LPBYTE User, LPBYTE RDP, LPBYTE Data0, LPBYTE Data1, 
								   LPBYTE WRP0, LPBYTE WRP1, LPBYTE WRP2, LPBYTE WRP3);

/************************************************************************************/
/* Set Rts Line State
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
SIMBLLIB_API BYTE BL_SetRts(bool Val);

/************************************************************************************/
/* Set Dtr Line State 
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
SIMBLLIB_API BYTE BL_SetDtr(bool Val);

/************************************************************************************/
/*   Set the state of TXD. Return: true if success.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
SIMBLLIB_API BYTE BL_setTxd(bool val);	
		
/************************************************************************************/
/*   Return: The state of CTS.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
SIMBLLIB_API BYTE BL_getCts(bool* pval);					
			
/************************************************************************************/
/*   Return: The state of DTR.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
SIMBLLIB_API BYTE BL_getDtr(bool* pval);	
					
/************************************************************************************/
/*   Return: The state of RI.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
SIMBLLIB_API BYTE BL_getRi(bool* pval);					
	
/************************************************************************************/
/*   Return: The state of DTR.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
SIMBLLIB_API BYTE BL_getCd(bool* pval);	


/************************************************************************************/
/* Set Echo Mode
/*   0   =  Echo Disabled
/*   1   =  Echo Back Enabled
/*   2   =  Listen Echo Enabled
/************************************************************************************/
SIMBLLIB_API BYTE BL_SetEcho(int val);	


/************************************************************************************/
/* SetFlowControl : Enable/Disable Flow Control of DTR and RTS
/* FALSE   =   Disabled
/* TRUE    =   Enabled
/************************************************************************************/
SIMBLLIB_API  BYTE BL_SetFlowControl(bool val);
		

}

#endif

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE******/
