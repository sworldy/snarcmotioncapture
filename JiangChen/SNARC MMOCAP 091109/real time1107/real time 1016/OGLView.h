///////////////////////////////////////////////////////////////////////////////
//
// OGLView.h : class definition file
//
// Purpose:	Implementation of OpenGL Window of Hierarchical Animation System
//
// Created:
//		JL 9/12/99
//
///////////////////////////////////////////////////////////////////////////////
//
//	Copyright 1999 Jeff Lander, All Rights Reserved.
//  For educational purposes only.
//  Please do not republish in electronic or print form without permission
//  Thanks - jeffl@darwin3d.com
//
///////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGLVIEW_H__2AB46761_27CD_11D1_83A0_004005308EB5__INCLUDED_)
#define AFX_OGLVIEW_H__2AB46761_27CD_11D1_83A0_004005308EB5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OGLView.h : header file
//

#include <GL/gl.h>
#include <GL/glu.h>

#include "Skeleton.h"
#include "LoadOBJ.h"
/////////////////////////////////////////////////////////////////////////////
// COGLView window

class COGLView : public CWnd
{
// Construction
public:
	COGLView();

// Attributes
public:
	HDC m_hDC;
	HGLRC m_hRC;
	CPoint m_mousepos;
	float m_Grab_Rot_X,m_Grab_Rot_Y,m_Grab_Rot_Z;
	float m_Grab_Trans_X,m_Grab_Trans_Y,m_Grab_Trans_Z;
	CStatusBar  *m_StatusBar;
	CRect   m_SelectRect;
	int		m_ScreenWidth, m_ScreenHeight;
	BOOL	m_DrawSkeleton;
	BOOL	m_DrawDeformed;
	int		m_PickX, m_PickY;
	int		m_BoneCnt;
	t_Bone		m_Camera,*m_SelectedBone;	// For the Camera and Pointer to Current Bone
	t_Bone		m_Skeleton;					// Storage for the Skeletal system
	t_Visual	m_Model;					// Actual Model to be deformed
//	t_faceIndex *m_face;
    int counter;
    FILE *freadout2;// add by wz
	int *cur_index;
	int *recod_ver_id;

	int Play_Animation;
	char animboneName0[80];
	char animboneName1[80];
	char animboneName2[80];
    char animboneName3[80];
	char animboneName4[80];
	char animboneName5[80];
	char animboneName6[80];
	char animboneName7[80];
	char animboneName8[80];
	char animboneName9[80];
	char animboneName10[80];
	char animboneName11[80];
	char animboneName12[80];
	char animboneName13[80];
	char animboneName14[80];
	char animboneName15[80];
	char animboneName16[80];

// Operations
public:
	BOOL	SetupPixelFormat(HDC hdc);
	void	LoadActor(CString filename);
	GLvoid	drawModel(t_Visual *model);
    GLvoid	drawModelTest(t_Visual *model);
	GLvoid  drawSkeleton(t_Bone *rootBone);

	GLvoid	drawScene(BOOL drawSelectRect);
	GLvoid	initializeGL(GLsizei width, GLsizei height);
	GLvoid	resize( GLsizei width, GLsizei height );
	void	CompareBuffer(GLint size, GLfloat *buffer,BOOL select);
	void	SelectVertices(BOOL select);
	void	GetGLInfo();
	void	HandleKeyUp(UINT nChar);
	void	HandleKeyDown(UINT nChar);
	void	OnViewResetskeleton();
	void	SetSkeletonList(t_Bone *skeleton);
	GLvoid	GetSkeletonMat(t_Bone *rootBone);
	GLvoid  GetBaseSkeletonMat(t_Bone *rootBone);
	GLvoid	WeightBones();
	GLvoid	DeformVertices(t_Bone *rootBone,t_Visual *visual);
	void	IterateBoneWeights(t_Bone *skeleton, BOOL read, FILE *fp) ;
	void    LoadBoneWeights(t_Bone *skeleton, BOOL read, FILE *fp,char* name); 
	void	ClearBoneWeights(t_Bone *skeleton);
	BOOL	LoadWeights(CString name);
	BOOL    LoadWeightsTest(CString name);
	BOOL	SaveWeights(CString name);
	BOOL	LoadOBJModel(CString name);
	BOOL	LoadSkeletonFile(CString name);
	void	SetBasePose();

    void    RenderStockScene();
	BOOL    IsThisBone(t_Bone *curbone,char* thename);

    int statThreeandFour(t_Visual *model,t_faceIndex *face,int *cur_index);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COGLView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COGLView();

	// Generated message map functions
protected:
	void UpdateStatusBar(int mode);
	void UpdateStatusBarFrameInfo();
	//{{AFX_MSG(COGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OGLVIEW_H__2AB46761_27CD_11D1_83A0_004005308EB5__INCLUDED_)