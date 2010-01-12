// UniSensDoc.h : interface of the CUniSensDoc class
//
/////////////////////////////////////////////////////////////////////////////
#include  <fstream>  
#include  <iostream>  
#include  <stdio.h>  
#include  <stdlib.h>  
#include  <vector>  
#include  <iterator>   
#include  <math.h>


#if !defined(AFX_UNISENSDOC_H__769F9EE4_44E5_4DC3_8C59_46C23C509FF0__INCLUDED_)
#define AFX_UNISENSDOC_H__769F9EE4_44E5_4DC3_8C59_46C23C509FF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
using namespace std;

class CUniSensDoc : public CDocument
{
protected: // create from serialization only
	CUniSensDoc();
	DECLARE_DYNCREATE(CUniSensDoc)

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
	// Attributes
public:
	vector< vector<double> > fileData;
	int fileSize;
// Operations
public:
	int ReadFile();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUniSensDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUniSensDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CUniSensDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRunQuat();
	afx_msg void OnRunPlay();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNISENSDOC_H__769F9EE4_44E5_4DC3_8C59_46C23C509FF0__INCLUDED_)
