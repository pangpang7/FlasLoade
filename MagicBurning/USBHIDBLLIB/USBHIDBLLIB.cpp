#include <malloc.h>
//#include <mem.h>
#include "stdafx.h"
#include "USBHIDBLLIB.h"

/* Options Bytes Areas*/

DWORD ADDR_F1_OPB     = 0x1FFFF800;
DWORD ADDR_F2_OPB     = 0x1FFFC000;
DWORD ADDR_L1_OPB     = 0x1FF80000;
 
/* STM32F1 Series*/
DWORD ADDR_USER_OPB    = 0x1FFFF800;
DWORD ADDR_DATA_OPB    = 0x1FFFF804;
DWORD ADDR_RDP_OPB     = 0x1FFFF800;
DWORD ADDR_WRP0_OPB    = 0x1FFFF808;
DWORD ADDR_WRP2_OPB    = 0x1FFFF80C;



DWORD  MAX_DATA_SIZE      = 0xFF;  // Packet size(in byte)
BYTE   ACK                = 0x79 ;
BYTE   NACK               = 0x1F ;

ACKS ACK_VALUE          = ST79;
LPTARGET_DESCRIPTOR Target;
LPPRO_DESCRIPTOR PRO;
CBLHID Cur_COM;
//CRS232 Cur_COM;
ACKS   McuTarget;
DWORD  Progress;
DWORD  ActivityTime ;
LPBYTE RQ_Buffer;


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
									//Target->PID                        = (LPBYTE)malloc(1);
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
/* USBHID - ...
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE TARGET_SetComIntType(BYTE com_int_type)
{
	return 0;
}
/************************************************************************************/
/*
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE SetCOMSettings(int numPort, long speedInBaud, int nbBit,
	                            int parity, float nbStopBit)
{
	if (Cur_COM.isConnected)  return COM_ALREADY_OPENED; 
    
	if(numPort<1 || numPort>255)		
		return INPUT_PARAMS_ERROR;

	if(speedInBaud<1)		
		return INPUT_PARAMS_ERROR;

	if(nbBit<5 || nbBit > 9)
		return INPUT_PARAMS_ERROR;

	if(parity<0 || parity > 2)
		return INPUT_PARAMS_ERROR;

	if(nbStopBit<1 || nbStopBit > 2)
		return INPUT_PARAMS_ERROR;

	Cur_COM.numPort      = numPort;
	Cur_COM.speedInBaud  = speedInBaud;
	Cur_COM.nbBit	     = nbBit;                      
	Cur_COM.parity       = parity;
	Cur_COM.nbStopBit    = nbStopBit;

	return SUCCESS; 
}

USBHIDBLLIB_API BYTE Send_RQ(LPBL_Request pRQ)
{
	BYTE DataSize = 1;

    if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE; 

	if (!Target) Target = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

	if (!PRO) PRO = (LPPRO_DESCRIPTOR)malloc(sizeof(PRO_DESCRIPTOR));

	if (!RQ_Buffer) RQ_Buffer = (LPBYTE) malloc(1200);
   
	// put command code in the buffer
	
    RQ_Buffer[0] = pRQ->_cmd;    
	
	if (ACK_VALUE == ST79)
	{
	  // put XOR command code in the buffer
	  RQ_Buffer[1] = ~pRQ->_cmd;
      DataSize = 2;
	}

	BYTE auxcmd = RQ_Buffer[0];
	BYTE auxxorcmd = RQ_Buffer[1];

    // Send command code (and its XOR value) 
	if(pRQ->_cmd==GET_CMD)
	{
		if (Cur_COM.sendData(DataSize, RQ_Buffer,0xffff) != DataSize)
			return SEND_FAIL; 
	}
	else if(pRQ->_cmd==GET_VER_ROPS_CMD)
	{
		if (Cur_COM.sendData(DataSize, RQ_Buffer,5) != DataSize)
			return SEND_FAIL; 
	}
	else if(pRQ->_cmd==GET_ID_CMD)
	{
		if (Cur_COM.sendData(DataSize, RQ_Buffer,46) != DataSize)
			return SEND_FAIL;
	}
	else if(pRQ->_cmd==GET_PRO_CMD)
	{
		if (Cur_COM.sendData(DataSize, RQ_Buffer,28) != DataSize)
			return SEND_FAIL;
	}
	else
	{
		if (Cur_COM.sendData(DataSize, RQ_Buffer,1) != DataSize)
			return SEND_FAIL;
	}

	//free(RQ_Buffer);
	DataSize = 1;

    // Get ACK (verify if the command was accepted)
	if (/*ACK_VALUE == ST79) */!((ACK_VALUE == ST75) && (pRQ->_cmd == GET_VER_ROPS_CMD)))
	{
	  //RQ_Buffer = (LPBYTE) malloc(1);
	  if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
 		return READ_FAIL; 

	  if(RQ_Buffer[0] != ACK)
  		return CMD_NOT_ALLOWED;
	}

    switch (pRQ->_cmd)  
	{
		case SET_PWD				   :
										{
											//RQ_Buffer = (LPBYTE) malloc(18);
											RQ_Buffer[0]=4+16+32+16+4-1;//count 
											RQ_Buffer[1]=pRQ->_target->MAX_AUTH_NUM;
											RQ_Buffer[2]=0;
											RQ_Buffer[3]=0;
											RQ_Buffer[4]=0;
											for(int i=0;i<16;i++)
												RQ_Buffer[5+i]=pRQ->_target->PWD[i];
											for(int i=0;i<32;i++)
												RQ_Buffer[21+i]=pRQ->_target->FlashLock[i];
											for(int i=0;i<16;i++)
												RQ_Buffer[53+i]=pRQ->_target->FlashBusKey[i];
											RQ_Buffer[69]=pRQ->_target->FlashBusKeyEnable;
											RQ_Buffer[70]=0;
											RQ_Buffer[71]=0;
											RQ_Buffer[72]=0;
											DataSize=RQ_Buffer[0]+1+1;
											if (Cur_COM.sendData(DataSize, RQ_Buffer,1) != DataSize)
		                                       return SEND_FAIL;
											
											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 
 
											//free(RQ_Buffer);
										}break;
		case AUTH_PWD				   :
										{
											//RQ_Buffer = (LPBYTE) malloc(17);
											RQ_Buffer[0]=16-1;//count
											for(int i=0;i<16;i++)
												RQ_Buffer[1+i]=pRQ->_target->PWD[i];
											DataSize=17;
											if (Cur_COM.sendData(DataSize, RQ_Buffer,1) != DataSize)
		                                       return SEND_FAIL;
											
											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 
											// if get ack which means pwds have been authed

											//free(RQ_Buffer);
										}break;
		case REMOVE_PWD				   :// remove password / restore to factory settings
										{
										}break;
		case CHK_PWD_VALID			   :
										{
											//RQ_Buffer = (LPBYTE) malloc(1);
											RQ_Buffer[0]=1;
											DataSize=1;
											if (Cur_COM.sendData(DataSize, RQ_Buffer,1) != DataSize)
		                                       return SEND_FAIL;

											// Get count
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											pRQ->_target->CUR_AUTH_NUM=RQ_Buffer[0];
											//free(RQ_Buffer);
										}break;
		case SET_PRO_CMD			   :// set config to programmer
										{
											//RQ_Buffer = (LPBYTE) malloc(14);
											RQ_Buffer[0]=13-1;//count
											RQ_Buffer[1]=pRQ->_pro->MCU_PID[0];
											RQ_Buffer[2]=pRQ->_pro->MCU_PID[1];
											RQ_Buffer[3]=pRQ->_pro->BURNING_TYPE;
											RQ_Buffer[4]=pRQ->_pro->MCU_COM;
											RQ_Buffer[5]=pRQ->_pro->MCU_page_size;
											RQ_Buffer[6]=(pRQ->_pro->ram_offset>>24)&0xff;
											RQ_Buffer[7]=(pRQ->_pro->ram_offset>>16)&0xff;
											RQ_Buffer[8]=(pRQ->_pro->ram_offset>>8)&0xff;
											RQ_Buffer[9]=pRQ->_pro->ram_offset&0xff;
											RQ_Buffer[10]=(pRQ->_pro->flash_offset>>24)&0xff;
											RQ_Buffer[11]=(pRQ->_pro->flash_offset>>16)&0xff;
											RQ_Buffer[12]=(pRQ->_pro->flash_offset>>8)&0xff;
											RQ_Buffer[13]=pRQ->_pro->flash_offset&0xff;
											
											DataSize=14;
											if (Cur_COM.sendData(DataSize, RQ_Buffer,1) != DataSize)
		                                       return SEND_FAIL;
											
											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 
 
											//free(RQ_Buffer);
										}break;
		case SET_OFFLINE_CMD           :
										{
											//RQ_Buffer = (LPBYTE) malloc(4);
											RQ_Buffer[0]=3-1;//count
											RQ_Buffer[1]=pRQ->_pro->ram_done;
											RQ_Buffer[2]=pRQ->_pro->flash_done;
											RQ_Buffer[3]=pRQ->_pro->download_type;
											DataSize=4;
											if (Cur_COM.sendData(DataSize, RQ_Buffer,1) != DataSize)
		                                       return SEND_FAIL;
											
											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 
 
											//free(RQ_Buffer);
										}break;
		case GET_PRO_CMD			   :
										{
											memset(pRQ->_pro,0x00,sizeof(PRO_DESCRIPTOR));
											//RQ_Buffer = (LPBYTE) malloc(26);
	                                        if (Cur_COM.receiveData(26, RQ_Buffer) != 26)
		                                       return READ_FAIL;
											pRQ->_pro->Version=RQ_Buffer[0];
											pRQ->_pro->PRO_PID[0]=RQ_Buffer[1];
											pRQ->_pro->PRO_PID[1]=RQ_Buffer[2];
											pRQ->_pro->MCU_RAM_BASE=((DWORD)(RQ_Buffer[3])<<24)
														| ((DWORD)(RQ_Buffer[4])<<16)
														| ((DWORD)(RQ_Buffer[5])<<8)
														| ((DWORD)(RQ_Buffer[6])<<0);
											pRQ->_pro->MCU_FLASH_BASE=((DWORD)(RQ_Buffer[7])<<24)
														| ((DWORD)(RQ_Buffer[8])<<16)
														| ((DWORD)(RQ_Buffer[9])<<8)
														| ((DWORD)(RQ_Buffer[10])<<0);
											pRQ->_pro->MCU_PID[0]=RQ_Buffer[11];
											pRQ->_pro->MCU_PID[1]=RQ_Buffer[12];
											pRQ->_pro->BURNING_TYPE=RQ_Buffer[13];
											pRQ->_pro->MCU_COM=RQ_Buffer[14];
											pRQ->_pro->MCU_page_size=RQ_Buffer[15];
											pRQ->_pro->ram_offset=((DWORD)(RQ_Buffer[16])<<24)
														| ((DWORD)(RQ_Buffer[17])<<16)
														| ((DWORD)(RQ_Buffer[18])<<8)
														| ((DWORD)(RQ_Buffer[19])<<0);
											pRQ->_pro->flash_offset=((DWORD)(RQ_Buffer[20])<<24)
														| ((DWORD)(RQ_Buffer[21])<<16)
														| ((DWORD)(RQ_Buffer[22])<<8)
														| ((DWORD)(RQ_Buffer[23])<<0);
											pRQ->_pro->ram_done=RQ_Buffer[24];
											pRQ->_pro->flash_done=RQ_Buffer[25];

											//free(RQ_Buffer);

                                            // Get ACK byte
											//RQ_Buffer = (LPBYTE) malloc(1);
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if(RQ_Buffer[0] != ACK)
		                                       return CMD_FAIL; 

											memcpy(PRO, pRQ->_pro, sizeof(PRO_DESCRIPTOR));
											//free(RQ_Buffer);
										}break;
        case GET_CMD                   : //Get the version and the allowed commands supported by the current version of the boot loader
										{
											memset(pRQ->_target, 0x00, sizeof(TARGET_DESCRIPTOR));
                                            // Get number of bytes (Version + commands)
											//RQ_Buffer = (LPBYTE) malloc(1);
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

                                            pRQ->_target->CmdCount = RQ_Buffer[0];

                                            // Get boot loader version
											if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											pRQ->_target->Version = RQ_Buffer[0];
                                            //free(RQ_Buffer);

                                            // Get supported commands
											//RQ_Buffer = (LPBYTE) malloc(pRQ->_target->CmdCount);
	                                        if (Cur_COM.receiveData(pRQ->_target->CmdCount, RQ_Buffer) != pRQ->_target->CmdCount)
		                                       return READ_FAIL;

                                            for (int i = 0; i<pRQ->_target->CmdCount; i++)
											{
												switch (RQ_Buffer[i]) 
												{
											      case GET_CMD                   :  pRQ->_target->GET_CMD = TRUE; 
													  break;
                                                  case GET_VER_ROPS_CMD          :  pRQ->_target->GET_VER_ROPS_CMD = TRUE; 
													  break;
                                                  case GET_ID_CMD                :  pRQ->_target->GET_ID_CMD = TRUE; 
													  break;
                                                  case READ_CMD                  :  pRQ->_target->READ_CMD = TRUE; 
													  break;
                                                  case GO_CMD                    :  pRQ->_target->GO_CMD = TRUE; 
													  break;
                                                  case WRITE_CMD                 :  pRQ->_target->WRITE_CMD = TRUE; 
													  break;
                                                  case ERASE_CMD                 :  pRQ->_target->ERASE_CMD = TRUE; 
													  break;
												  case ERASE_EXT_CMD             :  pRQ->_target->ERASE_EXT_CMD  = TRUE; 
													  break;
                                                  case WRITE_PROTECT_CMD         :  pRQ->_target->WRITE_PROTECT_CMD = TRUE; 
													  break;
                                                  case WRITE_TEMP_UNPROTECT_CMD  :  pRQ->_target->WRITE_TEMP_UNPROTECT_CMD = TRUE; 
													  break;
                                                  case WRITE_PERM_UNPROTECT_CMD  :  pRQ->_target->WRITE_PERM_UNPROTECT_CMD = TRUE; 
													  break;
                                                  case READOUT_PROTECT_CMD  :  pRQ->_target->READOUT_PERM_PROTECT_CMD = TRUE; 
													  break;
                                                  case READOUT_TEMP_UNPROTECT_CMD:  pRQ->_target->READOUT_TEMP_UNPROTECT_CMD = TRUE; 
													  break;
                                                  case READOUT_PERM_UNPROTECT_CMD:  pRQ->_target->READOUT_PERM_UNPROTECT_CMD = TRUE; 
													  break;
												}
											}
											//free(RQ_Buffer);

                                            // Get ACK byte
											//RQ_Buffer = (LPBYTE) malloc(1);
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if(RQ_Buffer[0] != ACK)
		                                       return CMD_FAIL; 

											memcpy(Target, pRQ->_target, sizeof(TARGET_DESCRIPTOR));
											//free(RQ_Buffer);
										}break;
        case GET_VER_ROPS_CMD          : //Get the BL version and the Read Protection status of the NVM
        								{
											memcpy(pRQ->_target, Target, sizeof(TARGET_DESCRIPTOR));
											// Get Version
											//RQ_Buffer = (LPBYTE) malloc(1);
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;
                                            
                                            pRQ->_target->Version = RQ_Buffer[0];
                                            
											// Get ROPE
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;
                                            
                                            pRQ->_target->ROPE = RQ_Buffer[0];

											// Get ROPD
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;
                                            
                                            pRQ->_target->ROPD = RQ_Buffer[0];

											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 

											memcpy(Target, pRQ->_target, sizeof(TARGET_DESCRIPTOR));
											//free(RQ_Buffer);
										}break;
        case GET_ID_CMD                : //Get the chip ID
        								{
											memcpy(pRQ->_target, Target, sizeof(TARGET_DESCRIPTOR));
											// Get PID Length
											//RQ_Buffer = (LPBYTE) malloc(1);
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;
                                            
                                            pRQ->_target->PIDLen = RQ_Buffer[0] + 1;
                                            
											// Get PID
                                            //RQ_Buffer = (LPBYTE) malloc(pRQ->_target->PIDLen);
	                                        if (Cur_COM.receiveData(pRQ->_target->PIDLen, RQ_Buffer) != pRQ->_target->PIDLen)
		                                       return READ_FAIL;

											//pRQ->_target->PID = (BYTE*)malloc(sizeof(pRQ->_target->PIDLen)); 
											memcpy(pRQ->_target->PID, RQ_Buffer, pRQ->_target->PIDLen);

											// get ChipVersion
											//RQ_Buffer = (LPBYTE) malloc(1);
											if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;
											memcpy(&pRQ->_target->ChipVersion, RQ_Buffer, 1);

											// get ChipID
											//RQ_Buffer = (LPBYTE) malloc(8);
											if (Cur_COM.receiveData(8, RQ_Buffer) != 8)
		                                       return READ_FAIL;
											memcpy(&pRQ->_target->ChipID, RQ_Buffer, 8);

											// get ChipID
											//RQ_Buffer = (LPBYTE) malloc(32);
											if (Cur_COM.receiveData(32, RQ_Buffer) != 32)
		                                       return READ_FAIL;
											memcpy(&pRQ->_target->FlashLock, RQ_Buffer, 32);


											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 

											memcpy(Target, pRQ->_target, sizeof(TARGET_DESCRIPTOR)); 
											//free(RQ_Buffer);
										}break;
        case READ_CMD                  : //Read up to 256 bytes of memory starting from an address specified by the user
        								{
											memcpy(pRQ->_target, Target, sizeof(TARGET_DESCRIPTOR));

											// Send Read address and checksum
											if (ACK_VALUE == ST79) DataSize = 5;
											else DataSize = 4;

											//RQ_Buffer = (LPBYTE) malloc(5);
											BYTE Checksum = 0x00;

                                            RQ_Buffer[0] = (pRQ->_address >> 24) & 0x000000FF; Checksum= Checksum ^ RQ_Buffer[0];
											RQ_Buffer[1] = (pRQ->_address >> 16) & 0x000000FF; Checksum= Checksum ^ RQ_Buffer[1];
											RQ_Buffer[2] = (pRQ->_address >> 8 ) & 0x000000FF; Checksum= Checksum ^ RQ_Buffer[2];
											RQ_Buffer[3] = (pRQ->_address      ) & 0x000000FF; Checksum= Checksum ^ RQ_Buffer[3];
											RQ_Buffer[4] = Checksum;

	                                        if (Cur_COM.sendData(DataSize, RQ_Buffer,1) != DataSize)
		                                       return SEND_FAIL;

											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL;
											
											// send data size to be read
											RQ_Buffer[0] = (pRQ->_length-1)>>8;
											Checksum=RQ_Buffer[0];
											RQ_Buffer[1] = (pRQ->_length-1)&0xff;
											Checksum=Checksum^RQ_Buffer[1];
											RQ_Buffer[2]=Checksum;

											if (ACK_VALUE == ST79) DataSize = 3;
											else DataSize = 2;

											if (Cur_COM.sendData(DataSize, RQ_Buffer,pRQ->_length+1) != DataSize)
		                                       return SEND_FAIL;

											// Get ACK
											if (ACK_VALUE == ST79)
											{
												if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
												   return READ_FAIL;

												if (RQ_Buffer[0] != ACK )
												   return CMD_FAIL;
											}

											//free(RQ_Buffer);

											// Get data
											if (Cur_COM.receiveData(pRQ->_length, pRQ->_data)!= pRQ->_length )
		                                       return READ_FAIL;

											

										}break;
        case GO_CMD                    : //Jump to an address specified by the user to execute (a loaded) code
        								{
											memcpy(pRQ->_target, Target, sizeof(TARGET_DESCRIPTOR));
                                            // Send Go address and checksum
											BYTE Checksum = 0x00;

											if (ACK_VALUE == ST79) DataSize = 5;
											else DataSize = 4;


											//RQ_Buffer = (LPBYTE) malloc(5);

                                            RQ_Buffer[0] = (pRQ->_address >> 24) & 0x000000FF; Checksum= Checksum ^ RQ_Buffer[0];
											RQ_Buffer[1] = (pRQ->_address >> 16) & 0x000000FF; Checksum= Checksum ^ RQ_Buffer[1];
											RQ_Buffer[2] = (pRQ->_address >> 8 ) & 0x000000FF; Checksum= Checksum ^ RQ_Buffer[2];
											RQ_Buffer[3] = (pRQ->_address      ) & 0x000000FF; Checksum= Checksum ^ RQ_Buffer[3];
											RQ_Buffer[4] = Checksum;

	                                        if (Cur_COM.sendData(DataSize, RQ_Buffer,1) != DataSize)
		                                       return SEND_FAIL;

											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											// to be added when Go command can return ACK when performed
											//if (RQ_Buffer[0] != ACK )
											   //return CMD_FAIL; 

											//free(RQ_Buffer);
										}break;
        case WRITE_CMD                 : //Write maximum 256 bytes to the RAM or the NVM starting from an address specified by the user
        								{
                                            memcpy(pRQ->_target, Target, sizeof(TARGET_DESCRIPTOR));

											// Send Read address and checksum
											BYTE Checksum = 0x00;

											if (ACK_VALUE == ST79) DataSize = 5;
											else DataSize = 4;

											//RQ_Buffer = (LPBYTE) malloc(5);
											//RQ_Buffer = (LPBYTE) malloc(pRQ->_length+0x10);


                                            RQ_Buffer[0] = (pRQ->_address >> 24) & 0x000000FF; Checksum= Checksum ^ RQ_Buffer[0];
											RQ_Buffer[1] = (pRQ->_address >> 16) & 0x000000FF; Checksum= Checksum ^ RQ_Buffer[1];
											RQ_Buffer[2] = (pRQ->_address >> 8 ) & 0x000000FF; Checksum= Checksum ^ RQ_Buffer[2];
											RQ_Buffer[3] = (pRQ->_address      ) & 0x000000FF; Checksum= Checksum ^ RQ_Buffer[3];
											RQ_Buffer[4] = Checksum;

	                                        if (Cur_COM.sendData(DataSize, RQ_Buffer,1) != DataSize)
		                                       return SEND_FAIL;

											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL;

											BYTE checksum = 0x00;
											
											// send data size to be writen // 2 bytes
											RQ_Buffer[0] = (pRQ->_length-1)>>8;
											checksum = /*checksum ^*/ RQ_Buffer[0];
											RQ_Buffer[1] = (pRQ->_length-1)&0xff;
											checksum = /*checksum ^*/ checksum^RQ_Buffer[1];

											//if (Cur_COM.sendData(1, RQ_Buffer) != 1)
		                                    //   return SEND_FAIL;

											if (ACK_VALUE == ST79)
											{
											  for (int i = 0; i< pRQ->_length; i++)
												 checksum = checksum ^ pRQ->_data[i];
											}

											memcpy(&RQ_Buffer[2],pRQ->_data,pRQ->_length);
											// Send data
											//if (Cur_COM.sendData(pRQ->_length, pRQ->_data)!= pRQ->_length )
		                                    //   return SEND_FAIL;

											if (ACK_VALUE == ST79) 
											{
											  // send checksum
											  RQ_Buffer[pRQ->_length+2] = checksum;

											  if (Cur_COM.sendData(pRQ->_length+3, RQ_Buffer,1) != pRQ->_length+3)
		                                         return SEND_FAIL;
											}
											else
											{
												if (Cur_COM.sendData(pRQ->_length+2, pRQ->_data,1)!= pRQ->_length+2 )
													return SEND_FAIL;
											}

                                            // Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 

											/*if (ACK_VALUE == ST79)
											{
												if (Cur_COM.receiveData(1, RQ_Buffer) != 1);
												   //return READ_FAIL;

												if (RQ_Buffer[0] != ACK );
												   //return CMD_FAIL; 
											}*/

											//free(RQ_Buffer);
										}break;


										/* TO DO */


        case ERASE_EXT_CMD                 : //Erase from one to all the NVM sectors
        								{
											memcpy(pRQ->_target, Target, sizeof(TARGET_DESCRIPTOR));
																																
											if ((pRQ->_wbSectors  & 0xFF00 ) != 0xFF00)
											{
											  BYTE checksum = ((pRQ->_wbSectors)-1) >> 8 ;
                                              checksum = checksum ^ ((pRQ->_wbSectors)-1) ;

											  //RQ_Buffer = (LPBYTE) malloc(pRQ->_length*2 + 3);  /* N ( 2 bytes)  and Checksum */
											  memset(RQ_Buffer,0xFF, pRQ->_length*2 + 3);
											  
											  RQ_Buffer[0] = ((pRQ->_wbSectors)-1) >> 8;
                                              RQ_Buffer[1] = ((pRQ->_wbSectors)-1);

                                              for (int i = 2; i<= pRQ->_wbSectors*2 ;i+=2)
											  {
												  RQ_Buffer[i] = pRQ->_data[i-1];
												  RQ_Buffer[i+1] = pRQ->_data[i-2];


												  checksum = checksum ^ pRQ->_data[i-1];
												  checksum = checksum ^ pRQ->_data[i-2];


											  }
                                              RQ_Buffer[pRQ->_wbSectors*2 + 2] = checksum;
											}
											else
											{
												//RQ_Buffer = (LPBYTE) malloc(3);

											    RQ_Buffer[0] = pRQ->_wbSectors >> 8;
												RQ_Buffer[1] = pRQ->_wbSectors;
												RQ_Buffer[2] = RQ_Buffer[0] ^ RQ_Buffer[1];
											}

											if (ACK_VALUE == ST79) DataSize = 3;
											else DataSize = 1;


                                            if (Cur_COM.sendData(pRQ->_length*2 + DataSize, RQ_Buffer,1) != pRQ->_length*2 + DataSize)
		                                       return SEND_FAIL;

											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 

											/*if (ACK_VALUE == ST79)
											{
												// Get ACK
												if (Cur_COM.receiveData(1, RQ_Buffer) != 1);
												   //return READ_FAIL;

												if (RQ_Buffer[0] != ACK );
												   //return CMD_FAIL; 
											}*/

											//free(RQ_Buffer);

										}break;
		case ERASE_CMD                 : //Erase from one to all the NVM sectors
        								{
											memcpy(pRQ->_target, Target, sizeof(TARGET_DESCRIPTOR));
																																
											if (pRQ->_nbSectors != 0xFF)
											{
											  BYTE checksum = /*0x00 ^ */(pRQ->_nbSectors-1);

											  //RQ_Buffer = (LPBYTE) malloc(pRQ->_length + 2);
											  memset(RQ_Buffer,0xFF, pRQ->_length + 2);
											  
											  RQ_Buffer[0] = pRQ->_nbSectors - 1;
                                              for (int i = 1; i<= pRQ->_nbSectors ;i++)
											  {
												  RQ_Buffer[i] = pRQ->_data[i-1];
												  checksum = checksum ^ pRQ->_data[i-1]; 
											  }
                                              RQ_Buffer[pRQ->_nbSectors + 1] = checksum;
											}
											else
											{
												//RQ_Buffer = (LPBYTE) malloc(2);

											    RQ_Buffer[0] = pRQ->_nbSectors;
												RQ_Buffer[1] = ~pRQ->_nbSectors;
											}

											if (ACK_VALUE == ST79) DataSize = 2;
											else DataSize = 1;


                                            if (Cur_COM.sendData(pRQ->_length + DataSize, RQ_Buffer,1) != pRQ->_length + DataSize)
		                                       return SEND_FAIL;

											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 

											/*if (ACK_VALUE == ST79)
											{
												// Get ACK
												if (Cur_COM.receiveData(1, RQ_Buffer) != 1);
												   //return READ_FAIL;

												if (RQ_Buffer[0] != ACK );
												   //return CMD_FAIL; 
											}*/

											//free(RQ_Buffer);

										}break;
        case WRITE_PROTECT_CMD         : //Enable the write protection in a permanent way for some sectors
        								{
											memcpy(pRQ->_target, Target, sizeof(TARGET_DESCRIPTOR));
																																				
											BYTE checksum = 0x00 ^ (pRQ->_nbSectors-1);

											//RQ_Buffer = (LPBYTE) malloc(pRQ->_length + 2);
											RQ_Buffer[0] = pRQ->_nbSectors - 1;
                                            for (int i = 1; i<=pRQ->_nbSectors ;i++)
											{
											  RQ_Buffer[i] = pRQ->_data[i-1];
											  checksum = checksum ^ pRQ->_data[i-1]; 
											}
                                            RQ_Buffer[pRQ->_nbSectors + 1] = checksum;
										

                                            if (Cur_COM.sendData(pRQ->_length + 2, RQ_Buffer,1) != pRQ->_length + 2)
		                                       return SEND_FAIL;

											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 

											//free(RQ_Buffer);
										}break;
        case WRITE_TEMP_UNPROTECT_CMD   :{
											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 

											//free(RQ_Buffer);
										}break; //Disable the write protection in a temporary way for all NVM sectors 
		case WRITE_PERM_UNPROTECT_CMD   :{
											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 

											//free(RQ_Buffer);
										}break; //Disable the write protection in a permanent way for all NVM sectors
		case READOUT_PROTECT_CMD        :{
											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 

											//free(RQ_Buffer);
										}break; //Disable the readout protection in a temporary way
		case READOUT_TEMP_UNPROTECT_CMD :{
											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 

											//free(RQ_Buffer);
										}break; //Disable the readout protection in a permanent way
        case READOUT_PERM_UNPROTECT_CMD :{
											// Get ACK
	                                        if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		                                       return READ_FAIL;

											if (RQ_Buffer[0] != ACK )
											   return CMD_FAIL; 

											//free(RQ_Buffer);
										}break; 
	}

	return SUCCESS;
}

USBHIDBLLIB_API BYTE COM_Open(char* devname)
{
	if (Cur_COM.isConnected)  return COM_ALREADY_OPENED;
	
	if (Cur_COM.open(devname))
		return SUCCESS;
	else
		return CANT_OPEN_COM;
}

USBHIDBLLIB_API BYTE COM_Close()
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;

	Cur_COM.closeCom();

	return SUCCESS;
}

USBHIDBLLIB_API BYTE BL_SetSpeed(DWORD speed)
{
	return CMD_NOT_ALLOWED;
}

USBHIDBLLIB_API BYTE BL_Init_BL()
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	
	if(!RQ_Buffer) RQ_Buffer=(LPBYTE) malloc(1200);
	
	
	//LPBYTE RQ_Buffer = (LPBYTE) malloc(1);

    RQ_Buffer[0] = INIT_CON; 
	//if (Cur_COM.setTxd(FALSE)) _sleep(100);

    if (Cur_COM.sendData(1, RQ_Buffer,1) != 1)
		return SEND_FAIL; 
	if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
		return READ_FAIL;

   
	
	//Work-Around : in case of the device send a 0x00 value 
	//after system reset , we reveive  again the real ack
	
	if(RQ_Buffer[0] == 0x00)
		if (Cur_COM.receiveData(1, RQ_Buffer) != 1)
			return READ_FAIL;

	//if (Cur_COM.setTxd(FALSE)) 
	//_sleep(100);

	switch (RQ_Buffer[0])
	{
	  case 0x75:{ // STR750 used
		          ACK_VALUE = ST75;
		          ACK  = 0x75;
                  NACK = 0x3F;
				  // Commented to avoid DTR/RTS reset signals Cur_COM.SetParity(0); // Set NONE parity  
				}break;
	  case 0x79:{ // STM32, STR911 or STM8 used
		          ACK_VALUE = ST79;
		          ACK  = 0x79;
                  NACK = 0x1F;
				  // Commented to avoid DTR/RTS reset signalsCur_COM.SetParity(2); // Set EVEN parity
				}break;
	  default:  { // Undefined device
		          ACK_VALUE = UNDEFINED;
				  ACK  = RQ_Buffer[0];
		          return UNREOGNIZED_DEVICE;
				}break;
	}

	return SUCCESS;
}

USBHIDBLLIB_API BYTE BL_SEC(BYTE* pwd, BYTE* max_auth_num, BYTE* flashlock, BYTE* flashbuskey,BYTE* flashbusenable, BYTE cmd)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;

	if (!pwd) return INPUT_PARAMS_MEMORY_ALLOCATION_ERROR;

	if((cmd!=SET_PWD)&&(cmd!=AUTH_PWD)&&(cmd!=REMOVE_PWD)&&(cmd!=CHK_PWD_VALID)) return INPUT_PARAMS_MEMORY_ALLOCATION_ERROR; 

	LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
	pRQ->_target = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));
	memcpy(pRQ->_target->PWD,pwd,16);//128bit
	pRQ->_target->MAX_AUTH_NUM=*max_auth_num;
	memcpy(pRQ->_target->FlashLock,flashlock,32);//256bit
	memcpy(pRQ->_target->FlashBusKey,flashbuskey,16);//128bit
	pRQ->_target->FlashBusKeyEnable=*flashbusenable;
	pRQ->_cmd = cmd;

	BYTE Result = Send_RQ(pRQ);

	if(Result==SUCCESS)
		*max_auth_num=pRQ->_target->CUR_AUTH_NUM;
	
	free(pRQ->_target);
	free(pRQ);

	return Result;
}

USBHIDBLLIB_API BYTE BL_PRO_GET(LPPRO_DESCRIPTOR pPro)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;

	if (!pPro) return INPUT_PARAMS_MEMORY_ALLOCATION_ERROR;

	LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
	pRQ->_pro = (LPPRO_DESCRIPTOR)malloc(sizeof(PRO_DESCRIPTOR));

	pRQ->_cmd = GET_PRO_CMD;

	BYTE Result = Send_RQ(pRQ);
	if (Result == SUCCESS)
	{
		memcpy(pPro, pRQ->_pro,sizeof(PRO_DESCRIPTOR));
	}

	free(pRQ->_pro);
	free(pRQ);

	return Result;
}

USBHIDBLLIB_API BYTE BL_SET_PRO(LPPRO_DESCRIPTOR pPro,BYTE cmd) //set 
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;

	if (!pPro) return INPUT_PARAMS_MEMORY_ALLOCATION_ERROR;

	if((cmd!=SET_PRO_CMD)&&(cmd!=SET_OFFLINE_CMD)) return INPUT_PARAMS_MEMORY_ALLOCATION_ERROR; 

	LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
	pRQ->_pro = pPro;

	pRQ->_cmd = cmd;

	BYTE Result = Send_RQ(pRQ);

	//free(pRQ->_pro);
	free(pRQ);


	return Result;
}

/******************************************************************************************/
/* Boot Loader commands implementation                     
/******************************************************************************************/                                                                                   
USBHIDBLLIB_API BYTE BL_GET(LPBYTE Version, LPCommands pCmds)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;

	if (!Version || !pCmds) return INPUT_PARAMS_MEMORY_ALLOCATION_ERROR;

	if(ACK_VALUE == ST75)
	{
		pCmds->GET_CMD                    = TRUE    ; //Get the version and the allowed commands supported by the current version of the boot loader
		pCmds->GET_VER_ROPS_CMD           = TRUE    ; //Get the BL version and the Read Protection status of the NVM
		pCmds->GET_ID_CMD                 = TRUE    ; //Get the chip ID
		pCmds->READ_CMD                   = TRUE    ; //Read up to 256 bytes of memory starting from an address specified by the user
		pCmds->GO_CMD                     = TRUE    ; //Jump to an address specified by the user to execute (a loaded) code
		pCmds->WRITE_CMD                  = TRUE    ; //Write maximum 256 bytes to the RAM or the NVM starting from an address specified by the user
		pCmds->ERASE_CMD                  = TRUE    ; //Erase from one to all the NVM sectors
		pCmds->WRITE_PROTECT_CMD          = TRUE    ; //Enable the write protection in a permanent way for some sectors
		pCmds->WRITE_TEMP_UNPROTECT_CMD   = TRUE    ; //Disable the write protection in a temporary way for all NVM sectors
		pCmds->WRITE_PERM_UNPROTECT_CMD   = TRUE    ; //Disable the write protection in a permanent way for all NVM sectors
		pCmds->READOUT_PROTECT_CMD        = TRUE    ; //Enable the readout protection in a permanent way
		pCmds->READOUT_TEMP_UNPROTECT_CMD = TRUE    ; //Disable the readout protection in a temporary way
		pCmds->READOUT_PERM_UNPROTECT_CMD = TRUE    ; //Disable the readout protection in a permanent way

		Target->GET_CMD                    = TRUE    ; //Get the version and the allowed commands supported by the current version of the boot loader
		Target->GET_VER_ROPS_CMD           = TRUE    ; //Get the BL version and the Read Protection status of the NVM
		Target->GET_ID_CMD                 = TRUE    ; //Get the chip ID
		Target->READ_CMD                   = TRUE    ; //Read up to 256 bytes of memory starting from an address specified by the user
		Target->GO_CMD                     = TRUE    ; //Jump to an address specified by the user to execute (a loaded) code
		Target->WRITE_CMD                  = TRUE    ; //Write maximum 256 bytes to the RAM or the NVM starting from an address specified by the user
		Target->ERASE_CMD                  = TRUE    ; //Erase from one to all the NVM sectors
		Target->WRITE_PROTECT_CMD          = TRUE    ; //Enable the write protection in a permanent way for some sectors
		Target->WRITE_TEMP_UNPROTECT_CMD   = TRUE    ; //Disable the write protection in a temporary way for all NVM sectors
		Target->WRITE_PERM_UNPROTECT_CMD   = TRUE    ; //Disable the write protection in a permanent way for all NVM sectors
		Target->READOUT_PERM_PROTECT_CMD   = TRUE    ; //Enable the readout protection in a permanent way
		Target->READOUT_TEMP_UNPROTECT_CMD = TRUE    ; //Disable the readout protection in a temporary way
		Target->READOUT_PERM_UNPROTECT_CMD = TRUE    ; //Disable the readout protection in a permanent way

		BYTE Ver; BYTE ROPEnabled; BYTE ROPDisabled;
		BYTE Result = BL_GET_VER_ROPS(&Ver, &ROPEnabled, &ROPDisabled);
		if (Result != SUCCESS) return Result;
		memcpy(Version, &Ver,1);
	}
	else if(ACK_VALUE = ST79)
	{

		LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
		pRQ->_target = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

		pRQ->_cmd = GET_CMD;

		BYTE Result = Send_RQ(pRQ);
		if (Result != SUCCESS) return Result;

		memcpy(Version, &pRQ->_target->Version,1);

		pCmds->GET_CMD                    = pRQ->_target->GET_CMD                    ; //Get the version and the allowed commands supported by the current version of the boot loader
		pCmds->GET_VER_ROPS_CMD           = pRQ->_target->GET_VER_ROPS_CMD           ; //Get the BL version and the Read Protection status of the NVM
		pCmds->GET_ID_CMD                 = pRQ->_target->GET_ID_CMD                 ; //Get the chip ID
		pCmds->READ_CMD                   = pRQ->_target->READ_CMD                   ; //Read up to 256 bytes of memory starting from an address specified by the user
		pCmds->GO_CMD                     = pRQ->_target->GO_CMD                     ; //Jump to an address specified by the user to execute (a loaded) code
		pCmds->WRITE_CMD                  = pRQ->_target->WRITE_CMD                  ; //Write maximum 256 bytes to the RAM or the NVM starting from an address specified by the user
		pCmds->ERASE_CMD                  = pRQ->_target->ERASE_CMD                  ; //Erase from one to all the NVM sectors
		pCmds->ERASE_EXT_CMD              = pRQ->_target->ERASE_EXT_CMD              ; //Erase from one to all the NVM sectors
		pCmds->WRITE_PROTECT_CMD          = pRQ->_target->WRITE_PROTECT_CMD          ; //Enable the write protection in a permanent way for some sectors
		pCmds->WRITE_TEMP_UNPROTECT_CMD   = pRQ->_target->WRITE_TEMP_UNPROTECT_CMD   ; //Disable the write protection in a temporary way for all NVM sectors
		pCmds->WRITE_PERM_UNPROTECT_CMD   = pRQ->_target->WRITE_PERM_UNPROTECT_CMD   ; //Disable the write protection in a permanent way for all NVM sectors
		pCmds->READOUT_PROTECT_CMD   = pRQ->_target->READOUT_PERM_PROTECT_CMD   ; //Enable the readout protection in a permanent way
		pCmds->READOUT_TEMP_UNPROTECT_CMD = pRQ->_target->READOUT_TEMP_UNPROTECT_CMD ; //Disable the readout protection in a temporary way
		pCmds->READOUT_PERM_UNPROTECT_CMD = pRQ->_target->READOUT_PERM_UNPROTECT_CMD ; //Disable the readout protection in a permanent way
	   
	    free(pRQ);
	}
	return SUCCESS;
}
                                                                                    
USBHIDBLLIB_API BYTE BL_GET_VER_ROPS(LPBYTE Version, LPBYTE ROPEnabled, LPBYTE ROPDisabled)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->GET_VER_ROPS_CMD) return CMD_NOT_ALLOWED;

	if (!Version || !ROPEnabled || !ROPDisabled) return INPUT_PARAMS_MEMORY_ALLOCATION_ERROR;

	LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
    pRQ->_target = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

    pRQ->_cmd = GET_VER_ROPS_CMD;

	BYTE Result = Send_RQ(pRQ) ;
	if (Result != SUCCESS) return Result;

    memcpy(ROPEnabled, &pRQ->_target->ROPD, 1);  
	memcpy(ROPDisabled, &pRQ->_target->ROPE, 1);
	memcpy(Version, &Target->Version, 1);
	
	free(pRQ);

	return SUCCESS;
}              
                                                                                    
USBHIDBLLIB_API BYTE BL_GET_ID(LPBYTE size, LPBYTE pID,LPBYTE ChipVersion, LPBYTE ChipID, LPBYTE FlashLockBit) 
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->GET_ID_CMD) return CMD_NOT_ALLOWED;

	//if (!pID) return INPUT_PARAMS_MEMORY_ALLOCATION_ERROR;

	LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
    pRQ->_target = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

    pRQ->_cmd = GET_ID_CMD;

	BYTE Result = Send_RQ(pRQ) ;
	if (Result != SUCCESS) return Result;

	memcpy(size, &pRQ->_target->PIDLen, 1);

	if (pID) memcpy(pID, pRQ->_target->PID, *size);	
	if (ChipVersion) memcpy(ChipVersion, &pRQ->_target->ChipVersion, 1);
	if (ChipID) memcpy(ChipID, pRQ->_target->ChipID, 8);
	if (FlashLockBit) memcpy(FlashLockBit, pRQ->_target->FlashLock, 32);
	
	free(pRQ);

	return SUCCESS;
}                   
                                                                                     
USBHIDBLLIB_API BYTE BL_READ(DWORD Address, WORD Size, LPBYTE pData) 
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->READ_CMD) return CMD_NOT_ALLOWED;

	if (!pData) return INPUT_PARAMS_MEMORY_ALLOCATION_ERROR;

	LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
    pRQ->_target = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

    pRQ->_cmd = READ_CMD;
    pRQ->_address = Address;

	if((Size%2) != 0) Size++;
	pRQ->_length  = Size ;

	pRQ->_data    = (LPBYTE) malloc(MAX_DATA_SIZE);
	memset(pRQ->_data, 0xFF, MAX_DATA_SIZE);   

	BYTE Result = Send_RQ(pRQ);
	
	if (Result != SUCCESS) 
	{
		free(pRQ->_target);
		free(pRQ->_data);
		free(pRQ);
		return Result;
	}

	memcpy(pData, pRQ->_data, Size);
	
	free(pRQ->_target);
	free(pRQ->_data);
	free(pRQ);

	return SUCCESS;
}                     
                                                                                    
USBHIDBLLIB_API BYTE BL_GO(DWORD Address) 
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->GO_CMD) return CMD_NOT_ALLOWED;

	LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
    pRQ->_target = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

    pRQ->_cmd = GO_CMD;
	
    pRQ->_address = Address;

	BYTE Result = Send_RQ(pRQ);
	if (Result != SUCCESS) return Result;
	
	free(pRQ);

	return SUCCESS;
}                       
                                                                                     
USBHIDBLLIB_API BYTE BL_WRITE(DWORD address, WORD size, LPBYTE pData)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->WRITE_CMD) return CMD_NOT_ALLOWED;

	if (!pData) return INPUT_PARAMS_MEMORY_ALLOCATION_ERROR;

	LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
    pRQ->_target   = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

    pRQ->_cmd     = WRITE_CMD;
    pRQ->_address = address;

	if((size%2) != 0) size++;  //ST to be checked
	pRQ->_length  = size;
	
	pRQ->_data    = (LPBYTE) malloc(MAX_DATA_SIZE);
	memset(pRQ->_data, 0xFF, MAX_DATA_SIZE); 
	memcpy(pRQ->_data, pData, size);

	BYTE Result = Send_RQ(pRQ);

	if (Result != SUCCESS)  
	{
		return Result;
	}
	 
	free(pRQ);

	return SUCCESS;
}                     
                                                                                    
USBHIDBLLIB_API BYTE BL_ERASE(WORD NbSectors, LPBYTE pSectors)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if ((Target->ERASE_CMD == 0) && ( Target->ERASE_EXT_CMD == 0 )) return CMD_NOT_ALLOWED;

	if ((Target->ERASE_CMD))
	{
	
	
	
			if ((!pSectors) && (NbSectors != 0xFFFF)) return INPUT_PARAMS_MEMORY_ALLOCATION_ERROR;

			LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
			pRQ->_target   = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

			pRQ->_cmd = ERASE_CMD;


			if (NbSectors == 0xFFFF)
			{
			   pRQ->_nbSectors = 0xFF;
			   pRQ->_length = 0;

			   BYTE Result = Send_RQ(pRQ);
			   if (Result != SUCCESS) return Result;

			   Progress = 0xFF / 10;
			}
			else
			{
			   WORD nErase = NbSectors / 10;
			   WORD Remain = NbSectors % 10;

			   int i = 0;
			   int j = 0; /*  This is for WORD */


			   if (nErase > 0)
			   {
				   for (i = 0; i< nErase; i++)
				   {
					   pRQ->_length = 10; 
					   pRQ->_nbSectors = 10; 
					   pRQ->_data = (LPBYTE)malloc(10);


					   BYTE Convert[0xFF];


				       for (j = 0; j< 10; j++)
					   {
					   
					     Convert[j]=  pSectors [i*10*2+j*2];
					   
					   }



					   /*memcpy(pRQ->_data, pSectors+(i*10), 10);*/

					   memcpy(pRQ->_data, Convert, 10);

					   BYTE Result = Send_RQ(pRQ);
					   if (Result != SUCCESS) return Result;

					   Progress ++;
				   }
			   }

				if (Remain > 0)
			   {
				   pRQ->_length = Remain; 
				   pRQ->_nbSectors =  Remain;
				   pRQ->_data = (LPBYTE)malloc(Remain);

                   BYTE Convert[0xFF];


				   for (j = 0; j< Remain; j++)
				   {
					   
					   Convert[j]=  pSectors [i*10*2+j*2];
					   
				   }

				   /*memcpy(pRQ->_data, pSectors+(i*10), Remain);*/

				   memcpy(pRQ->_data, Convert, Remain);


				   
				   

				   BYTE Result = Send_RQ(pRQ);
				   if (Result != SUCCESS) return Result;
				   
				   Progress ++;
			   }

			   
			}

				free(pRQ);


	}


	if ((Target->ERASE_EXT_CMD))
	{
	
	
	if ((!pSectors) && (NbSectors != 0xFFFF)) return INPUT_PARAMS_MEMORY_ALLOCATION_ERROR;

			LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
			pRQ->_target   = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

			pRQ->_cmd = ERASE_EXT_CMD;


			if (NbSectors == 0xFFFF)
			{
			   pRQ->_wbSectors = 0xFFFF;
			   pRQ->_length = 0;

			   BYTE Result = Send_RQ(pRQ);
			   if (Result != SUCCESS) return Result;

			   Progress = 0xFF / 10;
			}
			else
			{
			   WORD nErase = NbSectors / 10;
			   WORD Remain = NbSectors % 10;

			   int i = 0;
			   int j = 0; /*  This is for WORD */


			   if (nErase > 0)
			   {
				   for (i = 0; i< nErase; i++)
				   {
					   pRQ->_length = 10; 
					   pRQ->_wbSectors = 10; 
					   pRQ->_data = (LPBYTE)malloc(10*2);


					   BYTE Convert[0xFF];


				       for (j = 0; j< 10*2; j++)
					   {
					   
					     Convert[j]=  pSectors [i*10*2+ j];
					   
					   }



					   /*memcpy(pRQ->_data, pSectors+(i*10), 10);*/

					   memcpy(pRQ->_data, Convert, 10*2);

					   BYTE Result = Send_RQ(pRQ);
					   if (Result != SUCCESS) return Result;

					   Progress ++;
				   }
			   }

				if (Remain > 0)
			   {
				   pRQ->_length = Remain; 
				   pRQ->_wbSectors = Remain; 
				   pRQ->_data = (LPBYTE)malloc(Remain*2);

                   BYTE Convert[0xFF];


				   for (j = 0; j< Remain*2; j++)
				   {
					   
					   Convert[j]=  pSectors [i*10*2+j];
					   
				   }

				   /*memcpy(pRQ->_data, pSectors+(i*10), Remain);*/

				   memcpy(pRQ->_data, Convert, Remain*2);


				   
				   

				   BYTE Result = Send_RQ(pRQ);
				   if (Result != SUCCESS) return Result;
				   
				   Progress ++;
			   }

			   
			}

				free(pRQ);
	}


	return SUCCESS;
}                     
    
                                                                               
USBHIDBLLIB_API BYTE BL_WRITE_PROTECT(BYTE NbSectors, LPBYTE pSectors)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->WRITE_PROTECT_CMD) return CMD_NOT_ALLOWED;

    if (!pSectors) return INPUT_PARAMS_MEMORY_ALLOCATION_ERROR;

	LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
    pRQ->_target   = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

    pRQ->_cmd = WRITE_PROTECT_CMD;
	pRQ->_length = NbSectors; 
	pRQ->_nbSectors = NbSectors; 
	pRQ->_data = (LPBYTE)malloc(NbSectors);
	memcpy(pRQ->_data, pSectors, NbSectors);

	BYTE Result = Send_RQ(pRQ);
	if (Result != SUCCESS) return Result;

	free(pRQ);

	Progress ++;
	return SUCCESS;
}             
                                                                                    
USBHIDBLLIB_API BYTE BL_WRITE_TEMP_UNPROTECT() 
{
	//to be used only with STR750
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->WRITE_TEMP_UNPROTECT_CMD) return CMD_NOT_ALLOWED;

	LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
    pRQ->_target   = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

    pRQ->_cmd = WRITE_TEMP_UNPROTECT_CMD;


	BYTE Result = Send_RQ(pRQ);
	if (Result != SUCCESS) return Result;
	
	free(pRQ);

	Progress ++;
	return SUCCESS;
}     
                                                                                    
USBHIDBLLIB_API BYTE BL_WRITE_PERM_UNPROTECT() 
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->WRITE_PERM_UNPROTECT_CMD) return CMD_NOT_ALLOWED;

	if (GetAckValue() == 0x75)
		return BL_WRITE_TEMP_UNPROTECT();

	LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
    pRQ->_target   = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

    pRQ->_cmd = WRITE_PERM_UNPROTECT_CMD;


	BYTE Result = Send_RQ(pRQ);
	if (Result != SUCCESS) return Result;
	
	free(pRQ);

	Progress ++;
	return SUCCESS;
}     
                                                                                    
USBHIDBLLIB_API BYTE BL_READOUT_PROTECT() 
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->READOUT_PERM_PROTECT_CMD) return CMD_NOT_ALLOWED;

	LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
    pRQ->_target   = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

    pRQ->_cmd = READOUT_PROTECT_CMD;


	BYTE Result = Send_RQ(pRQ);
	if (Result != SUCCESS) return Result;
	
	free(pRQ);

	Progress++;
	return SUCCESS;
}     
                                                                                    
USBHIDBLLIB_API BYTE BL_READOUT_TEMP_UNPROTECT()  
{
	// to be used only with STR750
    if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->READOUT_TEMP_UNPROTECT_CMD) return CMD_NOT_ALLOWED;

	LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
    pRQ->_target   = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

    pRQ->_cmd = READOUT_TEMP_UNPROTECT_CMD;


	BYTE Result = Send_RQ(pRQ);
	if (Result != SUCCESS) return Result;
	
	free(pRQ);

	Progress ++;
	return SUCCESS;
}  
    
/************************************************************************************/
/* READOUT_PERM_UNPROTECT
/*
/*
/************************************************************************************/                                                                                 
USBHIDBLLIB_API BYTE BL_READOUT_PERM_UNPROTECT()  
{
    if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->READOUT_PERM_UNPROTECT_CMD) return CMD_NOT_ALLOWED;

	LPBL_Request pRQ = (LPBL_Request)malloc(sizeof(BL_Request));
    pRQ->_target   = (LPTARGET_DESCRIPTOR)malloc(sizeof(TARGET_DESCRIPTOR));

    pRQ->_cmd = READOUT_PERM_UNPROTECT_CMD;


	BYTE Result = Send_RQ(pRQ);
	if (Result != SUCCESS) return Result;
	
	free(pRQ);

	Progress++;
	return SUCCESS;
}

/************************************************************************************/
/* UPLOAD
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE BL_UPLOAD(DWORD Address, LPBYTE pData, DWORD Length)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->READ_CMD) return CMD_NOT_ALLOWED;

	LPBYTE Holder = pData;
	BYTE   Result = SUCCESS;
	LPBYTE buffer = (LPBYTE) malloc(MAX_DATA_SIZE);

    DWORD nbuffer = Length / MAX_DATA_SIZE;
	DWORD ramain  = Length % MAX_DATA_SIZE;

	if (nbuffer > 0)
	{
	  for(int i=1; i<=nbuffer; i++)
	  {
		memset(buffer, 0xFF, MAX_DATA_SIZE);
        Result = BL_READ(Address, MAX_DATA_SIZE, buffer);
		if (Result != SUCCESS) return Result;
		memcpy(pData, buffer, MAX_DATA_SIZE);
		pData = pData + MAX_DATA_SIZE;
		Address += MAX_DATA_SIZE;

		Progress++;
	  }
	}

	if (ramain > 0)
	{
		memset(buffer, 0xFF, MAX_DATA_SIZE);
        Result = BL_READ(Address, ramain, buffer);
		if (Result != SUCCESS) return Result; 
		memcpy(pData, buffer, ramain);

		Progress++;
	}

	free(buffer);
	pData = Holder;
	return Result;
}

/************************************************************************************/
/* VERIFY
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE BL_VERIFY(DWORD Address, LPBYTE pData, DWORD Length,BOOL bTruncateLeadFFForDnLoad)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->READ_CMD) return CMD_NOT_ALLOWED;

	LPBYTE Holder = pData;
	BYTE   Result = SUCCESS;
	LPBYTE buffer = (LPBYTE) malloc(MAX_DATA_SIZE);

    DWORD nbuffer = Length / MAX_DATA_SIZE;
	DWORD ramain  = Length % MAX_DATA_SIZE;

	LPBYTE Empty=new BYTE[MAX_DATA_SIZE];
	memset(Empty, 0xFF, MAX_DATA_SIZE);

	if (nbuffer > 0)
	{
	  for(int i=1; i<=nbuffer; i++)
	  {
		BOOL AllFFs = FALSE;
		if((memcmp(Empty, pData, MAX_DATA_SIZE) == 0) && bTruncateLeadFFForDnLoad)
		{
           AllFFs = TRUE;
		   //_sleep(1);
		}

		if(!AllFFs)
		{
			memset(buffer, 0xFF, MAX_DATA_SIZE);
			Result = BL_READ(Address, MAX_DATA_SIZE, buffer);
			if (Result != SUCCESS) return Result;
			if (memcmp(pData, buffer, MAX_DATA_SIZE) != 0)
			{
				return CMD_NOT_ALLOWED; // verify fail
			}
		}
		pData = pData + MAX_DATA_SIZE;
		Address += MAX_DATA_SIZE;

		Progress++;
	  }
	}

	if (ramain > 0)
	{
		BOOL AllFFs = FALSE;
		if((memcmp(Empty, pData, ramain) == 0) && bTruncateLeadFFForDnLoad)
           AllFFs = TRUE;

		if(!AllFFs) 
		{
			memset(buffer, 0xFF, MAX_DATA_SIZE);
			Result = BL_READ(Address, ramain, buffer);
			if (Result != SUCCESS) return Result; 
			if (memcmp(pData, buffer, ramain) != 0)
			{
				return CMD_NOT_ALLOWED; // verify fail
			}
		}
		Progress++;
	}

	free(buffer);
	pData = Holder;
	return Result;
}
/************************************************************************************/
/* DNLOAD
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE BL_DNLOAD(DWORD Address, LPBYTE pData, DWORD Length,BOOL bTruncateLeadFFForDnLoad)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->WRITE_CMD) return CMD_NOT_ALLOWED;

	//Progress = 0;
	LPBYTE Holder = pData;
	BYTE   Result = SUCCESS;
	LPBYTE buffer = (LPBYTE) malloc(MAX_DATA_SIZE);

    DWORD nbuffer = (DWORD)(Length / MAX_DATA_SIZE);
	DWORD ramain  = (DWORD)(Length % MAX_DATA_SIZE);

	DWORD Newramain = ramain;

	LPBYTE Empty=new BYTE[MAX_DATA_SIZE];
	memset(Empty, 0xFF, MAX_DATA_SIZE);

	if (nbuffer > 0)
	{
	  for(int i=1; i<=nbuffer; i++)
	  {
		memset(buffer, 0xFF, MAX_DATA_SIZE);
        memcpy(buffer, pData, MAX_DATA_SIZE);

		BOOL AllFFs = FALSE;
		if((memcmp(Empty, buffer, MAX_DATA_SIZE) == 0) && bTruncateLeadFFForDnLoad)
		{
           AllFFs = TRUE;
		   //_sleep(1);
		}

		if(!AllFFs)
		{
          Result = BL_WRITE(Address, MAX_DATA_SIZE, buffer);
		  if (Result != SUCCESS) return Result;
		}

		pData = pData + MAX_DATA_SIZE;

		Address += MAX_DATA_SIZE;

		Progress++;
	  }
	}

	if (ramain > 0)
	{
		memset(buffer, 0xFF, MAX_DATA_SIZE);


		/* This is a workaround for an issue on STM32 Boot-loader to be verified in v2.3.0*/

	    //DWORD newdiv  = (DWORD)(ramain % 4);

	    /*if((ramain%2) != 0) Newramain++;
	    if((ramain%4) != 0) Newramain+=4;*/

        //Newramain += 4-newdiv; // removed in version 2.8.0 upon customer case @ end of flash area.
		
		/* end of */

		//Result = BL_READ(Address,  Newramain, buffer);   // removed in version 2.8.0 upon customer case @ end of flash area.
		//if (Result != SUCCESS) return Result;
		
		memcpy(buffer, pData, ramain);

		BOOL AllFFs = FALSE;
		if((memcmp(Empty, buffer, ramain) == 0) && bTruncateLeadFFForDnLoad)
           AllFFs = TRUE;

		if(!AllFFs)
		{
	      
			Result = BL_WRITE(Address, Newramain, buffer);
	      if (Result != SUCCESS) return Result;
		}


		Progress++;
	}

	free(buffer);
	pData = Holder;
	return Result;
}
/************************************************************************************/
/* SET PACKET SIZE
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE SetPaketSize(WORD size)
{
	MAX_DATA_SIZE = (int)(size / 4) * 4 ;
	return SUCCESS;
}
 
/************************************************************************************/
/* GET PACKET SIZE
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE GetPaketSize(LPBYTE size)
{
	*size = MAX_DATA_SIZE;
	return SUCCESS;
}

/************************************************************************************/
/* GetAckValue
/*
/*
/************************************************************************************/
USBHIDBLLIB_API ACKS GetAckValue()
{
  return ACK_VALUE;  	
}

/************************************************************************************/
/* IsConnected
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BOOL COM_is_Open()
{
	return Cur_COM.isConnected;
}

/************************************************************************************/
/* SetTimeOut
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE SetTimeOut(DWORD vms)
{
    if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
    Cur_COM.setTimeOut(vms);
	return SUCCESS;
}

/************************************************************************************/
/* GetFlashSize
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE TARGET_GetFlashSize(DWORD Addr, LPWORD val)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->READ_CMD) return CMD_NOT_ALLOWED;

	LPBYTE pData = new BYTE[sizeof(*val)];
	memset(pData, 0xFF, sizeof(*val));

	BYTE Result = BL_READ(Addr/*ADDR_FLASH_SIZE*/, sizeof(*val),pData);
	if (Result != SUCCESS) return Result;
	else memcpy(val, pData, sizeof(*val));
    
	return SUCCESS;  
}

/************************************************************************************/
/* GetMemoryAddress
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE TARGET_GetMemoryAddress(DWORD Addr, LPBYTE val)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->READ_CMD) return CMD_NOT_ALLOWED;

	LPBYTE pData = new BYTE[sizeof(*val)];
	memset(pData, 0xFF, sizeof(*val));

	BYTE Result = BL_READ(Addr/*ADDR_RAM_SIZE*/, sizeof(*val),pData);
	if (Result != SUCCESS) return Result;
	else memcpy(val, pData, sizeof(*val));
    
	return SUCCESS;
}

/************************************************************************************/
/* GetRDPOptionByte
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE TARGET_GetRDPOptionByte(LPBYTE RDP)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->READ_CMD) return CMD_NOT_ALLOWED;

	LPBYTE pData = new BYTE[sizeof(DWORD)];
	memset(pData, 0xFF, sizeof(DWORD));

	BYTE Result = BL_READ(ADDR_RDP_OPB, sizeof(DWORD),pData);
	if (Result != SUCCESS) return Result;
	else 
	{
	   if ((BYTE) pData[0] == 0xFF && (BYTE) pData[1] == 0xFF)
          *RDP = 0xFF;
	   else if ( ((BYTE) pData[0] + (BYTE) pData[1]) != 0xFF )
		  *RDP = 0xFF;
	   else if ( ((BYTE) pData[0] + (BYTE) pData[1]) == 0xFF )
		  *RDP = 0xA5; 
	}
    
	return SUCCESS; 
}

/************************************************************************************/
/* GetWRPOptionBytes
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE TARGET_GetWRPOptionBytes(LPBYTE WRP0, LPBYTE WRP1, LPBYTE WRP2, LPBYTE WRP3)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->READ_CMD) return CMD_NOT_ALLOWED;

	LPBYTE pData = new BYTE[sizeof(DWORD)];
	memset(pData, 0xFF, sizeof(DWORD));

	BYTE Result;

	if (GetAckValue() == 0x79)
	   Result = BL_READ(ADDR_WRP0_OPB, sizeof(DWORD),pData);
	else if (GetAckValue() == 0x75)
       Result = BL_READ(0x2010DFB0 /*ADDR_WRP0_OPB*/, sizeof(DWORD),pData);

	if (Result != SUCCESS) return Result;
	else 
	{
		if (GetAckValue() == 0x79)
		{
		   *WRP0 = (BYTE) pData[0];
		   *WRP1 = (BYTE) pData[2];
		}
		else if (GetAckValue() == 0x75)
		{
		   *WRP0 = (BYTE) pData[0];
		   *WRP1 = (BYTE) pData[1];
		   *WRP2 = (BYTE) pData[2];
		   *WRP3 = (BYTE) pData[3];
		}
	}

	memset(pData, 0xFF, sizeof(DWORD));

	if (GetAckValue() == 0x79)
	{
		Result = BL_READ(ADDR_WRP2_OPB, sizeof(DWORD),pData);
		if (Result != SUCCESS) return Result;
		else 
		{
			*WRP2 = (BYTE) pData[0];
			*WRP3 = (BYTE) pData[2];
		}
	}
    
	return SUCCESS;  
}

/************************************************************************************/
/* GetUserOptionByte
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE TARGET_GetUserOptionByte(LPBYTE User)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->READ_CMD) return CMD_NOT_ALLOWED;

	LPBYTE pData = new BYTE[sizeof(DWORD)];
	memset(pData, 0xFF, sizeof(DWORD));

	BYTE Result = BL_READ(ADDR_USER_OPB, sizeof(DWORD),pData);
	if (Result != SUCCESS) return Result;
	else *User = (BYTE) pData[2];
    
	return SUCCESS;  
}

/************************************************************************************/
/* GetDataOptionByte
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE TARGET_GetDataOptionByte(LPBYTE Data0, LPBYTE Data1)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->READ_CMD) return CMD_NOT_ALLOWED;

	LPBYTE pData = new BYTE[sizeof(DWORD)];
	memset(pData, 0xFF, sizeof(DWORD));

	BYTE Result = BL_READ(ADDR_DATA_OPB, sizeof(DWORD),pData);
	if (Result != SUCCESS) return Result;
	else 
	{
		*Data0 = (BYTE) pData[0];
		*Data1 = (BYTE) pData[2];
	}
    
	return SUCCESS; 
}

/************************************************************************************/
/* SetOptionBytes
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE TARGET_SetOptionBytes(DWORD Addr,BYTE User, BYTE RDP, BYTE Data0, BYTE Data1, 
								   BYTE WRP0, BYTE WRP1, BYTE WRP2, BYTE WRP3)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->READ_CMD) return CMD_NOT_ALLOWED;

	LPBYTE pData = new BYTE[MAX_DATA_SIZE];
	memset(pData, 0xFF, MAX_DATA_SIZE);


	if ( Addr == ADDR_F1_OPB )
	{

	pData[0]  = RDP;
	pData[1]  = ~RDP;
	pData[2]  = User;
	pData[3]  = ~User;
	pData[4]  = Data0;
	pData[5]  = ~Data0;
	pData[6]  = Data1;
	pData[7]  = ~Data1;
	pData[8]  = WRP0;
	pData[9]  = ~WRP0;
	pData[10] = WRP1;
	pData[11] = ~WRP1;
	pData[12] = WRP2;
	pData[13] = ~WRP2;
	pData[14] = WRP3;
	pData[15] = ~WRP3;

	} else

	if ( Addr == ADDR_L1_OPB )
	{

	pData[0]  = RDP;
	pData[1]  = 0x00;
	pData[2]  = ~RDP;
	pData[3]  = 0x00;
	pData[4]  = User;
	pData[5]  = 0x00;
	pData[6]  = ~User;
	pData[7]  = 0x00;
	pData[8]  = WRP0;
	pData[9]  = WRP1;
	pData[10] = ~WRP0;
	pData[11] = ~WRP1;
	pData[12] = WRP2;
	pData[13] = WRP3;
	pData[14] = ~WRP2;
	pData[15] = ~WRP3;

	} else

	if ( Addr == ADDR_F2_OPB )
	{

	pData[0]  = User;
	pData[1]  = RDP;
	pData[2]  = 0xFF;
	pData[3]  = 0xFF;
	pData[4]  = 0xFF;
	pData[5]  = 0xFF;
	pData[6]  = 0xFF;
	pData[7]  = 0xFF;
	pData[8]  = WRP0;
	pData[9]  = WRP1;
	pData[10] = 0xFF;
	pData[11] = 0xFF;
	pData[12] = 0xFF;
	pData[13] = 0xFF;
	pData[14] = 0xFF;
	pData[15] = 0xFF;

	}


	BYTE Result = BL_WRITE(Addr, 16,pData);
	if (Result != SUCCESS) return Result;
    
	return SUCCESS; 
}

/************************************************************************************/
/* SetSIFData
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE TARGET_GetOptionBytes(DWORD Addr,LPBYTE User, LPBYTE RDP, LPBYTE Data0, LPBYTE Data1, 
								   LPBYTE WRP0, LPBYTE WRP1, LPBYTE WRP2, LPBYTE WRP3)
{
	if (!Cur_COM.isConnected)  return NO_CON_AVAILABLE;
	if (!Target->READ_CMD) return CMD_NOT_ALLOWED;

	LPBYTE pData = new BYTE[MAX_DATA_SIZE];
	memset(pData, 0xFF, MAX_DATA_SIZE);

	BYTE Result = BL_READ(Addr, 16,pData);
	if (Result != SUCCESS) return Result;

	if ( Addr == ADDR_F1_OPB )
	{

	*RDP   = pData[0] ;
	*User  = pData[2];
    *Data0 = pData[4];
	*Data1 = pData[6];
	*WRP0  = pData[8];
	*WRP1  = pData[10];
	*WRP2  = pData[12];
	*WRP3  = pData[14];
	}
	else if  ( Addr == ADDR_L1_OPB )
	{
	*RDP   = pData[0] ;
	*User  = pData[4];
    *Data0 = 0x00;
	*Data1 = 0x00;
	*WRP0  = pData[8];
	*WRP1  = pData[9];
	*WRP2  = pData[12];
	*WRP3  = pData[13];

	}
	else if  ( Addr == ADDR_F2_OPB )
	{
	*RDP   = pData[1] ;
	*User  = pData[0];
    *Data0 = 0xFF;
	*Data1 = 0xFF;
	*WRP0  = pData[8];
	*WRP1  = pData[9];
	*WRP2  = 0xFF;
	*WRP3  = 0xFF;

	}

    
	return SUCCESS; 
}

/************************************************************************************/
/* GET PROGRESS STATE
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE GetProgress(LPBYTE progress)
{
   if (ActivityTime == 0) ActivityTime = 1;

   int prog = (Progress*100)/ActivityTime;
   if (prog > 100) 
	   prog = 100;

   *progress = prog;
   return SUCCESS;
}
/************************************************************************************/
/* Get activity time
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE GetActivityTime(LPDWORD time)
{
   *time = ActivityTime;
   return SUCCESS;
}  

/************************************************************************************/
/* Set activity time
/*
/*
/************************************************************************************/
USBHIDBLLIB_API BYTE SetActivityTime(DWORD time)
{
   Progress = 0;
   ActivityTime = time;
   return SUCCESS;
}

/************************************************************************************/
/*   Set Rts Line State
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
USBHIDBLLIB_API BYTE BL_SetRts(bool Val)
{
	Cur_COM.setRts(Val);
	return SUCCESS;
}

/************************************************************************************/
/*   Set Dtr Line State
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
USBHIDBLLIB_API BYTE BL_SetDtr(bool Val)
{
    Cur_COM.setDtr(Val);
	return SUCCESS;
}

/************************************************************************************/
/*   Set the state of TXD. Return: true if success.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
USBHIDBLLIB_API BYTE BL_setTxd(bool val)
{
	Cur_COM.setTxd(val);
	return SUCCESS;
}
		
/************************************************************************************/
/*   Return: The state of CTS.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
USBHIDBLLIB_API BYTE BL_getCts(bool* pval)	
{
   *pval = Cur_COM.getCts();
   return SUCCESS;
}				
			
/************************************************************************************/
/*   Return: The state of DTR.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
USBHIDBLLIB_API BYTE BL_getDtr(bool* pval)	
{
   *pval = Cur_COM.getDtr();
   return SUCCESS;
}
					
/************************************************************************************/
/*   Return: The state of RI.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
USBHIDBLLIB_API BYTE BL_getRi(bool* pval)
{
   *pval = Cur_COM.getRi();
   return SUCCESS;
}					
	
/************************************************************************************/
/*   Return: The state of DTR.
/*   High = TRUE
/*   Low  = FALSE
/************************************************************************************/
USBHIDBLLIB_API BYTE BL_getCd(bool* pval)	
{
   *pval = Cur_COM.getCd();
   return SUCCESS;
}


/************************************************************************************/
/* Set Echo Mode
/*   0   =  Echo Disabled
/*   1   =  Echo Back Enabled
/*   2   =  Listen Echo Enabled
/************************************************************************************/
USBHIDBLLIB_API  BYTE BL_SetEcho(int val)
{
	Cur_COM.bEcho= val;
	return SUCCESS;

}


/************************************************************************************/
/* SetFlowControl : Enable/Disable Flow Control of DTR and RTS
/* FALSE   =   Disabled
/* TRUE    =   Enabled
/************************************************************************************/
USBHIDBLLIB_API  BYTE BL_SetFlowControl(bool val)
{
	Cur_COM.FlowControl= val;
	return SUCCESS;
}
/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE******/


