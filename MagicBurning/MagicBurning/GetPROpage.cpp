// GetPROpage.cpp : implementation file
//

#include "stdafx.h"
#include "MagicBurning.h"
#include "GetPROpage.h"

#include "MagicBurningDlg.h"
#include "ini.h"


// GetPROpage dialog

IMPLEMENT_DYNAMIC(GetPROpage, CDialog)

GetPROpage::GetPROpage(CWnd* pParent /*=NULL*/)
	: CDialog(GetPROpage::IDD, pParent)
{

}

GetPROpage::~GetPROpage()
{
}

void GetPROpage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_PRO, m_combopro);
	DDX_Control(pDX, IDC_COMBO_MCUS, m_combomcus);
	DDX_Control(pDX, IDC_COMBO_BR, m_combobr);
	DDX_Control(pDX, IDC_COMBO_DB, m_combodb);
	DDX_Control(pDX, IDC_COMBO_TO, m_comboto);
	DDX_Control(pDX, IDC_COMBO_BR3, m_combobr3);
	//DDX_Control(pDX, IDC_PROGRESS_COM, m_ProgressCom);
}

BOOL GetPROpage::OnInitDialog()
{
	CDialog::OnInitDialog();
	//init BL MODE RADIO
	((CButton *)GetDlgItem(IDC_RADIO_OFFLINE))->SetCheck(TRUE);
	GetPROpage::OnBnClickedRadioOffline();

	//init all combo box
	char    szBuf[256];
	char Drive[3], Dir[256], Fname[256], Ext[256];
	char tmpfile[256];
	char* Name;

	//read ini
	GetModuleFileName(NULL,szBuf,sizeof(szBuf));
			   
	_splitpath_s(szBuf, Drive, _MAX_DRIVE, Dir, _MAX_DIR, Fname, _MAX_FNAME, Ext, _MAX_EXT);
					
	sprintf(tmpfile, "%s%s%s%s", Drive, Dir , "DEV\\", "pdevice.lst");

	if (CBLMANAGE::FileExist((char*)tmpfile))
	{
		int NumberOfSections = 0;
		CIni Ini((LPCSTR)tmpfile);
		CStringArray sections;
		Ini.GetSectionNames(&sections);

		NumberOfSections = sections.GetSize();
		for(int pos=0; pos<NumberOfSections; pos++)
		{
			CString HexStr = CString("");
			CString section;
			section.Format(sections[pos]);

			HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"Name", "SI PROGRAMMER X1");
			Name = (char*) malloc(section.GetLength());
			memset(Name, 0xFF, section.GetLength());
			strcpy(Name,(LPSTR)(LPCTSTR)section);
			m_combopro.AddString(Name);
			//free(Name);
		}
	}
	m_combopro.SetCurSel(0);
	OnCbnSelchangeComboPro();
	CEdit* pEdit = (CEdit*)m_combopro.GetWindow(GW_CHILD);
	pEdit->SetReadOnly(TRUE);

	sprintf(tmpfile, "%s%s%s%s", Drive, Dir , "DEV\\", "mdevice.lst");

	if (CBLMANAGE::FileExist((char*)tmpfile))
	{
		int NumberOfSections = 0;
		CIni Ini((LPCSTR)tmpfile);
		CStringArray sections;
		Ini.GetSectionNames(&sections);

		NumberOfSections = sections.GetSize();
		for(int pos=0; pos<NumberOfSections; pos++)
		{
			CString HexStr = CString("");
			CString section;
			section.Format(sections[pos]);

			HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"Name", "SI6600_256K");
			Name = (char*) malloc(section.GetLength());
			memset(Name, 0xFF, section.GetLength());
			strcpy(Name,(LPSTR)(LPCTSTR)section);
			m_combomcus.AddString(Name);
			//free(Name);
		}
	}
	m_combomcus.SetCurSel(0);
	OnCbnSelchangeComboMcus();
	pEdit = (CEdit*)m_combomcus.GetWindow(GW_CHILD);
	pEdit->SetReadOnly(TRUE);

	m_combobr.AddString("UART");
	//m_combobr.AddString("Lite SWD");
	m_combobr.SetCurSel(0);
	OnCbnSelchangeComboBr();
	pEdit = (CEdit*)m_combobr.GetWindow(GW_CHILD);
	pEdit->SetReadOnly(TRUE);


	m_combobr3.AddString("76800");
	m_combobr3.SetCurSel(0);
	m_combobr3.EnableWindow(FALSE);
	pEdit = (CEdit*)m_combobr3.GetWindow(GW_CHILD);
	pEdit->SetReadOnly(TRUE);

	m_combodb.AddString("8 bits");
	m_combodb.SetCurSel(0);
	m_combodb.EnableWindow(FALSE);
	pEdit = (CEdit*)m_combodb.GetWindow(GW_CHILD);
	pEdit->SetReadOnly(TRUE);

	m_comboto.AddString("3500");
	m_comboto.SetCurSel(0);
	m_comboto.EnableWindow(FALSE);
	pEdit = (CEdit*)m_comboto.GetWindow(GW_CHILD);
	pEdit->SetReadOnly(TRUE);

	

	return TRUE;
}


BEGIN_MESSAGE_MAP(GetPROpage, CDialog)
	ON_BN_CLICKED(IDC_RADIO_OFFLINE, &GetPROpage::OnBnClickedRadioOffline)
	ON_BN_CLICKED(IDC_RADIO_ONLINE, &GetPROpage::OnBnClickedRadioOnline)
	ON_CBN_SELCHANGE(IDC_COMBO_PRO, &GetPROpage::OnCbnSelchangeComboPro)
	ON_CBN_SELCHANGE(IDC_COMBO_MCUS, &GetPROpage::OnCbnSelchangeComboMcus)
	ON_CBN_SELCHANGE(IDC_COMBO_BR, &GetPROpage::OnCbnSelchangeComboBr)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// GetPROpage message handlers

void GetPROpage::OnBnClickedRadioOffline()
{
	// TODO: Add your control notification handler code here
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	pDlg->man_op.pro.BURNING_TYPE=OFFLINE;
}

void GetPROpage::OnBnClickedRadioOnline()
{
	// TODO: Add your control notification handler code here
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	pDlg->man_op.pro.BURNING_TYPE=ONLINE;
}

void GetPROpage::OnCbnSelchangeComboPro()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	int nIndex = m_combopro.GetCurSel();
	m_combopro.GetLBText(nIndex, pDlg->man_op.Programmer_name);
}

void GetPROpage::OnCbnSelchangeComboMcus()
{
	// TODO: Add your control notification handler code here
	char    szBuf[256];
	char Drive[3], Dir[256], Fname[256], Ext[256];
	char tmpfile[256];
	char* Name;

	
	UpdateData(TRUE);
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	int nIndex = m_combomcus.GetCurSel();
	m_combomcus.GetLBText(nIndex, pDlg->man_op.MCU_name);

	//read ini
	GetModuleFileName(NULL,szBuf,sizeof(szBuf));
			   
	_splitpath_s(szBuf, Drive, _MAX_DRIVE, Dir, _MAX_DIR, Fname, _MAX_FNAME, Ext, _MAX_EXT);
	sprintf(tmpfile, "%s%s%s%s", Drive, Dir , "DEV\\", "mdevice.lst");
	if (CBLMANAGE::FileExist((char*)tmpfile))
	{
		int NumberOfSections = 0;
		CIni Ini((LPCSTR)tmpfile);
		CStringArray sections;
		Ini.GetSectionNames(&sections);

		NumberOfSections = sections.GetSize();
		for(int pos=0; pos<NumberOfSections; pos++)
		{
			CString HexStr = CString("");
			CString section;
			section.Format(sections[pos]);

			HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"Name", "SI6600_256K");
			if(!strcmp(pDlg->man_op.MCU_name,HexStr.GetBuffer(HexStr.GetLength())))
			{
				HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"FLASH_BASE", "08000000");
				pDlg->man_info.DEV_FLASH_BASE = (DWORD)_tcstoul(HexStr, 0, 16) ;

				HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"RAM_BASE", "08000000");
				pDlg->man_info.DEV_RAM_BASE = (DWORD)_tcstoul(HexStr, 0, 16) ;

				HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"PAGE_SIZE", "0400");
				pDlg->man_op.pro.MCU_page_size = (DWORD)_tcstoul(HexStr, 0, 16)/256 ;

				HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"PID", "1234");
				pDlg->man_op.pro.MCU_PID[0] = (DWORD)_tcstoul(HexStr, 0, 16)&0xff;
				pDlg->man_op.pro.MCU_PID[1] = ((DWORD)_tcstoul(HexStr, 0, 16)>>8)&0xff;

				pDlg->blmanage.SetMCUBase(pDlg->man_info.DEV_RAM_BASE,pDlg->man_info.DEV_FLASH_BASE);
			}
		}
	}

	
	

}

void GetPROpage::OnCbnSelchangeComboBr()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	char* ct = (char*)malloc(256);
	int nIndex = m_combobr.GetCurSel();
	m_combobr.GetLBText(nIndex, ct);
	if(!strcmp(ct,"Lite SWD"))
		pDlg->man_op.pro.MCU_COM=STC;
	else
		pDlg->man_op.pro.MCU_COM=UART;
}

BOOL GetPROpage::PreTranslateMessage(MSG* pMsg)  
{  
    if((pMsg->wParam == VK_RETURN)||(pMsg->wParam == VK_ESCAPE)) 
    {  
        return false;  
    }   
    return CDialog::PreTranslateMessage(pMsg);   
}

void GetPROpage::OnUpdateData(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
	//blue 0x00ff0000
	//red 0x000000ff
	//green 0x0080ff00
	/*string tmpstr;
	if(!pinfo->op_done)
	{
		m_ProgressCom.SetBarColor(0x0000ff00);
		m_ProgressCom.SetWindowText(pinfo->CurPhase.c_str());
	}
	else if(pinfo->op_done)
	{
		if(!pinfo->op_success)
		{
			m_ProgressCom.SetBarColor(0x000000ff);
			tmpstr=pinfo->CurPhase+"Failed !";
			m_ProgressCom.SetWindowText(tmpstr.c_str());
		}
		else
		{
			tmpstr=pinfo->CurPhase+"Complete!";
			m_ProgressCom.SetWindowText(tmpstr.c_str());
		}
	}
	
	m_ProgressCom.SetShowPercent(TRUE);
	m_ProgressCom.SetPos(pinfo->progress);
	UpdateData(false);*/
}

void GetPROpage::Enter(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	pDlg->GetDlgItem(ID_BTN_CANCEL)->SetWindowText("Restore");
}

void GetPROpage::Exit(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
}

BYTE GetPROpage::GetAccessCtrl(void)
{
	return 6;
}

HBRUSH GetPROpage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
