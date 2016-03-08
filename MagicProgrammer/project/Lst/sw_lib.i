#line 1 "..\\source\\sw_lib.c"
#line 1 "..\\include\\sw_lib.h"
#line 1 "..\\USB\\usb_type.h"













 

 



 
#line 1 "..\\include\\usb_conf.h"













 

 



 
 
 
 
 
 
 
 
 
 


 
 
 
 
 


 
 



 
 



 
 
 
 
 
 



 
 
#line 67 "..\\include\\usb_conf.h"


#line 75 "..\\include\\usb_conf.h"



 

#line 22 "..\\USB\\usb_type.h"

 
 






typedef signed long      s32;
typedef signed short     s16;
typedef signed char      s8;

typedef volatile signed long      vs32;
typedef volatile signed short     vs16;
typedef volatile signed char      vs8;

typedef unsigned long       u32;
typedef unsigned short      u16;
typedef unsigned char       u8;

typedef unsigned long  const    uc32;   
typedef unsigned short const    uc16;   
typedef unsigned char  const    uc8;    

typedef volatile unsigned long      vu32;
typedef volatile unsigned short     vu16;
typedef volatile unsigned char      vu8;

typedef volatile unsigned long  const    vuc32;   
typedef volatile unsigned short const    vuc16;   
typedef volatile unsigned char  const    vuc8;    


typedef enum
{
  FALSE = 0, TRUE  = !FALSE
}
bool;

typedef enum { RESET = 0, SET   = !RESET } FlagStatus, ITStatus;

typedef enum { DISABLE = 0, ENABLE  = !DISABLE} FunctionalState;

typedef enum { ERROR = 0, SUCCESS  = !ERROR} ErrorStatus;


 
 
 



 
#line 4 "..\\include\\sw_lib.h"

bool memcpy(u8 * _Dst, const u8 * _Src, u32 _Size);
bool memset(u8 * _Dst, const u8 _data, u32 _Size);
int memcmp(const u8 * _Dst, const u8 * _Src, u32 _Size);
u8 checksum(u8 *pd, u32 cnt);
bool memcmpfixed(const u8 * _Dst, const u8 _Val, u32 _Size);

#line 2 "..\\source\\sw_lib.c"

bool memcpy(u8 * _Dst, const u8 * _Src, u32 _Size)
{
	u32 i;
	for(i=0; i<_Size; i++)
		_Dst[i]=_Src[i];
	return TRUE;
}

bool memset(u8 * _Dst, const u8 _data, u32 _Size)
{
	u32 i;
	for(i=0; i<_Size; i++)
		_Dst[i]=_data;
	return TRUE;
}

int memcmp(const u8 * _Dst, const u8 * _Src, u32 _Size)
{
	u32 i;
	for(i=0; i<_Size; i++)
		if(_Dst[i] < _Src[i])
			return -1;
		else if(_Dst[i] > _Src[i])
			return 1;
	return 0;
}

bool memcmpfixed(const u8 * _Dst, const u8 _Val, u32 _Size)
{
	u32 i;
	for(i=0; i<_Size; i++)
		if(_Dst[i] != _Val)
			return FALSE;
	return TRUE;
}

u8 checksum(u8 *pd, u32 cnt)
{
	u32 i;
	u8 res=pd[0];
	if((cnt==0)||(cnt==1))
		res=1;
	else if(cnt==1)
	{
		return (0xff-cnt); 
	}
	else if(cnt==2)
	{
		if(pd[0]+pd[1]==0xff)
			res=0;
		else
			res=1;
	}
	else
	{
		for(i=1;i<cnt;i++)		
			res^=pd[i];
	}
	return res;
}
