// GetSTSpage.cpp : implementation file
//

#include "stdafx.h"
#include "MagicBurning.h"
#include "GetSTSpage.h"
#include "MagicBurningDlg.h"
#include "ini.h"

// GetSTSpage dialog

IMPLEMENT_DYNAMIC(GetSTSpage, CDialog)

GetSTSpage::GetSTSpage(CWnd* pParent /*=NULL*/)
	: CDialog(GetSTSpage::IDD, pParent)
{

}

GetSTSpage::~GetSTSpage()
{
}

BOOL GetSTSpage::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetTimer(1,10, NULL);
	op_flag=false;

	return TRUE;
}

void GetSTSpage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_OP, m_StatsProgress);
}


BEGIN_MESSAGE_MAP(GetSTSpage, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL GetSTSpage::PreTranslateMessage(MSG* pMsg)  
{  
    if((pMsg->wParam == VK_RETURN)||(pMsg->wParam == VK_ESCAPE))
    {  
        return false;  
    }   
    return CDialog::PreTranslateMessage(pMsg);   
}
// GetSTSpage message handlers

void GetSTSpage::OnUpdateData(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
	string tmpstr;
	if(!pinfo->op_done)
	{
		//printf("set bar color!!\n");
		m_StatsProgress.SetBarColor(0x0000ff00);
		//printf("SetWindowText!!\n");
		m_StatsProgress.SetWindowText(pinfo->CurPhase.c_str());
	}
	else if(pinfo->op_done)
	{
		if(!pinfo->op_success)
		{
			m_StatsProgress.SetBarColor(0x000000ff);
			tmpstr=pinfo->CurPhase+"Failed !";
			m_StatsProgress.SetWindowText(tmpstr.c_str());
		}
		else
		{
			tmpstr=pinfo->CurPhase+"Complete!";
			m_StatsProgress.SetWindowText(tmpstr.c_str());
		}
	}
	
	m_StatsProgress.SetShowPercent(TRUE);
	m_StatsProgress.SetPos(pinfo->progress);
	UpdateData(false);
}

void GetSTSpage::Enter(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
	char tmpstr[256];

	if(!pinfo->PIsConnected)
		return;
	if(!op_flag)
	{
		op_flag=true;
		op_start=clock();
		((CStatic *)GetDlgItem(IDC_STATIC_TIME_C))->SetWindowText("0 sec");
		((CStatic *)GetDlgItem(IDC_STATIC_TARGET_C))->SetWindowText(pop->MCU_name);
		sprintf(tmpstr,"%s.map",pop->MCU_name);
		((CStatic *)GetDlgItem(IDC_STATIC_MAP_C))->SetWindowText(tmpstr);
		CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
		switch(pDlg->OSetCFGpage.op_mode)
		{
			case 0:
				((CStatic *)GetDlgItem(IDC_STATIC_OP_C))->SetWindowText("Earse");
				((CEdit *)GetDlgItem(IDC_EDIT_FILE_C))->SetWindowText(" ");
				break;
			case 1:
				((CStatic *)GetDlgItem(IDC_STATIC_OP_C))->SetWindowText("Download");
				((CEdit *)GetDlgItem(IDC_EDIT_FILE_C))->SetWindowText(pop->FWfile_download);
				break;
			case 2:
				((CStatic *)GetDlgItem(IDC_STATIC_OP_C))->SetWindowText("Upload");
				((CEdit *)GetDlgItem(IDC_EDIT_FILE_C))->SetWindowText(pop->FWfile_upload);
				break;
			case 3:
				((CStatic *)GetDlgItem(IDC_STATIC_OP_C))->SetWindowText("Restore");
				((CEdit *)GetDlgItem(IDC_EDIT_FILE_C))->SetWindowText(" ");
				break;
			default:
				((CStatic *)GetDlgItem(IDC_STATIC_OP_C))->SetWindowText("Unknown");
				((CEdit *)GetDlgItem(IDC_EDIT_FILE_C))->SetWindowText(" ");
				break;
		}

		if(pDlg->OSetCFGpage.op_mode==0)//earse
		{
			pDlg->man_op.op=CBLMANAGE_EARSE;
			pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,3);
		}
		else if(pDlg->OSetCFGpage.op_mode==1)//download
		{
			CBLMANAGE_INFO info;
			pDlg->blmanage.GetCurInfo(&info);
			if(info.pro.BURNING_TYPE==0)//offline mode
			//if(0)
			{
				if(pDlg->OSetAUTHpage.PwdWriteOrNot)
				{
					pDlg->man_op.op=CBLMANAGE_SECURITY;
					pDlg->man_op.SecType=0;
					pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,1);

					pDlg->man_op.op=CBLMANAGE_SET_CONFIG;
					//pDlg->man_op.pro.MCU_PID[0]=pDlg->man_info.pro.MCU_PID[0];
					//pDlg->man_op.pro.MCU_PID[1]=pDlg->man_info.pro.MCU_PID[1];
					pDlg->man_op.pro.BURNING_TYPE=pDlg->man_info.pro.BURNING_TYPE;
					pDlg->man_op.pro.MCU_COM=pDlg->man_info.pro.MCU_COM;
					//pDlg->man_op.pro.MCU_page_size=pDlg->man_info.pro.MCU_page_size;
					pDlg->man_op.pro.flash_offset=pDlg->man_info.pro.MCU_FLASH_BASE-pDlg->man_info.DEV_FLASH_BASE;
					pDlg->man_op.pro.ram_offset=pDlg->man_info.pro.MCU_RAM_BASE-pDlg->man_info.DEV_RAM_BASE;
					pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,0);
				}
				else
				{
					pDlg->man_op.op=CBLMANAGE_SET_CONFIG;
					//pDlg->man_op.pro.MCU_PID[0]=pDlg->man_info.pro.MCU_PID[0];
					//pDlg->man_op.pro.MCU_PID[1]=pDlg->man_info.pro.MCU_PID[1];
					pDlg->man_op.pro.BURNING_TYPE=pDlg->man_info.pro.BURNING_TYPE;
					pDlg->man_op.pro.MCU_COM=pDlg->man_info.pro.MCU_COM;
					//pDlg->man_op.pro.MCU_page_size=pDlg->man_info.pro.MCU_page_size;
					pDlg->man_op.pro.flash_offset=pDlg->man_info.pro.MCU_FLASH_BASE-pDlg->man_info.DEV_FLASH_BASE;
					pDlg->man_op.pro.ram_offset=pDlg->man_info.pro.MCU_RAM_BASE-pDlg->man_info.DEV_RAM_BASE;
					pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,1);
				}

				pDlg->man_op.op=CBLMANAGE_SET_OFF_CONFIG;
				pDlg->man_op.pro.flash_done=0;
				pDlg->man_op.pro.ram_done=0;
				pDlg->man_op.pro.download_type=2;
				pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,0);

				if(pDlg->man_op.NeedEarse==2)
				{
					//erase before download
					pDlg->man_op.op=CBLMANAGE_EARSE;
					pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,0);
				}

				pDlg->man_op.op=CBLMANAGE_DOWNLOAD;
				pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,0);

				pDlg->man_op.op=CBLMANAGE_SET_OFF_CONFIG;
				pDlg->man_op.pro.flash_done=1;
				pDlg->man_op.pro.ram_done=0;
				pDlg->man_op.pro.download_type=2;
				pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,2);

			}
			else
			{
				if(pDlg->OSetAUTHpage.PwdWriteOrNot)
				{
					pDlg->man_op.op=CBLMANAGE_SECURITY;
					pDlg->man_op.SecType=0;
					pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,1);

					/*if(pDlg->man_op.NeedEarse==2)
					{
						//erase before download
						pDlg->man_op.op=CBLMANAGE_EARSE;
						pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,0);
					}*/

					pDlg->man_op.op=CBLMANAGE_DOWNLOAD;
					pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,2);
				}
				else
				{
					if(pDlg->man_op.NeedEarse==2)
					{
						//erase before download
						pDlg->man_op.op=CBLMANAGE_EARSE;
						pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,1);

						pDlg->man_op.op=CBLMANAGE_DOWNLOAD;
						pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,2);
					}
					else
					{
						pDlg->man_op.op=CBLMANAGE_DOWNLOAD;
						pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,3);
					}
				}
			}
		}
		else if(pDlg->OSetCFGpage.op_mode==2)//upload
		{
			CBLMANAGE_INFO info;
			pDlg->blmanage.GetCurInfo(&info);
			if(info.pro.BURNING_TYPE==0)//offline mode
			{
				pDlg->man_op.op=CBLMANAGE_SET_CONFIG;
				pDlg->man_op.pro.flash_offset=pDlg->man_info.pro.MCU_FLASH_BASE-pDlg->man_info.DEV_FLASH_BASE;
				pDlg->man_op.pro.ram_offset=pDlg->man_info.pro.MCU_RAM_BASE-pDlg->man_info.DEV_RAM_BASE;
				pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,1);

				pDlg->man_op.op=CBLMANAGE_UPLOAD;
				pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,2);
			}
			else
			{
				pDlg->man_op.op=CBLMANAGE_UPLOAD;
				pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,3);
			}
		}
		else if(pDlg->OSetCFGpage.op_mode==3)//restore
		{
			pDlg->man_op.op=CBLMANAGE_SECURITY;
			pDlg->man_op.SecType=4;//restore op is equal to remove pwd op
			pDlg->blmanage.OP_Req_asycn(&pDlg->man_op,3);
		}
	}

	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	pDlg->GetDlgItem(ID_BTN_CANCEL)->SetWindowText("Cancel");
}

void GetSTSpage::Exit(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop)
{
	op_flag=false;
}

BYTE GetSTSpage::GetAccessCtrl(void)
{
	return 5;
}

void GetSTSpage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CMagicBurningDlg *pDlg = (CMagicBurningDlg *)GetParent()->GetParent();
	if(pDlg->blmanage.CheckOPBusy())
	{
		char tmpstr[256];
		sprintf(tmpstr,"%.3f sec",((float)(clock()-op_start))/1000);
		((CStatic *)GetDlgItem(IDC_STATIC_TIME_C))->SetWindowText(tmpstr);
	}
	CDialog::OnTimer(nIDEvent);
}
