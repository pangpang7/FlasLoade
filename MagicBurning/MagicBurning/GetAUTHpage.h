#pragma once
#include "BLManage.h"

// GetAUTHpage dialog

class GetAUTHpage : public CDialog
{
	DECLARE_DYNAMIC(GetAUTHpage)

public:
	GetAUTHpage(CWnd* pParent = NULL);   // standard constructor
	virtual ~GetAUTHpage();

// Dialog Data
	enum { IDD = IDD_GETAUTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	
public:
	BOOL PreTranslateMessage(MSG* pMsg);
	void OnUpdateData(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	void Enter(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	void Exit(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	BYTE GetAccessCtrl(void);
	void SetAuthFunEnable(bool enable);
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedRadioAuthpass();
public:
	afx_msg void OnBnClickedRadioRemovepass();
	int auth_mode; //0.auth password 1.remove password
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnBnClickedButtonSec();
};
