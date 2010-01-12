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
#pragma once
#include "MainFrm.h"
#include <list>
#include <iterator>
#include "UniSensDoc.h"
#include "CurveView.h"

// CAngView view

class CAngView : public CCurveView
{
	DECLARE_DYNCREATE(CAngView)
protected:
	CAngView();           // protected constructor used by dynamic creation
	virtual ~CAngView();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

		// Generated message map functions
protected:
	//{{AFX_MSG(CAngView)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


