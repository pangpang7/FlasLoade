// SetAUTHpage.cpp : implementation file
//

#include "stdafx.h"
#include "MagicBurning.h"
#include "SetAUTHpage.h"
#include "MagicBurningDlg.h"


// SetAUTHpage dialog

IMPLEMENT_DYNAMIC(SetAUTHpage, CDialog)

SetAUTHpage::SetAUTHpage(CWnd* pParent /*=NULL*/)
	: CDialog(SetAUTHpage::IDD, pParent)
{

}

SetAUTHpage::~SetAUTHpage()
{
}

void SetAUTHpage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SetAUTHpage, CDialog)
	ON_BN_CLICKED(IDC_RADIO_PASSWR, &SetAUTHpage::OnBnClickedRadioPasswr)
	ON_BN_CLICKED(IDC_RADIO_PASSNOWR, &SetAUTHpage::OnBnClickedRadioPassnowr)
	ON_BN_CLICKED(IDC_RADIO_CP, &SetAUTHpage::OnBnClickedRadioCp)
	ON_BN_CLICKED(IDC_RADIO_CPD, &SetAUTHpage::OnBnClickedRadioCpd)
	ON_BN_CLICKED(IDC_RADIO_CFL, &SetAUTHpage::OnBnClickedRadioCfl)
	ON_BN_CLICKED(IDC_RADIO_CFLD, &SetAUTHpage::OnBnClickedRadioCfld)
	ON_BN_CLICKED(IDC_RADIO_CFBK, &SetAUTHpage::OnBnClickedRadioCfbk)
	ON_BN_CLICKED(IDC_RADIO_CFBKD, &SetAUTHpage::OnBnClickedRadioCfbkd)
	ON_BN_CLICKED(IDC_RADIO_CFBKNO, &SetAUTHpage::OnBnClickedRadioCfbkno)
	ON_EN_CHANGE(IDC_EDIT_CFBK, &SetAUTHpage::OnEnChangeEditCfbk)
END_MESSAGE_MAP()


// SetAUTHpage message handlers

BOOL SetAUTHpage::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CButton *)GetDlgItem(IDC_RADIO_PASSWR))->SetCheck(TRUE);
	OnBnClickedRadioPasswr();

	((CButton *)GetDlgItem(IDC_RADIO_CP))->SetCheck(TRUE);
	OnBnClickedRadioCp();

	((CButton *)GetDlgItem(IDC_RADIO_CFL))->SetCheck(TRUE);
	OnBnClickedRadioCfl();

	((CButton *)GetDlgItem(IDC_RADIO_CFBK))->SetCheck(TRUE);
	OnBnClickedRadioCfbk();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SetAUTHpage::ChangeVisible(void)
{
	((CButton *)GetDlgItem(IDC_RADIO_CP))->EnableWindow(PwdWriteOrNot);
	((CButton *)GetDlgItem(IDC_RADIO_CPD))->EnableWindow(PwdWriteOrNot);
	((CButton *)GetDlgItem(IDC_RADIO_CFL))->EnableWindow(PwdWriteOrNot);
	((CButton *)GetDlgItem(IDC_RADIO_CFLD))->EnableWindow(PwdWriteOrNot);
	((CButton *)GetDlgItem(IDC_RADIO_CFBK))->EnableWindow(PwdWriteOrNot);
	((CButton *)GetDlgItem(IDC_RADIO_CFBKD))->EnableWindow(PwdWriteOrNot);
	((CButton *)GetDlgItem(IDC_RADIO_CFBKNO))->EnableWindow(PwdWriteOrNot);
	

	((CEdit *)GetDlgItem(IDC_EDIT_CP))->EnableWindow(PwdCustomOrNot & PwdWriteOrNot);
	((CStatic *)GetDlgItem(IDC_STATIC_CPH))->EnableWindow(PwdCustomOrNot & PwdWriteOrNot);
	((CStatic *)GetDlgItem(IDC_STATIC_CPTT))->EnableWindow(PwdCustomOrNot & PwdWriteOrNot);
	((CEdit *)GetDlgItem(IDC_EDIT_CPTT))->EnableWindow(PwdCustomOrNot & PwdWriteOrNot);
	((CStatic *)GetDlgItem(IDC_STATIC_CPTT1))->EnableWindow(PwdCustomOrNot & PwdWriteOrNot);
	
	((CEdit *)GetDlgItem(IDC_EDIT_CFL))->EnableWindow(FLCustomOrNot & PwdWriteOrNot);
	((CStatic *)GetDlgItem(IDC_STATIC_CFLH))->EnableWindow(FLCustomOrNot & PwdWriteOrNot);

	((CEdit *)GetDlgItem(IDC_EDIT_CFBK))->EnableWindow(FBKCustomOrNot & PwdWriteOrNot & FBKEnableOrNot);
	((CStatic *)GetDlgItem(IDC_STATIC_CFBKH))->EnableWindow(FBKCustomOrNot & PwdWriteOrNot & FBKEnableOrNot);
	
}

BOOL SetAUTHpage::PreTranslateMessage(MSG* pMsg)  
{  
    if((pMsg->wParam == VK_RETURN)||(pMsg->wParam == VK_ESCAPE)) 
    {  
        return false;  
    }   
    return CDialog::PreTranslateMessage(pMsg);   
}

void SetAUTHpage::OnUpdateData(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
}

void SetAUTHpage::Enter(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
	int Res;
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	if((pDlg->OSetCFGpage.op_mode!=1)&&(pDlg->LastNextOrBack))//not download mode
	{
		pDlg->man_op.op=CBLMANAGE_NULL;
		Res=pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,3);
		if(Res)
			pDlg->req_next=true;
	}
	else if(!pDlg->LastNextOrBack)
	{
		pDlg->man_op.op=CBLMANAGE_NULL;
		Res=pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,3);
		if(Res)
			pDlg->req_back=true;
	}

	//CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	pDlg->GetDlgItem(ID_BTN_CANCEL)->SetWindowText("Cancel");
}

void SetAUTHpage::Exit(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
}

BYTE SetAUTHpage::GetAccessCtrl(void)
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

void SetAUTHpage::OnBnClickedRadioPasswr()
{
	// TODO: Add your control notification handler code here
	PwdWriteOrNot=true;
	ChangeVisible();
}

void SetAUTHpage::OnBnClickedRadioPassnowr()
{
	// TODO: Add your control notification handler code here
	PwdWriteOrNot=false;
	ChangeVisible();
}

void SetAUTHpage::OnBnClickedRadioCp()
{
	// TODO: Add your control notification handler code here
	PwdCustomOrNot=true;
	ChangeVisible();
}

void SetAUTHpage::OnBnClickedRadioCpd()
{
	// TODO: Add your control notification handler code here
	PwdCustomOrNot=false;
	ChangeVisible();
}

void SetAUTHpage::OnBnClickedRadioCfl()
{
	// TODO: Add your control notification handler code here
	FLCustomOrNot=true;
	ChangeVisible();
}

void SetAUTHpage::OnBnClickedRadioCfld()
{
	// TODO: Add your control notification handler code here
	FLCustomOrNot=false;
	ChangeVisible();
}

void SetAUTHpage::OnBnClickedRadioCfbk()
{
	// TODO: Add your control notification handler code here
	FBKCustomOrNot=true;
	FBKEnableOrNot=true;
	ChangeVisible();
}

void SetAUTHpage::OnBnClickedRadioCfbkd()
{
	// TODO: Add your control notification handler code here
	FBKCustomOrNot=false;
	FBKEnableOrNot=true;
	ChangeVisible();
}

void SetAUTHpage::OnBnClickedRadioCfbkno()
{
	// TODO: Add your control notification handler code here
	FBKEnableOrNot=false;
	FBKCustomOrNot=false;
	ChangeVisible();
}

void SetAUTHpage::OnEnChangeEditCfbk()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
