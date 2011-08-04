// TCP/IP server : test for ordeviceskeleton
//

#include <stdio.h>
#include <iostream>
#include <vector>
#define WS_VERSION_REQUIRED 0x0101
#define NAN 19
#define DSCALE 175.987693025/1.7;
#include <winsock.h>
#include <windows.h>
#include <fstream>
//--- SDK include

#include "DeviceBuffer.h"
#include "Fusion.h"

using namespace std;
//ifstream qFile[31] = {ifstream("0.txt"),ifstream("1.txt"),ifstream("2.txt"),ifstream("3.txt"),
//ifstream("4.txt"),ifstream("5.txt"),ifstream("6.txt"),ifstream("7.txt"),ifstream("8.txt"),
//ifstream("9.txt"),ifstream("10.txt"),ifstream("11.txt"),ifstream("12.txt"),ifstream("13.txt"),
//ifstream("14.txt"),ifstream("15.txt"),ifstream("16.txt"),ifstream("17.txt"),ifstream("18.txt"),
//ifstream("19.txt"),ifstream("20.txt"),ifstream("21.txt"),ifstream("22.txt"),ifstream("23.txt"),
//ifstream("24.txt"),ifstream("25.txt"),ifstream("26.txt"),ifstream("27.txt"),ifstream("28.txt"),
//ifstream("29.txt"),ifstream("displacement.txt")};

ifstream sFile[16] = {ifstream("0.txt"),ifstream("1.txt"),ifstream("2.txt"),ifstream("3.txt"),
ifstream("4.txt"),ifstream("5.txt"),ifstream("6.txt"),ifstream("7.txt"),ifstream("8.txt"),
ifstream("9.txt"),ifstream("10.txt"),ifstream("11.txt"),ifstream("12.txt"),ifstream("13.txt"),
ifstream("14.txt"),ifstream("15.txt")};
ofstream tFile[3] = {ofstream("q0.txt"),ofstream("q1.txt"),ofstream("q2.txt")};
vector<vector<vector<float> > > dQuaternion;
vector<vector<vector<double> > > VSensorRawData;
double dtmp[3] = {0,0,0};
bool statFile[31];
bool IsData[16];
int Cnt = 0;
size_t dataLen;
Fusion*	Quat_Dis=new Fusion();
int sensor2bone[31] = {3,7,8,9,NAN,NAN,0,1,2,NAN,NAN,4,4,5,6,NAN,16,10,11,12,NAN,NAN,NAN,17,13,14,15,NAN,NAN,NAN};
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


//void Quaternion2Matrix(FBQuaternion &pvector,FBMatrix &pmatrix)
//{
//	double x2,y2,z2,xy,yz,xz,xw,yw,zw;
//	x2 = pvector.mValue[0] * pvector.mValue[0] * 2;
//	y2 = pvector.mValue[1] * pvector.mValue[1] * 2;
//	z2 = pvector.mValue[2] * pvector.mValue[2] * 2;
//	xy = pvector.mValue[0] * pvector.mValue[1] * 2;
//	yz = pvector.mValue[2] * pvector.mValue[1] * 2;
//	xz = pvector.mValue[0] * pvector.mValue[2] * 2;
//	xw = pvector.mValue[0] * pvector.mValue[3] * 2;
//	yw = pvector.mValue[1] * pvector.mValue[3] * 2;
//	zw = pvector.mValue[2] * pvector.mValue[3] * 2;
//	pmatrix(0,0) = 1 - y2 - z2;
//	pmatrix(0,1) = xy + zw;
//	pmatrix(0,2) = xz - yw;
//	pmatrix(0,3) = 0;
//	pmatrix(1,0) = xy - zw;
//	pmatrix(1,1) = 1 - x2 - z2;
//	pmatrix(1,2) = yz + xw;
//	pmatrix(1,3) = 0;
//	pmatrix(2,0) = xz + yw;
//	pmatrix(2,1) = yz - xw;
//	pmatrix(2,2) = 1 - x2 - y2;
//	pmatrix(2,3) = 0;
//	pmatrix(3,0) = 0;
//	pmatrix(3,1) = 0;
//	pmatrix(3,2) = 0;
//	pmatrix(3,3) = 1;
//}

//void MatLocalRotation(FBMatrix parentMat,FBMatrix &pchildMat,FBMatrix TransMat)
//{
//	FBMatrix tmpMat,tmpParentRotMat,tmpTansMat;
//	FBGetLocalMatrix(tmpMat,parentMat,pchildMat);
//	tmpParentRotMat = parentMat;
//	for (int i = 0; i<3; i++)   // Delete Translation Information
//	{
//		tmpParentRotMat[12+i] = 0;  
//	}
//
//	FBMatrixMult(tmpMat,TransMat,tmpMat);
//	FBGetGlobalMatrix(pchildMat,parentMat,tmpMat);
//}


//void LocalRotation(sDataBuffer& mDataBuffer,int parent,int child,FBMatrix TransMat)
//{
//	FBMatrix parentMat,childMat,tmpMat;
//	//Generate parentMat
//	FBRotationToMatrix(parentMat,mDataBuffer.mChannel[parent].mR);
//	memcpy(&parentMat[12], mDataBuffer.mChannel[parent].mT,sizeof(mDataBuffer.mChannel[parent].mT));
//	//Generate childMat
//	FBRotationToMatrix(childMat,mDataBuffer.mChannel[child].mR);
//	memcpy(&childMat[12], mDataBuffer.mChannel[child].mT,sizeof(mDataBuffer.mChannel[child].mT));
//	//Calculate Mat
//	MatLocalRotation(parentMat,childMat,TransMat);
//	//Update Translation
//	memcpy(&mDataBuffer.mChannel[child].mT,&childMat[12],sizeof(mDataBuffer.mChannel[child].mT));
//
//}
//void ForceRotation(sDataBuffer& mDataBuffer,double * lcl,int parent,int child)
//{
//	for (int i = 0; i<3; i++)
//	{
//		mDataBuffer.mChannel[child].mT[i] = *(lcl + i) + mDataBuffer.mChannel[parent].mT[i];
//	}
//}
//void GetlclTrans(double * lcl,sDataBuffer& mDataBuffer,int parent,int child)
//{
//	for (int i = 0; i <3 ; i++)
//	{
//		*(lcl+i) = mDataBuffer.mChannel[child].mT[i] - mDataBuffer.mChannel[parent].mT[i];
//	}
//}
//void PartRotation(sDataBuffer& mDataBuffer,int parent, int child, FBMatrix Transmat)
//{
//	
//}
//void RotationLocalize(sDataBuffer& mDataBuffer,int child,FBMatrix TransMat)
//{
//	FBMatrix lclrefMat,lclMat,GblChannelMat;
//	FBRotationToMatrix(lclrefMat,mDataBuffer.mChannel[child].mR);
//	FBRotationToMatrix(GblChannelMat,mDataBuffer.mChannel[child].mR);
//	memcpy(&GblChannelMat[12], mDataBuffer.mChannel[child].mT,sizeof(mDataBuffer.mChannel[child].mT));
//	FBGetLocalMatrix(lclMat,lclrefMat,GblChannelMat);
//	FBMatrixMult(lclMat,TransMat,lclMat);
//	FBGetGlobalMatrix(GblChannelMat,lclrefMat,lclMat);
//	FBMatrixToRotation((FBRVector&)*mDataBuffer.mChannel[child].mR,GblChannelMat);
//
//}
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
//void UpdateInformation(sDataBuffer& mDataBuffer, int ID,FBMatrix TransMat)
//{
//	double lcl[3],lcl0[3],lcl1[3],lcl2[3],lcl3[3],lcl4[3],lcl5[3],lcl6[3],lcl7[3];
//	double lcl8[3],lcl9[3],lcla[3],lclb[3],lclc[3],lcld[3],lcle[3],lclf[3];
//
//	switch (ID)
//	{
//	case 0:
//		GetlclTrans(&lcl0[0],mDataBuffer,11,12);
//		GetlclTrans(&lcl1[0],mDataBuffer,11,13);
//		GetlclTrans(&lcl2[0],mDataBuffer,11,16);
//		GetlclTrans(&lcl3[0],mDataBuffer,11,17);
//		GetlclTrans(&lcl4[0],mDataBuffer,11,18);
//		GetlclTrans(&lcl5[0],mDataBuffer,11,19);
//		GetlclTrans(&lcl6[0],mDataBuffer,11,23);
//		GetlclTrans(&lcl7[0],mDataBuffer,11,24);
//		GetlclTrans(&lcl8[0],mDataBuffer,11,25);
//		GetlclTrans(&lcl9[0],mDataBuffer,11,26);
//		GetlclTrans(&lcla[0],mDataBuffer,11,14);
//		GetlclTrans(&lclb[0],mDataBuffer,11,15);
//		LocalRotation(mDataBuffer,0,11,TransMat);
//		ForceRotation(mDataBuffer,&lcl0[0],11,12);
//		ForceRotation(mDataBuffer,&lcl1[0],11,13);
//		ForceRotation(mDataBuffer,&lcl2[0],11,16);
//		ForceRotation(mDataBuffer,&lcl3[0],11,17);
//		ForceRotation(mDataBuffer,&lcl4[0],11,18);
//		ForceRotation(mDataBuffer,&lcl5[0],11,19);
//		ForceRotation(mDataBuffer,&lcl6[0],11,23);
//		ForceRotation(mDataBuffer,&lcl7[0],11,24);
//		ForceRotation(mDataBuffer,&lcl8[0],11,25);
//		ForceRotation(mDataBuffer,&lcl9[0],11,26);
//		ForceRotation(mDataBuffer,&lcla[0],11,14);
//		ForceRotation(mDataBuffer,&lclb[0],11,15);
//
//		GetlclTrans(&lcl0[0],mDataBuffer,1,2);
//		GetlclTrans(&lcl1[0],mDataBuffer,1,3);
//		LocalRotation(mDataBuffer,0,1,TransMat);
//		ForceRotation(mDataBuffer,&lcl0[0],1,2);
//		ForceRotation(mDataBuffer,&lcl1[0],1,3);
//		
//		GetlclTrans(&lcl0[0],mDataBuffer,6,7);
//		GetlclTrans(&lcl1[0],mDataBuffer,6,8);
//		LocalRotation(mDataBuffer,0,6,TransMat);
//		ForceRotation(mDataBuffer,&lcl0[0],6,7);
//		ForceRotation(mDataBuffer,&lcl1[0],6,8);
//
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//	case 1:
//		GetlclTrans(&lcl[0],mDataBuffer,2,3);
//		LocalRotation(mDataBuffer,1,2,TransMat);
//		ForceRotation(mDataBuffer,&lcl[0],2,3);
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//	case 2:
//		LocalRotation(mDataBuffer,2,3,TransMat);
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//	case 3:
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		InformCpy(mDataBuffer,ID,2);
//		break;
//	case 4:
//		break;
//	case 5:
//		break;
//	case 6:
//		GetlclTrans(&lcl[0],mDataBuffer,7,8);
//		LocalRotation(mDataBuffer,6,7,TransMat);
//		ForceRotation(mDataBuffer,&lcl[0],7,8);
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//	case 7:
//		LocalRotation(mDataBuffer,7,8,TransMat);
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//	case 8:
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		InformCpy(mDataBuffer,ID,2);
//		break;
//	case 9:
//		break;
//	case 10:
//		break;
//	case 11:
//		GetlclTrans(&lcl0[0],mDataBuffer,12,13);
//		GetlclTrans(&lcl1[0],mDataBuffer,12,14);
//		GetlclTrans(&lcl2[0],mDataBuffer,12,15);
//		GetlclTrans(&lcl3[0],mDataBuffer,12,16);
//		GetlclTrans(&lcl4[0],mDataBuffer,12,17);
//		GetlclTrans(&lcl5[0],mDataBuffer,12,18);
//		GetlclTrans(&lcl6[0],mDataBuffer,12,19);
//		GetlclTrans(&lcl7[0],mDataBuffer,12,23);
//		GetlclTrans(&lcl8[0],mDataBuffer,12,24);
//		GetlclTrans(&lcl9[0],mDataBuffer,12,25);
//		GetlclTrans(&lcla[0],mDataBuffer,12,26);
//		LocalRotation(mDataBuffer,11,12,TransMat);
//		ForceRotation(mDataBuffer,&lcl0[0],12,13);
//		ForceRotation(mDataBuffer,&lcl1[0],12,14);
//		ForceRotation(mDataBuffer,&lcl2[0],12,15);
//		ForceRotation(mDataBuffer,&lcl3[0],12,16);
//		ForceRotation(mDataBuffer,&lcl4[0],12,17);
//		ForceRotation(mDataBuffer,&lcl5[0],12,18);
//		ForceRotation(mDataBuffer,&lcl6[0],12,19);
//		ForceRotation(mDataBuffer,&lcl7[0],12,23);
//		ForceRotation(mDataBuffer,&lcl8[0],12,24);
//		ForceRotation(mDataBuffer,&lcl9[0],12,25);
//		ForceRotation(mDataBuffer,&lcla[0],12,26);
//
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//	case 12:
//		GetlclTrans(&lcl[0],mDataBuffer,16,17);
//		GetlclTrans(&lcl1[0],mDataBuffer,16,18);
//		GetlclTrans(&lcl2[0],mDataBuffer,16,19);
//		GetlclTrans(&lcl3[0],mDataBuffer,23,24);
//		GetlclTrans(&lcl4[0],mDataBuffer,23,25);
//		GetlclTrans(&lcl5[0],mDataBuffer,23,26);
//		LocalRotation(mDataBuffer,12,16,TransMat);
//		LocalRotation(mDataBuffer,12,23,TransMat);
//		ForceRotation(mDataBuffer,&lcl[0],16,17);
//		ForceRotation(mDataBuffer,&lcl1[0],16,18);
//		ForceRotation(mDataBuffer,&lcl2[0],16,19);
//		ForceRotation(mDataBuffer,&lcl3[0],23,24);
//		ForceRotation(mDataBuffer,&lcl4[0],23,25);
//		ForceRotation(mDataBuffer,&lcl5[0],23,26);
//		
//		GetlclTrans(&lcl[0],mDataBuffer,13,14);
//		GetlclTrans(&lcl1[0],mDataBuffer,13,15);
//		LocalRotation(mDataBuffer,12,13,TransMat);
//		ForceRotation(mDataBuffer,&lcl[0],13,14);
//		ForceRotation(mDataBuffer,&lcl1[0],13,15);
//
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//	case 13:
//		GetlclTrans(&lcl1[0],mDataBuffer,14,15);
//		LocalRotation(mDataBuffer,13,14,TransMat);
//		ForceRotation(mDataBuffer,&lcl[0],14,15);
//
//
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//	case 16:
//		GetlclTrans(&lcl[0],mDataBuffer,17,18);
//		GetlclTrans(&lcl1[0],mDataBuffer,17,19);
//		LocalRotation(mDataBuffer,16,17,TransMat);
//		ForceRotation(mDataBuffer,&lcl[0],17,18);
//		ForceRotation(mDataBuffer,&lcl1[0],17,19);
//
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//
//	case 17:
//		GetlclTrans(&lcl[0],mDataBuffer,18,19);
//		LocalRotation(mDataBuffer,17,18,TransMat);
//		ForceRotation(mDataBuffer,&lcl[0],18,19);
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//	case 18:
//		LocalRotation(mDataBuffer,18,19,TransMat);
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//	case 19:
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		InformCpy(mDataBuffer,ID,3);
//		break;
//	case 20:
//		break;
//	case 21:
//		break;
//	case 22:
//		break;
//	case 23:
//		GetlclTrans(&lcl[0],mDataBuffer,24,25);
//		GetlclTrans(&lcl1[0],mDataBuffer,24,26);
//		LocalRotation(mDataBuffer,23,24,TransMat);
//		ForceRotation(mDataBuffer,&lcl[0],24,25);
//		ForceRotation(mDataBuffer,&lcl1[0],24,26);
//
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//	case 24:
//		GetlclTrans(&lcl[0],mDataBuffer,25,26);
//		LocalRotation(mDataBuffer,24,25,TransMat);
//		ForceRotation(mDataBuffer,&lcl[0],25,26);
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//	case 25:
//		LocalRotation(mDataBuffer,25,26,TransMat);
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//	case 26:
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		InformCpy(mDataBuffer,ID,3);
//		break;
//	case 27:
//		break;
//	case 28:
//		break;
//	case 29:
//		break;
//	default:
//		RotationLocalize(mDataBuffer,ID,TransMat);
//		break;
//
//	}
//}
void TransUpdate(sDataBuffer& mDataBuffer,vector<vector<float> >* ptmp)
{
	double dt[3];
	if (Cnt != 0)
	{
		for (int i = 0; i < 3; i++)
			dt[i] = ptmp->at(Cnt).at(i) - ptmp->at(Cnt-1).at(i);
	}
	else
	{
		for (int i = 0; i < 3; i++)
			dt[i] = 0;
		//dt[0] = 300 / DSCALE;   // Double Test
	}
	
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mDataBuffer.mChannel[i].mT[j] += dt[j]*DSCALE;
		}
	}
}
void UpdateBuffer(sDataBuffer& mDataBuffer)
{
	vector<vector<float> >* ptmp;
	//FBQuaternion Quater[30];
	//FBMatrix lTransformation;
	vector<vector<double> >* ptmpp;
	vector<float>* ptmpf2;
	double dtmp3[4],dtmp4[4];
	if (Cnt == dataLen)
	{
		mDataBuffer.SetupBuffer();
		Cnt = 0;
	}

	for(int  i=0; i<18; i++) // SENSOR_NUM=16, 代表sensor个数
	{
		ptmp = &dQuaternion.at(i);  // 第i个Sensor
		ptmpf2 = &ptmp->at(Cnt);  // 第j个数据点
		for (int j = 0; j < 4; j++)
		{
			mDataBuffer.rawdata[i][j] = ptmpf2->at(j);
		}
	}
	ptmp = &dQuaternion.at(18);  // 第i个Sensor
	ptmpf2 = &ptmp->at(Cnt);  // 第j个数据点
	for (int j = 0; j < 3; j++)
	{
		mDataBuffer.displace[j] = ptmpf2->at(j);
	}


	ptmp = &dQuaternion.at(18);
	//TransUpdate(mDataBuffer,ptmp);

	for (int i = 0; i< 30; i++)
	{
		if (statFile[sensor2bone[i]])
		{
			ptmp = &dQuaternion.at(sensor2bone[i]);
			// Insert Difference
			if (Cnt != 0)
			{
				for (int k = 0; k < 4; k++)
				{
					dtmp3[k] = -1 * ptmp->at(Cnt-1).at(k);
					dtmp4[k] = ptmp->at(Cnt).at(k);
				}
				dtmp3[3] = -1 * dtmp3[3];
				Quat_Dis->Quaternion_Multiple(dtmp3,dtmp4);
				/*for (int j = 0; j < 4; j++)
				{
					Quater[i].mValue[j] = dtmp3[j];
				}*/
			}
			else
			{
				/*for (int j = 0; j < 4; j++)
				{
					Quater[i].mValue[j] = ptmp->at(Cnt).at(j);
				}*/
			}
			// Insert End
			
			//Quaternion2Matrix(Quater[i],lTransformation);
			//UpdateInformation(mDataBuffer,i,lTransformation);
		}
	}
	Cnt++;
	mDataBuffer.mCounter++;
}

int main(int argc, char* argv[])
{
	int lineNum;
	float tmp;
	vector<vector<double> >* ptmp;
	vector<vector<float> >* ptmpf;
	vector<double>* ptmp2;
	vector<float>* ptmpf2;
	double* pbuf;
	double Quat_Result[18][4]; // DIM_QUAT为4, quaternion维度；
	double Dis_Result[3]; // DIM_VEC=3. 3维向量，存储位移结果
	double Dis_AResult[3];
	double Quat_ResAug[2][DIM_QUAT];
	double dtmp[18][4],dtmp2[18][4],dtmp3[4],dtmp4[4];

	sDataBuffer mb;
	size_t a = sizeof(mb);
	
	//std::string s =new std::string;
	for (int i = 0; i < 16; i++)
	{
		VSensorRawData.push_back(vector <vector<double> >());
		if (!sFile[i])
		{
			//No File
			cerr << "Warning: Miss Sensor Data in " << i << "\n";
			IsData[i] = FALSE;
		}
		else
		{
			ptmp = &VSensorRawData.at(i);
			ptmp->push_back(vector<double>());
			vector<double>* p = &ptmp->back();
			lineNum = 0;
			while (!sFile[i].eof())
			{
				sFile[i] >> tmp;
				p->push_back(tmp);
				if(++lineNum % 9 == 0)
				{
					ptmp->push_back(std::vector<double>());
					p = &(ptmp->back());
				}
			}
			sFile[i].close();
			ptmp->pop_back();
			cout << i << "..." << "\n";
			statFile[i] = TRUE;
			dataLen = ptmp->size();
		}
	}
	statFile[16] = TRUE;statFile[17] = TRUE;
	cout << "Done...\n";
	cout << "Process Data...\n";
	
	for (int i = 0; i<19; i++)
	{
		dQuaternion.push_back(vector <vector<float> >());
	}

	for (int j = 0; j < (int)dataLen; j++)
	{
		for(int  i=0; i<SENSOR_NUM; i++) // SENSOR_NUM=16, 代表sensor个数
		{
			ptmp = &VSensorRawData.at(i);  // 第i个Sensor
			ptmp2 = &ptmp->at(j);  // 第j个数据点
			pbuf =  &(ptmp2->at(0));  //指针
			Quat_Dis->Get_Measure(i,pbuf); // 16个sensor遍历一遍，第一处
		}
		Quat_Dis->WholeBodyFusion(Quat_Result,Dis_Result); // 16个sensor遍历结束后，全身融合，第二处
		Quat_Dis->Quaternion_Augment2(Quat_Result);  // 写入左右肩膀的位移
		
		//FrameTrans of Dis
		Dis_AResult[2] = Dis_Result[0];
		Dis_AResult[0] = -1 * Dis_Result[1];
		Dis_AResult[1] = -1 * Dis_Result[2];
		Dis_AResult[1] += 0.80;
		//


		ptmpf = &dQuaternion.at(18);
		ptmpf->push_back(vector<float>());
		ptmpf2 = &ptmpf->back();
		for (int dim = 0; dim < 3; dim++)
			ptmpf2->push_back((float)Dis_AResult[dim]);


		for (int node = 0; node < 18; node++)
		{
			ptmpf = &dQuaternion.at(node);
			ptmpf->push_back(vector<float>());
			ptmpf2 = &ptmpf->back();
			for (int dim = 0; dim < 4; dim++)
				ptmpf2->push_back((float)Quat_Result[node][dim]);
		}
	}
	//Debug Module
	/*ptmpf = &dQuaternion.at(2);
	for (int xx = 0; xx < dataLen; xx++)
	{
		for (int yy = 0; yy < DIM_QUAT; yy++)
		{
			tFile[2] << ptmpf->at(xx).at(yy) << "\t";
		}
		tFile[2] << "\n";
	}
	exit(0);*/
	//Exit Debug

	cout << "Done...\n";
	printf("Server ...\n");
 
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

