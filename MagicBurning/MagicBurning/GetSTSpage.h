#pragma once
#include "afxwin.h"
#include "textprogressctrl.h"
#include "BLManage.h"

// GetSTSpage dialog

class GetSTSpage : public CDialog
{
	DECLARE_DYNAMIC(GetSTSpage)

public:
	GetSTSpage(CWnd* pParent = NULL);   // standard constructor
	virtual ~GetSTSpage();

// Dialog Data
	enum { IDD = IDD_GETSTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	BOOL PreTranslateMessage(MSG* pMsg);
	bool op_flag;
	clock_t op_start;
public:
	void OnUpdateData(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	void Enter(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	void Exit(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	BYTE GetAccessCtrl(void); //bit0:btn_back ; bit1:btn_next ; bit2:btn_cancel
public:
	CTextProgressCtrl m_StatsProgress;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
