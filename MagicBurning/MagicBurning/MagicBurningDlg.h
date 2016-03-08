// MagicBurningDlg.h : header file
//

#pragma once
#include "tabsheet.h"
#include "GetMCUpage.h"
#include "GetPROpage.h"
#include "SetCFGpage.h"
#include "GetSTSpage.h"
#include "GetCOMpage.h"
#include "GetAUTHpage.h"
#include "SetAUTHpage.h"
#include "BLManage.h"
#include "textprogressctrl.h"
#include "afxwin.h"

#include <io.h>
#include <fcntl.h>
#include <stdio.h>
//customized event
#define WM_UPDATEDATA WM_USER + 1999 
#define WM_WARNING WM_UPDATEDATA + 1 

// CMagicBurningDlg dialog
class CMagicBurningDlg : public CDialog
{
// Construction
public:
	GetMCUpage OGetMCUpage;
	GetPROpage OGetPROpage;
	SetCFGpage OSetCFGpage;
	GetSTSpage OGetSTSpage;
	GetCOMpage OGetCOMpage;
	GetAUTHpage OGetAUTHpage;
	SetAUTHpage OSetAUTHpage;
	CBLMANAGE blmanage;
	CMagicBurningDlg(CWnd* pParent = NULL);	// standard constructor
	CBLMANAGE_OP man_op;
	CBLMANAGE_INFO man_info;
	//PRO_DESCRIPTOR ProSetting; 
// Dialog Data
	enum { IDD = IDD_MAGICBURNING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	int subpage;
	
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT GetWarning(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CTabSheet m_sheet;
	bool req_next;
	bool req_back;
	bool LastNextOrBack;//true:Next action false: Back action
public:
	afx_msg void OnBnClickedBtnNext();
public:
	afx_msg void OnBnClickedBtnBack();
public:
	afx_msg void OnBnClickedBtnCancel();
	void InitConsoleWindow(void);
	BOOL PreTranslateMessage(MSG* pMsg);
	void SubPageEnterOrExit(bool EnterOrExit,const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
public:
	CStatic m_PIC_MCUCOM;
public:
	CStatic m_PIC_PROCOM;
	
public:
	CTextProgressCtrl m_gprogress;
	DWORD LogMode;
};
