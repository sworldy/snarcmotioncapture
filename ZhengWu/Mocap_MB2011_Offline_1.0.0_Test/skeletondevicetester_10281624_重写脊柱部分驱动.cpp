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
ifstream qFile[31] = {ifstream("0.txt"),ifstream("1.txt"),ifstream("2.txt"),ifstream("3.txt"),
ifstream("4.txt"),ifstream("5.txt"),ifstream("6.txt"),ifstream("7.txt"),ifstream("8.txt"),
ifstream("9.txt"),ifstream("10.txt"),ifstream("11.txt"),ifstream("12.txt"),ifstream("13.txt"),
ifstream("14.txt"),ifstream("15.txt"),ifstream("16.txt"),ifstream("17.txt"),ifstream("18.txt"),
ifstream("19.txt"),ifstream("20.txt"),ifstream("21.txt"),ifstream("22.txt"),ifstream("23.txt"),
ifstream("24.txt"),ifstream("25.txt"),ifstream("26.txt"),ifstream("27.txt"),ifstream("28.txt"),
ifstream("29.txt"),ifstream("displacement.txt")};
vector<vector<vector<float> > > dQuaternion;
double dtmp[3] = {0,0,0};
bool statFile[31];
int Cnt = 0;
size_t dataLen;
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
}

void MatLocalRotation(FBMatrix parentMat,FBMatrix &pchildMat,FBMatrix TransMat)
{
	FBMatrix tmpMat,tmpParentRotMat,tmpTansMat;
	FBGetLocalMatrix(tmpMat,parentMat,pchildMat);
	tmpParentRotMat = parentMat;
	for (int i = 0; i<3; i++)   // Delete Translation Information
	{
		tmpParentRotMat[12+i] = 0;  
	}

	FBMatrixMult(tmpMat,TransMat,tmpMat);
	FBGetGlobalMatrix(pchildMat,parentMat,tmpMat);
}


void LocalRotation(sDataBuffer& mDataBuffer,int parent,int child,FBMatrix TransMat)
{
	FBMatrix parentMat,childMat,tmpMat;
	//Generate parentMat
	FBRotationToMatrix(parentMat,mDataBuffer.mChannel[parent].mR);
	memcpy(&parentMat[12], mDataBuffer.mChannel[parent].mT,sizeof(mDataBuffer.mChannel[parent].mT));
	//Generate childMat
	FBRotationToMatrix(childMat,mDataBuffer.mChannel[child].mR);
	memcpy(&childMat[12], mDataBuffer.mChannel[child].mT,sizeof(mDataBuffer.mChannel[child].mT));
	//Calculate Mat
	MatLocalRotation(parentMat,childMat,TransMat);
	//Update Translation
	memcpy(&mDataBuffer.mChannel[child].mT,&childMat[12],sizeof(mDataBuffer.mChannel[child].mT));

}
void ForceRotation(sDataBuffer& mDataBuffer,double * lcl,int parent,int child)
{
	for (int i = 0; i<3; i++)
	{
		mDataBuffer.mChannel[child].mT[i] = *(lcl + i) + mDataBuffer.mChannel[parent].mT[i];
	}
}
void GetlclTrans(double * lcl,sDataBuffer& mDataBuffer,int parent,int child)
{
	for (int i = 0; i <3 ; i++)
	{
		*(lcl+i) = mDataBuffer.mChannel[child].mT[i] - mDataBuffer.mChannel[parent].mT[i];
	}
}
void PartRotation(sDataBuffer& mDataBuffer,int parent, int child, FBMatrix Transmat)
{
	
}
void RotationLocalize(sDataBuffer& mDataBuffer,int child,FBMatrix TransMat)
{
	FBMatrix lclrefMat,lclMat,GblChannelMat;
	FBRotationToMatrix(lclrefMat,mDataBuffer.mChannel[child].mR);
	FBRotationToMatrix(GblChannelMat,mDataBuffer.mChannel[child].mR);
	memcpy(&GblChannelMat[12], mDataBuffer.mChannel[child].mT,sizeof(mDataBuffer.mChannel[child].mT));
	FBGetLocalMatrix(lclMat,lclrefMat,GblChannelMat);
	FBMatrixMult(lclMat,TransMat,lclMat);
	FBGetGlobalMatrix(GblChannelMat,lclrefMat,lclMat);
	FBMatrixToRotation((FBRVector&)*mDataBuffer.mChannel[child].mR,GblChannelMat);

}
void InformCpy(sDataBuffer& mDataBuffer,int ID, int num)
{
	for (int i = 1; i< num +1; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mDataBuffer.mChannel[ID+i].mR[j] = mDataBuffer.mChannel[ID].mR[j];
			mDataBuffer.mChannel[ID+i].mT[j] = mDataBuffer.mChannel[ID].mT[j];
		}
	}
}
void UpdateInformation(sDataBuffer& mDataBuffer, int ID,FBMatrix TransMat)
{
	double lcl[3],lcl1[3],lcl2[3],lcl3[3],lcl4[3],lcl5[3],lcl6[3],lcl7[3];
	switch (ID)
	{
	case 0:
		GetlclTrans(&lcl[0],mDataBuffer,1,2);
		GetlclTrans(&lcl1[0],mDataBuffer,1,3);
		LocalRotation(mDataBuffer,0,1,TransMat);
		ForceRotation(mDataBuffer,&lcl[0],1,2);
		ForceRotation(mDataBuffer,&lcl1[0],1,3);

		GetlclTrans(&lcl[0],mDataBuffer,6,7);
		GetlclTrans(&lcl1[0],mDataBuffer,6,8);
		LocalRotation(mDataBuffer,0,6,TransMat);
		ForceRotation(mDataBuffer,&lcl[0],6,7);
		ForceRotation(mDataBuffer,&lcl1[0],6,8);

		LocalRotation(mDataBuffer,0,11,TransMat);
		LocalRotation(mDataBuffer,0,12,TransMat);
		
		GetlclTrans(&lcl6[0],mDataBuffer,13,16); // L Shoulder
		GetlclTrans(&lcl[0],mDataBuffer,13,17);
		GetlclTrans(&lcl1[0],mDataBuffer,13,18);
		GetlclTrans(&lcl2[0],mDataBuffer,13,19);
		GetlclTrans(&lcl7[0],mDataBuffer,13,23); // R Shoulder
		GetlclTrans(&lcl3[0],mDataBuffer,13,24);
		GetlclTrans(&lcl4[0],mDataBuffer,13,25);
		GetlclTrans(&lcl5[0],mDataBuffer,13,26);
		LocalRotation(mDataBuffer,0,13,TransMat);
		ForceRotation(mDataBuffer,&lcl6[0],13,16);
		ForceRotation(mDataBuffer,&lcl[0],13,17);
		ForceRotation(mDataBuffer,&lcl1[0],13,18);
		ForceRotation(mDataBuffer,&lcl2[0],13,19);
		ForceRotation(mDataBuffer,&lcl7[0],13,23);
		ForceRotation(mDataBuffer,&lcl3[0],13,24);
		ForceRotation(mDataBuffer,&lcl4[0],13,25);
		ForceRotation(mDataBuffer,&lcl5[0],13,26);

		LocalRotation(mDataBuffer,0,14,TransMat);
		LocalRotation(mDataBuffer,0,15,TransMat);

		RotationLocalize(mDataBuffer,ID,TransMat);
		break;
	case 1:
		GetlclTrans(&lcl[0],mDataBuffer,2,3);
		LocalRotation(mDataBuffer,1,2,TransMat);
		ForceRotation(mDataBuffer,&lcl[0],2,3);
		RotationLocalize(mDataBuffer,ID,TransMat);
		break;
	case 2:
		LocalRotation(mDataBuffer,2,3,TransMat);
		RotationLocalize(mDataBuffer,ID,TransMat);
		break;
	case 3:
		RotationLocalize(mDataBuffer,ID,TransMat);
		InformCpy(mDataBuffer,ID,2);
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		GetlclTrans(&lcl[0],mDataBuffer,7,8);
		LocalRotation(mDataBuffer,6,7,TransMat);
		ForceRotation(mDataBuffer,&lcl[0],7,8);
		RotationLocalize(mDataBuffer,ID,TransMat);
		break;
	case 7:
		LocalRotation(mDataBuffer,7,8,TransMat);
		RotationLocalize(mDataBuffer,ID,TransMat);
		break;
	case 8:
		RotationLocalize(mDataBuffer,ID,TransMat);
		InformCpy(mDataBuffer,ID,2);
		break;
	case 9:
		break;
	case 10:
		break;
	case 13:
		GetlclTrans(&lcl[0],mDataBuffer,16,17);
		GetlclTrans(&lcl1[0],mDataBuffer,16,18);
		GetlclTrans(&lcl2[0],mDataBuffer,16,19);
		GetlclTrans(&lcl3[0],mDataBuffer,23,24);
		GetlclTrans(&lcl4[0],mDataBuffer,23,25);
		GetlclTrans(&lcl5[0],mDataBuffer,23,26);
		LocalRotation(mDataBuffer,13,16,TransMat);
		LocalRotation(mDataBuffer,13,23,TransMat);
		ForceRotation(mDataBuffer,&lcl[0],16,17);
		ForceRotation(mDataBuffer,&lcl1[0],16,18);
		ForceRotation(mDataBuffer,&lcl2[0],16,19);
		ForceRotation(mDataBuffer,&lcl3[0],23,24);
		ForceRotation(mDataBuffer,&lcl4[0],23,25);
		ForceRotation(mDataBuffer,&lcl5[0],23,26);

		RotationLocalize(mDataBuffer,ID,TransMat);
		break;
	case 16:
		GetlclTrans(&lcl[0],mDataBuffer,17,18);
		GetlclTrans(&lcl1[0],mDataBuffer,17,19);
		LocalRotation(mDataBuffer,16,17,TransMat);
		ForceRotation(mDataBuffer,&lcl[0],17,18);
		ForceRotation(mDataBuffer,&lcl1[0],17,19);

		RotationLocalize(mDataBuffer,ID,TransMat);
		break;

	case 17:
		GetlclTrans(&lcl[0],mDataBuffer,18,19);
		LocalRotation(mDataBuffer,17,18,TransMat);
		ForceRotation(mDataBuffer,&lcl[0],18,19);
		RotationLocalize(mDataBuffer,ID,TransMat);
		break;
	case 18:
		LocalRotation(mDataBuffer,18,19,TransMat);
		RotationLocalize(mDataBuffer,ID,TransMat);
		break;
	case 19:
		RotationLocalize(mDataBuffer,ID,TransMat);
		InformCpy(mDataBuffer,ID,3);
		break;
	case 20:
		break;
	case 21:
		break;
	case 22:
		break;
	case 23:
		GetlclTrans(&lcl[0],mDataBuffer,24,25);
		GetlclTrans(&lcl1[0],mDataBuffer,24,26);
		LocalRotation(mDataBuffer,23,24,TransMat);
		ForceRotation(mDataBuffer,&lcl[0],24,25);
		ForceRotation(mDataBuffer,&lcl1[0],24,26);

		RotationLocalize(mDataBuffer,ID,TransMat);
		break;
	case 24:
		GetlclTrans(&lcl[0],mDataBuffer,25,26);
		LocalRotation(mDataBuffer,24,25,TransMat);
		ForceRotation(mDataBuffer,&lcl[0],25,26);
		RotationLocalize(mDataBuffer,ID,TransMat);
		break;
	case 25:
		LocalRotation(mDataBuffer,25,26,TransMat);
		RotationLocalize(mDataBuffer,ID,TransMat);
		break;
	case 26:
		RotationLocalize(mDataBuffer,ID,TransMat);
		InformCpy(mDataBuffer,ID,3);
		break;
	case 27:
		break;
	case 28:
		break;
	case 29:
		break;
	default:
		RotationLocalize(mDataBuffer,ID,TransMat);
		break;

	}
}
void TransUpdate(sDataBuffer& mDataBuffer,vector<vector<float> >* ptmp)
{
	double dt[3];
	for (int i = 0; i < 3; i++)
	{
		dt[i] = ptmp->at(Cnt).at(i) - dtmp[i];
		dtmp[i] = ptmp->at(Cnt).at(i);
	}

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mDataBuffer.mChannel[i].mT[j] += dt[j];
		}
	}
}
void UpdateBuffer(sDataBuffer& mDataBuffer)
{
	vector<vector<float> >* ptmp;
	FBQuaternion Quater[30];
	FBMatrix lTransformation;
	if (Cnt == dataLen)
	{
		mDataBuffer.SetupBuffer();
		Cnt = 0;
	}
	if (statFile[30])
	{
		ptmp = &dQuaternion.at(30);
		TransUpdate(mDataBuffer,ptmp);
	}
	


	for (int i = 0; i< 30; i++)
	{
		if (statFile[i])
		{
			ptmp = &dQuaternion.at(i);
			for (int j = 0; j < 4; j++)
			{
				Quater[i].mValue[j] = ptmp->at(Cnt).at(j);
			}
			Quaternion2Matrix(Quater[i],lTransformation);
			UpdateInformation(mDataBuffer,i,lTransformation);
		}
	}
	Cnt++;
	mDataBuffer.mCounter++;
}

int main(int argc, char* argv[])
{
	float tmp;
	int lineNum = 0;
	vector<vector<float> >* ptmp;
	
	for (int i = 0; i < 30; i++)
	{
		dQuaternion.push_back(vector <vector<float> >());
		if(!qFile[i])
		{
			cerr << "error reading files in" << i << "\n";
			statFile[i] = FALSE;
		}
		else
		{
			ptmp = &dQuaternion.at(i);
			ptmp->push_back(vector<float>());
			vector<float>* p = &ptmp->back();
			lineNum = 0;
			while (!qFile[i].eof())
			{
				qFile[i] >> tmp;
				p->push_back(tmp);
				if(++lineNum % 4 == 0)
				{
					ptmp->push_back(std::vector<float>());
					p = &(ptmp->back());
				}

			}
			qFile[i].close();
			ptmp->pop_back();
			cout << i << "..." << "\n";
			statFile[i] = TRUE;
			dataLen = ptmp->size();
		}
	}

	dQuaternion.push_back(vector <vector<float> >());
	if(!qFile[30])
	{
		cerr << "error reading files in" << 30 << "\n";
		statFile[30] = FALSE;
	}
	else
	{
		ptmp = &dQuaternion.at(30);
		ptmp->push_back(vector<float>());
		vector<float>* p = &ptmp->back();
		lineNum = 0;
		while (!qFile[30].eof())
		{
			qFile[30] >> tmp;
			p->push_back(tmp);
			if(++lineNum % 3 == 0)
			{
				ptmp->push_back(std::vector<float>());
				p = &(ptmp->back());
			}

		}
		qFile[30].close();
		ptmp->pop_back();
		cout << 30 << "..." << "\n";
		statFile[30] = TRUE;
		dataLen = ptmp->size();
	}



	cout << "Done...\n";
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

