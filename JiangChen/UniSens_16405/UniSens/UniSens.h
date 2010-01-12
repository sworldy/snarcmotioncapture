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

// UniSens.h : main header file for the UNISENS application
//

#if !defined(AFX_UNISENS_H__1E1DD1BB_EA64_4723_BBFB_8A0FF6B7B2E8__INCLUDED_)
#define AFX_UNISENS_H__1E1DD1BB_EA64_4723_BBFB_8A0FF6B7B2E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CUniSensApp:
// See UniSens.cpp for the implementation of this class
//

class CUniSensApp : public CWinApp
{
public:
	CUniSensApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUniSensApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CUniSensApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNISENS_H__1E1DD1BB_EA64_4723_BBFB_8A0FF6B7B2E8__INCLUDED_)
