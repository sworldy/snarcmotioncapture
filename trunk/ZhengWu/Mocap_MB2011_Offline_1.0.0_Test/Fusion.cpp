#include <stdlib.h>
#include "stdio.h"
//using namespace std;
//#include "stdafx.h"
#include "Fusion.h"
#include "math.h"
//#include "FilterAbstract.h"
//#include "Math.hpp"   

Fusion::Fusion(){
	//
	int i,j;

	// Variables
	Counter_Training = 0;
	for(i=0; i<DIM_VEC; i++)
	{
		Mag_Ref[i] = 0;
	}
	for(i=0; i<DIM_MEAS; i++)
	{
		MagRef_Training[i] = 0;
	}
	for(i=0; i<SENSOR_NUM; i++)
	{
		for(j=0; j<DIM_MEAS; j++)
		{
			Measure_Buf[i][j] = 0;
		}
		for(j=0; j<DIM_QUAT; j++)
		{
			if(j==3)
			{
				Quaternions[i][j] = 1;
				Quat_RS2GB[i][j] = 1;
				Quat_GB2MA[i][j] = 1;
			}
			else 
			{
				Quaternions[i][j] = 0;
				Quat_RS2GB[i][j] = 0;
				Quat_GB2MA[i][j] = 0;
			}
		}
		QuaternionsFilter[i] = new QuaternionFusion(i);
	}
	Displacement = new DisplacementFusion();
}


Fusion::~Fusion()
{
	int i;
	// Class and Obejects
	for(i=0; i<SENSOR_NUM; i++)
	{
		delete QuaternionsFilter[i];
	}
	delete Displacement;
}

bool Fusion::Get_Measure(int id, double *meas)
//set the parameters need in the DisplacementFusion estimation
{
	int i;
	double Quat_Temp[4];
	
	QuaternionsFilter[id]->QuaternionFusion_Filtering(meas,Quat_Temp);
	for(i=0; i<DIM_QUAT; i++)
	{
		Quaternions[id][i] = Quat_Temp[i];
	}
	
	for(i=0; i<DIM_MEAS; i++)
	{
		Measure_Buf[id][i] = meas[i];
	}
	return 1;//返回真
}
void Fusion::WBQuatFusion_Filtering() // Whole Body Quaternion Fusion Filtering
{
	int i;
	//double temp;
	Counter_Training++;
	if(Counter_Training<TRAINING_NUM)
	{
		for(i=0; i<DIM_MEAS; i++)
		{
			MagRef_Training[i] += Measure_Buf[SENID_MAGREF][i];
		}
		for (int jj = 0; jj <SENSOR_NUM; jj++)
		{
			for (int q = 0; q < DIM_QUAT; q++)
			{
				Quaternionnew[jj][q] = Quaternions[jj][q];
			}
		}
		
	}
	else if(Counter_Training==TRAINING_NUM)
	{
		for(i=0;i<DIM_MEAS;i++)
		{
			MagRef_Training[i] += Measure_Buf[SENID_MAGREF][i];
			MagRef_Training[i] = MagRef_Training[i]/TRAINING_NUM;
		}
		Meas_Normalization(MagRef_Training);
		Get_MagRef(MagRef_Training);
		Get_QuatRS2GB();
		Get_QuatGB2MA(); // Get the quaternion for frame transformation, from qGB to qMA 
		for (int jj = 0; jj <SENSOR_NUM; jj++)
		{
			for (int q = 0; q < DIM_QUAT; q++)
			{
				Quaternionnew[jj][q] = Quaternions[jj][q];
			}
		}
	}
	else
	{
		for(i=0; i<SENSOR_NUM; i++)
		{
			FrameTrans_RS2GB(Quaternions[i],Quat_RS2GB[i]); 
			//FrameTrans_RS2GB2(Quaternions[i],Quat_RS2GB[i],Quaternionnew[i]);
			// Frame Transformation, from qRS to qGB
			for (int q = 0; q < DIM_QUAT; q++)
			{
				Quaternionnew[i][q] = Quaternions[i][q];
			}
			
			FrameTrans_GB2MA(Quaternions[i],Quat_GB2MA[i]); 
			// Frame Transformation, from qGB to qMA
		}
	}
}

bool Fusion::WholeBodyFusion(double QuatResult[][DIM_QUAT],double *DisResult)
{
	int i,j;

	WBQuatFusion_Filtering(); // Whole Body Quaternion Fusion Filtering
	for(i=0; i<SENSOR_NUM; i++)
	{
		for(j=0; j<DIM_QUAT; j++)
		{
			QuatResult[i][j] = Quaternions[i][j];
		}
		// Sensor ID on the human body, using avatar
		// { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16} - true sensor ID in hardware
		// {19,20,21, 1, 3, 4, 6,15,16,17, 8, 9,10,12,13,14} - sensor ID in avatar
		// { 2=1, 5=4, 7=4, 11=4, 18=17, 22=21} - sensor ID equation in avatar
		if(i == 3)
			Displacement->SetPara(1, Quaternionnew[i], Measure_Buf[i]); //Displacement in Pelvis(No.1)
		if(i == 9)
			Displacement->SetPara(17, Quaternionnew[i], Measure_Buf[i]);//Displacement in LFoot(No.17)
		if(i == 2)
			Displacement->SetPara(21, Quaternionnew[i], Measure_Buf[i]);//Displacement in RFoot(No.21)
		if (i == 7)
			Displacement->SetPara(15, Quaternionnew[i], Measure_Buf[i]); //Displacement in LThigh(No.15)
		if (i == 8)
			Displacement->SetPara(16, Quaternionnew[i], Measure_Buf[i]); //Displacement in LCalf(No.16)
		if (i == 0)
			Displacement->SetPara(19, Quaternionnew[i], Measure_Buf[i]); //Displacement in RThigh(No.19)
		if (i == 1)
			Displacement->SetPara(20, Quaternionnew[i], Measure_Buf[i]); //Displacement in RCalf(No.20)
	}
	Displacement->EstimateDis(DisResult); //Get the dispalcement
	//EstimateDis
	
	// Displacement from meter 2 pixel
	for(i=0; i<DIM_VEC; i++)
	{
		DisResult[i] = DisResult[i]*COF_METER2PIXEL;
	}
	return 1;
}

void Fusion::Get_MagRef(double *Meas_Normalized)
// The classname is the own class name of fusion program.
// Get the earth magnetic reference.
// Get = Get.
// Mag = Magnetic field. 
// Ref = Reference.
{
	int i;
	double quat_pitch[4] ={0,1,0,1}; // quat_pitch -> y axis;
	double quat_roll[4] ={1,0,0,1}; // quat_roll -> x axis;
	double sin_p,sin_r,cos_p,cos_r;
	double sin_ph,sin_rh,cos_ph,cos_rh;
	double Acc[3],Mag[3];
	double MagRef_temp[3];
	double quat_temp1[4]={0,0,0,1}, quat_temp2[4]={0,0,0,1};
	double quat_GS[4];
	
	//
	for( i=0;i<3;i++)
	{
		Acc[i] = Meas_Normalized[i];
		Mag[i] = Meas_Normalized[i+6];
		MagRef_temp[i] = Mag[i];
	}

	// -------------------------
	// Frame Transformation
    // qx = cos(pi/2)*[0;0;0;1;]+sin(pi/2)*[1;0;0;0;];
	quat_temp1[0] = sin(PI/2);
	quat_temp1[1] = 0;
	quat_temp1[2] = 0;
	quat_temp1[3] = cos(PI/2);
	
	// qy = cos(-pi/4)*[0;0;0;1;]+sin(-pi/4)*[0;1;0;0];
	quat_temp2[0] = 0;
	quat_temp2[1] = sin(-PI/4);
	quat_temp2[2] = 0;
	quat_temp2[3] = cos(-PI/4);
	
	// qGS = qmult(qx,qy);
	Quaternion_Multiple(quat_temp1,quat_temp2);
	for(i=0; i<4; i++)
	{
		quat_GS[i] = quat_temp1[i];
	} 
	Quaternion_VecRot(quat_GS,Acc);
	Quaternion_VecRot(quat_GS,Mag);
	// -------------------------

	//
	for( i=0;i<3;i++)
	{
		MagRef_temp[i] = Mag[i];
	}

	// -------------------------
	// FQA to determine the pitch and roll angle
	// Pitch/elevation quaternion calculation
	sin_p = -Acc[0];
	cos_p = sqrt(1-sin_p*sin_p);
	sin_ph = Get_Sign(sin_p)*(sqrt(0.5*(1-cos_p)));
	cos_ph = sqrt((0.5*(1+cos_p)));
	quat_pitch[1] = sin_ph;
	quat_pitch[3] = cos_ph;

    //--- Roll/bank quaternion calculation
	 sin_r = Acc[1]/cos_p;
     cos_r = Acc[2]/cos_p;

	if(Get_Sign(sin_r) != 0)
		sin_rh = Get_Sign(sin_r)*(sqrt(0.5*(1-cos_r)));
    else
        sin_rh = sqrt((0.5*(1-cos_r)));
    cos_rh = sqrt(0.5*(1+cos_r));
	quat_roll[0] = sin_rh;
	quat_roll[3] = cos_rh;
	// -------------------------

	// -------------------------
	// Magnetism direction unit vector in global frame.
	Quaternion_Multiple(quat_pitch,quat_roll);
	Quaternion_VecRot(quat_pitch,MagRef_temp);
	// -------------------------

	for(i=0; i<DIM_VEC; i++)
	{
		Mag_Ref[i] = MagRef_temp[i];
	}
}

int Fusion::Get_Sign(double real_num)
// The classname is the own class name of fusion program.
// Get = Get.
// Sign = The positive or negtive sign of a real number. 
{
	if(real_num == 0)
		return 0;
	else if (real_num < 0)
		return -1;
	else if (real_num > 0)
		return 1;
	else 
		return 0;
}

void Fusion::Quaternion_Multiple(double *quat_result,double *quat_input){
// The output quaternion is quat_result = quat_result*quat_input
	double a[4],b[4];
	int i;
	for(i=0; i<4; i++)
	{
		a[i] = quat_result[i];
		b[i] = quat_input[i];
	}

	//double temp1=a[3]*b[3]-a[0]*b[0]-a[1]*b[1]-a[2]*b[2];
	//double temp2=a[3]*b[0]+b[3]*a[0]+a[1]*b[2]-b[1]*a[2];
	//double temp3=a[3]*b[1]+b[3]*a[1]+a[2]*b[0]-b[2]*a[0];
	//double temp4=a[3]*b[2]+b[3]*a[2]+a[0]*b[1]-b[0]*a[1];
	//a[0]=temp2;
	//a[1]=temp3;
	//a[2]=temp4;
	//a[3]=temp1;

	double temp1 = a[3]*b[3]-a[0]*b[0]-a[1]*b[1]-a[2]*b[2];
	double temp2 = a[3]*b[0]+b[3]*a[0]+a[1]*b[2]-b[1]*a[2];
	double temp3 = a[3]*b[1]+b[3]*a[1]+a[2]*b[0]-b[2]*a[0];
	double temp4 = a[3]*b[2]+b[3]*a[2]+a[0]*b[1]-b[0]*a[1];
	quat_result[0] = temp2;
	quat_result[1] = temp3;
	quat_result[2] = temp4;
	quat_result[3] = temp1;
}

void Fusion::Quaternion_VecRot(double *Quat, double *Vec_Result){
	//qvrot, q*v*q(-1)
	double Vec_Aug[]={Vec_Result[0],Vec_Result[1],Vec_Result[2],0};
	double Quat_Temp[]={Quat[0],Quat[1],Quat[2],Quat[3]};
	double Quat_Conj[]={-Quat_Temp[0],-Quat_Temp[1],-Quat_Temp[2],Quat_Temp[3]};
	Quaternion_Multiple(Vec_Aug,Quat_Conj);
	Quaternion_Multiple(Quat_Temp,Vec_Aug);
	for (int i=0;i<3;i++)
	{
		Vec_Result[i] = Quat_Temp[i];
	}

	//double aug_vc[]={vctors[0],vctors[1],vctors[2],0};
	//double aa[]={a[0],a[1],a[2],a[3]};
	//double inva[]={-aa[0],-aa[1],-aa[2],aa[3]};

	//Quaternion_Multiple(aug_vc,inva);
	//Quaternion_Multiple(aa,aug_vc);

	//for (int i=0;i<3;i++)
	//{
	//	vctors[i]=aa[i];
	//}
}

void Fusion::Quaternion_VecXform(double *Quat, double *Vec_Result){  
	//qvxform	
	double Vec_Aug[]={Vec_Result[0],Vec_Result[1],Vec_Result[2],0};
	double Quat_Temp[]={Quat[0],Quat[1],Quat[2],Quat[3]};
	double Quat_Conj[]={-Quat_Temp[0],-Quat_Temp[1],-Quat_Temp[2],Quat_Temp[3]};
	Quaternion_Multiple(Vec_Aug,Quat_Temp);
	Quaternion_Multiple(Quat_Conj,Vec_Aug);
	for (int i=0;i<3;i++)
	{
		Vec_Result[i] = Quat_Conj[i];
	}

	//double aug_vc[]={vctors[0],vctors[1],vctors[2],0};
	//double aa[]={a[0],a[1],a[2],a[3]};
	//double inva[]={-aa[0],-aa[1],-aa[2],aa[3]};
	//Quaternion_Multiple(aug_vc,aa);
	//Quaternion_Multiple(inva,aug_vc);
	//for (int i=0;i<3;i++)
	//{
	//	vctors[i]=inva[i];
	//}
}

void Fusion::FrameTrans_RS2GB(double *Quat_Result,double *Quat_Input)
// Frame Transformation, from qRS to qGB
// G, global frame
// B, body frame
// R, reference frame
// S, sensor frame
// Quat_Result, quaternions after filtering
// Quat_Input, quaternions which is qGR
{
	double temp1[4] = {Quat_Result[0],Quat_Result[1],Quat_Result[2],Quat_Result[3]};
	double temp2[4] = {Quat_Input[0],Quat_Input[1],Quat_Input[2],Quat_Input[3]};
	int i;
	double Quat_InConj[4] = {-Quat_Input[0],-Quat_Input[1],-Quat_Input[2],Quat_Input[3]};
	Quaternion_Multiple(temp1,Quat_InConj); // qRS*qGR_conj
	Quaternion_Multiple(temp2,temp1); // qGR*(qRS*qGR_conj)

	for(i=0; i<3; i++)
	{
		Quat_Result[i] = temp2[i];
	}
}

void Fusion::FrameTrans_GB2MA(double *Quat_Result, double *Quat_Input)
// Frame Transformation, from qGB to qMA
// G, global frame
// B, body frame
// M, reference frame in the model display
// A, body frame in the avatar
// Quat_Result, quaternions after filtering,qGB
// Quat_Input, quaternion which is qMG
{
	double Quat_InConj[DIM_QUAT] = {-Quat_Input[0],-Quat_Input[1],-Quat_Input[2],Quat_Input[3]};
	double Quat_GATemp[DIM_QUAT];
	double Quat_MGTemp[DIM_QUAT];
	int i;
	for(i=0; i<DIM_QUAT; i++)
	{
		Quat_GATemp[i] = Quat_Result[i];
		Quat_MGTemp[i] = Quat_Input[i];
	}
	Quaternion_Multiple(Quat_GATemp,Quat_InConj); // qGB*qBA
	Quaternion_Multiple(Quat_MGTemp,Quat_GATemp); // qMG*(qGB*qBA)
	for(i=0; i<DIM_QUAT; i++)
	{
		Quat_Result[i] = Quat_MGTemp[i];
	}
}

void Fusion::FrameTrans_RS2GB2(double *Quat_Result, double *Quat_Input,double *Quat_Output)
{
	double temp1[4] = {Quat_Result[0],Quat_Result[1],Quat_Result[2],Quat_Result[3]};
	double temp2[4] = {Quat_Input[0],Quat_Input[1],Quat_Input[2],Quat_Input[3]};
	int i;
	double Quat_InConj[4] = {-Quat_Input[0],-Quat_Input[1],-Quat_Input[2],Quat_Input[3]};
	Quaternion_Multiple(temp1,Quat_InConj); // qRS*qGR_conj
	Quaternion_Multiple(temp2,temp1); // qGR*(qRS*qGR_conj)

	for(i=0; i<3; i++)
	{
		Quat_Output[i] = temp2[i];
	}
}


void Fusion::Get_QuatRS2GB()
// Get the quaternion for frame transformation, from qGS to qGB
{
	int i,j;
	double Meas_filtering[DIM_MEAS];
	double Quat_filtering[DIM_QUAT];
	for(i=0; i<SENSOR_NUM; i++)
	{
		for(j=0; j<DIM_VEC; j++)
		{
			Meas_filtering[j] = QuaternionsFilter[i]->gravity_initial[j];
			Meas_filtering[j+3] = 0;
			Meas_filtering[j+6] = QuaternionsFilter[i]->mag_initial[j];
		}
		Meas_Normalization(Meas_filtering);
		FQA(Meas_filtering, Quat_filtering); // Get quaternion using quaternion
		for(j=0; j<DIM_QUAT; j++)
		{
			Quat_RS2GB[i][j] = Quat_filtering[j];
		}
	}
}
void Fusion::Get_QuatGB2MA()
// Get the quaternion for frame transformation, from qGB to qMA 
// Sensor ID on the human body, using avatar
// { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16} - true sensor ID in hardware
// {19,20,21, 1, 3, 4, 6,15,16,17, 8, 9,10,12,13,14} - sensor ID in avatar
// { 2=1, 5=4, 7=4, 11=4, 18=17, 22=21} - sensor ID equation in avatar
{
	double quat_temp1[4] = {0,0,0,1},quat_temp2[4] = {0,0,0,1};
	int i,j;
	//
	quat_temp1[0] = 0;
	quat_temp1[1] = 0;
	quat_temp1[2] = sin(PI/4);
	quat_temp1[3] = cos(PI/4);
	quat_temp2[0] = 0;
	quat_temp2[1] = sin(NPI/4);
	quat_temp2[2] = 0;
	quat_temp2[3] = cos(NPI/4);
	Quaternion_Multiple(quat_temp1,quat_temp2);
	for(i=0; i<10; i++){
		for(j=0; j<DIM_QUAT; j++)
			Quat_GB2MA[i][j] = quat_temp1[j];
	}
		
	quat_temp1[0] = 0;
	quat_temp1[1] = 0;
	quat_temp1[2] = sin(PI/2);
	quat_temp1[3] = cos(PI/2);
	quat_temp2[0] = 0;
	quat_temp2[1] = sin(NPI/4);
	quat_temp2[2] = 0;
	quat_temp2[3] = cos(NPI/4);
	Quaternion_Multiple(quat_temp1,quat_temp2);
	for(i=10; i<13; i++){
		for(j=0; j<DIM_QUAT; j++)
			Quat_GB2MA[i][j] = quat_temp1[j];
	}

	quat_temp1[0] = 0;
	quat_temp1[1] = sin(NPI/4);
	quat_temp1[2] = 0;
	quat_temp1[3] = cos(NPI/4);
	for(i=13; i<16; i++){
		for(j=0; j<DIM_QUAT; j++)
			Quat_GB2MA[i][j] = quat_temp1[j];
	}



}


void Fusion::FQA(double *norm_measure, double *Quat_Result)
//FQA(double *norm_measure)
{
	int i;
	float th_0 = 0.2; // A threshold to determine whether cos(Pitch) is near zero.
	float th_zero = 0.002;
	double alpha = 0.2*PI; // The offset to rotate Acc and Mag when cos(Pitch) is near ZERO
	double q_alpha[4] ={0,sin(0.5*alpha),0,cos(0.5*alpha) };
	double q_x[4] ={0,1,0,1};
	double q_y[4] ={1,0,0,1};
	double q_z[4] ={0,0,1,1};
	int flag = 0;
	double Mag_xy[3],Mag_e[3];
	double temp;
	double sin_x,sin_y,sin_z,cos_x,cos_y,cos_z;
	double sin_xh,sin_yh,sin_zh,cos_xh,cos_yh,cos_zh;
	double Acc[3],Mag[3];
	//double ErrPos[4],ErrNeg[4];

	for( i=0;i<3;i++)
	{
		Acc[i] = norm_measure[i];
		Mag[i] = norm_measure[i+6];
		Mag_xy[i] = Mag[i];
	}

	sin_x = -Acc[0];
	cos_x = sqrt(1-sin_x*sin_x);
	double kkk = fabs(cos_x);

	if(fabs(cos_x)<th_0)   // to determine whether cos(Pitch) is near zero
	{
		Quaternion_VecRot(q_alpha,Acc);
		Quaternion_VecRot(q_alpha,Mag);
		flag = 1;
		for(i=0;i<3;i++)
		{
			Mag_xy[i] = Mag[i];
		}
		sin_x = -Acc[0];
		cos_x = sqrt(1-sin_x*sin_x);
	}

	
	//--- Pitch/elevation quaternion calculation
	if (sin_x==0)
	{
		if(cos_x>0)
		{
			sin_xh=0;
			cos_x=1;
		}
		else if(cos_x<0)
		{
			sin_xh=1;
			cos_x=-1;
		}
	}	
	else
		sin_xh = (sin_x/fabs(sin_x))*(sqrt(0.5*(1-cos_x)));
	cos_xh = sqrt((0.5*(1+cos_x)));
	q_x[1] = sin_xh;
	q_x[3] = cos_xh;


    //--- Roll/bank quaternion calculation
	 sin_y = Acc[1]/cos_x;
     cos_y = Acc[2]/cos_x;

	if(sin_y==0)
	{
		if(cos_y>0)
		{
			sin_yh=0;
			cos_y=1;
		}
		else if(cos_y<0)
		{
			sin_yh=1;
			cos_y=-1;
		}
	}
    else if (fabs(sin_y) > th_zero)
		sin_yh = (sin_y/fabs(sin_y))*(sqrt(0.5*(1-cos_y)));
    else
        sin_yh=sqrt((0.5*(1-cos_y)));
    cos_yh=sqrt(0.5*(1+cos_y));
	q_y[0] = sin_yh;
	q_y[3] = cos_yh;

	double mag_initial[3];
	for(i=0; i<3; i++)
	{
		mag_initial[i] = Mag_Ref[i];
	}

	//--- yaw/bank quaternion calculation
	Quaternion_Multiple(q_x,q_y);
	Quaternion_VecRot(q_x,Mag_xy);
    temp = sqrt(mag_initial[1]*mag_initial[1]+mag_initial[0]*mag_initial[0]);
	for (i=0;i<2;i++)
	{
	   Mag_e[i] = mag_initial[i]/temp;
	}
    
	 temp = sqrt(Mag_xy[1]*Mag_xy[1]+Mag_xy[0]*Mag_xy[0]);
	for (i=0;i<2;i++)
	{
	   Mag_xy[i] = Mag_xy[i]/temp;
	}

	cos_z = Mag_xy[0]*Mag_e[0]+Mag_xy[1]*Mag_e[1];
	sin_z = -Mag_xy[1]*Mag_e[0]+Mag_xy[0]*Mag_e[1];

   if(sin_z==0)
	{
		if(cos_z>0)
		{
			sin_zh=0;
			cos_z=1;
		}
		else if(cos_z<0)
		{
			sin_zh=1;
			cos_z=-1;
		}
	}
    else if (fabs(sin_z) > th_zero)
		sin_zh = (sin_z/fabs(sin_z))*(sqrt(0.5*(1-cos_z)));
    else
        sin_zh=sqrt((0.5*(1-cos_z)));

    cos_zh = sqrt((0.5*(1+cos_z)));
	q_z[2] = sin_zh;
	q_z[3] = cos_zh;

   //--- whole quaternion calculation
	Quaternion_Multiple(q_z,q_x);

    if (flag == 1)
       Quaternion_Multiple(q_z,q_alpha);
 
	

	//if (first_flag==1)
	//{
	//	temp = 0;
	//	for (i=0;i<4;i++)
	//	{
	//		ErrPos[i] =  q_z[i]-pre_obs_quat[i];
	//		ErrNeg[i] =  q_z[i]+pre_obs_quat[i];
	//		temp = temp+ErrPos[i]*ErrPos[i]-ErrNeg[i]*ErrNeg[i];
	//	}
	//	if(temp>0)
	//	{
	//		q_z[0] = -q_z[0];
	//		q_z[1] = -q_z[1];
	//		q_z[2] = -q_z[2];
	//		q_z[3] = -q_z[3];
	//	}
	//	
	//	//FQA_to_Bodyframe(q_z);
	//}

	Quat_Normalization(q_z);
	for (int ii=0;ii<4;ii++)
	{
		Quat_Result[ii] = q_z[ii];
	//	obs_quat[i] = pre_obs_quat[i];
	}

}

//void Fusion::FQA(double *Meas_Normalized, double *Quat_Result)
//{
//	int i;
//	double th_0 = 0.2; // A threshold to determine whether cos(Pitch) is near zero.
//	double th_zero = 0.002;
//	double alpha = 0.2*PI; // The offset to rotate Acc and Mag when cos(Pitch) is near ZERO
//	double q_alpha[4] ={0,sin(0.5*alpha),0,cos(0.5*alpha) };
//	double Quat_Pitch[4] ={0,1,0,1};
//	double Quat_Roll[4] ={1,0,0,1};
//	double Quat_Yaw[4] ={0,0,1,1};
//	int flag = 0;
//	double Mag_Sig[3]; // Magnetic signal
//	double Mag_Ear[3]; // Magnetic reference in the reference frame, X and Y part
//	double temp;
//	double sin_Pitch,sin_Roll,sin_Yaw,cos_Pitch,cos_Roll,cos_Yaw;
//	double sin_PitchHalf,sin_RollHalf,sin_YawHalf,cos_PitchHalf,cos_RollHalf,cos_YawHalf;
//	double Acc[3],Mag[3];
//
//	// Acc and mag signal
//	for( i=0;i<3;i++)
//	{
//		Acc[i] = Meas_Normalized[i];
//		Mag[i] = Meas_Normalized[i+6];
//		Mag_Sig[i] = Mag[i]; // Magnetic signal
//	}
//
//	//--- Pitch/elevation quaternion calculation
//	sin_Pitch = -Acc[0];
//	cos_Pitch = sqrt(1-sin_Pitch*sin_Pitch);
//
//	if(fabs(cos_Pitch)<th_0)   // to determine whether cos(Pitch) is near zero
//	{
//		Quaternion_VecRot(q_alpha,Acc);
//		Quaternion_VecRot(q_alpha,Mag);
//		flag = 1;
//		sin_Pitch = -Acc[0];
//		cos_Pitch = sqrt(1-sin_Pitch*sin_Pitch);
//	}
//
//	
//	//--- Pitch/elevation quaternion calculation
//	if (sin_Pitch==0)
//	{
//		if(cos_Pitch>0)
//		{
//			sin_PitchHalf=0;
//			cos_Pitch=1;
//		}
//		else if(cos_Pitch<0)
//		{
//			sin_PitchHalf=1;
//			cos_Pitch=-1;
//		}
//	}	
//	else
//		sin_PitchHalf = Get_Sign(sin_Pitch)*(sqrt(0.5*(1-cos_Pitch)));
//	cos_PitchHalf = sqrt((0.5*(1+cos_Pitch)));
//	Quat_Pitch[1] = sin_PitchHalf;
//	Quat_Pitch[3] = cos_PitchHalf;
//
//    //--- Roll/bank quaternion calculation
//	 sin_Roll = Acc[1]/cos_Pitch;
//     cos_Roll = Acc[2]/cos_Pitch;
//
//	if(sin_Roll==0)
//	{
//		if(cos_Roll > 0)
//		{
//			sin_RollHalf = 0;
//			cos_Roll = 1;
//		}
//		else if(cos_Roll < 0)
//		{
//			sin_RollHalf = 1;
//			cos_Roll = -1;
//		}
//	}
//    else if (fabs(sin_Roll) > th_zero)
//		sin_RollHalf = Get_Sign(sin_Roll)*(sqrt(0.5*(1-cos_Roll)));
//    else
//        sin_RollHalf = sqrt((0.5*(1-cos_Roll)));
//    cos_RollHalf=sqrt(0.5*(1+cos_Roll));
//	Quat_Roll[0] = sin_RollHalf;
//	Quat_Roll[3] = cos_RollHalf;
//
//
//	//--- yaw/bank quaternion calculation
//	Quaternion_Multiple(Quat_Pitch,Quat_Roll);
//	Quaternion_VecRot(Quat_Pitch,Mag_Sig);
//    temp = sqrt(Mag_Ref[1]*Mag_Ref[1]+Mag_Ref[0]*Mag_Ref[0]);
//	for (i=0;i<2;i++)
//	{
//	   Mag_Ear[i] = Mag_Ref[i]/temp;
//	}
//    
//	temp = sqrt(Mag_Sig[1]*Mag_Sig[1]+Mag_Sig[0]*Mag_Sig[0]);
//	for (i=0;i<2;i++)
//	{
//	   Mag_Sig[i] = Mag_Sig[i]/temp;
//	}
//
//	cos_Yaw =  Mag_Sig[0]*Mag_Ear[0]+Mag_Sig[1]*Mag_Ear[1];
//	sin_Yaw = -Mag_Sig[1]*Mag_Ear[0]+Mag_Sig[0]*Mag_Ear[1];
//
//   if(sin_Yaw==0)
//	{
//		if(cos_Yaw>0)
//		{
//			sin_YawHalf=0;
//			cos_Yaw=1;
//		}
//		else if(cos_Yaw<0)
//		{
//			sin_YawHalf=1;
//			cos_Yaw=-1;
//		}
//	}
//    else if (fabs(sin_Yaw) > th_zero)
//		sin_YawHalf = Get_Sign(sin_Yaw)*(sqrt(0.5*(1-cos_Yaw)));
//    else
//        sin_YawHalf = sqrt((0.5*(1-cos_Yaw)));
//
//    cos_YawHalf = sqrt((0.5*(1+cos_Yaw)));
//	Quat_Yaw[2] = sin_YawHalf;
//	Quat_Yaw[3] = cos_YawHalf;
//
//	//--- whole quaternion calculation
//	Quaternion_Multiple(Quat_Yaw,Quat_Pitch);
//
//    if (flag == 1)
//       Quaternion_Multiple(Quat_Yaw,q_alpha);
//
//	Quat_Normalization(Quat_Yaw);
//	for (i=0; i<4; i++)
//	{
//		Quat_Result[i] = Quat_Yaw[i];
//	}
//}

void Fusion::Quat_Normalization(double *Quat_Result)
// Quaternion normalization
{
	double Quat_Norm;
	Quat_Norm = Quat_Result[0]*Quat_Result[0]+Quat_Result[1]*Quat_Result[1]+Quat_Result[2]*Quat_Result[2]+Quat_Result[3]*Quat_Result[3];
	Quat_Norm = sqrt(Quat_Norm);

	for (int i=0;i<4;i++)
	{
		Quat_Result[i] = Quat_Result[i]/Quat_Norm;
	}
}

void Fusion::Meas_Normalization(double *Meas_Result)
// Measurement normailization
{
	int i;
	double Acc[3],Mag[3];
    for(i=0; i<3; i++)
	{
		Acc[i] = Meas_Result[i];
		Mag[i] = Meas_Result[i+6];
	}
	double norm_Acc = Get_VecNorm(Acc);
	double norm_Mag = Get_VecNorm(Mag);
	for( i=0; i<3; i++)
	{
		Meas_Result[i]= Acc[i]/norm_Acc;
		Meas_Result[i+6]= Mag[i]/norm_Mag;
		//norm_measure[i+3] = measure[i+3]-angular_initial[i];//角速度去漂移
	}
}

double Fusion::Get_VecNorm(double *object)
// Get the norm of a vector
{
	double norm;
	norm = object[0]*object[0]+object[1]*object[1]+object[2]*object[2];
	norm = sqrt(norm);
	return(norm);
}

void Fusion::Vect_Normalization(double *Vect_Result)
// Measurement normailization
{
	int i;
	double Vect_Temp[DIM_VEC];
    for(i=0; i<3; i++)
	{
		Vect_Temp[i] = Vect_Result[i];
	}
	double norm_Vect = Get_VecNorm(Vect_Temp);
	for( i=0; i<3; i++)
	{
		Vect_Result[i]= Vect_Temp[i]/norm_Vect;
	}
}
void Fusion::Quaternion_Augment(double Quat_ResAug[][DIM_QUAT],double Quat_Result[][DIM_QUAT]){

	int i_a;
	int j_a;
	double quat_temp1[4]={0,0,0,1},quat_temp2[4]={0,0,0,1};
	
	double quat_temp[DIM_QUAT];
	for(j_a=0; j_a<DIM_QUAT; j_a++) 
	{
		quat_temp[j_a] = Quat_Result[5][j_a];
	}
	quat_temp1[0] = 0;
	quat_temp1[1] = 0;
	quat_temp1[2] = sin(PI/4);
	quat_temp1[3] = cos(PI/4);
	FrameTrans_GB2MA(quat_temp,quat_temp1);

	for(j_a=0; j_a<DIM_QUAT; j_a++) 
	{
		Quat_ResAug[0][j_a] = quat_temp[j_a]; 
	}

	for(j_a=0; j_a<DIM_QUAT; j_a++) 
	{
		quat_temp[j_a] = Quat_Result[5][j_a];
	}
	quat_temp1[0] = 0;
	quat_temp1[1] = 0;
	quat_temp1[2] = sin(NPI/4);
	quat_temp1[3] = cos(NPI/4);
	FrameTrans_GB2MA(quat_temp,quat_temp1);
	for(j_a=0; j_a<DIM_QUAT; j_a++) 
	{
		Quat_ResAug[1][j_a] = quat_temp[j_a];
	}
}

void Fusion::Quaternion_Augment2(double Quat_Result[][DIM_QUAT]){
	int i_a;
	int j_a;
	double quat_temp1[4]={0,0,0,1},quat_temp2[4]={0,0,0,1};

	double quat_temp[DIM_QUAT];
	for(j_a=0; j_a<DIM_QUAT; j_a++) 
	{
		quat_temp[j_a] = Quat_Result[5][j_a];
	}
	quat_temp1[0] = 0;
	quat_temp1[1] = 0;
	quat_temp1[2] = sin(PI/4);
	quat_temp1[3] = cos(PI/4);
	FrameTrans_GB2MA(quat_temp,quat_temp1);

	for(j_a=0; j_a<DIM_QUAT; j_a++) 
	{
		Quat_Result[16][j_a] = quat_temp[j_a]; 
	}

	for(j_a=0; j_a<DIM_QUAT; j_a++) 
	{
		quat_temp[j_a] = Quat_Result[5][j_a];
	}
	quat_temp1[0] = 0;
	quat_temp1[1] = 0;
	quat_temp1[2] = sin(NPI/4);
	quat_temp1[3] = cos(NPI/4);
	FrameTrans_GB2MA(quat_temp,quat_temp1);
	for(j_a=0; j_a<DIM_QUAT; j_a++) 
	{
		Quat_Result[17][j_a] = quat_temp[j_a];
	}
}