#pragma once


// GetMCUDlg dialog

class GetMCUDlg : public CDialog
{
	DECLARE_DYNAMIC(GetMCUDlg)

public:
	GetMCUDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GetMCUDlg();

// Dialog Data
	enum { IDD = IDD_GETMCU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
