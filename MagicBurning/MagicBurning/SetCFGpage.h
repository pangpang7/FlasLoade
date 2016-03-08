#pragma once
#include "afxwin.h"
#include "textprogressctrl.h"
#include "BLManage.h"

// SetCFGpage dialog

class SetCFGpage : public CDialog
{
	DECLARE_DYNAMIC(SetCFGpage)

public:
	SetCFGpage(CWnd* pParent = NULL);   // standard constructor
	virtual ~SetCFGpage();

// Dialog Data
	enum { IDD = IDD_SETCFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	BOOL PreTranslateMessage(MSG* pMsg);
	
public:
	afx_msg void OnBnClickedButtonDl();
public:
	afx_msg void OnBnClickedButtonUl();

	void InitContent(void);
	void OpmodeProc(void);
	void SetEarseCtrl(BOOL ctrl);
	void SetDownloadCtrl(BOOL ctrl);
	void SetUploadCtrl(BOOL ctrl);
public:
	afx_msg void OnBnClickedRadioErase();
public:
	afx_msg void OnBnClickedRadioDl();
public:
	afx_msg void OnBnClickedRadioUl();
public:
	void OnUpdateData(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	void Enter(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	void Exit(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	BYTE GetAccessCtrl(void); //bit0:btn_back ; bit1:btn_next ; bit2:btn_cancel
public:
	afx_msg void OnEnKillfocusEditDl();
public:
	afx_msg void OnEnKillfocusEditUl();
	int op_mode; //0.earse 1.download 2.upload 3.restore
public:
	afx_msg void OnBnClickedRadioDlEnp();
public:
	afx_msg void OnBnClickedRadioDlNe();
public:
	afx_msg void OnBnClickedCheckJuser();
public:
	afx_msg void OnBnClickedCheckRff();
public:
	afx_msg void OnBnClickedCheckVad();
public:
	afx_msg void OnBnClickedRadioRestore();
	afx_msg void OnEnKillfocusEditAdr();
};
