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

// CCurveView view

class CCurveView : public CView
{
	DECLARE_DYNCREATE(CCurveView)

public:
	int row, frames;
	vector<vector<double> >* v;

	CRect	m_rectCurve;                //小区域
	CRect	m_rectValue;
	CRect	m_rectValue1;
	CRect	m_rectValue2;

	CRect   m_rectCurvePanel;           //大区域
	CRect   m_rectValuePanel;
	CRect   m_rectScroll;

	CRgn	m_rgnCurve;
	CRgn	m_rgnCurve1;
	CRgn	m_rgnCurve2;	

	CRgn	m_rgnYPlus;
	CRgn	m_rgnYMinus;
	CRgn	m_rgnTPlus;
	CRgn	m_rgnTMinus;
    

	
	CBrush  m_BkBrush;
	CBrush  m_PanelBrush;
	CBrush  m_CurveBrush;

	CPen    m_PenRCurve;
	CPen    m_PenOCurve;//==
    CPen    m_PenECurve;//==
	CPen    m_PenGrid;
	CPen    m_PenBrightLine;
	CPen    m_PenDarkLine;

	CFont   m_FontTitle;
	CFont   m_FontValue;
	CFont   m_FontAxis;
	CFont   m_FontLabel;
	   
	double  m_dDimT;
	double  m_dDimY;
	double  m_dValue;
	double  m_dValue1;
	double  m_dValue2;

	int		m_nBase;
	int		m_nRange;
	int     m_nArrowIndex;
	BOOL    m_bPressed ;

	BOOL	m_bDrawing;

	CBitmap m_bitmapTitle;
	CBitmap m_bitmapTrack;


	CString m_strTitle;
	CString m_strValueName;
	CString m_strValueName1;
	CString m_strValueName2;
	CString m_strUnitName;

	std::list<double> m_TimeList;
	std::list<double> m_RefValueList;
	std::list<double> m_OutputValueList;
	std::list<double> m_ErrorValueList;

	BOOL isCheckOrder;
    BOOL isCheckOutput;
	BOOL isCheckError;

protected:
	CCurveView();           // protected constructor used by dynamic creation
	virtual ~CCurveView();
	CUniSensDoc* GetDocument(); // non-debug version is inline
public:
	void SetTitle(CString strTitle);
	void SetValue(double dValue, double dValue1, double dValue2);
	void SetValueString(CString strTitle,CString strUnit);
	void SetDimT(double newDimT);
	void SetDimy(double newDimY);
	void SetBase(int nNewABase,int nRange);
	void UpdateCurve();
	void AddValue(double dTime,double dValue,double dValue1,double dValue2);
	void AddvalueTime(double dTime);
	void AddvalueData(double dValue);
	void AddOvalueData(double dValue);
    void AddEvalueData(double dValue);
	void Clear();
	
	int ConvertTimeToAxisX(double dTime);
	int ConvertValueToAxisY(double dValue);
	void DrawValue(CDC *pDC);
	void DrawValuePanel(CDC *pDC);
	void DrawTitle(CDC *pDC);
	void DrawCurvePanel(CDC *pDC);
	void DrawGrid(CDC *pDC);
	void DrawCurve(CDC *pDC);
	void DrawAxis(CDC *pDC);
	void DrawTrack(CDC *pDC,BOOL bPressed);
	void DrawArrow(CDC *pDC,int nIndex,BOOL bLighted);
	void PlayFromFile();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	afx_msg virtual BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg virtual void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};


