#pragma once


// GetPRODlg dialog

class GetPRODlg : public CDialog
{
	DECLARE_DYNAMIC(GetPRODlg)

public:
	GetPRODlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GetPRODlg();

// Dialog Data
	enum { IDD = IDD_GETPRO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
