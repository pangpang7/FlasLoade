#pragma once
#include "afxwin.h"
#include "textprogressctrl.h"
#include "BLManage.h"


// GetPROpage dialog

class GetPROpage : public CDialog
{
	DECLARE_DYNAMIC(GetPROpage)

public:
	GetPROpage(CWnd* pParent = NULL);   // standard constructor
	virtual ~GetPROpage();
	int bl_mode;

// Dialog Data
	enum { IDD = IDD_GETPRO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combopro;
public:
	CComboBox m_combomcus;
public:
	CComboBox m_combobr;
public:
	CComboBox m_combodb;
public:
	CComboBox m_comboto;
public:
	CComboBox m_combobr3;
public:
	afx_msg void OnBnClickedRadioOffline();
public:
	afx_msg void OnBnClickedRadioOnline();
public:
	afx_msg void OnCbnSelchangeComboPro();
public:
	afx_msg void OnCbnSelchangeComboMcus();
public:
	afx_msg void OnCbnSelchangeComboBr();

	BOOL PreTranslateMessage(MSG* pMsg);
	//CTextProgressCtrl m_ProgressCom;
	void OnUpdateData(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	void Enter(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	void Exit(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	BYTE GetAccessCtrl(void); //bit0:btn_back ; bit1:btn_next ; bit2:btn_cancel
	
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
