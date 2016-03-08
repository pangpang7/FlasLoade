// GetAUTHpage.cpp : implementation file
//

#include "stdafx.h"
#include "MagicBurning.h"
#include "GetAUTHpage.h"
#include "MagicBurningDlg.h"
#include "ini.h"


// GetAUTHpage dialog

IMPLEMENT_DYNAMIC(GetAUTHpage, CDialog)

GetAUTHpage::GetAUTHpage(CWnd* pParent /*=NULL*/)
	: CDialog(GetAUTHpage::IDD, pParent)
{

}

GetAUTHpage::~GetAUTHpage()
{
}

void GetAUTHpage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GetAUTHpage, CDialog)
	ON_BN_CLICKED(IDC_RADIO_AUTHPASS, &GetAUTHpage::OnBnClickedRadioAuthpass)
	ON_BN_CLICKED(IDC_RADIO_REMOVEPASS, &GetAUTHpage::OnBnClickedRadioRemovepass)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SEC, &GetAUTHpage::OnBnClickedButtonSec)
END_MESSAGE_MAP()


// GetAUTHpage message handlers

BOOL GetAUTHpage::PreTranslateMessage(MSG* pMsg)  
{  
    if((pMsg->wParam == VK_RETURN)||(pMsg->wParam == VK_ESCAPE))
    {  
        return false;  
    }   
    return CDialog::PreTranslateMessage(pMsg);   
}
// GetSTSpage message handlers

void GetAUTHpage::OnUpdateData(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
}

void GetAUTHpage::Enter(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
	if(!pinfo->PIsConnected)
	{
		SetAuthFunEnable(false);
		return;
	}

	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	if((!pDlg->man_info.ValidChecked)||(pDlg->man_info.authed))
		SetAuthFunEnable(false);
	else
		SetAuthFunEnable(true);

	char tmpstr[32];
	sprintf(tmpstr,"Left %d times",pinfo->CUR_AUTH_NUM);
	((CStatic *)GetDlgItem(IDC_STATIC_LEFTTIMES))->SetWindowText(tmpstr);

	if((pop->op==CBLMANAGE_SECURITY)&&((pop->SecType==1)||(pop->SecType==2)))
	{
		//chk valid and get cur max num
		pDlg->man_op.op=CBLMANAGE_SECURITY;
		pDlg->man_op.SecType=3;
		pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,3);
	}

	//CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	pDlg->GetDlgItem(ID_BTN_CANCEL)->SetWindowText("Cancel");
}

void GetAUTHpage::Exit(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
}

BYTE GetAUTHpage::GetAccessCtrl(void)
{
#ifndef UI_TEST
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	if((!pDlg->man_info.MIsConnected)||(!pDlg->man_info.PIsConnected)
		||((pDlg->man_info.ValidChecked)&&(!pDlg->man_info.authed)))
		return 1;
	else
		return 3;
#else
	return 3;
#endif
}

BOOL GetAUTHpage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	((CButton *)GetDlgItem(IDC_RADIO_AUTHPASS))->SetCheck(TRUE);
	OnBnClickedRadioAuthpass();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void GetAUTHpage::SetAuthFunEnable(bool enable)
{
	if(!enable)
	{
		((CEdit *)GetDlgItem(IDC_EDIT_AUTHPASS))->EnableWindow(false);
		((CStatic *)GetDlgItem(IDC_STATIC_AUTH))->EnableWindow(false);
		((CButton *)GetDlgItem(IDC_RADIO_AUTHPASS))->EnableWindow(false);
		((CStatic *)GetDlgItem(IDC_STATIC_LEFTTIMES))->EnableWindow(false);
		((CButton *)GetDlgItem(IDC_RADIO_REMOVEPASS))->EnableWindow(false);
		((CStatic *)GetDlgItem(IDC_STATIC_CAUTION))->EnableWindow(false);
		((CButton *)GetDlgItem(IDC_BUTTON_SEC))->EnableWindow(false);
		((CStatic *)GetDlgItem(IDC_STATIC_MCUSEC))->ShowWindow(SW_SHOW);
		((CStatic *)GetDlgItem(IDC_STATIC_SH1))->EnableWindow(false);
	}
	else
	{
		//((CEdit *)GetDlgItem(IDC_EDIT_AUTHPASS))->EnableWindow(true);
		((CStatic *)GetDlgItem(IDC_STATIC_AUTH))->EnableWindow(true);
		((CButton *)GetDlgItem(IDC_RADIO_AUTHPASS))->EnableWindow(true);
		((CStatic *)GetDlgItem(IDC_STATIC_LEFTTIMES))->EnableWindow(true);
		((CButton *)GetDlgItem(IDC_RADIO_REMOVEPASS))->EnableWindow(true);
		((CStatic *)GetDlgItem(IDC_STATIC_CAUTION))->EnableWindow(true);
		((CButton *)GetDlgItem(IDC_BUTTON_SEC))->EnableWindow(true);
		((CStatic *)GetDlgItem(IDC_STATIC_MCUSEC))->ShowWindow(SW_HIDE);
		((CStatic *)GetDlgItem(IDC_STATIC_SH1))->EnableWindow(true);

		//((CButton *)GetDlgItem(IDC_RADIO_AUTHPASS))->SetCheck(TRUE);
		//OnBnClickedRadioAuthpass();
		if(GetCheckedRadioButton(IDC_RADIO_AUTHPASS,IDC_RADIO_REMOVEPASS)==IDC_RADIO_AUTHPASS)
		//if((((CButton *)GetDlgItem(IDC_RADIO_AUTHPASS))->GetCheckedRadioButton(IDC_RADIO_AUTHPASS,IDC_RADIO_REMOVEPASS))==IDC_RADIO_AUTHPASS)
			OnBnClickedRadioAuthpass();
		else
			OnBnClickedRadioRemovepass();
	}
}

void GetAUTHpage::OnBnClickedRadioAuthpass()
{
	// TODO: Add your control notification handler code here
	auth_mode=0;
	((CEdit *)GetDlgItem(IDC_EDIT_AUTHPASS))->EnableWindow(true);
}

void GetAUTHpage::OnBnClickedRadioRemovepass()
{
	// TODO: Add your control notification handler code here
	auth_mode=1;
	((CEdit *)GetDlgItem(IDC_EDIT_AUTHPASS))->EnableWindow(false);
}

HBRUSH GetAUTHpage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_CAUTION)
     {
         pDC->SetTextColor(RGB(255, 0, 0));
     } 

	if(pWnd->GetDlgCtrlID() == IDC_STATIC_LEFTTIMES)
	{
         pDC->SetTextColor(RGB(255, 0, 0));
     }
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void GetAUTHpage::OnBnClickedButtonSec()
{
	// TODO: Add your control notification handler code here
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	char tmpstr[32];
	if(auth_mode==0)
	{
		//auth mode
		CString InText,TmpText;

		GetDlgItemText(IDC_EDIT_AUTHPASS,InText);
		if(InText.GetLength()!=32)
		{
			AfxGetMainWnd()->SendMessage(WM_WARNING, (WPARAM)"Invalid Password Length",NULL);
			return;
		}
		for(int i=0;i<16;i++)
		{
			TmpText = InText.Mid(i*2,2);
			sprintf(tmpstr,"0x%s",TmpText.GetBuffer(TmpText.GetLength()));
			pDlg->man_op.pwd[i]=strtol(tmpstr,NULL, 16);  
		}
		pDlg->man_op.op=CBLMANAGE_SECURITY;
		pDlg->man_op.SecType=1;
		pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,3);
	}
	else if(auth_mode==1)
	{
		//remove mode
		pDlg->man_op.op=CBLMANAGE_SECURITY;
		pDlg->man_op.SecType=2;
		pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,1);
		pDlg->man_op.op=CBLMANAGE_SECURITY;
		pDlg->man_op.SecType=3;
		pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,2);
	}
}
