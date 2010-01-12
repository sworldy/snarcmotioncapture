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
#if !defined(AFX_RAWDATAVIEW_H__9B3E90DF_4EFB_44B9_A187_947FB105775D__INCLUDED_)
#define AFX_RAWDATAVIEW_H__9B3E90DF_4EFB_44B9_A187_947FB105775D__INCLUDED_

#include "MainFrm.h"
#include <list>
#include <iterator>
#include "UniSensDoc.h"
#include "CurveView.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RawDataView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRawDataView view


class CRawDataView : public CCurveView
{
protected:
	CRawDataView();           // protected constructor used by dynamic creation

	DECLARE_DYNCREATE(CRawDataView)
// Attributes
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRawDataView)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRawDataView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CRawDataView)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAWDATAVIEW_H__9B3E90DF_4EFB_44B9_A187_947FB105775D__INCLUDED_)
