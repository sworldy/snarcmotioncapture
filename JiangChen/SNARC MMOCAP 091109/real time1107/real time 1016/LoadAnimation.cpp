///////////////////////////////////////////////////////////////////////////////
//
// LoadAnim.cpp 
//
// Purpose: implementation of the Biovision BVA Loaded
//
// Created:
//		JL 9/5/97		
//
// Todo:
//		I WILL PROBABLY PULL OUT THE CHANNELS INTO A NEW STRUCT
//		ADD SPEED OF PLAYBACK VARIABLE TO CHANNEL STRUCT
//
///////////////////////////////////////////////////////////////////////////////
//
//	Copyright 1997 Jeff Lander, All Rights Reserved.
//  For educational purposes only.
//  Please do not republish in electronic or print form without permission
//  Thanks - jeffl@darwin3d.com
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LoadAnimation.h"
#include "LoadOBJ.h"
#include "EulerAngles.h"
#include "QuatTypes.h"
#include <math.h>;


#include <cstring>
#include <fstream>
using namespace std;
int number = 5;
HANDLE PipeHandle[17];
char* PipeName[] = {"\\\\.\\Pipe\\sensor0","\\\\.\\Pipe\\sensor1","\\\\.\\Pipe\\sensor2","\\\\.\\Pipe\\sensor3",
"\\\\.\\Pipe\\sensor4","\\\\.\\Pipe\\sensor5","\\\\.\\Pipe\\sensor6","\\\\.\\Pipe\\sensor7",
"\\\\.\\Pipe\\sensor8","\\\\.\\Pipe\\sensor9","\\\\.\\Pipe\\sensor10","\\\\.\\Pipe\\sensor11",
"\\\\.\\Pipe\\sensor12","\\\\.\\Pipe\\sensor13","\\\\.\\Pipe\\sensor14","\\\\.\\Pipe\\sensor15",
"\\\\.\\Pipe\\sensor16"};

///////////////////////////////////////////////////////////////////////////////
// Function:	ParseString
// Purpose:		Actually breaks a string of words into individual pieces
// Arguments:	Source string in, array to put the words and the count


//////////////////////////////////////////////////////////
////////////打开管道//////////////////////////////////////
BOOL LoadAnimationTest(int n,t_Bone *m_SelectedBone,t_Bone *rootBone,int *anima)
{ 
	*anima = 1;
	for( int i=0; i<16; ++i)
	{
		if (WaitNamedPipe(PipeName[i], NMPWAIT_WAIT_FOREVER) == 0)
		{
			CString error;
			error.Format("Pipe %d connecting failed\n",i);
			AfxMessageBox(error);
			//		return;
		}
	}
	if (WaitNamedPipe(PipeName[16], NMPWAIT_WAIT_FOREVER) == 0)
		{
			CString error;
			error.Format("Pipe %d connecting failed\n",16);
			AfxMessageBox(error);
			//		return;
		}
	for(int  i=0; i<16; ++i)
	{
		if ((PipeHandle[i] = CreateFile(PipeName[i],GENERIC_READ,
			0,(LPSECURITY_ATTRIBUTES) NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL)) == INVALID_HANDLE_VALUE)
		{
			CString error;
			error.Format("Pipe %d failed with error %d\n", i,GetLastError());
			AfxMessageBox(error);
			//		return;
		}
		Sleep(1500);
	}
	if ((PipeHandle[16] = CreateFile(PipeName[16],GENERIC_READ,
			0,(LPSECURITY_ATTRIBUTES) NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL)) == INVALID_HANDLE_VALUE)
		{
			CString error;
			error.Format("Pipe %d failed with error %d\n", 16,GetLastError());
			AfxMessageBox(error);
			//		return;
		}
	AfxMessageBox("All the pipes has been connected!");
	return 0;
}


ofstream file = ofstream("1.txt");
//读取根节点数据///////////////////////////////////////////////
void BoneAdvanceFrameTest0(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[56];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
				bone->axisangle.x = -p[5]*10;
			    bone->axisangle.y = p[4]*10;
		     	bone->axisangle.z = -p[6]*10;
			    bone->axisangle.theta = p[3];
                bone->axisangle.ax = p[2];
			    bone->axisangle.ay = p[0];
			    bone->axisangle.az = p[1];
				/*if (bone->axisangle.ay > 170)
				{
					bone->axisangle.ay = 170;
				}
				if (bone->axisangle.ay < -170)
				{
					bone->axisangle.ay = -170;
				}*/
				//bone->axisangle.x = 0;
				//bone->axisangle.y = 0;
			    //bone->axisangle.z = 0;
				
				//bone->axisangle.theta = 0;
				//bone->axisangle.ax = 0;
				//bone->axisangle.ay = 0;
				//bone->axisangle.az = 0;
              // file << p[4] << "\t"<< p[5] << "\t"<< p[6] << "\t"<< bone->axisangle.ax << "\t"<< bone->axisangle.ay << "\t"<< bone->axisangle.az <<"\n";
			}
		}
}

//读取脊柱1数据///////////////////////////////////////////////
void BoneAdvanceFrameTest1(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
               if(abs(bone->parent->axisangle.ay) > 90  )
			   {
				   bone->axisangle.theta = p[3];
				   bone->axisangle.ax = -p[0];
				   //bone->axisangle.ay = p[2];
				   //bone->axisangle.az = p[1];
				   bone->axisangle.ay = 0.0;
				   bone->axisangle.az = 0.0;
			   }
			   else{
                bone->axisangle.theta = p[3];
			    bone->axisangle.ax = p[0];
			    bone->axisangle.ay = -p[2];
			    bone->axisangle.az = p[1];
			    }
			}
		}
}

//读取脊柱2数据///////////////////////////////////////////////
void BoneAdvanceFrameTest2(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
				if(abs(bone->parent->parent->axisangle.ay) > 90  )
				{
					bone->axisangle.theta = bone->parent->parent->axisangle.ay*PI/180;
					bone->axisangle.ax = 1.0;
					//bone->axisangle.ay = p[2];
					//bone->axisangle.az = p[1];
					bone->axisangle.ay = 0.0;
					bone->axisangle.az = 0.0;
				}
				
				else {
				bone->axisangle.theta = p[0];
			    bone->axisangle.ax = p[1];
			    bone->axisangle.ay = -p[3];
			    bone->axisangle.az = p[2];
				/*bone->axisangle.theta = 0;
			    bone->axisangle.ax = -0;
			    bone->axisangle.ay = 0;
			    bone->axisangle.az = 0;*/
				}
			   

			}
		}
}

//读取脊柱3数据///////////////////////////////////////////////
void BoneAdvanceFrameTest3(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
				if(abs(bone->parent->parent->parent->axisangle.ay) > 90  )
				{
					bone->axisangle.theta = bone->parent->parent->parent->axisangle.ay*PI/180;
					bone->axisangle.ax = 1.0;
					//bone->axisangle.ay = p[2];
					//bone->axisangle.az = p[1];
					bone->axisangle.ay = 0.0;
					bone->axisangle.az = 0.0;
				}
				else{
					bone->axisangle.theta = p[0];
					bone->axisangle.ax = p[1];
					bone->axisangle.ay = -p[3];
					bone->axisangle.az = p[2];
					/*bone->axisangle.theta = 0;
					bone->axisangle.ax = -0;
					bone->axisangle.ay = 0;
					bone->axisangle.az = 0;*/
				}
				

			}
		}
}

//读取左肩数据///////////////////////////////////////////////
void BoneAdvanceFrameTest4(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
				if(abs(bone->parent->parent->parent->parent->parent->axisangle.ay) > 90  )
				{
					bone->axisangle.theta = PI/2;
					bone->axisangle.ax = 0.0;
					bone->axisangle.ay = 1.0;
					bone->axisangle.az = 0.0;
				}
				else{
					bone->axisangle.theta = p[0];
					bone->axisangle.ax = p[1];
					bone->axisangle.ay = p[2];
					bone->axisangle.az = p[3];

				}

			}
		}
}

//读取左肘关节数据///////////////////////////////////////////////
void BoneAdvanceFrameTest5(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
				if(abs(bone->parent->parent->parent->parent->parent->parent->axisangle.ay) > 90  )
				{
					bone->axisangle.theta = PI/2;
					bone->axisangle.ax = 0.0;
					bone->axisangle.ay = 1.0;
					bone->axisangle.az = 0.0;
				}
				else{
                bone->axisangle.theta = p[0];
			    bone->axisangle.ax = p[1];
			    bone->axisangle.ay = p[2];
			    bone->axisangle.az = p[3];
				}
			}
		}
}

//读取左手数据///////////////////////////////////////////////
void BoneAdvanceFrameTest6(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
				if(abs(bone->parent->parent->parent->parent->parent->parent->parent->axisangle.ay) > 90  )
				{
					bone->axisangle.theta = PI/2;
					bone->axisangle.ax = 0.0;
					bone->axisangle.ay = 1.0;
					bone->axisangle.az = 0.0;
				}
				else{
				bone->axisangle.theta = p[0];
			    bone->axisangle.ax = p[1];
			    bone->axisangle.ay = p[2];
			    bone->axisangle.az = p[3];
				/*bone->axisangle.theta = 0;
			    bone->axisangle.ax = 0;
			    bone->axisangle.ay = 0;
			    bone->axisangle.az = 0;*/
				}
			}
		}
}

//读取右肩数据///////////////////////////////////////////////
void BoneAdvanceFrameTest7(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
				if (abs(bone->parent->parent->parent->parent->parent->axisangle.ay) > 90)
			{
				bone->axisangle.theta = -PI/2;
				bone->axisangle.ax = 0.0;
				bone->axisangle.ay = 1.0;
				bone->axisangle.az = 0.0;
			}
			else
			{
                bone->axisangle.theta = p[0];
			    bone->axisangle.ax = p[1];
			    bone->axisangle.ay = -p[2];
			    bone->axisangle.az = -p[3];

			}
			}
		}
}

//读取右肘关节数据///////////////////////////////////////////////
void BoneAdvanceFrameTest8(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
				if(abs(bone->parent->parent->parent->parent->parent->parent->axisangle.ay) > 90  )
				{
					bone->axisangle.theta = -PI/2;
					bone->axisangle.ax = 0.0;
					bone->axisangle.ay = 1.0;
					bone->axisangle.az = 0.0;
				}
				else{
				bone->axisangle.theta = p[0];
			    bone->axisangle.ax = p[1];
			    bone->axisangle.ay = -p[2];
			    bone->axisangle.az = -p[3];
				}

			}
		}
}

//读取右手数据///////////////////////////////////////////////
void BoneAdvanceFrameTest9(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
               
				if(abs(bone->parent->parent->parent->parent->parent->parent->parent->axisangle.ay) > 90  )
				{
					bone->axisangle.theta = -PI/2;
					bone->axisangle.ax = 0.0;
					bone->axisangle.ay = 1.0;
					bone->axisangle.az = 0.0;
				}
				else{
				bone->axisangle.theta = p[0];
			    bone->axisangle.ax = p[1];
			    bone->axisangle.ay = -p[2];
			    bone->axisangle.az = -p[3];
				/*bone->axisangle.theta = 0;
			    bone->axisangle.ax = 0;
			    bone->axisangle.ay = 0;
			    bone->axisangle.az = 0;*/
				}
			}
		}
}
//读取头数据///////////////////////////////////////////////
void BoneAdvanceFrameTest10(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
				if(abs(bone->parent->parent->parent->parent->parent->axisangle.ay) > 90  )
				{
					bone->axisangle.theta = bone->parent->parent->parent->parent->parent->axisangle.ay*PI/180;
					bone->axisangle.ax = 1.0;
					//bone->axisangle.ay = p[2];
					//bone->axisangle.az = p[1];
					bone->axisangle.ay = 0.0;
					bone->axisangle.az = 0.0;
				}
				else{
					bone->axisangle.theta = p[0];
					bone->axisangle.ax = p[3];
					bone->axisangle.ay = p[1];
					bone->axisangle.az = p[2];
					/*bone->axisangle.theta = 0;
					bone->axisangle.ax = 0;
					bone->axisangle.ay = 0;
					bone->axisangle.az = 0;*/

				}

               
			}
		}
}
//读取左大腿数据///////////////////////////////////////////////
void BoneAdvanceFrameTest11(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
                if(abs(bone->parent->axisangle.ay) > 90 )
				{
					bone->axisangle.theta = p[3];
			        bone->axisangle.ax = -p[0];
			        //bone->axisangle.ay = p[1];
                    bone->axisangle.ay = 0.0;
			        bone->axisangle.az = -p[2];
				}
				else
				{
                    bone->axisangle.theta = p[3];
			        bone->axisangle.ax = -p[0];
			        bone->axisangle.ay = -p[1];
			        bone->axisangle.az = p[2];
				}
			}
		}
}
//读取左小腿数据///////////////////////////////////////////////
void BoneAdvanceFrameTest12(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
                 if(abs(bone->parent->parent->axisangle.ay) > 90 )
				{
					bone->axisangle.theta = p[3];
			        bone->axisangle.ax = -p[0];
			        bone->axisangle.ay = p[1];
			        bone->axisangle.az = -p[2];
				}
				else
				{
                    bone->axisangle.theta = p[3];
			        bone->axisangle.ax = -p[0];
			        bone->axisangle.ay = -p[1];
			        bone->axisangle.az = p[2];
				}

			}
		}
}
//读取左脚数据///////////////////////////////////////////////
void BoneAdvanceFrameTest13(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
                //bone->axisangle.theta = p[3];
			    //bone->axisangle.ax = -p[0];
			    //bone->axisangle.ay = -p[2];
			    //bone->axisangle.az = -p[1];
				bone->axisangle.theta = 0;
			    bone->axisangle.ax = 0;
			    bone->axisangle.ay = 0;
			    bone->axisangle.az = 0;

			}
		}
}
//读取右大腿数据///////////////////////////////////////////////
void BoneAdvanceFrameTest14(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
                if(abs(bone->parent->axisangle.ay) > 90  )
				{
					bone->axisangle.theta = p[3];
			        bone->axisangle.ax = -p[0];
			       // bone->axisangle.ay = p[1];
					 bone->axisangle.ay = 0.0;
			        bone->axisangle.az = p[2];
				}
				else
				{
				bone->axisangle.theta = p[3];
			    bone->axisangle.ax = p[0];
			    bone->axisangle.ay = -p[1];
			    bone->axisangle.az = -p[2];
				}

			}
		}
}
//读取右小腿数据///////////////////////////////////////////////
void BoneAdvanceFrameTest15(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
                if(abs(bone->parent->parent->axisangle.ay) > 90 )
				{
			    bone->axisangle.theta = p[3];
			    bone->axisangle.ax = -p[0];
			    bone->axisangle.ay = p[1];
			    bone->axisangle.az = p[2];
				}
				else
				{
				bone->axisangle.theta = p[3];
			    bone->axisangle.ax = p[0];
			    bone->axisangle.ay = -p[1];
			    bone->axisangle.az = -p[2];
				}

			}
		}
}
//读取右脚数据///////////////////////////////////////////////
void BoneAdvanceFrameTest16(t_Bone *bone,int num,BOOL doChildren)
{   
	char buffer[32];
	CString temp;
	unsigned long BytesRead;
	double *p;
	for(int i=0;i<number;++i)
		{
			if (ReadFile(PipeHandle[num], buffer, sizeof(buffer), &BytesRead,  NULL) <= 0)
			{
				CString error;
				error.Format("Read pipe%d failed with error %d\n", num, GetLastError());
				AfxMessageBox(error);
			}
			p = reinterpret_cast<double*>(buffer);
			if((i%number) == 0)
			{
                //bone->axisangle.theta = p[3];
			    //bone->axisangle.ax = -p[0];
			    //bone->axisangle.ay = -p[2];
			    //bone->axisangle.az = -p[1];
				bone->axisangle.theta = 0;
			    bone->axisangle.ax = 0;
			    bone->axisangle.ay = 0;
			    bone->axisangle.az = 0;

			}
		}
}
