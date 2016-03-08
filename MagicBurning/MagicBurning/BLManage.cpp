#include "stdafx.h"


#include "ini.h"
#include "BLManage.h"

/*******************************************************************************************/
/* Function    : FileExist                                                                 */
/* IN          : file name                                                                 */
/* OUT         : boolean                                                                   */
/* Description : verify if the given file exists                                           */
/*******************************************************************************************/
bool CBLMANAGE::FileExist(char* filename)
{
	// Data structure for FindFirstFile
	WIN32_FIND_DATAA findData;

	// Clear find structure
	ZeroMemory(&findData, sizeof(findData));

	// Search the file
	HANDLE hFind = FindFirstFileA( filename, &findData );
	if ( hFind == INVALID_HANDLE_VALUE )
	{
	// File not found
	return false;
	}

	// File found

	// Release find handle
	FindClose( hFind );
	hFind = NULL;

	// The file exists
	return true;
}

CBLMANAGE::CBLMANAGE()
{
	MainpWnd=NULL;
	OP_Busy=false;
	flagThreadMonitor=true;
	current_info.PIsConnected=false;
	current_info.MIsConnected=false;
	current_info.pmMapping=NULL;
	TARGET_SetComIntType(0);
	hThreadMonitor = CreateThread( NULL,                        
						   0,                           
						   ThreadMonitor,     
						   this,              
						   0,                           
						   NULL);
}

void CBLMANAGE::COMreset(void)
{
	current_info.PIsConnected=false;
	current_info.MIsConnected=false;
	
	if(current_info.pmMapping!=NULL)
	{
		if(current_info.pmMapping->pSectors!=NULL)
			free(current_info.pmMapping->pSectors);
		free(current_info.pmMapping);
		current_info.pmMapping=NULL;
	}

	COM_Close();
}

CBLMANAGE::~CBLMANAGE()
{
	flagThreadMonitor=false;
	WaitForSingleObject(hThreadMonitor, 100);
	CloseHandle(hThreadMonitor);

	if(COM_is_Open())  
		COM_Close();
}

bool CBLMANAGE::CheckOPBusy(void)
{
	return OP_Busy;
}

bool CBLMANAGE::OP_Req_asycn(LPCBLMANAGE_OP lpop,BYTE order) // bit0: clear flag; bit1: end flag
{
	if(OP_Busy)
		return false;
	if(MainpWnd==NULL)
		return false;
	
	OP_sync=false;
	if(order&0x01)
		man_queue.c.clear();
	if(order&0x02)
		OP_Busy=true;
	printf("push %d %d\n",lpop->op,lpop->SecType);
	man_queue.push(*lpop);
	//memcpy(&current_op,lpop,sizeof(CBLMANAGE_OP));
	
	return true;
}

bool CBLMANAGE::OP_Req_sycn(LPCBLMANAGE_INFO lpinfo,LPCBLMANAGE_OP lpop)
{
	if(OP_Busy)
		return false;
	man_queue.c.clear();
	OP_sync=true;
	man_queue.push(*lpop);
	OP_Busy=true;
	while(OP_Busy);
	memcpy(lpop,&current_op,sizeof(CBLMANAGE_OP));
	memcpy(lpinfo,&current_info,sizeof(CBLMANAGE_INFO));
	if(current_info.op_done)
		return true;
	else
		return false;
}

void CBLMANAGE::SetMainPwnd(CWnd *pWnd)
{
	MainpWnd=pWnd;
}

void CBLMANAGE::GetCurInfo(LPCBLMANAGE_INFO lpinfo)
{
	memcpy(lpinfo,&current_info,sizeof(CBLMANAGE_INFO));
}

void CBLMANAGE::GetCurOp(LPCBLMANAGE_INFO lpop)
{
	memcpy(lpop,&current_op,sizeof(CBLMANAGE_OP));
}

void CBLMANAGE::SetMCUBase(DWORD rambase,DWORD flashbase)
{
	current_info.DEV_RAM_BASE=rambase;
	current_info.DEV_FLASH_BASE=flashbase;
}

void CBLMANAGE::ProgressProc(BYTE prog)
{
	if(prog<100)
	{
		current_info.progress=prog;
	}
	else if(prog==100)
	{
		current_info.progress=prog;
		current_info.op_done=true;
		current_info.op_success=true;
	}
	else if((prog==101)&&(current_info.progress!=100))
	{
		current_info.op_done=true;
		current_info.op_success=false;
		man_queue.c.clear();
	}

	printf("%s %d %% %d %d\n",current_info.CurPhase.c_str(),current_info.progress,current_info.op_done,current_info.op_success);
	
	if((MainpWnd!=NULL)&&(!OP_sync))
		MainpWnd->SendMessage(WM_UPDATEDATA, (WPARAM)&current_info,(LPARAM)&current_op);
}

DWORD WINAPI ThreadMonitor (LPVOID param)
{
	CBLMANAGE* pThreadMonitor=(CBLMANAGE*)param;
	BYTE Res;
	DWORD lasttick;

	char devname[256];
	char MapFile[256];
	HINSTANCE   hInst;     
	char    szBuf[256];
	char Drive[3], Dir[256], Fname[256], Ext[256];
	WORD PacketSize;
	WORD Size;
	char MapName[256];
	BYTE PagePerSector;
	BYTE Version ;
	Commands pCmds;
	BYTE size;
	char *ptr;
	HANDLE Handle;
	DWORD NbElements;


	lasttick=GetTickCount();
	while(pThreadMonitor->flagThreadMonitor)
	{
		if(pThreadMonitor->OP_Busy)
		{
			printf("OP %d %d\n",pThreadMonitor->current_op.op,pThreadMonitor->current_op.SecType);
			while(!pThreadMonitor->man_queue.empty())
			{
				printf("OP1 %d %d\n",pThreadMonitor->current_op.op,pThreadMonitor->current_op.SecType);
				pThreadMonitor->current_info.op_done=false;
				pThreadMonitor->current_info.op_success=false;
				pThreadMonitor->current_info.progress=0;
				Handle=NULL;

				memcpy(&pThreadMonitor->current_op,&pThreadMonitor->man_queue.front(),sizeof(CBLMANAGE_OP));
				if(!pThreadMonitor->man_queue.empty())
					pThreadMonitor->man_queue.pop();
				if(pThreadMonitor->current_op.op==CBLMANAGE_NULL)
				{
					pThreadMonitor->current_info.CurPhase="NULL ";
					pThreadMonitor->ProgressProc(20);
					pThreadMonitor->ProgressProc(100);
				}
				else if(pThreadMonitor->current_op.op==CBLMANAGE_TEST)
				{
					pThreadMonitor->current_info.CurPhase="Test1 ";
					pThreadMonitor->ProgressProc(20);
					for(int i=0;i<99;i++)
					{
						pThreadMonitor->ProgressProc(i);
						_sleep(100);
					}
					pThreadMonitor->current_info.CurPhase="Test2 ";
					for(int i=0;i<99;i++)
					{
						pThreadMonitor->ProgressProc(i);
						_sleep(100);
					}
					pThreadMonitor->ProgressProc(100);
				}
				if(pThreadMonitor->current_op.op==CBLMANAGE_CONNECT)
				{
				   pThreadMonitor->current_info.CurPhase="Connect ";
				   pThreadMonitor->ProgressProc(1);
				   pThreadMonitor->COMreset();
				   
				   if(pThreadMonitor->current_op.pro.MCU_COM==1)
					   sprintf(devname,"%s_IAP.SImap", pThreadMonitor->current_op.MCU_name);
				   else
					   sprintf(devname,"%s.SImap", pThreadMonitor->current_op.MCU_name);

				   //hInst
				   GetModuleFileNameA(NULL,szBuf,sizeof(szBuf));
				   
				   _splitpath_s(szBuf, Drive, _MAX_DRIVE, Dir, _MAX_DIR, Fname, _MAX_FNAME, Ext, _MAX_EXT);
						
				   sprintf(MapFile, "%s%s%s%s", Drive, Dir , "Map\\", devname);

				   PacketSize = 0;
				   Size = 0;
				   pThreadMonitor->current_info.pmMapping = NULL;
				   PagePerSector = 0;
				   
				   if (!pThreadMonitor->FileExist((char*)MapFile))
				   {
					   goto OP_FINISH;
				   }

				   FILES_GetMemoryMapping((LPSTR)(LPCTSTR)MapFile, &Size, (LPSTR)MapName, &PacketSize, pThreadMonitor->current_info.pmMapping, &PagePerSector);
				   // Allocate the mapping structure memory
				   pThreadMonitor->current_info.pmMapping = (PMAPPING)malloc(sizeof(MAPPING));
				   pThreadMonitor->current_info.pmMapping->NbSectors = 0;
				   pThreadMonitor->current_info.pmMapping->pSectors = (PMAPPINGSECTOR) malloc((Size) * sizeof(MAPPINGSECTOR));

				   // Get the mapping info
				   FILES_GetMemoryMapping((LPSTR)(LPCTSTR)MapFile, &Size, (LPSTR)(LPCTSTR)MapName, &PacketSize, pThreadMonitor->current_info.pmMapping, &PagePerSector);
					   
				   SetPaketSize(PacketSize);

				   //30%
				   pThreadMonitor->ProgressProc(50);
					//need set config
				   string str=pThreadMonitor->current_op.Programmer_name;
				   Res = COM_Open(pThreadMonitor->current_op.Programmer_name);
					SetTimeOut(3500);
					if ((Res != SUCCESS) && (Res != COM_ALREADY_OPENED)) 
					{
						goto OP_FINISH;
					}
					else
					{
						if(BL_Init_BL()!=SUCCESS)
							goto OP_FINISH;
					}

					pThreadMonitor->current_info.MIsConnected=true;
					pThreadMonitor->current_info.PIsConnected=true;
					pThreadMonitor->current_info.authed=false;
					pThreadMonitor->current_info.ValidChecked=false;
					if((pThreadMonitor->MainpWnd!=NULL)&&(!pThreadMonitor->OP_sync))
						pThreadMonitor->MainpWnd->SendMessage(WM_UPDATEDATA, (WPARAM)&(pThreadMonitor->current_info),NULL);
					pThreadMonitor->ProgressProc(100);
				}
				else if(pThreadMonitor->current_op.op==CBLMANAGE_DISCONNECT)
				{
					pThreadMonitor->current_info.CurPhase="Disconnect ";
					pThreadMonitor->ProgressProc(20);
					//if(pThreadMonitor->current_info.PIsConnected&&pThreadMonitor->current_info.MIsConnected)
					//	BL_GO(0);
					if(COM_is_Open())  
						COM_Close();
					pThreadMonitor->ProgressProc(60);
					pThreadMonitor->current_info.PIsConnected=false;
					pThreadMonitor->current_info.MIsConnected=false;
					pThreadMonitor->current_info.ValidChecked=false;
					pThreadMonitor->current_info.authed=false;
					if((pThreadMonitor->MainpWnd!=NULL)&&(!pThreadMonitor->OP_sync))
						pThreadMonitor->MainpWnd->SendMessage(WM_UPDATEDATA, (WPARAM)&(pThreadMonitor->current_info),NULL);
					pThreadMonitor->ProgressProc(100);
				}
				else if(pThreadMonitor->current_op.op==CBLMANAGE_SET_CONFIG)
				{
					pThreadMonitor->current_info.CurPhase="Set Config ";
					pThreadMonitor->ProgressProc(50);
					if ((!pThreadMonitor->current_info.PIsConnected)||(!pThreadMonitor->current_info.MIsConnected))
						goto OP_FINISH;

					Res = BL_SET_PRO(&(pThreadMonitor->current_op.pro), SET_PRO_CMD);
					if (Res == SUCCESS)
						pThreadMonitor->current_info.op_done=true;
					else
						goto OP_FINISH;

					if(BL_Init_BL()!=SUCCESS)
							goto OP_FINISH;

					pThreadMonitor->ProgressProc(100);
				}
				else if(pThreadMonitor->current_op.op==CBLMANAGE_SET_OFF_CONFIG)
				{
					pThreadMonitor->current_info.CurPhase="Set OffCFG ";
					pThreadMonitor->ProgressProc(50);
					if ((!pThreadMonitor->current_info.PIsConnected)||(!pThreadMonitor->current_info.MIsConnected))
						goto OP_FINISH;

					Res = BL_SET_PRO(&(pThreadMonitor->current_op.pro), SET_OFFLINE_CMD);
					if (Res != SUCCESS)
						goto OP_FINISH;
				
					pThreadMonitor->ProgressProc(100);
				}
				else if(pThreadMonitor->current_op.op==CBLMANAGE_GET_INFO)
				{
					pThreadMonitor->current_info.CurPhase="Get Info ";
					pThreadMonitor->ProgressProc(1);
					if ((!pThreadMonitor->current_info.PIsConnected)||(!pThreadMonitor->current_info.MIsConnected))
						goto OP_FINISH;
					
					if (BL_GET(&pThreadMonitor->current_info.target.Version, &pCmds)!=SUCCESS)
						goto OP_FINISH;
					if (BL_GET_ID(&size,NULL,&pThreadMonitor->current_info.target.ChipVersion,pThreadMonitor->current_info.target.ChipID,pThreadMonitor->current_info.target.FlashLock) != SUCCESS)
						goto OP_FINISH;
					if (size!=2)
						goto OP_FINISH;
					pThreadMonitor->ProgressProc(30);
					if (BL_GET_ID(&size,pThreadMonitor->current_info.target.PID,&pThreadMonitor->current_info.target.ChipVersion,pThreadMonitor->current_info.target.ChipID,pThreadMonitor->current_info.target.FlashLock) != SUCCESS)
						goto OP_FINISH;
					pThreadMonitor->ProgressProc(60);
					if (BL_PRO_GET(&pThreadMonitor->current_info.pro) != SUCCESS)
						goto OP_FINISH;

					pThreadMonitor->current_info.target.ERASE_CMD=pCmds.ERASE_CMD;
					pThreadMonitor->current_info.target.ERASE_EXT_CMD=pCmds.ERASE_EXT_CMD;
					pThreadMonitor->current_info.target.GET_CMD=pCmds.GET_CMD;
					pThreadMonitor->current_info.target.GET_ID_CMD=pCmds.GET_ID_CMD;
					pThreadMonitor->current_info.target.GET_VER_ROPS_CMD=pCmds.GET_VER_ROPS_CMD;
					pThreadMonitor->current_info.target.GO_CMD=pCmds.GO_CMD;
					pThreadMonitor->current_info.target.READ_CMD=pCmds.READ_CMD;
					pThreadMonitor->current_info.target.READOUT_PERM_PROTECT_CMD=pCmds.READOUT_PROTECT_CMD;
					pThreadMonitor->current_info.target.READOUT_PERM_UNPROTECT_CMD=pCmds.READOUT_PERM_UNPROTECT_CMD;
					pThreadMonitor->current_info.target.READOUT_TEMP_UNPROTECT_CMD=pCmds.READOUT_TEMP_UNPROTECT_CMD;
					pThreadMonitor->current_info.target.WRITE_CMD=pCmds.WRITE_CMD;
					pThreadMonitor->current_info.target.WRITE_PERM_UNPROTECT_CMD=pCmds.WRITE_PERM_UNPROTECT_CMD;
					pThreadMonitor->current_info.target.WRITE_PROTECT_CMD=pCmds.WRITE_PROTECT_CMD;
					pThreadMonitor->current_info.target.WRITE_TEMP_UNPROTECT_CMD=pCmds.WRITE_TEMP_UNPROTECT_CMD;

					pThreadMonitor->ProgressProc(100);
				}
				else if(pThreadMonitor->current_op.op==CBLMANAGE_EARSE)
				{
					pThreadMonitor->current_info.CurPhase="Earse ";
					pThreadMonitor->ProgressProc(10);
					if ((!pThreadMonitor->current_info.PIsConnected)||(!pThreadMonitor->current_info.MIsConnected))
						goto OP_FINISH;
					SetTimeOut(8000);
					Res = BL_ERASE(0xFFFF, NULL);
					SetTimeOut(3500);
					if (Res == SUCCESS)
						pThreadMonitor->current_info.op_done=true;
					else
						goto OP_FINISH;
					pThreadMonitor->ProgressProc(100);
				}
				else if(pThreadMonitor->current_op.op==CBLMANAGE_DOWNLOAD)
				{
					pThreadMonitor->current_info.CurPhase="Download ";
					pThreadMonitor->ProgressProc(1);
					if ((!pThreadMonitor->current_info.PIsConnected)||(!pThreadMonitor->current_info.MIsConnected))
						goto OP_FINISH;

					if(pThreadMonitor->current_info.pmMapping==NULL)
						goto OP_FINISH;

					_splitpath_s(pThreadMonitor->current_op.FWfile_download, Drive, _MAX_DRIVE, Dir, _MAX_DIR, Fname, _MAX_FNAME, Ext, _MAX_EXT);
					ptr=strupr(Ext);
					strcpy(Ext, ptr);

					 PMAPPINGSECTOR pSector = pThreadMonitor->current_info.pmMapping->pSectors;
				   for(int i = 1; i<= (int)pThreadMonitor->current_info.pmMapping->NbSectors; i++)
				   {
					   if ((strcmp(Ext, ".BIN")!=0) && (i==0))
						   pThreadMonitor->current_op.start_addr = pSector->dwStartAddress;

					   pSector->UseForOperation = TRUE;
					   pSector++;
				   }

				   if(!pThreadMonitor->FileExist((char*)pThreadMonitor->current_op.FWfile_download))
					   goto OP_FINISH;

				   if (FILES_ImageFromFile((LPSTR)(LPCSTR)pThreadMonitor->current_op.FWfile_download,&Handle, 1) == FILES_NOERROR)
				   {
					  FILES_SetImageName(Handle,(LPSTR)(LPCSTR)pThreadMonitor->current_op.FWfile_download);

					  DWORD NbElements = 0;
					  if (FILES_GetImageNbElement(Handle, &NbElements) == FILES_NOERROR)
					  {
							if ( NbElements > 0 )
							{   // if binary file -> change the elemnts address
								if (strcmp(Ext, ".BIN")==0)
								{
									
									for (int i=0;i< (int)NbElements;i++)
									{
										IMAGEELEMENT Element={0};
										if (FILES_GetImageElement(Handle, i, &Element) == FILES_NOERROR)
										{
										   Element.Data= (LPBYTE)malloc(Element.dwDataLength);
										   if (FILES_GetImageElement(Handle, i, &Element) == FILES_NOERROR)
										   {
											   //Fixed in V2.8.0// Element.dwAddress = Element.dwAddress + address; 
											   Element.dwAddress = pThreadMonitor->current_op.start_addr;									  
											   FILES_SetImageElement(Handle, i, FALSE, Element);
										   }
										}
									}
								}
							}
					  }

					  FILES_FilterImageForOperation(Handle, pThreadMonitor->current_info.pmMapping, OPERATION_UPLOAD, pThreadMonitor->current_op.optimize);
				   }
				   else 
				   {  
						goto OP_FINISH;
				   }
				   pThreadMonitor->ProgressProc(5);

				   DWORD NbElements = 0;
				   if (FILES_GetImageNbElement(Handle, &NbElements) == FILES_NOERROR)
				   {
					   for (int el=0; el< (int)NbElements;el++)
					   {
							IMAGEELEMENT Element={0};
							if (FILES_GetImageElement(Handle, el, &Element) == FILES_NOERROR)
							{	   
							   Element.Data= (LPBYTE)malloc(Element.dwDataLength);
							   if (FILES_GetImageElement(Handle, el, &Element) == FILES_NOERROR)
							   {
								  if ((strcmp(Ext, ".BIN")==0) && (el==0)) 
									  Element.dwAddress = pThreadMonitor->current_op.start_addr;
								  printf("downloading, Address: %08X Length: %d Optimize: %d \n",Element.dwAddress,Element.dwDataLength,pThreadMonitor->current_op.optimize);
								  if (BL_DNLOAD(Element.dwAddress, Element.Data, Element.dwDataLength, pThreadMonitor->current_op.optimize) != SUCCESS)
								  {
									  goto OP_FINISH;
								  }

								  pThreadMonitor->ProgressProc((DWORD)((90-5)*el/NbElements+5));
								  //write_debug_info( "downloading", el ,Element.dwAddress, (float)Element.dwDataLength/(float)1024, OK);
							   }
							}
					   }
				   }

				   bool VerifySuccess = true;
				   if (pThreadMonitor->current_op.verify)
				   {
					   //printf("\n VERIFYING ... \n\n");
						pThreadMonitor->current_info.CurPhase="Verify ";
						pThreadMonitor->ProgressProc(1);

						for (int el=0; el< (int)NbElements;el++)
						{
							IMAGEELEMENT Element={0};
							if (FILES_GetImageElement(Handle, el, &Element) == FILES_NOERROR)
							{
								Element.Data=(LPBYTE)malloc(Element.dwDataLength);
								if (FILES_GetImageElement(Handle, el, &Element) == FILES_NOERROR)
								{
									if ((strcmp(Ext, ".BIN")==0) && (el==0)) 
										Element.dwAddress = pThreadMonitor->current_op.start_addr;
									printf("Verifying, Address: %08X Length: %d Optimize: %d \n",Element.dwAddress,Element.dwDataLength,pThreadMonitor->current_op.optimize);
									if (BL_VERIFY(Element.dwAddress, Element.Data, Element.dwDataLength, pThreadMonitor->current_op.optimize) != SUCCESS)
									{
										goto OP_FINISH;
									}

									//write_debug_info("verifying" ,el ,Element.dwAddress, (float)Element.dwDataLength/(float)1024, OK);
									pThreadMonitor->ProgressProc((DWORD)(90*el/NbElements+5));
								}
							}
						}
				   }
				   if((pThreadMonitor->current_op.jmptoapp)&&(pThreadMonitor->current_info.pro.BURNING_TYPE!=0))//not offline mode
				   {
						BL_GO(0);
						if(COM_is_Open())  
							COM_Close();
						pThreadMonitor->ProgressProc(60);
						pThreadMonitor->current_info.PIsConnected=false;
						pThreadMonitor->current_info.MIsConnected=false;
						pThreadMonitor->current_info.ValidChecked=false;
						pThreadMonitor->current_info.authed=false;
				   }

				   pThreadMonitor->ProgressProc(100);


				}
				else if(pThreadMonitor->current_op.op==CBLMANAGE_UPLOAD)
				{
					pThreadMonitor->current_info.CurPhase="Upload ";
					pThreadMonitor->ProgressProc(1);
					if ((!pThreadMonitor->current_info.PIsConnected)||(!pThreadMonitor->current_info.MIsConnected))
						goto OP_FINISH;

					if(pThreadMonitor->current_info.pmMapping==NULL)
						goto OP_FINISH;
					//FILES_CreateImage(&Handle, 0);

					FILES_CreateImageFromMapping(&Handle,pThreadMonitor->current_info.pmMapping);

					NbElements = 0;
				   if (FILES_GetImageNbElement(Handle, &NbElements) == FILES_NOERROR)
				   {
					   if (NbElements > 0)
					   {
								for(int i = 0; i< (int)NbElements; i++)
								{
									IMAGEELEMENT Element={0};
									// Get element data size
									if (FILES_GetImageElement(Handle, i, &Element) == FILES_NOERROR)
									{
										//Upload element data
										Element.Data = (LPBYTE)malloc(Element.dwDataLength);
										if (BL_UPLOAD(Element.dwAddress, Element.Data, Element.dwDataLength) == SUCCESS)
										{
										   //Insert elment in the Image
										   //write_debug_info("Uploading", i ,Element.dwAddress, (float)Element.dwDataLength/(float)1024, OK);
											printf("Upload, Address: %08X Length: %d \n",Element.dwAddress,Element.dwDataLength);
										   FILES_SetImageElement(Handle,i,FALSE,Element);
										}
										else 
										{
											goto OP_FINISH;
										}
									}

									pThreadMonitor->ProgressProc((DWORD)(90*i/NbElements));
									
								}
					   }

					   
				   }

				   if(!pThreadMonitor->FileExist((char*)pThreadMonitor->current_op.FWfile_upload))
				   {
						  //printf( "file %s does not exist .. Creating file\n", filename);
						  FILE* fp = fopen(pThreadMonitor->current_op.FWfile_upload, "a+");
						  fclose(fp);
				   }

				   //printf( "Writing data ...\n");

				   if (FILES_ImageToFile((LPSTR)(LPCSTR)pThreadMonitor->current_op.FWfile_upload,Handle) != FILES_NOERROR)
				   {
						 goto OP_FINISH;
				   }
				   else
				   {
					   pThreadMonitor->ProgressProc(100);
					   //printf("\n Uploaded data is dumped on %s\n", filename);
				   }

				   /*if (FILES_GetImageNbElement(Handle, &NbElements) == FILES_NOERROR)
				   {
					   if (NbElements > 0)
					   {
								for(int i = 0; i< (int)NbElements; i++)
								{
									FILES_DestroyImageElement(Handle,i);
								}
					   }

					   
				   }

				   if(Handle!=NULL)
					   FILES_DestroyImage(&Handle);*/

					/*for(int i=0;i<128;i++)
					   {
							BYTE data[2048];
							if (BL_UPLOAD(0x80000000, data, 2048) == SUCCESS)
							{
							   //Insert elment in the Image
							   //write_debug_info("Uploading", i ,Element.dwAddress, (float)Element.dwDataLength/(float)1024, OK);
								printf("Upload, Address: 8000000 Length: 2048 \n");
							   
							}
							else 
							{
								goto OP_FINISH;
							}
					   }

					pThreadMonitor->ProgressProc(100);*/

				}
				else if(pThreadMonitor->current_op.op==CBLMANAGE_SECURITY)
				{
					printf("CBLMANAGE_SECURITY %d \n",pThreadMonitor->current_op.SecType);
					if ((!pThreadMonitor->current_info.PIsConnected)||(!pThreadMonitor->current_info.MIsConnected))
						goto OP_FINISH;

					printf("CBLMANAGE_SECURITY1 %d \n",pThreadMonitor->current_op.SecType);
					if(pThreadMonitor->current_op.SecType==0)
					{
						SetTimeOut(8000);
						pThreadMonitor->current_info.CurPhase="PWD Write ";
						pThreadMonitor->ProgressProc(50);
						Res = BL_SEC(pThreadMonitor->current_op.pwd, &pThreadMonitor->current_op.MAX_AUTH_NUM, pThreadMonitor->current_op.FlashLock,pThreadMonitor->current_op.FlashBusKey,&pThreadMonitor->current_op.FlashBusEnable,SET_PWD);
						SetTimeOut(3500);
					}
					else if(pThreadMonitor->current_op.SecType==1)
					{
						pThreadMonitor->current_info.CurPhase="PWD AUTH ";
						pThreadMonitor->ProgressProc(50);
						Res = BL_SEC(pThreadMonitor->current_op.pwd, &pThreadMonitor->current_op.MAX_AUTH_NUM, pThreadMonitor->current_op.FlashLock,pThreadMonitor->current_op.FlashBusKey,&pThreadMonitor->current_op.FlashBusEnable, AUTH_PWD);
						if(Res==SUCCESS)
							pThreadMonitor->current_info.authed=true;
					}
					else if(pThreadMonitor->current_op.SecType==2)
					{
						SetTimeOut(8000);
						pThreadMonitor->current_info.CurPhase="PWD REMOVE ";
						pThreadMonitor->ProgressProc(50);
						Res = BL_SEC(pThreadMonitor->current_op.pwd, &pThreadMonitor->current_op.MAX_AUTH_NUM, pThreadMonitor->current_op.FlashLock,pThreadMonitor->current_op.FlashBusKey,&pThreadMonitor->current_op.FlashBusEnable, REMOVE_PWD);
						SetTimeOut(3500);
					}
					else if(pThreadMonitor->current_op.SecType==3)
					{
						pThreadMonitor->current_info.CurPhase="PWD Check ";
						pThreadMonitor->ProgressProc(50);
						if(BL_SEC(pThreadMonitor->current_op.pwd, &pThreadMonitor->current_info.CUR_AUTH_NUM, pThreadMonitor->current_op.FlashLock,pThreadMonitor->current_op.FlashBusKey,&pThreadMonitor->current_op.FlashBusEnable, CHK_PWD_VALID)==SUCCESS)
							pThreadMonitor->current_info.ValidChecked=true;
						else
							pThreadMonitor->current_info.ValidChecked=false;
						Res = SUCCESS;
					}
					else if(pThreadMonitor->current_op.SecType==4)
					{
						pThreadMonitor->current_op.SecType=2;
						pThreadMonitor->current_info.CurPhase="Restore ";
						pThreadMonitor->ProgressProc(50);
						Res = BL_SEC(pThreadMonitor->current_op.pwd, &pThreadMonitor->current_op.MAX_AUTH_NUM, pThreadMonitor->current_op.FlashLock,pThreadMonitor->current_op.FlashBusKey,&pThreadMonitor->current_op.FlashBusEnable, REMOVE_PWD);
					}
					else
					{
						pThreadMonitor->current_info.CurPhase="PWD Unknown";
						pThreadMonitor->ProgressProc(1);
						goto OP_FINISH;
					}
					
					if (Res == SUCCESS)
						pThreadMonitor->current_info.op_done=true;
					else
						goto OP_FINISH;
					pThreadMonitor->ProgressProc(100);
				}

OP_FINISH:
				//if(pThreadMonitor->getstatusHandler!=NULL)
				//	(*(pThreadMonitor->getstatusHandler))((pThreadMonitor->current_info),(pThreadMonitor->current_op));
				
				if(!pThreadMonitor->man_queue.empty())
					pThreadMonitor->ProgressProc(101);
			}
			pThreadMonitor->OP_Busy=false;
			pThreadMonitor->ProgressProc(101);
		}
		else
		{
			//loop heart beat
			
			if(GetTickCount()-lasttick>1000)
			{
				if(pThreadMonitor->current_info.PIsConnected)
				{
					if (BL_PRO_GET(&pThreadMonitor->current_info.pro) != SUCCESS)
					{
						if(COM_is_Open())  
							COM_Close();
						pThreadMonitor->current_info.MIsConnected=false;
						pThreadMonitor->current_info.PIsConnected=false;
						pThreadMonitor->current_info.ValidChecked=false;
						pThreadMonitor->current_info.authed=false;
						if(pThreadMonitor->MainpWnd!=NULL)
							pThreadMonitor->MainpWnd->SendMessage(WM_UPDATEDATA, (WPARAM)&(pThreadMonitor->current_info),NULL);
					}
				}
				/*if(pThreadMonitor->current_info.MIsConnected)
				{
					if (BL_GET_ID(&size,NULL) != SUCCESS)
					{
						pThreadMonitor->current_info.MIsConnected=false;
						pThreadMonitor->current_info.ValidChecked=false;
						pThreadMonitor->current_info.authed=false;
						if(pThreadMonitor->MainpWnd!=NULL)
							pThreadMonitor->MainpWnd->SendMessage(WM_UPDATEDATA, (WPARAM)&(pThreadMonitor->current_info),NULL);
					}
				}*/
				lasttick=GetTickCount();
			}
		}
		Sleep(10);
	}
	return 0;
}