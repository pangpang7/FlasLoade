#include "sw_lib.h"

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
		return (0xff-cnt); //just for cal
	}
	else if(cnt==2)
	{
		if(pd[0]+pd[1]==0xff)//just for validation
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
