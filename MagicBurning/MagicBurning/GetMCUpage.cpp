// GetMCUpage.cpp : implementation file
//

#include "stdafx.h"
#include "MagicBurning.h"
#include "GetMCUpage.h"


// GetMCUpage dialog

IMPLEMENT_DYNAMIC(GetMCUpage, CDialog)

GetMCUpage::GetMCUpage(CWnd* pParent /*=NULL*/)
	: CDialog(GetMCUpage::IDD, pParent)
{
	
}

GetMCUpage::~GetMCUpage()
{
}

BOOL GetMCUpage::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pagelist.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT); 
	m_pagelist.InsertColumn(0,"Name",NULL,70,-1);
	m_pagelist.InsertColumn(1,"StartAdress",NULL,90,-1);
	m_pagelist.InsertColumn(2,"EndAdress",NULL,90,-1);
	m_pagelist.InsertColumn(3,"R",NULL,30,-1);
	m_pagelist.InsertColumn(4,"W",NULL,30,-1);

	return TRUE;
}

void GetMCUpage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PAGE, m_pagelist);
}


BEGIN_MESSAGE_MAP(GetMCUpage, CDialog)
END_MESSAGE_MAP()

BOOL GetMCUpage::PreTranslateMessage(MSG* pMsg)  
{  
    if((pMsg->wParam == VK_RETURN)||(pMsg->wParam == VK_ESCAPE))
    {  
        return false;  
    }   
    return CDialog::PreTranslateMessage(pMsg);   
}

// GetMCUpage message handlers

void GetMCUpage::OnUpdateData(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
}

void GetMCUpage::Enter(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
}

void GetMCUpage::Exit(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
}

BYTE GetMCUpage::GetAccessCtrl(void)
{
	return 0;
}
