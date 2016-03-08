#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "textprogressctrl.h"
#include "BLManage.h"


// GetMCUpage dialog

class GetMCUpage : public CDialog
{
	DECLARE_DYNAMIC(GetMCUpage)

public:
	GetMCUpage(CWnd* pParent = NULL);   // standard constructor
	virtual ~GetMCUpage();

// Dialog Data
	enum { IDD = IDD_GETMCU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_pagelist;
	BOOL PreTranslateMessage(MSG* pMsg);
	void OnUpdateData(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	void Enter(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	void Exit(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	BYTE GetAccessCtrl(void); //bit0:btn_back ; bit1:btn_next ; bit2:btn_cancel
};
