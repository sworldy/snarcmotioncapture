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

// UniSensView.h : interface of the CUniSensView class
//
/////////////////////////////////////////////////////////////////////////////
#include "MainFrm.h"

#if !defined(AFX_UNISENSVIEW_H__D38E8858_962B_4311_8E7F_721A11AB82D7__INCLUDED_)
#define AFX_UNISENSVIEW_H__D38E8858_962B_4311_8E7F_721A11AB82D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAXTEXTURE 6
#define PI 3.1415926535
class CUniSensView : public CView
{
protected: // create from serialization only
	CUniSensView();
	DECLARE_DYNCREATE(CUniSensView)

// Attributes
public:

	CUniSensDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUniSensView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	int		frames;
	vector<vector<double> >* v;

	int		m_Zoom;
	int		row;
	long	m_xRect,m_yRect;
	GLfloat  xrof,yrof,zrof;
	GLfloat  theta;
	double Q_w, Q_x, Q_y, Q_z;

	CClientDC	*m_pDC;
	CMainFrame	*m_pFr;

	GLuint texture[MAXTEXTURE];
	CPoint m_MouseDownPoint;			//used for mouse drag rotation
	virtual ~CUniSensView();
	void DrawScreen();
	int ReadFile();
	void DrawLines();
	void Draw3DRect();
	void DrawText();
	void SetViewPort(int cx, int cy);
	void InitOpenGL();
	bool bSetPixelFormat();
	BOOL LoadTextureGL(void);
	AUX_RGBImageRec *LoadImage(char *Filename);
	void PlayFromFile();
	void SetQuaternion(double Quat_w, double Quat_x, double Quat_y, double Quat_z, int i);
	void SetQuaternion(double Quat_w, double Quat_x, double Quat_y, double Quat_z);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CUniSensView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnRunQuat();
//	afx_msg void OnRunPlay();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in UniSensView.cpp
inline CUniSensDoc* CUniSensView::GetDocument()
   { return (CUniSensDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNISENSVIEW_H__D38E8858_962B_4311_8E7F_721A11AB82D7__INCLUDED_)
