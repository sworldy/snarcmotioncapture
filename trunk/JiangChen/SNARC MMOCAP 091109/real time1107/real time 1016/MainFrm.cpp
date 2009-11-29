///////////////////////////////////////////////////////////////////////////////
//
// MainFrm.cpp : implementation of the CMainFrame class
//
// Purpose:	Implementation of Main Window of Hierarchical Animation System
//
// Created:
//		JL 9/1/97		
// Modified:
//		JL 7/10/99		Created skeleton Demo for Oct 99 GDMag
//
///////////////////////////////////////////////////////////////////////////////
//
//	Copyright 1999 Jeff Lander, All Rights Reserved.
//  For educational purposes only.
//  Please do not republish in electronic or print form without permission
//  Thanks - jeffl@darwin3d.com
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mmsystem.h"		// NEED THIS FOR THE TIMEGETTIME
#include "Skully.h"
#include "HierWin.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/// Local Defines /////////////////////////////////////////////////////////////
#define HIERWIN_START_X	0			// STARTING X POSITION OF HIERARCHY WINDOW
#define HIERWIN_START_Y	32			// STARTING Y POSITION OF HIERARCHY WINDOW
#define HIERWIN_WIDTH	160			// WIDTH OF HIERARCHY WINDOW
#define HIERWIN_BOTTOM	52			// BOTTOM BORDER OF HIERARCHY WINDOW
#define OGLWIN_START_X	162			// STARTING X POSITION OF OPENGL WINDOW
#define OGLWIN_START_Y	1			// STARTING Y POSITION OF OPENGL WINDOW
#define OGLWIN_WIDTH	164			// WIDTH OF OPENGL WINDOW SUBTRACTED FROM MAX
#define OGLWIN_BOTTOM	20			// BOTTOM BORDER OF OPENGL WINDOW
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

int LoadEnabled = 0;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_COMMAND(ID_WHICHOGL, OnWhichogl)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_SKELETON_RESETSKELETON, OnSkeletonResetskeleton)
	ON_COMMAND(ID_VIEW_OUTLINE, OnViewOutline)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTLINE, OnUpdateViewOutline)
	ON_COMMAND(ID_FILE_OPENCHARACTERMESH, OnFileOpencharactermesh)
	ON_COMMAND(ID_VIEW_VIEWSKELETON, OnViewViewskeleton)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWSKELETON, OnUpdateViewViewskeleton)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_VIEW_DRAWDEFORMED, OnViewDrawdeformed)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DRAWDEFORMED, OnUpdateViewDrawdeformed)
	ON_COMMAND(ID_SKELETON_SETRESTPOSE, OnSkeletonSetrestpose)
	ON_COMMAND(ID_SKELETON_SETBONEWEIGHTS, OnSkeletonSetboneweights)
	ON_COMMAND(ID_FILE_OPENWEIGHT, OnFileOpenweight)
	ON_COMMAND(ID_SKELETON_CLEARSELECTEDWEIGHTS, OnSkeletonClearselectedweights)
	ON_COMMAND(ID_STOP, OnStop)
	ON_COMMAND(ID_PLAY_FORWARD, OnPlayForward)
	ON_COMMAND(ID_PLAY_BACK, OnPlayBack)
	ON_COMMAND(ID_FORWARD_FRAME, OnForwardFrame)
	ON_COMMAND(ID_BACK_FRAME, OnBackFrame)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_ANIMATION_LOADANIMATION, CMainFrame::OnAnimationLoadanimation)
	ON_COMMAND(ID_ANIMATION_PLAY, CMainFrame::OnAnimationPlay)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_FRAME2,		// FOR DISPLAY OF FRAMECOUNT AND CURRENT FRAME
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_Wireframe = TRUE;
	m_HArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);	

	m_Animating = FALSE;
	m_ElapsedTime = ~(DWORD)0; // initialize to a big number
	m_Animation_Direction = 1;
	m_AnimSpeed = 30;			// DEFAULT PLAYBACK SPEED AS FRAME DELAY

}

CMainFrame::~CMainFrame()
{
	//m_HierWin.DestroyWindow();
      m_HierWin.PostMessage(WM_CLOSE,0,0); 
      m_HierWin.DestroyWindow();
}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
/// Local Variables ///////////////////////////////////////////////////////////
	RECT rect;
///////////////////////////////////////////////////////////////////////////////
	GetClientRect(&rect); 

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
  
		// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);	
	
	
	m_HierWin.Create(WS_BORDER |TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS, 
		CRect(HIERWIN_START_X, HIERWIN_START_Y,HIERWIN_WIDTH,rect.bottom-20), this, 1001);
	m_HierWin.ShowWindow(TRUE);
    
	m_OGLView.m_StatusBar = &m_wndStatusBar;
	m_OGLView.Create(NULL,"Render Window",WS_CHILD | WS_VISIBLE, 
		CRect(OGLWIN_START_X, OGLWIN_START_Y,rect.right - OGLWIN_WIDTH,rect.bottom - OGLWIN_BOTTOM),this,104);
	m_OGLView.ShowWindow(TRUE);

//	m_OGLView.m_StatusBar = &m_wndStatusBar;

	m_OGLView.Invalidate(TRUE);



	// Now we initialize the animation code

	m_StartTime = ::timeGetTime(); // get time in ms

	// need a previous time if we start off animated
	if ( m_Animating )
	{
		m_previousElapsedTime = m_StartTime;
	}


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	HICON hicon;
	
	hicon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_ClassName = AfxRegisterWndClass(NULL,
		(HCURSOR)m_HArrow, (HBRUSH)::GetStockObject(DKGRAY_BRUSH), hicon); //m_HArrow
	cs.lpszClass = m_ClassName;

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame implementation

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

///////////////////////////////////////////////////////////////////////////////
// Procedure:	OnWhichogl
// Purpose:		Create dialog to Show which version of OGL is running
///////////////////////////////////////////////////////////////////////////////		
void CMainFrame::OnWhichogl()
{
	m_OGLView.GetGLInfo();
}
// OnWhichogl

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

///////////////////////////////////////////////////////////////////////////////
// Procedure:	OnPaint
// Purpose:		This routine grabs the message loop if I am animating and
//				handles the messages.  This way I can play back as fast
//              as possible
// Reference:	OpenGL Programming for Windows 95 by Ron Fosner
//				Sort of a variation on that code
///////////////////////////////////////////////////////////////////////////////		
void CMainFrame::OnPaint() 
{
    int i=0;
	MSG msg;
	CPaintDC dc(this); // device context for painting

	// IF I AM IN ANIMATING MODE
	while ( m_Animating == TRUE)
	{
		while (::PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				m_Animating = FALSE;
				::PostQuitMessage(0);
			}

			// Dispatch any messages as needed
			if (!AfxGetApp()->PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			// Give the Idle system some time
			AfxGetApp()->OnIdle(0);
			AfxGetApp()->OnIdle(1);

		}
		m_ElapsedTime = ::timeGetTime(); // get current time

		// CHECK IF ENOUGH TIME HAS GONE BY TO DRAW THE NEXT FRAME
	
		if (ElapsedTimeRender() > (unsigned int)(1000/m_AnimSpeed) )
		{
			// ADVANCE THE ANIMATION
			strcpy(m_OGLView.animboneName0,m_OGLView.m_Skeleton.children->name);//¸ù½Úµã
			strcpy(m_OGLView.animboneName1,m_OGLView.m_Skeleton.children->children->name);//¼¹Öù1
		    strcpy(m_OGLView.animboneName2,m_OGLView.m_Skeleton.children->children[0].children->name);//¼¹Öù2
            strcpy(m_OGLView.animboneName3,m_OGLView.m_Skeleton.children->children[0].children->children->name);//¼¹Öù3
			strcpy(m_OGLView.animboneName4,m_OGLView.m_Skeleton.children->children[0].children->children->children[0].children->name);//×óÉÏ±Û
			strcpy(m_OGLView.animboneName5,m_OGLView.m_Skeleton.children->children[0].children->children->children[0].children->children->name);//×óÇ°±Û
			strcpy(m_OGLView.animboneName6,m_OGLView.m_Skeleton.children->children[0].children->children->children[0].children->children->children->name);//×óÊÖ
			strcpy(m_OGLView.animboneName7,m_OGLView.m_Skeleton.children->children[0].children->children->children[1].children->name);//ÓÒÉÏ±Û
			strcpy(m_OGLView.animboneName8,m_OGLView.m_Skeleton.children->children[0].children->children->children[1].children->children->name);//ÓÒÇ°±Û
			strcpy(m_OGLView.animboneName9,m_OGLView.m_Skeleton.children->children[0].children->children->children[1].children->children->children->name);//ÓÒÊÖ
			strcpy(m_OGLView.animboneName10,m_OGLView.m_Skeleton.children->children[0].children->children->children[2].children->name);//Í·
            strcpy(m_OGLView.animboneName11,m_OGLView.m_Skeleton.children->children[1].name);//×ó´óÍÈ
			strcpy(m_OGLView.animboneName12,m_OGLView.m_Skeleton.children->children[1].children->name);//×óÐ¡ÍÈ
			strcpy(m_OGLView.animboneName13,m_OGLView.m_Skeleton.children->children[1].children->children->name);//×ó½Å
			strcpy(m_OGLView.animboneName14,m_OGLView.m_Skeleton.children->children[2].name);//ÓÒ´óÍÈ
			strcpy(m_OGLView.animboneName15,m_OGLView.m_Skeleton.children->children[2].children->name);//ÓÒÐ¡ÍÈ
		    strcpy(m_OGLView.animboneName16,m_OGLView.m_Skeleton.children->children[2].children->children->name);//ÓÒ½Å
			
			BoneAdvanceFrameTest0(m_OGLView.m_Skeleton.children,16,FALSE);//Ñü		    
			BoneAdvanceFrameTest1(m_OGLView.m_Skeleton.children->children,3,FALSE);//¼¹Öù1
		    BoneAdvanceFrameTest2(m_OGLView.m_Skeleton.children->children[0].children,4,FALSE);//¼¹Öù2
	        BoneAdvanceFrameTest3(m_OGLView.m_Skeleton.children->children[0].children->children,5,FALSE);//¼¹Öù3
			BoneAdvanceFrameTest4(m_OGLView.m_Skeleton.children->children[0].children->children->children[0].children,10,FALSE);//×óÉÏ±Û
			BoneAdvanceFrameTest5(m_OGLView.m_Skeleton.children->children[0].children->children->children[0].children->children,11,FALSE);//×óÇ°±Û
			BoneAdvanceFrameTest6(m_OGLView.m_Skeleton.children->children[0].children->children->children[0].children->children->children,12,FALSE);//×óÊÖ
			BoneAdvanceFrameTest7(m_OGLView.m_Skeleton.children->children[0].children->children->children[1].children,13,FALSE);//ÓÒÉÏ±Û
			BoneAdvanceFrameTest8(m_OGLView.m_Skeleton.children->children[0].children->children->children[1].children->children,14,FALSE);//ÓÒÇ°±Û
			BoneAdvanceFrameTest9(m_OGLView.m_Skeleton.children->children[0].children->children->children[1].children->children->children,15,FALSE);//ÓÒÊÖ
			BoneAdvanceFrameTest10(m_OGLView.m_Skeleton.children->children[0].children->children->children[2].children,6,FALSE);//Í·
			BoneAdvanceFrameTest11(&m_OGLView.m_Skeleton.children->children[1],7,FALSE);//×ó´óÍÈ
			BoneAdvanceFrameTest12(m_OGLView.m_Skeleton.children->children[1].children,8,FALSE);//×óÐ¡ÍÈ
			BoneAdvanceFrameTest13(m_OGLView.m_Skeleton.children->children[1].children->children,9,FALSE);//×ó½Å
	    	BoneAdvanceFrameTest14(&m_OGLView.m_Skeleton.children->children[2],0,FALSE);//ÓÒ´óÍÈ
			BoneAdvanceFrameTest15(m_OGLView.m_Skeleton.children->children[2].children,1,FALSE);//ÓÒÐ¡ÍÈ
			BoneAdvanceFrameTest16(m_OGLView.m_Skeleton.children->children[2].children->children,2,FALSE);//ÓÒ½Å

			// REDRAW THE OGL WINDOW
			m_OGLView.drawScene(FALSE);	
	    	m_previousElapsedTime = m_ElapsedTime;
		 }
	}

	m_OGLView.drawScene(FALSE);

	/*if(LoadEnabled == 1)// ¶ÁÈëÊý¾Ýºó£¬ÔÚ²¥·Å 
	{
		//LoadAnimationTest(1,m_OGLView.m_SelectedBone,&m_OGLView.m_Skeleton,&(m_OGLView.Play_Animation));
		m_Animating = TRUE;
		Invalidate(TRUE);
	 }*/

}
/// OnPaint ////////////////////////////////////////////////////////////

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	// RESET THE HierWin WINDOW SIZE
	m_HierWin.SetWindowPos( &wndTopMost, HIERWIN_START_X, HIERWIN_START_Y, HIERWIN_WIDTH, cy - HIERWIN_BOTTOM, SWP_NOZORDER ); 
	// RESET THE m_OGLView WINDOW SIZE
	m_OGLView.SetWindowPos( &wndTopMost, OGLWIN_START_X, OGLWIN_START_Y, cx - OGLWIN_WIDTH, cy - OGLWIN_BOTTOM, SWP_NOZORDER );
	// RESET THE ACTUAL OPENGL WINDOW SIZE
	m_OGLView.resize(  cx - OGLWIN_WIDTH, cy - OGLWIN_BOTTOM);
	CFrameWnd::OnSize(nType, cx, cy);
   
}

// HAVEN'T IMPLEMENTED ADDING A BONE
#if 0
void CMainFrame::OnAddBone() 
{
	m_HierWin.AddBone();
}
#endif

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_OGLView.HandleKeyDown(nChar);
	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
	m_OGLView.HandleKeyUp(nChar);
}

/////////////////////////////////////////////////////////////////////////////
// Hierarchy Manipulation Functions

/////////////////////////////////////////////////////////////////////////////
// View Manipulation Functions

/////////////////////////////////////////////////////////////////////////////


void CMainFrame::OnFileOpen() 
{
/// Local Variables ///////////////////////////////////////////////////////////
//	char BASED_CODE szFilter[] = "Darwin Skeleton (*.dar)|*.dar||";  
//	CFileDialog	*dialog;
//	CString exten;
///////////////////////////////////////////////////////////////////////////////
    	m_OGLView.LoadSkeletonFile("man-bone.dar");
		m_HierWin.ResetSkeleton();
		m_HierWin.SetSkeleton(&m_OGLView.m_Skeleton, NULL);
		m_OGLView.drawScene(FALSE);
		m_OGLView.OnViewResetskeleton();
		m_OGLView.LoadOBJModel("man-mash.obj");
		m_OGLView.drawScene(FALSE);
		m_OGLView.OnViewResetskeleton();
	    m_OGLView.LoadWeightsTest("man-weight.wgt");
	
}

void CMainFrame::OnFileOpencharactermesh() 
{
/// Local Variables ///////////////////////////////////////////////////////////
	char BASED_CODE szFilter[] = "Character Mesh OBJ (*.obj)|*.obj||";  
	CFileDialog	*dialog;
	CString exten;
///////////////////////////////////////////////////////////////////////////////
	dialog = new CFileDialog(TRUE,"dcm",NULL, NULL,szFilter);
	if (dialog->DoModal() == IDOK)
	{
		exten = dialog->GetFileExt();
		exten.MakeUpper();
		m_OGLView.LoadOBJModel(dialog->GetPathName());
		m_OGLView.drawScene(FALSE);
		m_OGLView.OnViewResetskeleton();
	}
	delete dialog;
}

void CMainFrame::OnFileOpenweight() 
{
/// Local Variables ///////////////////////////////////////////////////////////
	char BASED_CODE szFilter[] = "Weight File (*.wgt)|*.wgt||";  
	CFileDialog	*dialog;
	CString exten;
///////////////////////////////////////////////////////////////////////////////
	dialog = new CFileDialog(TRUE,"wgt",NULL, NULL,szFilter);
	if (dialog->DoModal() == IDOK)
	{
		exten = dialog->GetFileExt();
		exten.MakeUpper();
		m_OGLView.LoadWeightsTest(dialog->GetPathName());
	}
	delete dialog;
}

void CMainFrame::OnFileSave() 
{
/// Local Variables ///////////////////////////////////////////////////////////
	char BASED_CODE szFilter[] = "Weight File (*.wgt)|*.wgt||";  
	CFileDialog	*dialog;
	CString exten;
///////////////////////////////////////////////////////////////////////////////
	dialog = new CFileDialog(FALSE,"wgt",NULL, NULL,szFilter);
	if (dialog->DoModal() == IDOK)
	{
		exten = dialog->GetFileExt();
		exten.MakeUpper();
		m_OGLView.SaveWeights(dialog->GetPathName());
	}
	delete dialog;
}

void CMainFrame::OnSkeletonResetskeleton() 
{
	// PASS THIS MESSAGE OFF TO THE OGL CLASS
	m_OGLView.OnViewResetskeleton();
}

void CMainFrame::OnViewOutline() 
{
	m_Wireframe = !m_Wireframe;
	if (m_Wireframe)
		glPolygonMode(GL_FRONT,GL_LINE);
	else
		glPolygonMode(GL_FRONT,GL_FILL);
	Invalidate(TRUE );
}

void CMainFrame::OnUpdateViewOutline(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_Wireframe );	
}

void CMainFrame::OnViewViewskeleton() 
{
	m_OGLView.m_DrawSkeleton = !m_OGLView.m_DrawSkeleton;
	m_OGLView.drawScene(FALSE);
}

void CMainFrame::OnUpdateViewViewskeleton(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_OGLView.m_DrawSkeleton );		
}


void CMainFrame::OnViewDrawdeformed() 
{
	m_OGLView.m_DrawDeformed = !m_OGLView.m_DrawDeformed;
	m_OGLView.drawScene(FALSE);
}

void CMainFrame::OnUpdateViewDrawdeformed(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_OGLView.m_DrawDeformed );		
}

void CMainFrame::OnSkeletonSetrestpose() 
{
	m_OGLView.SetBasePose();
}

void CMainFrame::OnSkeletonSetboneweights() 
{
	m_OGLView.WeightBones();
}


void CMainFrame::OnSkeletonClearselectedweights() 
{
	m_OGLView.ClearBoneWeights(&m_OGLView.m_Skeleton);
}

void CMainFrame::OnAnimationLoadanimation()
{
	LoadAnimationTest(0,m_OGLView.m_SelectedBone,&m_OGLView.m_Skeleton,&(m_OGLView.Play_Animation));
}

void CMainFrame::OnAnimationPlay()
{
	m_Animation_Direction = 1;
	m_Animating = TRUE;
	Invalidate(TRUE);
}
/////////////////////////////////////////////////////////////////////////////
// Animation Control Functions

void CMainFrame::OnStop() 
{
	// TODO: Add your command handler code here
	m_Animating = FALSE;
}

void CMainFrame::OnPlayForward() 
{
	// TODO: Add your command handler code here
	m_Animation_Direction = 1;
	m_Animating = TRUE;
			LoadEnabled = 1;
	Invalidate(TRUE);
}

void CMainFrame::OnPlayBack() 
{
	// TODO: Add your command handler code here
	m_Animating = TRUE;
	m_Animation_Direction = -1;
	Invalidate(TRUE);
}

void CMainFrame::OnForwardFrame() 
{
	// TODO: Add your command handler code here
	m_Animating = FALSE;
	BoneAdvanceFrame(&m_Skeleton,1,TRUE);
	m_OGLView.drawScene(FALSE);
}

void CMainFrame::OnBackFrame() 
{
	// TODO: Add your command handler code here
	m_Animating = FALSE;
	BoneAdvanceFrame(&m_Skeleton,-1,TRUE);
	m_OGLView.drawScene(FALSE);
}