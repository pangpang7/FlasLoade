#pragma once
#include "afxwin.h"
#include "textprogressctrl.h"
#include "BLManage.h"

// GetCOMpage dialog

class GetCOMpage : public CDialog
{
	DECLARE_DYNAMIC(GetCOMpage)

public:
	GetCOMpage(CWnd* pParent = NULL);   // standard constructor
	virtual ~GetCOMpage();

// Dialog Data
	enum { IDD = IDD_GETCOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	BOOL PreTranslateMessage(MSG* pMsg);
	void ContentRefresh(void);
public:
	virtual BOOL OnInitDialog();
	void OnUpdateData(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	void Enter(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	void Exit(const CBLMANAGE_INFO * pinfo,const CBLMANAGE_OP * pop);
	BYTE GetAccessCtrl(void); //bit0:btn_back ; bit1:btn_next ; bit2:btn_cancel
};
