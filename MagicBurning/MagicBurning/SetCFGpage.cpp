// SetCFGpage.cpp : implementation file
//

#include "stdafx.h"
#include "MagicBurning.h"
#include "SetCFGpage.h"

#include "MagicBurningDlg.h"
#include "ini.h"


// SetCFGpage dialog

IMPLEMENT_DYNAMIC(SetCFGpage, CDialog)

SetCFGpage::SetCFGpage(CWnd* pParent /*=NULL*/)
	: CDialog(SetCFGpage::IDD, pParent)
{

}

SetCFGpage::~SetCFGpage()
{
}

void SetCFGpage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL SetCFGpage::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitContent();

	return TRUE;
}


BEGIN_MESSAGE_MAP(SetCFGpage, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DL, &SetCFGpage::OnBnClickedButtonDl)
	ON_BN_CLICKED(IDC_BUTTON_UL, &SetCFGpage::OnBnClickedButtonUl)
	ON_BN_CLICKED(IDC_RADIO_ERASE, &SetCFGpage::OnBnClickedRadioErase)
	ON_BN_CLICKED(IDC_RADIO_DL, &SetCFGpage::OnBnClickedRadioDl)
	ON_BN_CLICKED(IDC_RADIO_UL, &SetCFGpage::OnBnClickedRadioUl)
	ON_EN_KILLFOCUS(IDC_EDIT_DL, &SetCFGpage::OnEnKillfocusEditDl)
	ON_EN_KILLFOCUS(IDC_EDIT_UL, &SetCFGpage::OnEnKillfocusEditUl)
	ON_BN_CLICKED(IDC_RADIO_DL_ENP, &SetCFGpage::OnBnClickedRadioDlEnp)
	ON_BN_CLICKED(IDC_RADIO_DL_NE, &SetCFGpage::OnBnClickedRadioDlNe)
	ON_BN_CLICKED(IDC_CHECK_JUSER, &SetCFGpage::OnBnClickedCheckJuser)
	ON_BN_CLICKED(IDC_CHECK_RFF, &SetCFGpage::OnBnClickedCheckRff)
	ON_BN_CLICKED(IDC_CHECK_VAD, &SetCFGpage::OnBnClickedCheckVad)
	ON_BN_CLICKED(IDC_RADIO_RESTORE, &SetCFGpage::OnBnClickedRadioRestore)
	ON_EN_KILLFOCUS(IDC_EDIT_ADR, &SetCFGpage::OnEnKillfocusEditAdr)
END_MESSAGE_MAP()

BOOL SetCFGpage::PreTranslateMessage(MSG* pMsg)  
{  
    if((pMsg->wParam == VK_RETURN)||(pMsg->wParam == VK_ESCAPE))
    {  
        return false;  
    }   
    return CDialog::PreTranslateMessage(pMsg);   
}
// SetCFGpage message handlers

void SetCFGpage::OnBnClickedButtonDl()
{
	// TODO: Add your control notification handler code here
	CString strFile = _T("");

    CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Hex Files (*.hex)|*.hex|BIN Files (*.bin)|*.bin|"), NULL);

    if (dlgFile.DoModal())
    {
        strFile = dlgFile.GetPathName();
		((CEdit*)GetDlgItem(IDC_EDIT_DL))->SetWindowText(strFile);
		CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
		sprintf(pDlg->man_op.FWfile_download,"%s",strFile.GetBuffer(strFile.GetLength()));
    }
}

void SetCFGpage::OnBnClickedButtonUl()
{
	// TODO: Add your control notification handler code here
	CString strFile = _T("");

    CFileDialog    dlgFile(FALSE, NULL, NULL, OFN_HIDEREADONLY, _T("Hex Files (*.hex)|*.hex|BIN Files (*.bin)|*.bin|"), NULL);

    if (dlgFile.DoModal())
    {
        strFile = dlgFile.GetPathName();
		((CEdit*)GetDlgItem(IDC_EDIT_UL))->SetWindowText(strFile);
		CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
		sprintf(pDlg->man_op.FWfile_upload,"%s",strFile.GetBuffer(strFile.GetLength()));
    }
}

void SetCFGpage::SetEarseCtrl(BOOL ctrl)
{
	((CButton *)GetDlgItem(IDC_RADIO_ERASE_ALL))->EnableWindow(ctrl);
	((CButton *)GetDlgItem(IDC_RADIO_ERASE_SEL))->EnableWindow(ctrl);
	((CButton *)GetDlgItem(IDC_BUTTON_ERASE))->EnableWindow(ctrl);

	//uncompleted function
	((CButton *)GetDlgItem(IDC_RADIO_ERASE_SEL))->EnableWindow(false);
	((CButton *)GetDlgItem(IDC_BUTTON_ERASE))->EnableWindow(false);
}

void SetCFGpage::SetDownloadCtrl(BOOL ctrl)
{
	((CEdit *)GetDlgItem(IDC_EDIT_DL))->EnableWindow(ctrl);
	((CButton *)GetDlgItem(IDC_BUTTON_DL))->EnableWindow(ctrl);
	((CButton *)GetDlgItem(IDC_RADIO_DL_ENP))->EnableWindow(ctrl);
	((CButton *)GetDlgItem(IDC_RADIO_DL_NE))->EnableWindow(ctrl);
	//((CEdit *)GetDlgItem(IDC_EDIT_ADR))->EnableWindow(ctrl);
	((CButton *)GetDlgItem(IDC_CHECK_JUSER))->EnableWindow(ctrl);
	((CButton *)GetDlgItem(IDC_CHECK_RFF))->EnableWindow(ctrl);
	((CButton *)GetDlgItem(IDC_CHECK_VAD))->EnableWindow(ctrl);
}

void SetCFGpage::SetUploadCtrl(BOOL ctrl)
{
	((CEdit *)GetDlgItem(IDC_EDIT_UL))->EnableWindow(ctrl);
	((CButton *)GetDlgItem(IDC_BUTTON_UL))->EnableWindow(ctrl);
}

void SetCFGpage::OpmodeProc(void)
{
	switch(op_mode)
	{
	case 0:
		SetEarseCtrl(TRUE);
		SetDownloadCtrl(FALSE);
		SetUploadCtrl(FALSE);
		break;
	case 1:
		SetEarseCtrl(FALSE);
		SetDownloadCtrl(TRUE);
		SetUploadCtrl(FALSE);
		break;
	case 2:
		SetEarseCtrl(FALSE);
		SetDownloadCtrl(FALSE);
		SetUploadCtrl(TRUE);
		break;
	case 3:
		SetEarseCtrl(FALSE);
		SetDownloadCtrl(FALSE);
		SetUploadCtrl(FALSE);
		break;
	default:
		break;
	}
	
}

void SetCFGpage::InitContent(void)
{
	//init to download op
	

	((CButton *)GetDlgItem(IDC_RADIO_ERASE))->SetCheck(TRUE);
	OnBnClickedRadioErase();

	((CButton *)GetDlgItem(IDC_RADIO_ERASE_ALL))->SetCheck(TRUE);

	((CButton *)GetDlgItem(IDC_RADIO_DL_ENP))->SetCheck(TRUE);
	OnBnClickedRadioDlEnp();

	OnBnClickedCheckJuser();
	OnBnClickedCheckRff();
	OnBnClickedCheckVad();
}

void SetCFGpage::OnBnClickedRadioErase()
{
	// TODO: Add your control notification handler code here
	op_mode=0;
	OpmodeProc();
}

void SetCFGpage::OnBnClickedRadioDl()
{
	// TODO: Add your control notification handler code here
	op_mode=1;
	OpmodeProc();
}

void SetCFGpage::OnBnClickedRadioUl()
{
	// TODO: Add your control notification handler code here
	op_mode=2;
	OpmodeProc();
}

void SetCFGpage::OnUpdateData(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
}

void SetCFGpage::Enter(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	CBLMANAGE_INFO info;
	pDlg->blmanage.GetCurInfo(&info);
	PMAPPINGSECTOR pSector =info.pmMapping->pSectors;
	char strtmp[32];
	sprintf(strtmp, "%08X", pSector->dwStartAddress);
	((CEdit*)GetDlgItem(IDC_EDIT_ADR))->SetWindowText(strtmp);

	((CEdit *)GetDlgItem(IDC_EDIT_ADR))->EnableWindow(false);

	((CButton *)GetDlgItem(IDC_RADIO_RESTORE))->ShowWindow(SW_HIDE);

	//CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	pDlg->GetDlgItem(ID_BTN_CANCEL)->SetWindowText("Cancel");
}

void SetCFGpage::Exit(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
}

BYTE SetCFGpage::GetAccessCtrl(void)
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

void SetCFGpage::OnEnKillfocusEditDl()
{
	// TODO: Add your control notification handler code here
	CString strFile = _T("");
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	((CEdit *)GetDlgItem(IDC_EDIT_DL))->GetWindowText(strFile);
	sprintf(pDlg->man_op.FWfile_download,"%s",strFile.GetBuffer(strFile.GetLength()));
}

void SetCFGpage::OnEnKillfocusEditUl()
{
	// TODO: Add your control notification handler code here
	CString strFile = _T("");
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	((CEdit *)GetDlgItem(IDC_EDIT_UL))->GetWindowText(strFile);
	sprintf(pDlg->man_op.FWfile_upload,"%s",strFile.GetBuffer(strFile.GetLength()));
}

void SetCFGpage::OnBnClickedRadioDlEnp()
{
	// TODO: Add your control notification handler code here
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	pDlg->man_op.NeedEarse=2;
}

void SetCFGpage::OnBnClickedRadioDlNe()
{
	// TODO: Add your control notification handler code here
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	pDlg->man_op.NeedEarse=0;
}

void SetCFGpage::OnBnClickedCheckJuser()
{
	// TODO: Add your control notification handler code here
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	if(((CButton *)GetDlgItem(IDC_CHECK_JUSER))->GetCheck())
		pDlg->man_op.jmptoapp=true;
	else
		pDlg->man_op.jmptoapp=false;
}

void SetCFGpage::OnBnClickedCheckRff()
{
	// TODO: Add your control notification handler code here
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	if(((CButton *)GetDlgItem(IDC_CHECK_RFF))->GetCheck())
		pDlg->man_op.optimize=true;
	else
		pDlg->man_op.optimize=false;
}

void SetCFGpage::OnBnClickedCheckVad()
{
	// TODO: Add your control notification handler code here
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	if(((CButton *)GetDlgItem(IDC_CHECK_VAD))->GetCheck())
		pDlg->man_op.verify=true;
	else
		pDlg->man_op.verify=false;
}

void SetCFGpage::OnBnClickedRadioRestore()
{
	// TODO: Add your control notification handler code here
	op_mode=3;
	OpmodeProc();
}

void SetCFGpage::OnEnKillfocusEditAdr()
{
	// TODO: Add your control notification handler code here
	
}
