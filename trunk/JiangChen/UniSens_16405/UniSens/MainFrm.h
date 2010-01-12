// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__05254DB8_21B4_433B_9EA4_DBED68C59795__INCLUDED_)
#define AFX_MAINFRM_H__05254DB8_21B4_433B_9EA4_DBED68C59795__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "QKF.h"

#include <mmsystem.h>
void pascal MyTimeProc( UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1,DWORD dw2);

typedef	std::vector<CString>	DeviceList;
/*===================================================*\
|                                                     |
|  Uni-Sensor Demostration System                     |
|                                                     |
|  Copyright (c) 2009 - Guanhong Tao                  |
|      macrotao86@gmail.com                           |
|      tgh05@software.nju.edu.cn                      |
|                                                     |
|  Created:                                           |
|         2009/03/25                                  |
|  License:                                           |
|                                                     |
|  Copyright 2009 Guanhong Tao, All Rights Reserved.  |
|   For educational purposes only.                    |
|   Please do not republish in electronic or          |
|   print form without permission                     |
|                                                     |
|  Email me for the  docs and new versions            |
|                                                     |
\*===================================================*/
#define WM_MY_MESSAGE (WM_USER+100) 

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	HANDLE		m_hUSBDevice;

	BOOL		m_bReadError;
	BOOL		m_bWriteError;

	DeviceList	m_DeviceList;
	int			m_nDeviceNum;
	CString		m_sDeviceName;
	int			cnt;
	double		averageGrav[9];
	QKF*		qkf;

	UINT		TimerID;
	UINT		wAccuracy;
	UINT		wTimer;
	TIMECAPS	tc;

// Operations
public:
	BOOL		GetDeviceList();
	BOOL		SelectDevice();
	void		PlayFromDevice();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
public :
	CSplitterWnd m_wndSplitter1;
	CSplitterWnd m_wndSplitter2;
	CSplitterWnd m_wndSplitter3;

	//{{AFX_MSG(CMainFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnOpenOpenusbdevice();
	afx_msg void OnUsbPlay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnClose();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__05254DB8_21B4_433B_9EA4_DBED68C59795__INCLUDED_)
