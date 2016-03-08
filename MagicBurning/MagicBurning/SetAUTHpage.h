#pragma once
#include "BLManage.h"

// SetAUTHpage dialog

class SetAUTHpage : public CDialog
{
	DECLARE_DYNAMIC(SetAUTHpage)

public:
	SetAUTHpage(CWnd* pParent = NULL);   // standard constructor
	virtual ~SetAUTHpage();
	bool PwdWriteOrNot;
	bool PwdCustomOrNot;
	bool FLCustomOrNot;
	bool FBKCustomOrNot;
	bool FBKEnableOrNot;

// Dialog Data
	enum { IDD = IDD_SETAUTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	void OnUpdateData(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	void Enter(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	void Exit(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	BYTE GetAccessCtrl(void);
	void ChangeVisible(void);
public:
	afx_msg void OnBnClickedRadioPasswr();
public:
	afx_msg void OnBnClickedRadioPassnowr();
public:
	afx_msg void OnBnClickedRadioCp();
public:
	afx_msg void OnBnClickedRadioCpd();
public:
	afx_msg void OnBnClickedRadioCfl();
public:
	afx_msg void OnBnClickedRadioCfld();
public:
	afx_msg void OnBnClickedRadioCfbk();
public:
	afx_msg void OnBnClickedRadioCfbkd();
public:
	afx_msg void OnBnClickedRadioCfbkno();
public:
	afx_msg void OnEnChangeEditCfbk();
};
