#pragma once


// SelectPort dialog

class SelectPort : public CDialog
{
	DECLARE_DYNAMIC(SelectPort)

public:
	SelectPort(CWnd* pParent = NULL);   // standard constructor
	virtual ~SelectPort();

// Dialog Data
	enum { IDD = IDD_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_PortNo;
	CString m_strPortNO;
	afx_msg void OnBnClickedOk();
};
