///////////////////////////////////////////////////////////////////////////////
//
// LoadAnim.h 
//
// Purpose: Header for the Biovision BVA Loader
//
// Created:
//		JL 9/5/97		
//
///////////////////////////////////////////////////////////////////////////////
//
//	Copyright 1997 Jeff Lander, All Rights Reserved.
//  For educational purposes only.
//  Please do not republish in electronic or print form without permission
//  Thanks - jeffl@darwin3d.com
//
///////////////////////////////////////////////////////////////////////////////

#if !defined(LoadAnim_H__INCLUDED_)
#define LoadAnim_H__INCLUDED_

#define MAX_STRINGLENGTH	255

#include "Skeleton.h"

BOOL LoadAnimation(CString name,t_Bone *m_SelectedBone,t_Bone *rootBone);
//BOOL LoadAnimationTest(CString name,t_Bone *m_SelectedBone,t_Bone *rootBone);
BOOL LoadAnimationTest(int n,t_Bone *m_SelectedBone,t_Bone *rootBone,int *anima);

#endif // !defined(LoadAnim_H__INCLUDED_)