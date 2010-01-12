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

// UniSensView.cpp : implementation of the CUniSensView class
//
#include "stdafx.h"
#include "UniSens.h"
#include "UniSensDoc.h"
#include "UniSensView.h"

#include  <fstream>  
#include  <iostream>  
#include  <stdio.h>  
#include  <stdlib.h>  
#include  <vector>  
#include  <iterator>   
#include  <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using   namespace   std;

/////////////////////////////////////////////////////////////////////////////
// CUniSensView

IMPLEMENT_DYNCREATE(CUniSensView, CView)

BEGIN_MESSAGE_MAP(CUniSensView, CView)
	//{{AFX_MSG_MAP(CUniSensView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
//	ON_COMMAND(ID_RUN_QUAT, OnRunQuat)
//	ON_COMMAND(ID_RUN_PLAY, OnRunPlay)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUniSensView construction/destruction


CUniSensView::CUniSensView()
{
	// TODO: add construction code here
	xrof  = 1.0;
	yrof  = 1.0;
	zrof  = 1.0;
	Q_w = 0.0;
	Q_x = 0.0;
	Q_y = 0.0;
	Q_z = 0.0;

	theta = 0;
	m_Zoom = 0;
	row = 0;
	m_pFr = NULL;
}

CUniSensView::~CUniSensView()
{

}

BOOL CUniSensView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CUniSensView::InitOpenGL()
{
	m_pDC = new CClientDC(this);
	PIXELFORMATDESCRIPTOR pfd;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	GLfloat lightpos[] = { 5.5, 5.5, 9.0, 0.0 };
	int n;

	HGLRC   hrc;
	
	ASSERT(m_pDC != NULL);
	

	if (!bSetPixelFormat())
		return;

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	

	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	hrc = wglCreateContext(m_pDC->GetSafeHdc());
	

	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);		
	

	glClearColor(0.3f,0.3f,0.3f,0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	
	glShadeModel(GL_SMOOTH);
	

	glBlendFunc(GL_SRC_ALPHA,GL_ONE); 	
	glEnable(GL_BLEND); 
	

    glEnable (GL_LINE_SMOOTH);
    glEnable (GL_BLEND);
	
	
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_BLEND);
	
	
    glHint (GL_POLYGON_SMOOTH_HINT|GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	glEnable(GL_DEPTH_TEST);
	LoadTextureGL();													
	glEnable(GL_TEXTURE_2D);											

	glDepthFunc(GL_LEQUAL); 

	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

bool CUniSensView::bSetPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{   
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
			1,  // version number
			PFD_DRAW_TO_WINDOW |// support window
			PFD_SUPPORT_OPENGL |  // support OpenGL  支持OpenGL
			PFD_DOUBLEBUFFER, // double buffered 支持又缓冲
			PFD_TYPE_RGBA,  // RGBA type使用RGBA模式，不用调色板
			24, // 24-bit color depth  使用24位真彩色
			0, 0, 0, 0, 0, 0,   // color bits ignored
			0,  // no alpha buffer
			0,  // shift bit ignored
			0,  // no accumulation buffer
			0, 0, 0, 0, // accum bits ignored
			32, // 32-bit z-buffer   32位Z轴缓冲
			0,  // no stencil buffer
			0,  // no auxiliary buffer
			PFD_MAIN_PLANE, // main layer
			0,  // reserved
			0, 0, 0 // layer masks ignored
	};
	int pixelformat;
	
	//如果可以得到指定的
	if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == FALSE )
	{
		AfxMessageBox("ChoosePixelFormat failed");
		return false;
	}
	
	//用上面取到的格式设置设备环境
	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		AfxMessageBox("SetPixelFormat failed");
		return false;
	}
	return true;

}
/////////////////////////////////////////////////////////////////////////////
// CUniSensView drawing
void CUniSensView::DrawText()
{
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f,1.0f,1.0f);	
	
	wglUseFontBitmaps(wglGetCurrentDC(),0,255,100);
	
	glListBase(100);
	glRasterPos3f(5.20f+m_Zoom,-0.5f,0.0f);
	glCallLists(2,GL_UNSIGNED_BYTE,_T("+x")); 
	
	glRasterPos3f(-0.5,5.2f+m_Zoom,0.0f);
	glCallLists(2,GL_UNSIGNED_BYTE,_T("+y")); 
	
	glRasterPos3f(-0.5,0.0f,5.2f+m_Zoom);
	glCallLists(2,GL_UNSIGNED_BYTE,_T("+z")); 
	
	glRasterPos3f(0.0,0.0f,0.0f);
	glCallLists(6,GL_UNSIGNED_BYTE,_T("Center")); 
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}

void CUniSensView::Draw3DRect()
{
	GLfloat x, y, z;
	GLfloat x_off, y_off, z_off;

	glColor3f(1.0f,1.0f,1.0f);

//	glEnable (GL_LIGHTING);
//	glEnable(GL_LIGHT0);
	glPushMatrix ();                                   

	
	x = 10.4f;
	y = 6.4f;
	z = 4.0f;
	x_off =  0.0;
	y_off =  0.0;
	z_off =  0.0;
	//fore
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f);glVertex3f( x+x_off,  y+y_off, z+z_off); 
		glTexCoord2f(1.0f, 0.0f);glVertex3f( x+x_off, -y+y_off, z+z_off);
		glTexCoord2f(0.0f, 0.0f);glVertex3f(-x+x_off, -y+y_off, z+z_off);
		glTexCoord2f(0.0f, 1.0f);glVertex3f(-x+x_off,  y+y_off, z+z_off);
	glEnd();
   
	//back
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f);glVertex3f( -x+x_off,  y+y_off, -z+z_off);
		glTexCoord2f(1.0f, 0.0f);glVertex3f( -x+x_off, -y+y_off, -z+z_off);
		glTexCoord2f(0.0f, 0.0f);glVertex3f(  x+x_off, -y+y_off, -z+z_off);
		glTexCoord2f(0.0f, 1.0f);glVertex3f(  x+x_off,  y+y_off, -z+z_off);
	glEnd();

	//right
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);glVertex3f( x+x_off, y+y_off, z+z_off);
		glTexCoord2f(1.0f, 1.0f);glVertex3f( x+x_off, y+y_off, -z+z_off);
		glTexCoord2f(1.0f, 0.0f);glVertex3f( x+x_off, -y+y_off, -z+z_off);
		glTexCoord2f(0.0f, 0.0f);glVertex3f( x+x_off, -y+y_off, z+z_off);
	glEnd();

	//left
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f);glVertex3f( -x+x_off, y+y_off, z+z_off);
		glTexCoord2f(0.0f, 1.0f);glVertex3f( -x+x_off, y+y_off, -z+z_off);
		glTexCoord2f(0.0f, 0.0f);glVertex3f( -x+x_off, -y+y_off, -z+z_off);
		glTexCoord2f(1.0f, 0.0f);glVertex3f( -x+x_off, -y+y_off, z+z_off);
	glEnd();

	//buttom
	glBindTexture(GL_TEXTURE_2D,texture[4]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);glVertex3d( -x+x_off, -y+y_off, z+z_off);       
		glTexCoord2f(1.0f, 1.0f);glVertex3d( x+x_off, -y+y_off, z+z_off);
		glTexCoord2f(1.0f, 0.0f);glVertex3d( x+x_off, -y+y_off, -z+z_off);
		glTexCoord2f(0.0f, 0.0f);glVertex3d( -x+x_off, -y+y_off, -z+z_off);
	glEnd();

	//upper 
	glBindTexture(GL_TEXTURE_2D,texture[5]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);glVertex3d( -x+x_off, y+y_off, z+z_off);
		glTexCoord2f(1.0f, 0.0f);glVertex3d( x+x_off, y+y_off, z+z_off);
		glTexCoord2f(1.0f, 1.0f);glVertex3d( x+x_off, y+y_off, -z+z_off);
		glTexCoord2f(0.0f, 1.0f);glVertex3d( -x+x_off, y+y_off, -z+z_off);
	glEnd();


	glPopMatrix ();
}


void CUniSensView::DrawLines()                                          
{
	//Draw Lines
	glBegin(GL_LINES);
	//X
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.0f-m_Zoom,0.0f,0.0f);
	glVertex3f(5.0f+m_Zoom,0.0f,0.0f);
	
	//Arrow
	glVertex3f(4.60f+m_Zoom,0.3f,0.0f);
	glVertex3f(5.0f+m_Zoom,0.0f,0.0f);
	glVertex3f(4.60f+m_Zoom,-0.3f,0.0f);
	glVertex3f(5.0f+m_Zoom,0.0f,0.0f);
	
	glVertex3f(4.60f+m_Zoom,0.0f,0.3f);
	glVertex3f(5.0f+m_Zoom,0.0f,0.0f);
	glVertex3f(4.60f+m_Zoom,0.0f,-0.3f);
	glVertex3f(5.0f+m_Zoom,0.0f,0.0f);	

	//Y
	glColor3f(1.0f,0.0f,0.0f);	
	glVertex3f(0.0f,0.0f-m_Zoom,0.0f);
	glVertex3f(0.0f,5.0f+m_Zoom,0.0f);
	//Arrow
	glVertex3f(-0.3f,4.6f+m_Zoom,0.0f);
	glVertex3f(0.0f,5.0f+m_Zoom,0.0f);
	glVertex3f(0.3f,4.6F+m_Zoom,0.0f);
	glVertex3f(0.0f,5.0f+m_Zoom,0.0f);
	
	glVertex3f(0.0f,4.6f+m_Zoom,-0.3f);
	glVertex3f(0.0f,5.0f+m_Zoom,0.0f);
	glVertex3f(0.0f,4.6F+m_Zoom,0.3f);
	glVertex3f(0.0f,5.0f+m_Zoom,0.0f);
	
	//Z
	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f(0.0f,0.0f,0.0f-m_Zoom);
	glVertex3f(0.0f,0.0f,5.0f+m_Zoom);
	//Arrow
	glVertex3f(0.3f,0.0f,4.60f+m_Zoom);
	glVertex3f(0.0f,0.0f,5.0f+m_Zoom);
	glVertex3f(-0.3f,0.0f,4.60f+m_Zoom);
	glVertex3f(0.0f,0.0f,5.0f+m_Zoom);
	
	glVertex3f(0.0f,0.3f,4.60f+m_Zoom);
	glVertex3f(0.0f,0.0f,5.0f+m_Zoom);
	glVertex3f(0.0f,-0.3f,4.60f+m_Zoom);
	glEnd();
}

void CUniSensView::DrawScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();	
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glTranslatef(-15.0,-10.0, 0.0);
	

	glRotatef(theta * 180 /PI,xrof,yrof,zrof);

	this->DrawLines();
	this->DrawText();
	glEnable(GL_LIGHTING);

	glPopMatrix();

	glPushMatrix();
	glDisable(GL_LIGHTING);
	glTranslatef(0.0f,0.0f,0.0f);

	glRotatef(theta * 180 /PI,xrof,yrof,zrof);

	this->Draw3DRect();
	glEnable(GL_LIGHTING);

	glPopMatrix();

	glFinish();

	SwapBuffers(wglGetCurrentDC());
	
}

void CUniSensView::OnDraw(CDC* pDC)
{
	CUniSensDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	DrawScreen();

	//this->DrawSelArea(pDC);	
}

/////////////////////////////////////////////////////////////////////////////
// CUniSensView diagnostics

#ifdef _DEBUG
void CUniSensView::AssertValid() const
{
	CView::AssertValid();
}

void CUniSensView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUniSensDoc* CUniSensView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUniSensDoc)));
	return (CUniSensDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUniSensView message handlers

int CUniSensView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;	
	return 0;
}

void CUniSensView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(0);
	HGLRC   hrc;
	hrc = ::wglGetCurrentContext();
	::wglMakeCurrent(NULL,  NULL);
	if (hrc)
		::wglDeleteContext(hrc);
}

void CUniSensView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	SetViewPort(cx,cy);
}

void CUniSensView::SetViewPort(int cx, int cy)
{
	this->m_xRect = cx;
	this->m_yRect = cy;

	if(m_yRect==0)
		m_yRect=1;

	glViewport(0,0,m_xRect,m_yRect);
	
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();	
	if(m_xRect>=m_yRect)
	{
		glOrtho(-20.0,20.0,-20.0*(GLfloat)m_yRect/(GLfloat)m_xRect,	20.0*(GLfloat)m_yRect/(GLfloat)m_xRect,-50.0,50.0);
	}
	else
	{
		glOrtho(-20.0*(GLfloat)m_xRect/(GLfloat)m_yRect,20.0*(GLfloat)m_xRect/(GLfloat)m_yRect,-20.0,20.0,-50.0,50.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CUniSensView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	m_pFr = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	this->InitOpenGL();

}

AUX_RGBImageRec *CUniSensView::LoadImage(char *Filename)								
{
	FILE *File = NULL;													

	if (!Filename)														
	{
		return NULL;													
	}

	File = fopen(Filename,"r");											

	if (File)															
	{
		fclose(File);													
		return auxDIBImageLoad(Filename);								
	}

	return NULL;														
}

BOOL CUniSensView::LoadTextureGL(void)												
{
	BOOL State = FALSE;													

	AUX_RGBImageRec *TextureImage[MAXTEXTURE];							

	memset(TextureImage, 0, sizeof(void *) * MAXTEXTURE);				
	 if ((TextureImage[0]=LoadImage("res/front.bmp"))
		&&(TextureImage[1]=LoadImage("res/back.bmp"))
		&&(TextureImage[2]=LoadImage("res/right.bmp"))
		&&(TextureImage[3]=LoadImage("res/left.bmp"))
		&&(TextureImage[4]=LoadImage("res/buttom.bmp"))
		&&(TextureImage[5]=LoadImage("res/top.bmp"))
		)
	{   
		State=TRUE;														
		glGenTextures(MAXTEXTURE, &texture[0]);							

		for (int loop=0; loop<MAXTEXTURE; ++loop)						
		{
			glBindTexture(GL_TEXTURE_2D, texture[loop]);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[loop]->sizeX, TextureImage[loop]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[loop]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
		}
	}
	for (int loop=0; loop<MAXTEXTURE; ++loop)
	{
		if (TextureImage[loop])											
		{
			if (TextureImage[loop]->data)								
			{
				free(TextureImage[loop]->data);							
			}
			free(TextureImage[loop]);									
		}
	}
	return State;														
}

void CUniSensView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_MouseDownPoint = point;
	SetCapture();
	CView::OnLButtonDown(nFlags, point);
}

void CUniSensView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_MouseDownPoint = CPoint(0,0);
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}

void CUniSensView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(GetCapture() == this)
	{
		xrof += (point.y - m_MouseDownPoint.y)/3.6;
		yrof += (point.x - m_MouseDownPoint.x)/3.6;
		InvalidateRect(NULL,FALSE);
		m_MouseDownPoint = point;
	}
	CView::OnMouseMove(nFlags, point);
}


//void CUniSensView::OnRunPlay() 
//{
//	CUniSensDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//	// TODO: Add your command handler code here
//	frames = pDoc->fileSize;
//	v      = &(pDoc->fileData);
//	SetTimer(0, 1, NULL);
//}
void CUniSensView::PlayFromFile()
{
	CUniSensDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: Add your command handler code here
	frames = pDoc->fileSize;
	v      = &(pDoc->fileData);
	SetTimer(0, 20, NULL);
}
void CUniSensView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//double normal, scale;
	double Quat_w,Quat_x,Quat_y,Quat_z;
	if(row < frames)
	{
		Quat_w = v->at(row).at(9); Quat_x = v->at(row).at(10); Quat_y = v->at(row).at(11); Quat_z = v->at(row).at(12);
		row++;
		SetQuaternion(Quat_w,Quat_x,Quat_y,Quat_z);
	}
	
	CView::OnTimer(nIDEvent);

	// Eat spurious WM_TIMER messages
	MSG msg;
	while(::PeekMessage(&msg, m_hWnd, WM_TIMER, WM_TIMER, PM_REMOVE));
}
void CUniSensView::SetQuaternion(double Quat_w, double Quat_x, double Quat_y, double Quat_z, int i)
{
		
		Q_w += Quat_w;
		Q_x += Quat_x;
		Q_y += Quat_y;
		Q_z += Quat_z;

		if(i % 5 == 0)
		{
			double normal, scale;
			Q_w/=5;Q_x/=5;Q_y/=5;Q_z/=5;
			normal  = sqrt(Q_x*Q_x  +  Q_y*Q_y  +  Q_z*Q_z  +  Q_w*Q_w);
			Q_w/=normal;Q_x/=normal;Q_y/=normal;Q_z/=normal;
			
			if (Quat_w > 1.0)
			{
				normal = sqrt(Q_x*Q_x  +  Q_y*Q_y  +  Q_z*Q_z  +  Q_w*Q_w);
				Q_x = Q_x/normal;
				Q_y = Q_y/normal;
				Q_z = Q_z/normal;
				Q_w = Q_w/normal;
			}
			theta  = 2 * acos(Q_w);
			scale = sqrt(1 - Q_w*Q_w);
			if (scale > 0.01)
			{
				xrof = Q_x/scale;
				yrof = Q_y/scale;
				zrof = Q_z/scale;
			}
			else
			{
				xrof = Q_x;
				yrof = Q_y;
				zrof = Q_z;
			}
			InvalidateRect(NULL,FALSE);
			Q_w = 0.0;Q_x = 0.0;Q_y = 0.0;Q_z = 0.0;
		}
		
}
void CUniSensView::SetQuaternion(double Quat_w, double Quat_x, double Quat_y, double Quat_z)
{

			double normal, scale;
			if (Quat_w > 1.0)
			{
				normal = sqrt(Quat_x*Quat_x  +  Quat_y*Quat_y  +  Quat_z*Quat_z  +  Quat_w*Quat_w);
				Quat_x = Quat_x/normal;
				Quat_y = Quat_y/normal;
				Quat_z = Quat_z/normal;
				Quat_w = Quat_w/normal;
			}
			theta  = 2 * acos(Quat_w);
			scale = sqrt(1 - Quat_w*Quat_w);
			if (scale > 0.01)
			{
				xrof = Quat_x/scale;
				yrof = Quat_y/scale;
				zrof = Quat_z/scale;
			}
			else
			{
				xrof = Quat_x;
				yrof = Quat_y;
				zrof = Quat_z;
			}
			
			InvalidateRect(NULL,FALSE);
}