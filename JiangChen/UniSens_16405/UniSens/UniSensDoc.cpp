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

// UniSensDoc.cpp : implementation of the CUniSensDoc class
//

#include "stdafx.h"
#include "UniSens.h"
#include "MainFrm.h"
#include "UniSensDoc.h"
#include "RawDataView.h"
#include "AngView.h"
#include "EulerView.h"
#include "UniSensView.h"
#include "MagView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUniSensDoc

IMPLEMENT_DYNCREATE(CUniSensDoc, CDocument)

BEGIN_MESSAGE_MAP(CUniSensDoc, CDocument)
	//{{AFX_MSG_MAP(CUniSensDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_RUN_QUAT, &CUniSensDoc::OnRunQuat)
	ON_COMMAND(ID_RUN_PLAY, &CUniSensDoc::OnRunPlay)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUniSensDoc construction/destruction

CUniSensDoc::CUniSensDoc()
{
	// TODO: add one-time construction code here

}

CUniSensDoc::~CUniSensDoc()
{
}

BOOL CUniSensDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CUniSensDoc serialization

void CUniSensDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CUniSensDoc diagnostics

#ifdef _DEBUG
void CUniSensDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUniSensDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG     

/////////////////////////////////////////////////////////////////////////////
// CUniSensDoc commands

void CUniSensDoc::OnRunQuat()
{
	// TODO: Add your command handler code here
	fileSize = ReadFile();
}
int CUniSensDoc::ReadFile()
{
	ifstream   in("Data/raw.txt");  
	double   tmp;  
	int i = 0;
	fileData.push_back(vector<double>());  
	vector<double>*   p   =   &fileData.back();  


	while(!in.eof())
	{  
		in   >>   tmp;  
		p->push_back(tmp);
		if(++i %13 == 0)
		{  
			fileData.push_back(vector<double>());  
			p   =   &fileData.back(); 
		}  
  
	}  
	fileData.pop_back();

	return fileData.size();
}
void CUniSensDoc::OnRunPlay()
{
	// TODO: Add your command handler code here
	CMainFrame* pMF=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRawDataView* pRDV = (CRawDataView*)pMF->m_wndSplitter3.GetPane(0,0);
	CAngView	* pAV  = (CAngView*)pMF->m_wndSplitter3.GetPane(1,0);
	CMagView	* pMV  = (CMagView*)pMF->m_wndSplitter3.GetPane(2,0);
	CEulerView	* pEV  = (CEulerView*)pMF->m_wndSplitter2.GetPane(0,0);
	CUniSensView* pUSV = (CUniSensView*)pMF->m_wndSplitter2.GetPane(1,0);

	pRDV->PlayFromFile();
	pUSV->PlayFromFile();
	pAV->PlayFromFile();
	pMV->PlayFromFile();
	pEV->PlayFromFile();
}
