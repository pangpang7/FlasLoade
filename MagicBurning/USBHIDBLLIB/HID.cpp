#include <stdio.h>
#include <string.h>
#include "stdafx.h"

#include "HID.h"

#define ACK 0x79
#define DATA 0x55


CBLHID::CBLHID()
{
	
	DWORD cnt;
	isConnected = FALSE;
	timeout=2000;
	HID_Init();
	
}
CBLHID::~CBLHID()
{
	HID_UnInit();
}

void CBLHID::SetComSettings(int _numPort, long _speedInBaud, int _nbBit, int _parity, float _nbStopBit)
{

}

bool CBLHID::open(char* devname)
{
  char  buf[1024];
  int   n, i;

  if (isConnected)
	  return FALSE;

  n = HID_FindDevices();

  for (i = 0; i < n; i++) 
  {
    if (HID_GetName(i, buf, sizeof(buf))) 
	{
	  if (strcmp(devname,buf))
	  {
		  if (HID_Open(i))
		  {
			  isConnected=TRUE;
			  return TRUE;
		  }
		  else
			  return FALSE;
	  }
	}
  }

  return FALSE;
}

bool CBLHID::open()
{	
	char devname[256]="SI PROGRAMMER X1";
	//string devname="Device 0";
	return open(devname);
}

void CBLHID::closeCom()
{
	HID_Close();
	isConnected=FALSE;
}

bool CBLHID::setTimeOut(DWORD ms)
{
	timeout=ms;
	return TRUE;
}

bool CBLHID::setSpeed(DWORD baudrate)
{
	return TRUE;
}

int CBLHID::sendData(DWORD lg, LPBYTE data,DWORD rlg)
{
	DWORD cnt;
	DWORD tmplg=lg;

	m_tx_flag=TRUE;
	if(m_rx_flag)
	{
		m_tx_flag=FALSE;
		goto tx_error;
	}

	m_tolsize=lg;
	m_offset=0;
	tx_pack[0]=0x06;
	tx_pack[1]=DATA;
	tx_pack[7]=(rlg>>8)&0xff;
	tx_pack[8]=rlg&0xff;
	while(lg>=VALID_DATA_SIZE)
	{
		m_cnt=VALID_DATA_SIZE;
		lg-=m_cnt;
		tx_pack[3]=m_tolsize&0xff;
		tx_pack[2]=m_tolsize>>8;
		tx_pack[5]=m_offset&0xff;
		tx_pack[4]=m_offset>>8;
		tx_pack[6]=m_cnt;
		memcpy(&tx_pack[9],&data[m_offset],m_cnt);
		m_offset+=m_cnt;
		if(!HID_Write(tx_pack, PACKAGE_SIZE, &cnt)) 
			goto tx_error;
		if (cnt<PACKAGE_SIZE) 
			goto tx_error;
		/*if(lg>0)
		{
			if(!HID_Read(rx_pack, PACKAGE_SIZE, &cnt)) 
				goto tx_error;
			if (cnt<PACKAGE_SIZE) 
				goto tx_error;
			if (rx_pack[1]!=ACK)
				goto tx_error;
		}*/
	}
	if(lg>0)
	{
		m_cnt=lg;
		tx_pack[3]=m_tolsize&0xff;
		tx_pack[2]=m_tolsize>>8;
		tx_pack[5]=m_offset&0xff;
		tx_pack[4]=m_offset>>8;
		tx_pack[6]=m_cnt;
		memcpy(&tx_pack[9],&data[m_offset],m_cnt);
		m_offset+=m_cnt;
		if(!HID_Write(tx_pack, PACKAGE_SIZE, &cnt)) 
			goto tx_error;
		if (cnt<PACKAGE_SIZE) 
			goto tx_error;
	}

	m_tx_flag=FALSE;
	return tmplg;

tx_error:
	m_tx_flag=FALSE;
	return 0;
}

int CBLHID::sendData(string* data,DWORD rlg)
{
	if( data == NULL )
		return false;

	return sendData((DWORD)data->size(), (LPBYTE)data->data(),rlg);
}

int CBLHID::receiveData(DWORD lg, LPBYTE data)
{
	DWORD cnt;
	WORD tmp_tolsize;
	WORD tmp_offset;
	BYTE tmp_cnt;
	DWORD tmp;
	
	if(!lg)
		goto rx_error;
	tmp=rx_queue.size();
	if(rx_queue.size()>=lg)
	{
		for(DWORD i=0;i<lg;i++)
		{
			data[i]=rx_queue.front();
			if(!rx_queue.empty())
				rx_queue.pop();
			else
				goto rx_error;
		}
		return lg;
	}
	else
	{
		rx_queue.c.clear();
	}

	m_rx_flag=TRUE;
	if(m_tx_flag)
	{
		m_rx_flag=FALSE;
		goto rx_error;
	}

	if(!HID_Read(rx_pack, PACKAGE_SIZE, &cnt)) 
		goto rx_error;
	if (cnt<PACKAGE_SIZE) 
		goto rx_error;

	if(rx_pack[1]!=DATA)
		goto rx_error;

	m_tolsize=(((WORD)rx_pack[2])<<8)| rx_pack[3];
	m_offset=(((WORD)rx_pack[4])<<8)| rx_pack[5];
	m_cnt=rx_pack[6];

	if((m_offset!=0)||(m_tolsize<lg)||(m_offset+m_cnt>m_tolsize))
		goto rx_error;

	for(int i=0;i<m_cnt;i++)
		rx_queue.push(rx_pack[9+i]);

	/*if(m_offset+m_cnt<m_tolsize)
	{
		//send usb ack
		tx_pack[0]=0x06;
		tx_pack[1]=ACK;
		if(!HID_Write(tx_pack, PACKAGE_SIZE, &cnt)) 
			goto rx_error;
		if (cnt<PACKAGE_SIZE) 
			goto rx_error;
	}*/

	while(m_offset+m_cnt<m_tolsize)
	{
		if(!HID_Read(rx_pack, PACKAGE_SIZE, &cnt)) 
			goto rx_error;
		if (cnt<PACKAGE_SIZE) 
			goto rx_error;

		tmp_tolsize=(((WORD)rx_pack[2])<<8)| rx_pack[3];
		tmp_offset=(((WORD)rx_pack[4])<<8)| rx_pack[5];
		tmp_cnt=rx_pack[6];
		
		if((m_tolsize != tmp_tolsize) || (tmp_offset+tmp_cnt>m_tolsize))
			goto rx_error;
		tmp=rx_queue.size();
		for(int i=0;i<tmp_cnt;i++)
			rx_queue.push(rx_pack[9+i]);


		m_offset+=tmp_cnt;
		//send usb ack
		/*tx_pack[0]=0x06;
		tx_pack[1]=ACK;
		if(!HID_Write(tx_pack, PACKAGE_SIZE, &cnt)) 
			goto rx_error;
		if (cnt<PACKAGE_SIZE) 
			goto rx_error;*/
	}

	for(DWORD i=0;i<lg;i++)
	{
		data[i]=rx_queue.front();
		if(!rx_queue.empty())
			rx_queue.pop();
		else
			goto rx_error;
	}

	m_rx_flag=FALSE;
	return lg;

rx_error:
	m_rx_flag=FALSE;
	rx_queue.c.clear();
	return 0;
}

int CBLHID::receiveData(string* data)
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

bool CBLHID::setRts(bool val)
{
	return TRUE;
}

bool CBLHID::setDtr(bool val)
{
	return TRUE;
}

bool CBLHID::setTxd(bool val)
{
	return TRUE;
}

bool CBLHID::getCts()
{
	return FALSE;
}

bool CBLHID::getDtr()
{
	return FALSE;
}

bool CBLHID::getRi()
{
	return FALSE;
}

bool CBLHID::getCd()
{
	return FALSE;
}

string CBLHID::getErrorMsg()
{
	return "NO error";
}

/*
 *  HID Initialization
 *    Parameters:      None
 *    Return Value:    None
 */

void CBLHID::HID_Init() {
  int  i;

  DevCount = 0;

  for (i = 0; i < DEV_NUM; i++) {
    DevDetailData[i] = NULL;
  }
}


/*
 *  HID Uninitialization
 *    Parameters:      None
 *    Return Value:    None
 */

void CBLHID::HID_UnInit() {
  int  i;

  for (i = 0; i < DEV_NUM; i++) {
    if (DevDetailData[i]) free(DevDetailData[i]);
  }
}


/*
 *  HID Find Devices
 *    Parameters:      None
 *    Return Value:    Number of Devices found
 */

int CBLHID::HID_FindDevices() {
  GUID                             HidGuid;
  HDEVINFO                         DevInfo;
  SP_DEVICE_INTERFACE_DATA         DevData;
  PSP_DEVICE_INTERFACE_DETAIL_DATA DevDetail;
  PHIDP_PREPARSED_DATA	           PreparsedData;
  HIDP_CAPS	                       Capabilities;
  ULONG                            Length;
  int                              Index;
  BOOL                             ok;

  /* Get GUID for all System HIDs */
  HidD_GetHidGuid(&HidGuid);

  /* Get Device Information for all present devices */
  DevInfo = SetupDiGetClassDevs(&HidGuid, 
                                NULL, 
                                NULL, 
                                (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE)
                                );

  DevData.cbSize = sizeof(DevData);

  DevDetail = NULL;

  Index = -1;

  /* Scan all Devices */
  do {

    Index++;

    /* Device Interface Element of a Device Information set */
    ok = SetupDiEnumDeviceInterfaces(DevInfo, 
                                     0, 
                                     &HidGuid, 
                                     Index, 
                                     &DevData
                                     );
    if (!ok) break;

    /* Get Device Interface Details - Get Length */
    ok = SetupDiGetDeviceInterfaceDetail(DevInfo, 
                                         &DevData, 
                                         NULL, 
                                         0, 
                                         &Length, 
                                         NULL
                                         );

    /* Allocate memory for Device Detailed Data */
    if (DevDetail) free(DevDetail);
    DevDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc(Length);
      
    /* Set cbSize in the DevDetail structure */
    DevDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

    /* Get Device Interface Details */
    ok = SetupDiGetDeviceInterfaceDetail(DevInfo, 
                                         &DevData, 
                                         DevDetail, 
                                         Length, 
                                         NULL, 
                                         NULL
                                         );
    if (!ok) continue;

    /* Create File for Device Read/Write */
    DevHandle = CreateFile(DevDetail->DevicePath, 
                           GENERIC_READ | GENERIC_WRITE, 
                           FILE_SHARE_READ | FILE_SHARE_WRITE, 
                           (LPSECURITY_ATTRIBUTES)NULL,
                           OPEN_EXISTING, 
                           0, 
                           NULL
                           );

    if (DevHandle == INVALID_HANDLE_VALUE) continue;

    /* Get Preparsed Data */
    ok = HidD_GetPreparsedData(DevHandle, &PreparsedData);
    if (!ok) continue;

    /* Get Device's Capabilities */
    HidP_GetCaps(PreparsedData, &Capabilities);

    /* Free the PreparsedData */
    HidD_FreePreparsedData(PreparsedData);

    /* Remember Device Interface Detail Data for acceptable Devices */
    //if ((Capabilities.UsagePage == 0xFF00) && (Capabilities.Usage == 0x0001)) {
      DevDetailData[DevCount++] = DevDetail;
      DevDetail = NULL;
   // }

    CloseHandle (DevHandle);

  } while (DevCount < DEV_NUM);

  if (DevDetail) free(DevDetail);

  SetupDiDestroyDeviceInfoList (DevInfo);

  return (DevCount);
}


/*
 *  HID Get Name (Product String)
 *    Parameters:      num:   Device number
 *                     buf:   Pointer to buffer that receives data
 *                     sz:    Number of bytes to read
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL CBLHID::HID_GetName(int num, char *buf, int sz) {
  HANDLE DevHandle;
  WCHAR  wbuf[128];
  int    i;
  BOOL   ok;

  if (DevDetailData[num] == NULL) return (FALSE);

  /* Create File for Device Read/Write */
 DevHandle = CreateFile(DevDetailData[num]->DevicePath, 
                         GENERIC_READ | GENERIC_WRITE, 
                         FILE_SHARE_READ | FILE_SHARE_WRITE, 
                         (LPSECURITY_ATTRIBUTES)NULL,
                         OPEN_EXISTING, 
                         0, 
                         NULL
                         );
  

  if (DevHandle == INVALID_HANDLE_VALUE) return (FALSE);

  ok = HidD_GetProductString(DevHandle, wbuf, sizeof(wbuf));

  if (ok) {
    for (i = 0; i < sz; i++) {
      *buf++ = (char)wbuf[i];
    }
  }

  CloseHandle (DevHandle);

  return (ok);
}


/*
 *  HID Open
 *    Parameters:      num:   Device number
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL CBLHID::HID_Open(int num) {

  DevHandle = INVALID_HANDLE_VALUE;

  if (DevDetailData[num] == NULL) return (FALSE);

  /* Create File for Device Read/Write */
  DevHandle = CreateFile(DevDetailData[num]->DevicePath, 
                         GENERIC_READ | GENERIC_WRITE, 
                         FILE_SHARE_READ | FILE_SHARE_WRITE, 
                         (LPSECURITY_ATTRIBUTES)NULL,
                         OPEN_EXISTING, 
                         FILE_FLAG_OVERLAPPED, 
                         NULL
                         );

  if (DevHandle == INVALID_HANDLE_VALUE) return (FALSE);

  return (TRUE);
}


/*
 *  HID Open
 *    Parameters:      None
 *    Return Value:    None
 */

void CBLHID::HID_Close() {

  if (DevHandle != INVALID_HANDLE_VALUE) {
    CloseHandle(DevHandle);
    DevHandle = INVALID_HANDLE_VALUE;
  }
}


/*
 *  HID Read with timeout
 *    Parameters:      buf:   Pointer to buffer that receives data
 *                     sz:    Number of bytes to read
 *                     cnt:   Pointer to number of bytes read
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL CBLHID::HID_Read(BYTE *buf, DWORD sz, DWORD *cnt) 
{
	OVERLAPPED	HIDOverlapped;
	int Result;

	HIDOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	HIDOverlapped.Offset = 0;  
    HIDOverlapped.OffsetHigh = 0;

	ResetEvent(HIDOverlapped.hEvent);

	Result = ReadFile 
	(DevHandle, 
	buf, 
	sz, 
	cnt,
	(LPOVERLAPPED) &HIDOverlapped);

	Result = WaitForSingleObject   
			(HIDOverlapped.hEvent,   
				timeout);     // set timeout 

	switch (Result)  
	{  
		case WAIT_OBJECT_0:  
		{ 
			Result=TRUE; 
			*cnt=sz;
			break;  
		 }  
		case WAIT_TIMEOUT:  
		{
			
			Result=FALSE; 
			break;  
		}
		default:
			Result=FALSE; 
			break;
	}

	return Result;
}


/*
 *  HID Write with timeout
 *    Parameters:      buf:   Pointer to buffer with data to write
 *                     sz:    Number of bytes to write
 *                     cnt:   Pointer to number of bytes written
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL CBLHID::HID_Write(BYTE *buf, DWORD sz, DWORD *cnt) 
{
	OVERLAPPED	HIDOverlapped;
	int Result;

	HIDOverlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	HIDOverlapped.Offset = 0;  
    HIDOverlapped.OffsetHigh = 0;

	//CancelIo(DevHandle);
	ResetEvent(HIDOverlapped.hEvent);

	Result = WriteFile 
		   (DevHandle, 
		   buf, 
		   sz, 
		   cnt,
		   (LPOVERLAPPED) &HIDOverlapped);

	Result = WaitForSingleObject   
			(HIDOverlapped.hEvent,   
				timeout);     // set timeout 
	
	switch (Result)  
	{  
		case WAIT_OBJECT_0:  
		{ 
			Result=TRUE;
			*cnt=sz;
			break;  
		 }  
		case WAIT_TIMEOUT:  
		{  
			Result=FALSE;
			*cnt=300;
			break;  
		}
		default:
			Result=FALSE;
			break;
	}

	return Result;
}
