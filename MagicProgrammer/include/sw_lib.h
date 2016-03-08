#ifndef __SW_LIB_H
#define __SW_LIB_H
#include "usb_type.h"

bool memcpy(u8 * _Dst, const u8 * _Src, u32 _Size);
bool memset(u8 * _Dst, const u8 _data, u32 _Size);
int memcmp(const u8 * _Dst, const u8 * _Src, u32 _Size);
u8 checksum(u8 *pd, u32 cnt);
bool memcmpfixed(const u8 * _Dst, const u8 _Val, u32 _Size);

#endif
