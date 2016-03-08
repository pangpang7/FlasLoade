// GetCOMpage.cpp : implementation file
//

#include "stdafx.h"
#include "MagicBurning.h"
#include "GetCOMpage.h"
#include "MagicBurningDlg.h"


// GetCOMpage dialog

IMPLEMENT_DYNAMIC(GetCOMpage, CDialog)

GetCOMpage::GetCOMpage(CWnd* pParent /*=NULL*/)
	: CDialog(GetCOMpage::IDD, pParent)
{
	
}

GetCOMpage::~GetCOMpage()
{
}

void GetCOMpage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GetCOMpage, CDialog)
END_MESSAGE_MAP()

BOOL GetCOMpage::PreTranslateMessage(MSG* pMsg)  
{  
    if((pMsg->wParam == VK_RETURN)||(pMsg->wParam == VK_ESCAPE)) 
    {  
        return false;  
    }   
    return CDialog::PreTranslateMessage(pMsg);   
}

void GetCOMpage::ContentRefresh(void)
{
	char tmp[130],tmp1[4];
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	sprintf(tmp,"%02X%02X",pDlg->man_info.pro.PRO_PID[1],pDlg->man_info.pro.PRO_PID[0]);
	((CEdit*)GetDlgItem(IDC_EDIT_PROPID))->SetWindowText(tmp);
	sprintf(tmp,"%01X.%01X",pDlg->man_info.pro.Version>>4,pDlg->man_info.pro.Version&0xf);
	((CEdit*)GetDlgItem(IDC_EDIT_PROVER))->SetWindowText(tmp);
	sprintf(tmp,"%02X%02X",pDlg->man_info.target.PID[1],pDlg->man_info.target.PID[0]);
	((CEdit*)GetDlgItem(IDC_EDIT_MCUPID))->SetWindowText(tmp);
	sprintf(tmp,"%01X.%01X",pDlg->man_info.target.Version>>4,pDlg->man_info.target.Version&0xf);
	((CEdit*)GetDlgItem(IDC_EDIT_MCUVER))->SetWindowText(tmp);
	sprintf(tmp,"%02X",pDlg->man_info.target.FlashLock[0]);
	for(int i=1;i<32;i++)
	{
		sprintf(tmp1,"%02X",pDlg->man_info.target.FlashLock[i]);
		strcat(tmp,tmp1);
	}
	((CEdit*)GetDlgItem(IDC_EDIT_MCUFLB))->SetWindowText(tmp);
	
}
// GetCOMpage message handlers

BOOL GetCOMpage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	ContentRefresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void GetCOMpage::OnUpdateData(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
}

void GetCOMpage::Enter(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
	ContentRefresh();
	//CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	//pDlg->SendMessage(WM_WARNING, (WPARAM)"Invalid MCU ID",NULL);

	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	pDlg->GetDlgItem(ID_BTN_CANCEL)->SetWindowText("Cancel");
}

void GetCOMpage::Exit(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
}

BYTE GetCOMpage::GetAccessCtrl(void)
{
#ifndef UI_TEST
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	if((pDlg->man_info.MIsConnected)&&(pDlg->man_info.PIsConnected))
	{
		if((pDlg->man_info.authed)||(!pDlg->man_info.ValidChecked))
			return 3;
	}
	else
		return 1;
#else
	return 3;
#endif
}
