// USBTestDlg.h : header file
//

#if !defined(AFX_USBTESTDLG_H__A6A66517_BBBE_4B75_BA11_67CFC720468B__INCLUDED_)
#define AFX_USBTESTDLG_H__A6A66517_BBBE_4B75_BA11_67CFC720468B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Custom Controls
#include <vector>



//
// buffer to transmit/receive USB data
//
#pragma   pack(1)   
typedef struct USB_iobuf { 
  unsigned char id;
  unsigned char data[18];
} Buffer;
#pragma   pack()  

typedef	std::vector<CString>	DeviceList;

/////////////////////////////////////////////////////////////////////////////
// CUSBTestDlg dialog

class CUSBTestDlg : public CDialog
{
// Construction
public:
	CUSBTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUSBTestDlg)
	enum { IDD = IDD_USBTEST_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUSBTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	CString	m_sRXFileName; 
//	static   DWORD   WINAPI   WriteThreadFUNC(LPVOID pParam);
//	static   DWORD   WINAPI   ReadThreadFUNC(LPVOID pParam);

protected:
	HICON		m_hIcon;
	HANDLE		m_hUSBDevice;

	
	BOOL		m_bReadError;
	BOOL		m_bWriteError;

	DeviceList	m_DeviceList;
	int			m_nDeviceNum;
	CString		m_sDeviceName;

	BOOL	    GetDeviceList();
	BOOL	    SelectDevice();



	// Generated message map functions
	//{{AFX_MSG(CUSBTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOKExit();
	virtual void OnCancel();
	afx_msg void OnRecieve();
	afx_msg void OnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USBTESTDLG_H__A6A66517_BBBE_4B75_BA11_67CFC720468B__INCLUDED_)
