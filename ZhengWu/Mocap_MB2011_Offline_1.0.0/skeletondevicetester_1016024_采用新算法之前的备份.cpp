// TCP/IP server : test for ordeviceskeleton
//

#include <stdio.h>
#include <iostream>
#include <vector>
#define WS_VERSION_REQUIRED 0x0101
#include <winsock.h>
#include <windows.h>
#include <fstream>
//--- SDK include
#include <fbsdk/fbsdk.h>

#include "DeviceBuffer.h"


using namespace std;
ifstream quadfromfile1,quadfromfile2,quadfromfile3,quadfromfile4,quadfromfile5,quadfromfile6;
vector<vector<float>> QLeftLeg,QLeftUpperLeg,QLeftFoot,QRightLeg,QRightUpperLeg,QRightFoot;
int Cnt = 0;
bool iFlag = FALSE;
bool qFlag = FALSE;
bool Cleanup()
{
    if (WSACleanup()) 
	{
//         GetErrorStr();
        WSACleanup();
        
        return false;
    }
    return true;
}
bool Initialize()
{
    WSADATA wsadata;
    if (WSAStartup(WS_VERSION_REQUIRED, &wsadata)) 
	{
//          GetErrorStr();
        Cleanup();
        return false;
    }
    return true;
}

void bzero(char *b, size_t length)
{
    memset( b,0,length );
}

int StartServer(int pPort)
{
    int lSocket;
    struct protoent* lP;
    struct sockaddr_in  lSin;

    Initialize();

    lP = getprotobyname("tcp");
    
    lSocket = socket(AF_INET, SOCK_STREAM, lP->p_proto);
    
    if (lSocket)
    {
        bzero((char *)&lSin, sizeof(lSin));

		lSin.sin_family = AF_INET;
		lSin.sin_port = htons(pPort);
		lSin.sin_addr.s_addr = 0L;

		//Bind socket
		if( bind(lSocket, (struct sockaddr*)&lSin, sizeof(lSin)) < 0 )
		{
			return 0;
		}

		if( listen(lSocket, 5) < 0 )
		{
			return 0;
		}
	}

	return lSocket;
}

nsTime GetNanoSeconds()
{
	static double dmUnits = 0.0;

	LARGE_INTEGER	t;

	if(QueryPerformanceCounter(&t)) 
	{
		double	count;
		count = (double) t.QuadPart;
		if (!dmUnits) 
		{
			QueryPerformanceFrequency(&t);
			dmUnits = 1000000000.0 / (double) t.QuadPart;
		}
		return (unsigned __int64) (count * dmUnits);
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////
void Quaternion2Matrix(FBQuaternion &pvector,FBMatrix &pmatrix)
{
	double x2,y2,z2,xy,yz,xz,xw,yw,zw;
	x2 = pvector.mValue[0] * pvector.mValue[0] * 2;
	y2 = pvector.mValue[1] * pvector.mValue[1] * 2;
	z2 = pvector.mValue[2] * pvector.mValue[2] * 2;
	xy = pvector.mValue[0] * pvector.mValue[1] * 2;
	yz = pvector.mValue[2] * pvector.mValue[1] * 2;
	xz = pvector.mValue[0] * pvector.mValue[2] * 2;
	xw = pvector.mValue[0] * pvector.mValue[3] * 2;
	yw = pvector.mValue[1] * pvector.mValue[3] * 2;
	zw = pvector.mValue[2] * pvector.mValue[3] * 2;
//////////////////////////////////////////////////////////////////////////
	pmatrix(0,0) = 1 - y2 - z2;
	pmatrix(0,1) = xy + zw;
	pmatrix(0,2) = xz - yw;
	pmatrix(0,3) = 0;
	pmatrix(1,0) = xy - zw;
	pmatrix(1,1) = 1 - x2 - z2;
	pmatrix(1,2) = yz + xw;
	pmatrix(1,3) = 0;
	pmatrix(2,0) = xz + yw;
	pmatrix(2,1) = yz - xw;
	pmatrix(2,2) = 1 - x2 - y2;
	pmatrix(2,3) = 0;
	pmatrix(3,0) = 0;
	pmatrix(3,1) = 0;
	pmatrix(3,2) = 0;
	pmatrix(3,3) = 1;
//////////////////////////////////////////////////////////////////////////
	//pmatrix(0,0) = 1 - y2 - z2;
	//pmatrix(0,1) = xy - zw;
	//pmatrix(0,2) = xz + yw;
	//pmatrix(0,3) = 0;
	//pmatrix(1,0) = xy + zw;
	//pmatrix(1,1) = 1 - x2 - z2;
	//pmatrix(1,2) = yz - xw;
	//pmatrix(1,3) = 0;
	//pmatrix(2,0) = xz - yw;
	//pmatrix(2,1) = yz + xw;
	//pmatrix(2,2) = 1 - x2 - y2;
	//pmatrix(2,3) = 0;
	//pmatrix(3,0) = 0;
	//pmatrix(3,1) = 0;
	//pmatrix(3,2) = 0;
	//pmatrix(3,3) = 1;
//////////////////////////////////////////////////////////////////////////

}

void MatLocalRotation(FBMatrix prefPointMat,FBMatrix &pTransPointMat,FBMatrix TransMat)
{
	FBMatrix tmpMat;
	FBGetLocalMatrix(tmpMat,prefPointMat,pTransPointMat);
	FBMatrixMult(tmpMat,TransMat,tmpMat);
	FBGetGlobalMatrix(pTransPointMat,prefPointMat,tmpMat);
}

void LocalRotation(sDataBuffer& mDataBuffer,int parent,int child,FBMatrix TransMat)
{
	FBMatrix parentMat,childMat;
	//Generate parentMat
	FBRotationToMatrix(parentMat,mDataBuffer.mChannel[parent].mR);
	memcpy(&parentMat[12], mDataBuffer.mChannel[parent].mT,sizeof(mDataBuffer.mChannel[parent].mT));
	//Generate childMat
	FBRotationToMatrix(parentMat,mDataBuffer.mChannel[child].mR);
	memcpy(&parentMat[12], mDataBuffer.mChannel[child].mT,sizeof(mDataBuffer.mChannel[parent].mT));
	//Calculate Mat
	MatLocalRotation(parentMat,childMat,TransMat);
	//Update Translation
	memcpy(&mDataBuffer.mChannel[child].mT,&childMat[12],sizeof(mDataBuffer.mChannel[child].mT));

}
//////////////////////////////////////////////////////////////////////////
void UpdateBuffer(sDataBuffer& mDataBuffer)
{
	// Read from files
	/////////////////////////////////////////////
	size_t dataLen = QLeftUpperLeg.size();
	FBMatrix lTransformation, lChannelTransformation;
	FBRVector quadVector,vqLeftFoot,vqLeftLeg,vqLeftUpperLeg,vqRightFoot,vqRightLeg,vqRightUpperLeg;
	FBQuaternion mbquad1,mbquad2,mbquad3,mbquad4,mbquad5,mbquad6;
	double lcl25to24[3],lcl26to25[3],lcl26to24[3];
	double lcl18to17[3],lcl19to18[3],lcl19to17[3];
	for (int i = 0; i <3; i++)
	{
		lcl25to24[i] = mDataBuffer.mChannel[25].mT[i] - mDataBuffer.mChannel[24].mT[i];
		lcl26to25[i] = mDataBuffer.mChannel[26].mT[i] - mDataBuffer.mChannel[25].mT[i];
		lcl26to24[i] = mDataBuffer.mChannel[26].mT[i] - mDataBuffer.mChannel[24].mT[i];
		lcl18to17[i] = mDataBuffer.mChannel[18].mT[i] - mDataBuffer.mChannel[17].mT[i];
		lcl19to18[i] = mDataBuffer.mChannel[19].mT[i] - mDataBuffer.mChannel[18].mT[i];
		lcl19to17[i] = mDataBuffer.mChannel[19].mT[i] - mDataBuffer.mChannel[17].mT[i];

	}
	if(Cnt == dataLen) Cnt = 0;
	mbquad1.mValue[0] = QLeftUpperLeg.at(Cnt).at(0);
	mbquad1.mValue[1] = QLeftUpperLeg.at(Cnt).at(1);
	mbquad1.mValue[2] = QLeftUpperLeg.at(Cnt).at(2);
	mbquad1.mValue[3] = QLeftUpperLeg.at(Cnt).at(3);
	FBQuaternionToRotation(vqLeftUpperLeg,mbquad1);
	mbquad2.mValue[0] = QLeftLeg.at(Cnt).at(0);
	mbquad2.mValue[1] = QLeftLeg.at(Cnt).at(1);
	mbquad2.mValue[2] = QLeftLeg.at(Cnt).at(2);
	mbquad2.mValue[3] = QLeftLeg.at(Cnt).at(3);
	FBQuaternionToRotation(vqLeftLeg,mbquad2);
	mbquad3.mValue[0] = QLeftFoot.at(Cnt).at(0);
	mbquad3.mValue[1] = QLeftFoot.at(Cnt).at(1);
	mbquad3.mValue[2] = QLeftFoot.at(Cnt).at(2);
	mbquad3.mValue[3] = QLeftFoot.at(Cnt).at(3);
	FBQuaternionToRotation(vqLeftFoot,mbquad3);
	mbquad4.mValue[0] = QRightFoot.at(Cnt).at(0);
	mbquad4.mValue[1] = QRightFoot.at(Cnt).at(1);
	mbquad4.mValue[2] = QRightFoot.at(Cnt).at(2);
	mbquad4.mValue[3] = QRightFoot.at(Cnt).at(3);
	FBQuaternionToRotation(vqRightFoot,mbquad4);
	mbquad5.mValue[0] = QRightLeg.at(Cnt).at(0);
	mbquad5.mValue[1] = QRightLeg.at(Cnt).at(1);
	mbquad5.mValue[2] = QRightLeg.at(Cnt).at(2);
	mbquad5.mValue[3] = QRightLeg.at(Cnt).at(3);
	FBQuaternionToRotation(vqRightLeg,mbquad5);
	mbquad6.mValue[0] = QRightUpperLeg.at(Cnt).at(0);
	mbquad6.mValue[1] = QRightUpperLeg.at(Cnt).at(1);
	mbquad6.mValue[2] = QRightUpperLeg.at(Cnt).at(2);
	mbquad6.mValue[3] = QRightUpperLeg.at(Cnt++).at(3);
	FBQuaternionToRotation(vqRightUpperLeg,mbquad6);

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
	//mDataBuffer.mChannel[24].mR[0] = vqLeftUpperLeg.mValue[0];
	//mDataBuffer.mChannel[24].mR[1] = vqLeftUpperLeg.mValue[1];
	//mDataBuffer.mChannel[24].mR[2] = vqLeftUpperLeg.mValue[2];
	//mDataBuffer.mChannel[2].mR[0] = vqLeftLeg.mValue[0];
	//mDataBuffer.mChannel[2].mR[1] = vqLeftLeg.mValue[1];
	//mDataBuffer.mChannel[2].mR[2] = vqLeftLeg.mValue[2];
	//mDataBuffer.mChannel[3].mR[0] = vqLeftFoot.mValue[0];
	//mDataBuffer.mChannel[3].mR[1] = vqLeftFoot.mValue[1];
	//mDataBuffer.mChannel[3].mR[2] = vqLeftFoot.mValue[2];

//////////////////////////////////////////////////////////////////////////
	//����ר��ģ��
	/*quadVector[0] = 0;	quadVector[1] = 0.5;	quadVector[2] = 0;
	vqLeftFoot = quadVector;
	vqLeftUpperLeg = quadVector;
	vqLeftLeg = quadVector;*/
//////////////////////////////////////////////////////////////////////////
//Step 1: Rt 1, MatrixMult 2 and 3
	FBRotationToMatrix(lTransformation, vqLeftUpperLeg);
	Quaternion2Matrix(mbquad1,lTransformation);
//////////////////////////////////////////////////////////////////////////
// Rt 1
//////////////////////////////////////////////////////////////////////////
	FBRotationToMatrix(lChannelTransformation, mDataBuffer.mChannel[17].mR);
	FBMatrixMult(lChannelTransformation,lTransformation,lChannelTransformation);
	FBMatrixToRotation((FBRVector&)*mDataBuffer.mChannel[17].mR,lChannelTransformation);
	//mDataBuffer.mChannel[24].mR[0] = vqLeftUpperLeg.mValue[0];
	//mDataBuffer.mChannel[24].mR[1] = vqLeftUpperLeg.mValue[1];
	//mDataBuffer.mChannel[24].mR[2] = vqLeftUpperLeg.mValue[2];

	// Mult 2
	FBRotationToMatrix(lChannelTransformation, mDataBuffer.mChannel[18].mR);
	memcpy(&lChannelTransformation[12], mDataBuffer.mChannel[18].mT,sizeof(mDataBuffer.mChannel[18].mT));

	//memcpy(&lChannelTransformation[12], lcl18to17,sizeof(lcl18to17));
	//FBMatrixMult(lChannelTransformation,lTransformation,lChannelTransformation);
	//FBMatrixToRotation((FBRVector&)*mDataBuffer.mChannel[2].mR,lChannelTransformation);
	memcpy(&mDataBuffer.mChannel[18].mT,&lChannelTransformation[12],sizeof(mDataBuffer.mChannel[18].mT));
	for (int i = 0; i< 3; i++)
	{
		mDataBuffer.mChannel[18].mT[i] += mDataBuffer.mChannel[17].mT[i];
	}
	// Mult 3
	FBRotationToMatrix(lChannelTransformation, mDataBuffer.mChannel[19].mR);
	memcpy(&lChannelTransformation[12], lcl19to17,sizeof(lcl19to17));
	FBMatrixMult(lChannelTransformation,lTransformation,lChannelTransformation);
	//FBMatrixToRotation((FBRVector&)*mDataBuffer.mChannel[26].mR,lChannelTransformation);
	memcpy(&mDataBuffer.mChannel[19].mT,&lChannelTransformation[12],sizeof(mDataBuffer.mChannel[19].mT));
	for (int i = 0; i< 3; i++)
	{
		mDataBuffer.mChannel[19].mT[i] += mDataBuffer.mChannel[17].mT[i];
	}
//////////////////////////////////////////////////////////////////////////
//Step 2: Rot 2, Mult 3
	FBRotationToMatrix(lTransformation, vqLeftLeg);
	Quaternion2Matrix(mbquad2,lTransformation);

//////////////////////////////////////////////////////////////////////////
	FBRotationToMatrix(lChannelTransformation, mDataBuffer.mChannel[18].mR);
	FBMatrixMult(lChannelTransformation,lTransformation,lChannelTransformation);
	FBMatrixToRotation((FBRVector&)*mDataBuffer.mChannel[18].mR,lChannelTransformation);
	//mDataBuffer.mChannel[25].mR[0] = vqLeftLeg.mValue[0];
	//mDataBuffer.mChannel[25].mR[1] = vqLeftLeg.mValue[1];
	//mDataBuffer.mChannel[2].mR[2] = vqLeftLeg.mValue[2];

	// Mult 3
	FBRotationToMatrix(lChannelTransformation, mDataBuffer.mChannel[19].mR);
	memcpy(&lChannelTransformation[12], lcl19to18,sizeof(lcl19to18));
	FBMatrixMult(lChannelTransformation,lTransformation,lChannelTransformation);
	//FBMatrixToRotation((FBRVector&)*mDataBuffer.mChannel[26].mR,lChannelTransformation);
	memcpy(&mDataBuffer.mChannel[19].mT,&lChannelTransformation[12],sizeof(mDataBuffer.mChannel[19].mT));
	for (int i = 0; i< 3; i++)
	{
		mDataBuffer.mChannel[19].mT[i] += mDataBuffer.mChannel[18].mT[i];
	}
//////////////////////////////////////////////////////////////////////////
//Step 3: Rot 3
	FBRotationToMatrix(lTransformation, vqLeftFoot);
	Quaternion2Matrix(mbquad3,lTransformation);
//////////////////////////////////////////////////////////////////////////
	FBRotationToMatrix(lChannelTransformation, mDataBuffer.mChannel[19].mR);
	FBMatrixMult(lChannelTransformation,lTransformation,lChannelTransformation);
	FBMatrixToRotation((FBRVector&)*mDataBuffer.mChannel[19].mR,lChannelTransformation);
	//mDataBuffer.mChannel[26].mR[0] = vqLeftFoot.mValue[0];
	//mDataBuffer.mChannel[26].mR[1] = vqLeftFoot.mValue[1];
	//mDataBuffer.mChannel[26].mR[2] = vqLeftFoot.mValue[2];

//////////////////////////////////////////////////////////////////////////
//Another Wave of Rot

	//////////////////////////////////////////////////////////////////////////
	//Step 1: Rt 1, MatrixMult 2 and 3
	FBRotationToMatrix(lTransformation, vqRightUpperLeg);
	Quaternion2Matrix(mbquad4,lTransformation);
	//////////////////////////////////////////////////////////////////////////
	// Rt 1
	//////////////////////////////////////////////////////////////////////////
	FBRotationToMatrix(lChannelTransformation, mDataBuffer.mChannel[24].mR);
	FBMatrixMult(lChannelTransformation,lTransformation,lChannelTransformation);
	FBMatrixToRotation((FBRVector&)*mDataBuffer.mChannel[24].mR,lChannelTransformation);
	// Mult 2
	FBRotationToMatrix(lChannelTransformation, mDataBuffer.mChannel[25].mR);
	memcpy(&lChannelTransformation[12], lcl25to24,sizeof(lcl25to24));
	FBMatrixMult(lChannelTransformation,lTransformation,lChannelTransformation);
	memcpy(&mDataBuffer.mChannel[25].mT,&lChannelTransformation[12],sizeof(mDataBuffer.mChannel[25].mT));
	for (int i = 0; i< 3; i++)
	{
		mDataBuffer.mChannel[25].mT[i] += mDataBuffer.mChannel[24].mT[i];
	}
	// Mult 3
	FBRotationToMatrix(lChannelTransformation, mDataBuffer.mChannel[26].mR);
	memcpy(&lChannelTransformation[12], lcl26to24,sizeof(lcl26to24));
	FBMatrixMult(lChannelTransformation,lTransformation,lChannelTransformation);
	memcpy(&mDataBuffer.mChannel[26].mT,&lChannelTransformation[12],sizeof(mDataBuffer.mChannel[26].mT));
	for (int i = 0; i< 3; i++)
	{
		mDataBuffer.mChannel[26].mT[i] += mDataBuffer.mChannel[24].mT[i];
	}
	//////////////////////////////////////////////////////////////////////////
	//Step 2: Rot 2, Mult 3
	FBRotationToMatrix(lTransformation, vqRightLeg);
	Quaternion2Matrix(mbquad5,lTransformation);
//////////////////////////////////////////////////////////////////////////
	FBRotationToMatrix(lChannelTransformation, mDataBuffer.mChannel[25].mR);
	FBMatrixMult(lChannelTransformation,lTransformation,lChannelTransformation);
	FBMatrixToRotation((FBRVector&)*mDataBuffer.mChannel[25].mR,lChannelTransformation);

	// Mult 3
	FBRotationToMatrix(lChannelTransformation, mDataBuffer.mChannel[26].mR);
	memcpy(&lChannelTransformation[12], lcl26to25,sizeof(lcl26to25));
	FBMatrixMult(lChannelTransformation,lTransformation,lChannelTransformation);
	memcpy(&mDataBuffer.mChannel[26].mT,&lChannelTransformation[12],sizeof(mDataBuffer.mChannel[26].mT));
	for (int i = 0; i< 3; i++)
	{
		mDataBuffer.mChannel[26].mT[i] += mDataBuffer.mChannel[25].mT[i];
	}
	//////////////////////////////////////////////////////////////////////////
	//Step 3: Rot 3
	FBRotationToMatrix(lTransformation, vqRightFoot);
	Quaternion2Matrix(mbquad6,lTransformation);

//////////////////////////////////////////////////////////////////////////
	FBRotationToMatrix(lChannelTransformation, mDataBuffer.mChannel[26].mR);
	FBMatrixMult(lChannelTransformation,lTransformation,lChannelTransformation);
	FBMatrixToRotation((FBRVector&)*mDataBuffer.mChannel[26].mR,lChannelTransformation);



//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i< 3; i++)
	{
		mDataBuffer.mChannel[27].mT[i] = mDataBuffer.mChannel[26].mT[i];
		mDataBuffer.mChannel[27].mR[i] = mDataBuffer.mChannel[26].mR[i];
		mDataBuffer.mChannel[28].mT[i] = mDataBuffer.mChannel[26].mT[i];
		mDataBuffer.mChannel[28].mR[i] = mDataBuffer.mChannel[26].mR[i];
		mDataBuffer.mChannel[29].mT[i] = mDataBuffer.mChannel[26].mT[i];
		mDataBuffer.mChannel[29].mR[i] = mDataBuffer.mChannel[26].mR[i];
		mDataBuffer.mChannel[20].mT[i] = mDataBuffer.mChannel[19].mT[i];
		mDataBuffer.mChannel[20].mR[i] = mDataBuffer.mChannel[19].mR[i];
		mDataBuffer.mChannel[21].mT[i] = mDataBuffer.mChannel[19].mT[i];
		mDataBuffer.mChannel[21].mR[i] = mDataBuffer.mChannel[19].mR[i];
		mDataBuffer.mChannel[22].mT[i] = mDataBuffer.mChannel[19].mT[i];
		mDataBuffer.mChannel[22].mR[i] = mDataBuffer.mChannel[19].mR[i];

	}

//////////////////////////////////////////////////////////////////////////
	mDataBuffer.mCounter++;
	/*
	std::vector<std::vector<float> > posData;

	posFile = new std::ifstream();
	posFile->open( FileName.c_str() );
	posData.push_back(std::vector<float>());
	std::vector<float>* p = &posData.back();
	float tmp;
	int lineNum = 0;

	while(!posFile->eof())
	{
	*posFile >> tmp;
	p->push_back(tmp);
	if(++lineNum % 3 == 0)
	{
	posData.push_back(std::vector<float>());
	p = &posData.back();
	}
	}
	posData.pop_back();

	*/
	
	// Send last data + 0.5 degree global rotation in the Y direction

	/*FBMatrix lTransformation, lChannelTransformation;
	FBRotationToMatrix(lTransformation,FBRVector(0,0.5,0));
	
	for(int i = 0; i < ChannelCount; i++)
	{
		FBRotationToMatrix(lChannelTransformation, mDataBuffer.mChannel[i].mR);
		memcpy(&lChannelTransformation[12], mDataBuffer.mChannel[i].mT,sizeof(mDataBuffer.mChannel[i].mT));
		
		FBMatrixMult(lChannelTransformation,lTransformation,lChannelTransformation);
		
		FBMatrixToRotation((FBRVector&)*mDataBuffer.mChannel[i].mR,lChannelTransformation);
		memcpy(&mDataBuffer.mChannel[i].mT,&lChannelTransformation[12],sizeof(mDataBuffer.mChannel[i].mT));
	}
	mDataBuffer.mCounter++;*/
}

int main(int argc, char* argv[])
{
	if (!iFlag)
	{

////////////////////////////////////////////////////////////////
		quadfromfile1.open("G:\\Users\\Arthas\\Desktop\\test\\19.txt"); //3
		if(!quadfromfile1)
		{
			cerr << "error";
		}
		QLeftFoot.push_back(vector<float>());
		vector<float>* p = &QLeftFoot.back();
		float tmp;
		int lineNum = 0;

		while (!quadfromfile1.eof())
		{
			quadfromfile1 >> tmp;
			p->push_back(tmp);
			if(++lineNum % 4 == 0)
			{
				QLeftFoot.push_back(std::vector<float>());
				p = &QLeftFoot.back();
			}

		}
		quadfromfile1.close();
		QLeftFoot.pop_back();
		printf("1...\n");
//////////////////////////////////////////////////////////////////////////
		quadfromfile2.open("G:\\Users\\Arthas\\Desktop\\test\\18.txt"); //2
		if(!quadfromfile2)
		{
			cerr << "error";
		}
		QLeftLeg.push_back(vector<float>());
		p = &QLeftLeg.back();
		//float tmp;
		lineNum = 0;

		while (!quadfromfile2.eof())
		{
			quadfromfile2 >> tmp;
			p->push_back(tmp);
			if(++lineNum % 4 == 0)
			{
				QLeftLeg.push_back(std::vector<float>());
				p = &QLeftLeg.back();
			}

		}
		quadfromfile2.close();
		QLeftLeg.pop_back();
		printf("2...\n");
//////////////////////////////////////////////////////////////////////////
		quadfromfile3.open("G:\\Users\\Arthas\\Desktop\\test\\17.txt"); //1
		if(!quadfromfile3)
		{
			cerr << "error";
		}
		QLeftUpperLeg.push_back(vector<float>());
		p = &QLeftUpperLeg.back();
		//float tmp;
		lineNum = 0;

		while (!quadfromfile3.eof())
		{
			quadfromfile3 >> tmp;
			p->push_back(tmp);
			if(++lineNum % 4 == 0)
			{
				QLeftUpperLeg.push_back(std::vector<float>());
				p = &QLeftUpperLeg.back();
			}

		}
		quadfromfile3.close();
		QLeftUpperLeg.pop_back();
		printf("3...\n");
//////////////////////////////////////////////////////////////////////////
		quadfromfile4.open("G:\\Users\\Arthas\\Desktop\\test\\24.txt"); //1
		if(!quadfromfile4)
		{
			cerr << "error";
		}
		QRightUpperLeg.push_back(vector<float>());
		p = &QRightUpperLeg.back();
		//float tmp;
		lineNum = 0;

		while (!quadfromfile4.eof())
		{
			quadfromfile4 >> tmp;
			p->push_back(tmp);
			if(++lineNum % 4 == 0)
			{
				QRightUpperLeg.push_back(std::vector<float>());
				p = &QRightUpperLeg.back();
			}

		}
		quadfromfile4.close();
		QRightUpperLeg.pop_back();
		printf("4...\n");
//////////////////////////////////////////////////////////////////////////
		quadfromfile5.open("G:\\Users\\Arthas\\Desktop\\test\\25.txt"); //1
		if(!quadfromfile5)
		{
			cerr << "error";
		}
		QRightLeg.push_back(vector<float>());
		p = &QRightLeg.back();
		//float tmp;
		lineNum = 0;

		while (!quadfromfile5.eof())
		{
			quadfromfile5 >> tmp;
			p->push_back(tmp);
			if(++lineNum % 4 == 0)
			{
				QRightLeg.push_back(std::vector<float>());
				p = &QRightLeg.back();
			}

		}
		quadfromfile5.close();
		QRightLeg.pop_back();
		printf("5...\n");
//////////////////////////////////////////////////////////////////////////
		quadfromfile6.open("G:\\Users\\Arthas\\Desktop\\test\\26.txt"); //1
		if(!quadfromfile6)
		{
			cerr << "error";
		}
		QRightFoot.push_back(vector<float>());
		p = &QRightFoot.back();
		//float tmp;
		lineNum = 0;

		while (!quadfromfile6.eof())
		{
			quadfromfile6 >> tmp;
			p->push_back(tmp);
			if(++lineNum % 4 == 0)
			{
				QRightFoot.push_back(std::vector<float>());
				p = &QRightFoot.back();
			}

		}
		quadfromfile6.close();
		QRightFoot.pop_back();
		printf("6...\n");


//////////////////////////////////////////////////////////////////////////
		iFlag = TRUE;
		printf("Read...\n");
	}
	
	printf("Server ...\n");
    //
	//sDataBuffer mDataBuffer1;

    //UpdateBuffer(mDataBuffer1);
	//
	//FBQuaternion testvec;
	//FBMatrix testmat1,testmat2;
	//FBRVector testrvec;
	//testvec.mValue[0] = 0.000006125;			
	//testvec.mValue[1] = 0.000003618;
	//testvec.mValue[2] = -0.000160294;
	//testvec.mValue[3] = 0.999999988;
	//FBQuaternionToRotation(testrvec,testvec);
	//FBRotationToMatrix(testmat1,testrvec);
	//Quaternion2Matrix(testvec,testmat2);
    int Soc=0;
    while (1)
    {
		static double ServerStartedOn = 0.0;
        if (!Soc) 
        {
			Soc = StartServer( PORTNUMBER );
			if (Soc) 
			{
				printf("Server started on port %d\n",PORTNUMBER);
			}
		} else
		{
			printf("Waiting for connection\n");
			Cnt = 0;
			sockaddr_in	lClientAddr;
			int		lSize;
			int		lSocket;

			lSize = sizeof(lClientAddr);
	        bzero((char *)&lClientAddr, sizeof(lClientAddr));

			lSocket = accept(Soc, (struct sockaddr*)&lClientAddr, &lSize);
			if( lSocket >= 0 ) 
			{
				sockaddr_in	lAddr;
				sDataBuffer mDataBuffer;

		        bzero((char *)&lAddr, sizeof(lAddr));
				if( getsockname(lSocket, (struct sockaddr*)&lAddr, &lSize) < 0 )
				{
					return -1;
				}
				printf("Connection established\n");
				ServerStartedOn = (double)GetNanoSeconds();

				mDataBuffer.SetupBuffer();

				for (;;)
				{
					mDataBuffer.Simulate(GetNanoSeconds());

					if (send( lSocket, (char*)&mDataBuffer,sizeof(mDataBuffer), 0)==SOCKET_ERROR)
					{
						break;
					}
					
					UpdateBuffer(mDataBuffer);

					Sleep( 1000/SIM_FPS );
				}
			
				shutdown(lSocket, 2);
				closesocket(lSocket);

				printf("Connection closed, connection time = %f ms\n",(GetNanoSeconds()-ServerStartedOn)/1000000.0);

			}
		}
	}
	
    if (Soc) 
    {
        closesocket( Soc );
    }
	Cleanup();
	return 0;
}

