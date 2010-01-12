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

// MagView.cpp : implementation file
//

#include "stdafx.h"
#include "UniSens.h"
#include "MagView.h"
#include <list>
#include <iterator>

#include "mmsystem.h"
#include <math.h>
#pragma comment(lib,"winmm.lib")
// CMagView

IMPLEMENT_DYNCREATE(CMagView, CView)

CMagView::CMagView()
{
	m_strTitle = _T("Magnetic Field");
	m_strValueName = _T("Mag X");
	m_strValueName1 = _T("Mag Y");
	m_strValueName2 = _T("Mag Z");
	m_strUnitName = _T("AU");

	m_dDimT	= 3.0;
	m_dDimY	= 25.0;
	m_nBase = 5;
	m_nRange = 10;
	m_dValue= 0.0;
	m_dValue1 = 0.0;
	m_dValue2 = 0.0;

	m_nArrowIndex =-1;

	row = 0;
	frames = 0;

	m_bPressed = FALSE;
	isCheckOrder = TRUE ;
	isCheckOutput = TRUE ;
	isCheckError = TRUE ;
}

CMagView::~CMagView()
{

}

BEGIN_MESSAGE_MAP(CMagView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMagView drawing

BOOL CMagView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;

	GetClientRect(&rect);
	pDC->FillRect(&rect,&m_BkBrush);//Draw background
	rect.InflateRect(-3,-25,-4,-60);//curve panel
	m_rectCurvePanel = rect;
	m_rectCurve = rect;
	m_rectCurve.InflateRect(-45,-5,-23,-25);//curve area
	m_rectScroll = m_rectCurve;
	m_rectScroll.left = m_rectCurve.right+8;
	m_rectScroll.right = m_rectScroll.left+10;//rect scroll created
	m_rectValuePanel = rect;
    m_rectValuePanel.top = rect.bottom+5;
 	m_rectValuePanel.bottom= m_rectValuePanel.top+51;//rectValuePanel created

	CFont *pOldFont;
	pOldFont = pDC->SelectObject(&m_FontLabel);
	CSize size = pDC->GetTextExtent(m_strValueName);
	m_rectValue.left = m_rectValuePanel.left+20+size.cx;
	m_rectValue.top = m_rectValuePanel.top+(m_rectValuePanel.Height()-size.cy)/2-2;
	m_rectValue.bottom = m_rectValue.top+size.cy+4;
	size = pDC->GetTextExtent(m_strUnitName);
	m_rectValue.right = m_rectValuePanel.right/3 - 20 - size.cx;

	m_rectValue1.left = m_rectValue.right + size.cx + 65;
	m_rectValue1.top  = m_rectValue.top;
	m_rectValue1.bottom = m_rectValue.bottom;
	m_rectValue1.right  = m_rectValue1.left + m_rectValue.right - m_rectValue.left ;

	m_rectValue2.left = m_rectValue1.right + size.cx + 65;
	m_rectValue2.top  = m_rectValue.top;
	m_rectValue2.bottom = m_rectValue.bottom;
	m_rectValue2.right  = m_rectValue2.left + m_rectValue.right - m_rectValue.left ;

	pDC->SelectObject(pOldFont);

	

	if(m_rgnCurve.m_hObject==NULL)
		m_rgnCurve.CreateRectRgn(m_rectCurve.left+2,m_rectCurve.top+2,m_rectCurve.right-2,m_rectCurve.bottom-2);
	
	POINT ptYPlus[3];
	POINT ptYMinus[3];
	POINT ptTPlus[3];
	POINT ptTMinus[3];
	ptYPlus[0].x = m_rectCurvePanel.left+15;
	ptYPlus[0].y = m_rectCurve.bottom+3;
	ptYPlus[1].x = ptYPlus[0].x-5;
	ptYPlus[1].y = ptYPlus[0].y+6;
	ptYPlus[2].x = ptYPlus[0].x+5;
	ptYPlus[2].y = ptYPlus[0].y+6;

	ptYMinus[0].x = m_rectCurvePanel.left+15;
	ptYMinus[0].y = m_rectCurve.bottom+18;
	ptYMinus[1].x = ptYMinus[0].x-5;
	ptYMinus[1].y = ptYMinus[0].y-6;
	ptYMinus[2].x = ptYMinus[0].x+5;
	ptYMinus[2].y = ptYMinus[0].y-6;

	ptTPlus[0].x = m_rectScroll.left+m_rectScroll.Width()/2;
	ptTPlus[0].y = m_rectCurve.bottom+3;
	ptTPlus[1].x = ptTPlus[0].x-5;
	ptTPlus[1].y = ptTPlus[0].y+6;
	ptTPlus[2].x = ptTPlus[0].x+5;
	ptTPlus[2].y = ptTPlus[0].y+6;

	ptTMinus[0].x = m_rectScroll.left+m_rectScroll.Width()/2;
	ptTMinus[0].y = m_rectCurve.bottom+18;
	ptTMinus[1].x = ptTMinus[0].x-5;
	ptTMinus[1].y = ptTMinus[0].y-6;
	ptTMinus[2].x = ptTMinus[0].x+5;
	ptTMinus[2].y = ptTMinus[0].y-6;
	if(m_rgnYPlus.m_hObject==NULL)
		m_rgnYPlus.CreatePolygonRgn( ptYPlus, 3, ALTERNATE );
	if(m_rgnYMinus.m_hObject==NULL)
		m_rgnYMinus.CreatePolygonRgn( ptYMinus, 3, ALTERNATE );
	if(m_rgnTPlus.m_hObject==NULL)
		m_rgnTPlus.CreatePolygonRgn( ptTPlus, 3, ALTERNATE );
	if(m_rgnTMinus.m_hObject==NULL)
		m_rgnTMinus.CreatePolygonRgn( ptTMinus, 3, ALTERNATE );
	
	return TRUE;
}
// CMagView diagnostics

#ifdef _DEBUG
void CMagView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMagView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMagView message handlers

void CMagView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	double mag_x, mag_y, mag_z;
	double dTime = ::timeGetTime()/1000.0;
	if(row < frames)
	{
		mag_x = v->at(row).at(6);
		mag_y = v->at(row).at(7); 
		mag_z = v->at(row).at(8); 
		row++;
		AddValue(dTime, mag_x, mag_y, mag_z);
		SetValue(mag_x, mag_y, mag_z);
		UpdateCurve();
	}
	CView::OnTimer(nIDEvent);
}
