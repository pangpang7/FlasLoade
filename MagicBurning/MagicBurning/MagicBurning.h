// MagicBurning.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
//#define UI_TEST

// CMagicBurningApp:
// See MagicBurning.cpp for the implementation of this class
//

class CMagicBurningApp : public CWinApp
{
public:
	CMagicBurningApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMagicBurningApp theApp;