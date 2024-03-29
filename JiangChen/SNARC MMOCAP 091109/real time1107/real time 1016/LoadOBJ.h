///////////////////////////////////////////////////////////////////////////////
//
// LoadOBJ.h : header file
//
// Purpose:	Header of OpenGL Window of OBJ Loader
//
// Created:
//		JL 9/23/98		
//
///////////////////////////////////////////////////////////////////////////////
//
//	Copyright 1998 Jeff Lander, All Rights Reserved.
//  For educational purposes only.
//  Please do not republish in electronic or print form without permission
//  Thanks - jeffl@darwin3d.com
//
///////////////////////////////////////////////////////////////////////////////
#if !defined(LoadOBJ_H__INCLUDED_)
#define LoadOBJ_H__INCLUDED_

#define MAX_STRINGLENGTH	255

#define FACE_TYPE_TRI		1
#define FACE_TYPE_QUAD		2
#define FACE_TYPE_NORMAL	4
#define FACE_TYPE_TEXTURE	8
#define FACE_TYPE_TRI_OR_QUAD 16

// MODIFIED FROM THE DECEMBER CODE TO HANDLE QUADS AND TRIS
struct t_faceIndex
{
	long	v[4],n[4],t[4];
	int		flags;				// FACE TYPES
	int     ver_count; //add by wz
} ;

#include "Skeleton.h"

BOOL LoadOBJ(const char *filename,t_Visual *visual);
void ParseString(char *buffer,CStringArray *words,int *cnt);

BOOL LoadOBJTest(const char *filename,t_Visual *visual,int *cur_index,int *counter,int *recod_ver_id);

#endif // !defined(LoadOBJ_H__INCLUDED_)
