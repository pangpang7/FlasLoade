// MagicBurningDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MagicBurning.h"
#include "MagicBurningDlg.h"
#include "afxwin.h"
#include "ini.h"
#include <windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMagicBurningDlg dialog
void CMagicBurningDlg::InitConsoleWindow(void)
{
if (LogMode==1)
{
    int nCrt = 0;
    FILE* fp;
    AllocConsole();
    nCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
    fp = _fdopen(nCrt, "w");
    *stdout = *fp;
    setvbuf(stdout, NULL, _IONBF, 0);
}
else if(LogMode==2)
{
	FILE* fp=0;

	char szBuf[256];
	char Drive[3], Dir[256], Fname[256], Ext[256];
	char tmpfile[256];
	char* Name;

	//read ini
	GetModuleFileName(NULL,szBuf,sizeof(szBuf));
			   
	_splitpath_s(szBuf, Drive, _MAX_DRIVE, Dir, _MAX_DIR, Fname, _MAX_FNAME, Ext, _MAX_EXT);
					
	sprintf(tmpfile, "%s%s%s%s", Drive, Dir , ".\\", "Log.txt");

	if(0==(fp=freopen(tmpfile,"w+",stdout)))
		printf("Cannot open file.\n");
}

	

}



CMagicBurningDlg::CMagicBurningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMagicBurningDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMagicBurningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_sheet);
	DDX_Control(pDX, IDC_STATIC_MCUCOM, m_PIC_MCUCOM);
	DDX_Control(pDX, IDC_STATIC_PROCOM, m_PIC_PROCOM);
	DDX_Control(pDX, IDC_PROGRESS_GLOBAL, m_gprogress);
}

BEGIN_MESSAGE_MAP(CMagicBurningDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_BTN_NEXT, &CMagicBurningDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(ID_BTN_BACK, &CMagicBurningDlg::OnBnClickedBtnBack)
	ON_BN_CLICKED(ID_BTN_CANCEL, &CMagicBurningDlg::OnBnClickedBtnCancel)
	ON_MESSAGE(WM_UPDATEDATA, OnUpdateData)
	ON_MESSAGE(WM_WARNING, GetWarning)
END_MESSAGE_MAP()


// CMagicBurningDlg message handlers

BOOL CMagicBurningDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	char szBuf[256];
	char Drive[3], Dir[256], Fname[256], Ext[256];
	char tmpfile[256];
	char* Name;

	//read ini
	GetModuleFileName(NULL,szBuf,sizeof(szBuf));
			   
	_splitpath_s(szBuf, Drive, _MAX_DRIVE, Dir, _MAX_DIR, Fname, _MAX_FNAME, Ext, _MAX_EXT);
					
	sprintf(tmpfile, "%s%s%s%s", Drive, Dir , ".\\", "Config.ini");

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

			HexStr = Ini.GetString((LPCTSTR)section,(LPCTSTR)"LogMode", "1");
			LogMode = (DWORD)_tcstoul(HexStr, 0, 10);
			
		}
	}

	InitConsoleWindow();

	subpage=0;

	memset(&man_op,0,sizeof(man_op));
	memset(&man_info,0,sizeof(man_info));

	
	m_sheet.AddPage("0", &OGetPROpage, IDD_GETPRO);
	m_sheet.AddPage("1", &OGetAUTHpage, IDD_GETAUTH);
	m_sheet.AddPage("2", &OGetCOMpage, IDD_GETCOM);
	//m_sheet.AddPage("GETMCU", &OGetMCUpage, IDD_GETMCU);
	m_sheet.AddPage("3", &OSetCFGpage, IDD_SETCFG);
	m_sheet.AddPage("4", &OSetAUTHpage, IDD_SETAUTH);
	m_sheet.AddPage("5", &OGetSTSpage, IDD_GETSTS);
	m_sheet.Show();

	//blmanage.SetMainPwnd(CWnd::FindWindow(NULL,"MagicBurning"));
	blmanage.SetMainPwnd(AfxGetMainWnd());

	req_next=false;
	req_back=false;

	BYTE actrl=OGetPROpage.GetAccessCtrl();
	(CButton *)GetDlgItem(ID_BTN_BACK)->EnableWindow(actrl&0x01);
	(CButton *)GetDlgItem(ID_BTN_NEXT)->EnableWindow(actrl&0x02);
	(CButton *)GetDlgItem(ID_BTN_CANCEL)->EnableWindow(actrl&0x04);

	OGetPROpage.Enter(NULL,NULL);
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMagicBurningDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);

		
	}
	else
	{
		CDialog::OnPaint();

		CBitmap bitmap1,bitmap2;
		bitmap1.LoadBitmap(IDB_BITMAP1);

		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_V);
		p->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);
		p->SetBitmap(bitmap1);

		bitmap2.LoadBitmap(IDB_BITMAP2);
		p=(CStatic *)GetDlgItem(IDC_STATIC_H);
		p->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);
		p->SetBitmap(bitmap2);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMagicBurningDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*((CComboBox*)OGetPROpage.GetDlgItem(IDC_COMBO_MCUS))->GetWindowText(tmpcstr);
			sprintf(tmpop.MCU_name,"%s",tmpcstr.GetBuffer(tmpcstr.GetLength()));
			((CComboBox*)OGetPROpage.GetDlgItem(IDC_COMBO_PRO))->GetWindowText(tmpcstr);
			sprintf(tmpop.Programmer_name,"%s",tmpcstr.GetBuffer(tmpcstr.GetLength()));
			tmpop.op=CBLMANAGE_CONNECT;*/

void CMagicBurningDlg::OnBnClickedBtnNext()
{
	// TODO: Add your control notification handler code here
#ifndef UI_TEST
	bool Res;
	switch(m_sheet.GetCurSel())
	{
		case 0: //GETPRO
			//connect
			
			man_op.op=CBLMANAGE_CONNECT;
			Res=blmanage.OP_Req_asycn(&man_op,1);
			//set config
			man_op.op=CBLMANAGE_SET_CONFIG;
			Res&=blmanage.OP_Req_asycn(&man_op,0);
			//check password valid
			man_op.op=CBLMANAGE_SECURITY;
			man_op.SecType=3;
			Res&=blmanage.OP_Req_asycn(&man_op,2);

			if(Res)
				req_next=true;
			break;
		case 1: //GETAUTH
			man_op.op=CBLMANAGE_GET_INFO;
			Res=blmanage.OP_Req_asycn(&man_op,3);
			if(Res)
				req_next=true;
			break;
		case 2: //GETCOM
			if(((man_info.target.PID[0]!=man_op.pro.MCU_PID[0])||(man_info.target.PID[1]!=man_op.pro.MCU_PID[1]))
				&&(man_info.pro.BURNING_TYPE!=0))
			{
				if(!SendMessage(WM_WARNING, (WPARAM)"MCU ID Mismatch, Continue?",1))
					return;
			}
			man_op.op=CBLMANAGE_NULL;
			Res=blmanage.OP_Req_asycn(&man_op,3);
			if(Res)
				req_next=true;
			break;
		case 3: //SETCFG
			if(OSetCFGpage.op_mode==0)//earse
			{
				man_op.op=CBLMANAGE_NULL;
				Res=blmanage.OP_Req_asycn(&man_op,3);
				if(Res)
					req_next=true;
			}
			else if(OSetCFGpage.op_mode==1)//download
			{
				if(!CBLMANAGE::FileExist(man_op.FWfile_download))
				{
					AfxGetMainWnd()->SendMessage(WM_WARNING, (WPARAM)"Download File Doesn't Existed",NULL);
					return;
				}
				man_op.op=CBLMANAGE_NULL;
				Res=blmanage.OP_Req_asycn(&man_op,3);
				if(Res)
					req_next=true;
			}
			else if(OSetCFGpage.op_mode==2)//upload
			{
				man_op.op=CBLMANAGE_NULL;
				Res=blmanage.OP_Req_asycn(&man_op,3);
				if(Res)
					req_next=true;
			}
			else if(OSetCFGpage.op_mode==3)//restore
			{
				man_op.op=CBLMANAGE_NULL;
				Res=blmanage.OP_Req_asycn(&man_op,3);
				if(Res)
					req_next=true;
			}
			break;
		case 4: //SETAUTH
			if(OSetAUTHpage.PwdWriteOrNot)
			{
				CString InText,TmpText;
				char tmpstr[32];
				int tmp;
				man_op.SecType=0;
				if(OSetAUTHpage.PwdCustomOrNot)
				{
					OSetAUTHpage.GetDlgItemText(IDC_EDIT_CP,InText);
					if(InText.GetLength()!=32)
					{
						AfxGetMainWnd()->SendMessage(WM_WARNING, (WPARAM)"Invalid Password Length",NULL);
						return;
					}
					for(int i=0;i<16;i++)
					{
						TmpText = InText.Mid(i*2,2);
						sprintf(tmpstr,"0x%s",TmpText.GetBuffer(TmpText.GetLength()));
						man_op.pwd[i]=strtol(tmpstr,NULL, 16);  
					}

					OSetAUTHpage.GetDlgItemText(IDC_EDIT_CPTT,InText);
					sprintf(tmpstr,"%s",InText.GetBuffer(InText.GetLength()));
					tmp=strtol(tmpstr,NULL, 10);
					if((tmp>=0)&&(tmp<=31))
					{
						man_op.MAX_AUTH_NUM=tmp;
					}
					else
					{
						AfxGetMainWnd()->SendMessage(WM_WARNING, (WPARAM)"Invalid Password Try Times",NULL);
						return;
					}
				}
				else
				{
					memset(man_op.pwd,0xFF,16);
					man_op.MAX_AUTH_NUM=5;
				}

				if(OSetAUTHpage.FLCustomOrNot)
				{
					OSetAUTHpage.GetDlgItemText(IDC_EDIT_CFL,InText);
					if(InText.GetLength()!=64)
					{
						AfxGetMainWnd()->SendMessage(WM_WARNING, (WPARAM)"Invalid Flash Lock Bit Length",NULL);
						return;
					}
					for(int i=0;i<32;i++)
					{
						TmpText = InText.Mid(i*2,2);
						sprintf(tmpstr,"0x%s",TmpText.GetBuffer(TmpText.GetLength()));
						man_op.FlashLock[i]=strtol(tmpstr,NULL, 16);  
					}
				}
				else
				{
					memset(man_op.FlashLock,0xFF,32);
				}

				/*if(OSetAUTHpage.FBKEnableOrNot)
				{

					if(OSetAUTHpage.FBKCustomOrNot)
					{
						OSetAUTHpage.GetDlgItemText(IDC_EDIT_CFBK,InText);
						if(InText.GetLength()!=32)
						{
							AfxGetMainWnd()->SendMessage(WM_WARNING, (WPARAM)"Invalid Flash Bus Key Length",NULL);
							return;
						}
						for(int i=0;i<16;i++)
						{
							TmpText = InText.Mid(i*2,2);
							sprintf(tmpstr,"0x%s",TmpText.GetBuffer(TmpText.GetLength()));
							man_op.FlashBusKey[i]=strtol(tmpstr,NULL, 16);  
						}
						man_op.FlashBusEnable=0x55;
					}
					else
					{
						//all FFs means use chip ID
						//memcpy(man_op.FlashBusKey,man_info.target.ChipID,8);
						//memcpy(&man_op.FlashBusKey[8],man_info.target.ChipID,8);
						memset(man_op.FlashBusKey,0xFF,16);
						man_op.FlashBusEnable=0x55;
					}
				}
				else
				{
					man_op.FlashBusEnable=0xFF;
					memset(man_op.FlashBusKey,0xFF,16);
				}*/
				man_op.FlashBusEnable=0xFF;
				memset(man_op.FlashBusKey,0xFF,16);
			}
			man_op.op=CBLMANAGE_NULL;
			Res=blmanage.OP_Req_asycn(&man_op,3);
			if(Res)
				req_next=true;
			break;
		case 5: //GETSTS
			break;
		default:
			break;
	}
#else

	man_op.op=CBLMANAGE_NULL;
	if(blmanage.OP_Req_asycn(&man_op,3))
		req_next=true;
#endif
}

void CMagicBurningDlg::OnBnClickedBtnBack()
{
	// TODO: Add your control notification handler code here
	//CBLMANAGE_OP tmpop;
	switch(m_sheet.GetCurSel())
	{
		case 0: //GETPRO
			man_op.op=CBLMANAGE_NULL;
			if(blmanage.OP_Req_asycn(&man_op,3))
				req_back=true;
			break;
		case 1: //GETAUTH
			man_op.op=CBLMANAGE_DISCONNECT;
			if(blmanage.OP_Req_asycn(&man_op,3))
			{
				req_back=true;
			}
			break;
		case 2: //GETCOM
			man_op.op=CBLMANAGE_NULL;
			if(blmanage.OP_Req_asycn(&man_op,3))
				req_back=true;
			break;
		case 3: //SETCFG
			man_op.op=CBLMANAGE_NULL;
			if(blmanage.OP_Req_asycn(&man_op,3))
				req_back=true;
			break;
		case 4: //SETAUTH
			man_op.op=CBLMANAGE_NULL;
			if(blmanage.OP_Req_asycn(&man_op,3))
				req_back=true;
			break;
		case 5: //GETSTS
			man_op.op=CBLMANAGE_NULL;
			if(blmanage.OP_Req_asycn(&man_op,3))
				req_back=true;
			break;
		default:
			break;
	}
}

void CMagicBurningDlg::OnBnClickedBtnCancel()
{
	CBLMANAGE_OP iop;
	CBLMANAGE_INFO iinfo;
	int Res;
	// TODO: Add your control notification handler code here
	if(m_sheet.GetCurSel()==0)//restore
	{
		if(!SendMessage(WM_WARNING, (WPARAM)"Restore Operation will erase full chip data and code, Continue?",1))
			return;

		memcpy(&iop,&man_op,sizeof(CBLMANAGE_OP));
		iop.pro.BURNING_TYPE=1;
		iop.pro.MCU_COM=0;
		iop.op=CBLMANAGE_CONNECT;
		Res=blmanage.OP_Req_sycn(&iinfo,&iop);

		if((Res!=true)||(!iinfo.op_success))
		{
			AfxGetMainWnd()->SendMessage(WM_WARNING, (WPARAM)"Restore Operation Failed!!!",NULL);
			return;
		}

		//set config
		iop.op=CBLMANAGE_SET_CONFIG;
		Res&=blmanage.OP_Req_sycn(&iinfo,&iop);

		if((Res!=true)||(!iinfo.op_success))
		{
			AfxGetMainWnd()->SendMessage(WM_WARNING, (WPARAM)"Restore Operation Failed!!!",NULL);
			return;
		}
		
		iop.op=CBLMANAGE_SECURITY;
		iop.SecType=2;
		Res=blmanage.OP_Req_sycn(&iinfo,&iop);

		if((Res!=true)||(!iinfo.op_success))
		{
			AfxGetMainWnd()->SendMessage(WM_WARNING, (WPARAM)"Restore Operation Failed!!!",NULL);
			return;
		}

		iop.op=CBLMANAGE_DISCONNECT;
		Res=blmanage.OP_Req_sycn(&iinfo,&iop);

		if((Res!=true)||(!iinfo.op_success))
		{
			AfxGetMainWnd()->SendMessage(WM_WARNING, (WPARAM)"Restore Operation Failed!!!",NULL);
			return;
		}

		AfxGetMainWnd()->SendMessage(WM_WARNING, (WPARAM)"Restore Operation Completed!!!",NULL);

	}
}

BOOL CMagicBurningDlg::PreTranslateMessage(MSG* pMsg)  
{  
    if((pMsg->wParam == VK_RETURN)||(pMsg->wParam == VK_ESCAPE)) 
    {  
        return false;  
    }   
    return CDialog::PreTranslateMessage(pMsg);   
}  

void CMagicBurningDlg::SubPageEnterOrExit(bool EnterOrExit,const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)// true for Enter; false for Exit
{
	switch(m_sheet.GetCurSel())
	{
		case 0: //GETPRO
			if(EnterOrExit)
			{
				OGetPROpage.Enter(pinfo,pop);
				BYTE actrl=OGetPROpage.GetAccessCtrl();
				(CButton *)GetDlgItem(ID_BTN_BACK)->EnableWindow(actrl&0x01);
				(CButton *)GetDlgItem(ID_BTN_NEXT)->EnableWindow(actrl&0x02);
				(CButton *)GetDlgItem(ID_BTN_CANCEL)->EnableWindow(actrl&0x04);
			}
			else
				OGetPROpage.Exit(pinfo,pop);
			break;
		case 1: //GETAUTH
			if(EnterOrExit)
			{
				OGetAUTHpage.Enter(pinfo,pop);
				BYTE actrl=OGetAUTHpage.GetAccessCtrl();
				(CButton *)GetDlgItem(ID_BTN_BACK)->EnableWindow(actrl&0x01);
				(CButton *)GetDlgItem(ID_BTN_NEXT)->EnableWindow(actrl&0x02);
				(CButton *)GetDlgItem(ID_BTN_CANCEL)->EnableWindow(actrl&0x04);
			}
			else
				OGetAUTHpage.Exit(pinfo,pop);
			break;
		case 2: //GETCOM
			if(EnterOrExit)
			{
				OGetCOMpage.Enter(pinfo,pop);
				BYTE actrl=OGetCOMpage.GetAccessCtrl();
				(CButton *)GetDlgItem(ID_BTN_BACK)->EnableWindow(actrl&0x01);
				(CButton *)GetDlgItem(ID_BTN_NEXT)->EnableWindow(actrl&0x02);
				(CButton *)GetDlgItem(ID_BTN_CANCEL)->EnableWindow(actrl&0x04);
			}
			else
				OGetCOMpage.Exit(pinfo,pop);
			break;
		case 3: //SETCFG
			if(EnterOrExit)
			{
				OSetCFGpage.Enter(pinfo,pop);
				BYTE actrl=OSetCFGpage.GetAccessCtrl();
				(CButton *)GetDlgItem(ID_BTN_BACK)->EnableWindow(actrl&0x01);
				(CButton *)GetDlgItem(ID_BTN_NEXT)->EnableWindow(actrl&0x02);
				(CButton *)GetDlgItem(ID_BTN_CANCEL)->EnableWindow(actrl&0x04);
			}
			else
				OSetCFGpage.Exit(pinfo,pop);
			break;
		case 4: //SETAUTH
			if(EnterOrExit)
			{
				OSetAUTHpage.Enter(pinfo,pop);
				BYTE actrl=OSetAUTHpage.GetAccessCtrl();
				(CButton *)GetDlgItem(ID_BTN_BACK)->EnableWindow(actrl&0x01);
				(CButton *)GetDlgItem(ID_BTN_NEXT)->EnableWindow(actrl&0x02);
				(CButton *)GetDlgItem(ID_BTN_CANCEL)->EnableWindow(actrl&0x04);
			}
			else
				OSetAUTHpage.Exit(pinfo,pop);
			break;
		case 5: //GETSTS
			if(EnterOrExit)
			{
				OGetSTSpage.Enter(pinfo,pop);
				BYTE actrl=OGetSTSpage.GetAccessCtrl();
				(CButton *)GetDlgItem(ID_BTN_BACK)->EnableWindow(actrl&0x01);
				(CButton *)GetDlgItem(ID_BTN_NEXT)->EnableWindow(actrl&0x02);
				(CButton *)GetDlgItem(ID_BTN_CANCEL)->EnableWindow(actrl&0x04);
			}
			else
				OGetSTSpage.Exit(pinfo,pop);
			break;
		default:
			break;
	}
}

LRESULT CMagicBurningDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	if(lParam==NULL)
	{
		memcpy(&man_info,(const CBLMANAGE_INFO *)wParam,sizeof(CBLMANAGE_INFO));
		if(((const CBLMANAGE_INFO *)wParam)->MIsConnected)
			m_PIC_MCUCOM.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_LEDON));
		else
			m_PIC_MCUCOM.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_LEDOFF));

		if(((const CBLMANAGE_INFO *)wParam)->PIsConnected)
			m_PIC_PROCOM.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_LEDON));
		else
			m_PIC_PROCOM.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_LEDOFF));
	}

	switch(m_sheet.GetCurSel())
	{
		case 0: //GETPRO
			OGetPROpage.OnUpdateData((const CBLMANAGE_INFO *)wParam,(const CBLMANAGE_OP *)lParam);
			if(lParam==NULL)
			{
				BYTE actrl=OGetPROpage.GetAccessCtrl();
				(CButton *)GetDlgItem(ID_BTN_BACK)->EnableWindow(actrl&0x01);
				(CButton *)GetDlgItem(ID_BTN_NEXT)->EnableWindow(actrl&0x02);
				(CButton *)GetDlgItem(ID_BTN_CANCEL)->EnableWindow(actrl&0x04);
			}
			break;
		case 1: //GETAUTH
			OGetAUTHpage.OnUpdateData((const CBLMANAGE_INFO *)wParam,(const CBLMANAGE_OP *)lParam);
			if(lParam==NULL)
			{
				BYTE actrl=OGetAUTHpage.GetAccessCtrl();
				(CButton *)GetDlgItem(ID_BTN_BACK)->EnableWindow(actrl&0x01);
				(CButton *)GetDlgItem(ID_BTN_NEXT)->EnableWindow(actrl&0x02);
				(CButton *)GetDlgItem(ID_BTN_CANCEL)->EnableWindow(actrl&0x04);
			}
			break;
		case 2: //GETCOM
			OGetCOMpage.OnUpdateData((const CBLMANAGE_INFO *)wParam,(const CBLMANAGE_OP *)lParam);
			if(lParam==NULL)
			{
				BYTE actrl=OGetCOMpage.GetAccessCtrl();
				(CButton *)GetDlgItem(ID_BTN_BACK)->EnableWindow(actrl&0x01);
				(CButton *)GetDlgItem(ID_BTN_NEXT)->EnableWindow(actrl&0x02);
				(CButton *)GetDlgItem(ID_BTN_CANCEL)->EnableWindow(actrl&0x04);
			}
			break;
		case 3: //SETCFG
			OSetCFGpage.OnUpdateData((const CBLMANAGE_INFO *)wParam,(const CBLMANAGE_OP *)lParam);
			if(lParam==NULL)
			{
				BYTE actrl=OSetCFGpage.GetAccessCtrl();
				(CButton *)GetDlgItem(ID_BTN_BACK)->EnableWindow(actrl&0x01);
				(CButton *)GetDlgItem(ID_BTN_NEXT)->EnableWindow(actrl&0x02);
				(CButton *)GetDlgItem(ID_BTN_CANCEL)->EnableWindow(actrl&0x04);
			}
			break;
		case 4: //SETAUTH
			OSetAUTHpage.OnUpdateData((const CBLMANAGE_INFO *)wParam,(const CBLMANAGE_OP *)lParam);
			if(lParam==NULL)
			{
				BYTE actrl=OGetSTSpage.GetAccessCtrl();
				(CButton *)GetDlgItem(ID_BTN_BACK)->EnableWindow(actrl&0x01);
				(CButton *)GetDlgItem(ID_BTN_NEXT)->EnableWindow(actrl&0x02);
				(CButton *)GetDlgItem(ID_BTN_CANCEL)->EnableWindow(actrl&0x04);
			}
			break;
		case 5: //GETSTS
			OGetSTSpage.OnUpdateData((const CBLMANAGE_INFO *)wParam,(const CBLMANAGE_OP *)lParam);
			if(lParam==NULL)
			{
				BYTE actrl=OGetSTSpage.GetAccessCtrl();
				(CButton *)GetDlgItem(ID_BTN_BACK)->EnableWindow(actrl&0x01);
				(CButton *)GetDlgItem(ID_BTN_NEXT)->EnableWindow(actrl&0x02);
				(CButton *)GetDlgItem(ID_BTN_CANCEL)->EnableWindow(actrl&0x04);
			}
			break;
		default:
			break;
	}
	if(((const CBLMANAGE_INFO *)wParam)->op_done)
	{
		if(((const CBLMANAGE_INFO *)wParam)->op_success)
		{
			memcpy(&man_info,(const CBLMANAGE_INFO *)wParam,sizeof(CBLMANAGE_INFO));
			if(!blmanage.CheckOPBusy())
			{
				if((!req_next)&&(!req_back))
					SubPageEnterOrExit(true,(const CBLMANAGE_INFO *)wParam,(const CBLMANAGE_OP *)lParam);
				if(req_next)
				{
					req_next=false;
					LastNextOrBack=true;
					if(m_sheet.GetCurSel()<m_sheet.GetNumOfPages()-1)
					{
						SubPageEnterOrExit(false,(const CBLMANAGE_INFO *)wParam,(const CBLMANAGE_OP *)lParam);
						m_sheet.SetCurSel(m_sheet.GetCurSel()+1);
						SubPageEnterOrExit(true,(const CBLMANAGE_INFO *)wParam,(const CBLMANAGE_OP *)lParam);
					}
				}
				if(req_back)
				{
					req_back=false;
					LastNextOrBack=false;
					if(m_sheet.GetCurSel()>0)
					{
						SubPageEnterOrExit(false,(const CBLMANAGE_INFO *)wParam,(const CBLMANAGE_OP *)lParam);
						m_sheet.SetCurSel(m_sheet.GetCurSel()-1);
						SubPageEnterOrExit(true,(const CBLMANAGE_INFO *)wParam,(const CBLMANAGE_OP *)lParam);
					}
				}
			}
		}
		else
		{
			if((!req_next)&&(!req_back))
				SubPageEnterOrExit(true,(const CBLMANAGE_INFO *)wParam,(const CBLMANAGE_OP *)lParam);
			if(req_next)
				req_next=false;
			if(req_back)
				req_back=false;
			
		}

	}
	

	string tmpstr;
	CBLMANAGE_INFO * pinfo=(CBLMANAGE_INFO *)wParam;
	if(!pinfo->op_done)
	{
		m_gprogress.SetBarColor(0x0000ff00);
		m_gprogress.SetWindowText(pinfo->CurPhase.c_str());
	}
	else if(pinfo->op_done)
	{
		if(!pinfo->op_success)
		{
			m_gprogress.SetBarColor(0x000000ff);
			tmpstr=pinfo->CurPhase+"Failed !";
			m_gprogress.SetWindowText(tmpstr.c_str());
		}
		else
		{
			tmpstr=pinfo->CurPhase+"Complete!";
			m_gprogress.SetWindowText(tmpstr.c_str());
		}
	}
	
	m_gprogress.SetShowPercent(TRUE);
	m_gprogress.SetPos(pinfo->progress);
	UpdateData(false);
    return 0;
}

LRESULT CMagicBurningDlg::GetWarning(WPARAM wParam, LPARAM lParam)
{
	if(lParam==NULL)
	{
		::MessageBox(0, (const char *)wParam, "Warning!!!" ,  MB_TASKMODAL | MB_ICONWARNING );
		return 1;
	}
	else
	{
		if(::MessageBox(0, (const char *)wParam, "Warning!!!" ,  MB_TASKMODAL | MB_ICONWARNING | MB_YESNO)==IDYES)
			return 1;
		else
			return 0;
	}
}
