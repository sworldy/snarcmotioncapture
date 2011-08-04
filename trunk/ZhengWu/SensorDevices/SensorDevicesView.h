// SensorDevicesView.h : interface of the CSensorDevicesView class
//


#pragma once


class CSensorDevicesView : public CView
{
protected: // create from serialization only
	CSensorDevicesView();
	DECLARE_DYNCREATE(CSensorDevicesView)

// Attributes
public:
	CSensorDevicesDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CSensorDevicesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SensorDevicesView.cpp
inline CSensorDevicesDoc* CSensorDevicesView::GetDocument() const
   { return reinterpret_cast<CSensorDevicesDoc*>(m_pDocument); }
#endif

