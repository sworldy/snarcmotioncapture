// SensorDevicesDoc.cpp : implementation of the CSensorDevicesDoc class
//

#include "stdafx.h"
#include "SensorDevices.h"

#include "SensorDevicesDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSensorDevicesDoc

IMPLEMENT_DYNCREATE(CSensorDevicesDoc, CDocument)

BEGIN_MESSAGE_MAP(CSensorDevicesDoc, CDocument)
END_MESSAGE_MAP()


// CSensorDevicesDoc construction/destruction

CSensorDevicesDoc::CSensorDevicesDoc()
{
	// TODO: add one-time construction code here

}

CSensorDevicesDoc::~CSensorDevicesDoc()
{
}

BOOL CSensorDevicesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSensorDevicesDoc serialization

void CSensorDevicesDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CSensorDevicesDoc diagnostics

#ifdef _DEBUG
void CSensorDevicesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSensorDevicesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSensorDevicesDoc commands
