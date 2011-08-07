// SensorDevicesView.cpp : implementation of the CSensorDevicesView class
//

#include "stdafx.h"
#include "SensorDevices.h"

#include "SensorDevicesDoc.h"
#include "SensorDevicesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSensorDevicesView

IMPLEMENT_DYNCREATE(CSensorDevicesView, CView)

BEGIN_MESSAGE_MAP(CSensorDevicesView, CView)
END_MESSAGE_MAP()

// CSensorDevicesView construction/destruction

CSensorDevicesView::CSensorDevicesView()
{
	// TODO: add construction code here

}

CSensorDevicesView::~CSensorDevicesView()
{
}

BOOL CSensorDevicesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSensorDevicesView drawing

void CSensorDevicesView::OnDraw(CDC* /*pDC*/)
{
	CSensorDevicesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSensorDevicesView diagnostics

#ifdef _DEBUG
void CSensorDevicesView::AssertValid() const
{
	CView::AssertValid();
}

void CSensorDevicesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSensorDevicesDoc* CSensorDevicesView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSensorDevicesDoc)));
	return (CSensorDevicesDoc*)m_pDocument;
}
#endif //_DEBUG


// CSensorDevicesView message handlers
