//#include "stdafx.h"
#include "DisplacementFusion.h"

//using namespace std;

DisplacementFusion::DisplacementFusion()
//���캯��
{
	cur_quat_pelvis[0] = 0.0;
	cur_quat_pelvis[1] = 0.0;
	cur_quat_pelvis[2] = 0.0;
	cur_quat_pelvis[3] = 1.0;//������Ԫ����ʼ��

	cur_quat_Lthigh[0] = 0.0;
	cur_quat_Lthigh[1] = 0.0;
	cur_quat_Lthigh[2] = 0.0;
	cur_quat_Lthigh[3] = 1.0;//�������Ԫ����ʼ��

	cur_quat_Rthigh[0] = 0.0;
	cur_quat_Rthigh[1] = 0.0;
	cur_quat_Rthigh[2] = 0.0;
	cur_quat_Rthigh[3] = 1.0;//�Ҵ�����Ԫ����ʼ��

	cur_quat_Lcalf[0] = 0.0;
	cur_quat_Lcalf[1] = 0.0;
	cur_quat_Lcalf[2] = 0.0;
	cur_quat_Lcalf[3] = 1.0;//��С����Ԫ����ʼ��

	cur_quat_Rcalf[0] = 0.0;
	cur_quat_Rcalf[1] = 0.0;
	cur_quat_Rcalf[2] = 0.0;
	cur_quat_Rcalf[3] = 1.0;//��С����Ԫ����ʼ��

	cur_quat_Lfoot[0] = 0.0;
	cur_quat_Lfoot[1] = 0.0;
	cur_quat_Lfoot[2] = 0.0;
	cur_quat_Lfoot[3] = 1.0;//�����Ԫ����ʼ��

	cur_quat_Rfoot[0] = 0.0;
	cur_quat_Rfoot[1] = 0.0;
	cur_quat_Rfoot[2] = 0.0;
	cur_quat_Rfoot[3] = 1.0;//�ҽ���Ԫ����ʼ��

	acc_Lthigh[0] = 0.0;//����ȵļ��ٶȳ�ʼ��
	acc_Lthigh[1] = 0.0;
	acc_Lthigh[2] = 0.0;
	acc_Lcalf[0] = 0.0;//��С�ȵļ��ٶȳ�ʼ��
	acc_Lcalf[1] = 0.0;
	acc_Lcalf[2] = 0.0;
	acc_Rthigh[0] = 0.0;//�Ҵ��ȵļ��ٶȳ�ʼ��
	acc_Rthigh[1] = 0.0;
	acc_Rthigh[2] = 0.0;
	acc_Rcalf[0] = 0.0;//��С�ȵļ��ٶȳ�ʼ��
	acc_Rcalf[1] = 0.0;
	acc_Rcalf[2] = 0.0;
	acc_Lfoot[0] = 0.0;//��ŵļ��ٶȳ�ʼ��
	acc_Lfoot[1] = 0.0;
	acc_Lfoot[2] = 0.0;
	acc_Rfoot[0] = 0.0;//�ҽŵļ��ٶȳ�ʼ��
	acc_Rfoot[1] = 0.0;
	acc_Rfoot[2] = 0.0;
	acc_pelvis[0] = 0.0;//�����ļ��ٶȳ�ʼ��
	acc_pelvis[1] = 0.0;
	acc_pelvis[2] = 0.0;

	angular_Lthigh[0] = 0.0;//����ȵĽ��ٶȳ�ʼ��
	angular_Lthigh[1] = 0.0;
	angular_Lthigh[2] = 0.0;
	angular_Lcalf[0] = 0.0;//��С�ȵĽ��ٶȳ�ʼ��
	angular_Lcalf[1] = 0.0;
	angular_Lcalf[2] = 0.0;
	angular_Rthigh[0] = 0.0;//�Ҵ��ȵĽ��ٶȳ�ʼ��
	angular_Rthigh[1] = 0.0;
	angular_Rthigh[2] = 0.0;
	angular_Rcalf[0] = 0.0;//�Ҵ��ȵĽ��ٶȳ�ʼ��
	angular_Rcalf[1] = 0.0;
	angular_Rcalf[2] = 0.0;
	angular_Lfoot[0] = 0.0;//��ŵĽ��ٶȳ�ʼ��
	angular_Lfoot[1] = 0.0;
	angular_Lfoot[2] = 0.0;
	angular_Rfoot[0] = 0.0;//�ҽŵĽ��ٶȳ�ʼ��
	angular_Rfoot[1] = 0.0;
	angular_Rfoot[2] = 0.0;	
	angular_pelvis[0] = 0.0;//�����Ľ��ٶȳ�ʼ��
	angular_pelvis[1] = 0.0;
	angular_pelvis[2] = 0.0;

	Root_pos[0] = 0.0;//���ڵ��λ�Ƴ�ʼ�� 
	Root_pos[1] = 0.0;
	Root_pos[2] = -0.93;

	pre_aPos[0] = 0.0;//ǰһ��absolute position of the anchor��ʼ��
	pre_aPos[1] = 0.0;
	pre_aPos[2] = 0.0;

	cur_aPos[0] = 0.0;//��ǰabsolute position of the anchor��ʼ��
	cur_aPos[1] = 0.0;
	cur_aPos[2] = 0.0;

	pre_Lanklepos[0] = 0.0;//ǰһ�� position of Lankle��ʼ��
	pre_Lanklepos[1] = 0.0;//ǰһ�� position of Lankle��ʼ��
	pre_Lanklepos[2] = 0.0;//ǰһ�� position of Lankle��ʼ��

	pre_Ranklepos[0] = 0.0;//ǰһ�� position of Rankle��ʼ��
	pre_Ranklepos[1] = 0.0;//ǰһ�� position of Rankle��ʼ��
	pre_Ranklepos[2] = 0.0;//ǰһ�� position of Rankle��ʼ��

	cur_Lanklepos[0] = 0.0;//��ǰ position of Lankle��ʼ��
	cur_Lanklepos[1] = 0.0;//��ǰ position of Lankle��ʼ��
	cur_Lanklepos[2] = 0.0;//��ǰ position of Lankle��ʼ��

	cur_Ranklepos[0] = 0.0;//��ǰ position of Rankle��ʼ��
	cur_Ranklepos[1] = 0.0;//��ǰ position of Rankle��ʼ��
	cur_Ranklepos[2] = 0.0;//��ǰ position of Rankle��ʼ��

	counterL = 0;//Llogical�ļ�����ʼ��
	counterR = 0;//Rlogical�ļ�����ʼ��

	countR_acc = 0; //�ҽż��ٶ����Rlogical�ļ�����ʼ��
	countR_gyr = 0; //�ҽŽ��ٶ����Rlogical�ļ�����ʼ��
	countL_acc = 0; //��ż��ٶ����Llogical�ļ�����ʼ��
	countL_gyr = 0; //�ҽŽ��ٶ����Llogical�ļ�����ʼ��

	curRslogical = -1;//����stance�׶εı�־
	curLslogical = -1;//����stance�׶εı�־

	preRslogical = -1;//����stance�׶εı�־
	preLslogical = -1;//����stance�׶εı�־

	preSupportLeg = -1;//ǰһʱ��support leg
	curSupportLeg = -1;//��ǰʱ��support leg

	isfirstL = true; //��ŵ�һ�ν���swing�׶�
	isfirstR = true; //�ҽŵ�һ�ν���swing�׶�

}

DisplacementFusion::~DisplacementFusion()
//��������
{
}

bool DisplacementFusion::SetPara(int id, double *quat, double *meas)
//set the parameters need in the DisplacementFusion estimation
{
	switch(id)
	{
	case 1://���������Ԫ�������ٶȺͽ��ٶ�

		cur_quat_pelvis[0] = quat[0];
		cur_quat_pelvis[1] = quat[1];
		cur_quat_pelvis[2] = quat[2];
		cur_quat_pelvis[3] = quat[3];

		acc_pelvis[0] = meas[0];
		acc_pelvis[1] = meas[1];
		acc_pelvis[2] = meas[2];
		angular_pelvis[0] = meas[3];
		angular_pelvis[1] = meas[4];
		angular_pelvis[2] = meas[5];

		//CST_MA2GB_W(cur_quat_pelvis);//��Ԫ��ת����NED����ϵ

		break;

	case 19://����Ҵ�����Ԫ�������ٶȺͽ��ٶ�

		cur_quat_Rthigh[0] = quat[0];
		cur_quat_Rthigh[1] = quat[1];
		cur_quat_Rthigh[2] = quat[2];
		cur_quat_Rthigh[3] = quat[3];

		acc_Rthigh[0] = meas[0];
		acc_Rthigh[1] = meas[1];
		acc_Rthigh[2] = meas[2];
		angular_Rthigh[0] = meas[3];
		angular_Rthigh[1] = meas[4];
		angular_Rthigh[2] = meas[5];

		//CST_MA2GB_LF(cur_quat_Rthigh);//��Ԫ��ת����NED����ϵ

		break;

	case 20://��С����Ԫ�������ٶȺͽ��ٶ�

		cur_quat_Rcalf[0] = quat[0];
		cur_quat_Rcalf[1] = quat[1];
		cur_quat_Rcalf[2] = quat[2];
		cur_quat_Rcalf[3] = quat[3];

		acc_Rcalf[0] = meas[0];
		acc_Rcalf[1] = meas[1];
		acc_Rcalf[2] = meas[2];
		angular_Rcalf[0] = meas[3];
		angular_Rcalf[1] = meas[4];
		angular_Rcalf[2] = meas[5];

		//CST_MA2GB_LF(cur_quat_Rcalf);//��Ԫ��ת����NED����ϵ

		break;	

	case 21://�ҽ���Ԫ�������ٶȺͽ��ٶ�

		cur_quat_Rfoot[0] = quat[0];
		cur_quat_Rfoot[1] = quat[1];
		cur_quat_Rfoot[2] = quat[2];
		cur_quat_Rfoot[3] = quat[3];

		acc_Rfoot[0] = meas[0];
		acc_Rfoot[1] = meas[1];
		acc_Rfoot[2] = meas[2];
		angular_Rfoot[0] = meas[3];
		angular_Rfoot[1] = meas[4];
		angular_Rfoot[2] = meas[5];

		//CST_MA2GB_LF(cur_quat_Rfoot);//��Ԫ��ת����NED����ϵ

		break;		


	case 15://�������Ԫ�������ٶȺͽ��ٶ�

		cur_quat_Lthigh[0] = quat[0];
		cur_quat_Lthigh[1] = quat[1];
		cur_quat_Lthigh[2] = quat[2];
		cur_quat_Lthigh[3] = quat[3];	

		acc_Lthigh[0] = meas[0];
		acc_Lthigh[1] = meas[1];
		acc_Lthigh[2] = meas[2];
		angular_Lthigh[0] = meas[3];
		angular_Lthigh[1] = meas[4];
		angular_Lthigh[2] = meas[5];

		//CST_MA2GB_LF(cur_quat_Lthigh);//��Ԫ��ת����NED����ϵ

		break;

	case 16://��С����Ԫ�������ٶȺͽ��ٶ�

		cur_quat_Lcalf[0] = quat[0];
		cur_quat_Lcalf[1] = quat[1];
		cur_quat_Lcalf[2] = quat[2];
		cur_quat_Lcalf[3] = quat[3];

		acc_Lcalf[0] = meas[0];
		acc_Lcalf[1] = meas[1];
		acc_Lcalf[2] = meas[2];
		angular_Lcalf[0] = meas[3];
		angular_Lcalf[1] = meas[4];
		angular_Lcalf[2] = meas[5];	

		//CST_MA2GB_LF(cur_quat_Lcalf);//��Ԫ��ת����NED����ϵ

		break;

	case 17://�����Ԫ�������ٶȺͽ��ٶ�

		cur_quat_Lfoot[0] = quat[0];
		cur_quat_Lfoot[1] = quat[1];
		cur_quat_Lfoot[2] = quat[2];
		cur_quat_Lfoot[3] = quat[3];

		acc_Lfoot[0] = meas[0];
		acc_Lfoot[1] = meas[1];
		acc_Lfoot[2] = meas[2];
		angular_Lfoot[0] = meas[3];
		angular_Lfoot[1] = meas[4];
		angular_Lfoot[2] = meas[5];

		//CST_MA2GB_LF(cur_quat_Lfoot);//��Ԫ��ת����NED����ϵ

		break;	

	default:
		break;

	}

	return 1;//������

}

//estimate the displacement
bool DisplacementFusion::EstimateDis(double *result)
//////////compute the dispalcement in tridimensional space////////////////////////
/*	//position estimation
/*  // 1.����ϵΪNED��X��ǰ��y���ң�z���¡��Գ�ʼʱ�̽��׹ؽ���Ϊ����ԭ��.
/*	// 2.���ü��ٶȺͽ��ٶȹ�ͬ�ж�stance�׶�,����stance�׶ε��׹ؽڴ�����͵�.
/*	// 3.�ý�stance����,��֧�Ž�,��ֻ�Ŵ���stance�׶�,��ʱ��ֻ�ŵ��׹ؽڴ�����͵�.
/*	// 4.�������ʱ�̸��ؽ�����ڸ��ڵ��λ�� 
/*  // 5.���λ����Ҫת����ģ������ϵ
////////////////////////////////////////////////////////////////////////////////*/
{
	//������������ʼ��
	//////////////////////////////////////////////////////////////////////////////////////////
	double thighlen = 0.43; // ���ȳ���
	double shanklen = 0.50; //С�ȳ���

	double separate_angle = 0.0;//�������ȵļн�
	double separate_angleL = 0.0;//������봹ֱ����ļн�
	double separate_angleR = 0.0;//�Ҵ����봹ֱ����ļн�
	//��������·��������ת����
	double vec_Lthigh_rotation[3], vec_Rthigh_rotation[3], vec_Lcalf_rotation[3], vec_Rcalf_rotation[3];

	//��ǰ���С����ת֮���������ʼ��
	vec_Lcalf_rotation[0] = 0.0;
	vec_Lcalf_rotation[1] = 0.0;
	vec_Lcalf_rotation[2] = -shanklen;
	vec_Lthigh_rotation[0] = 0.0;
	vec_Lthigh_rotation[1] = 0.0;
	vec_Lthigh_rotation[2] = -thighlen;

	//��ǰ�Ҵ�С����ת֮���������ʼ��
	vec_Rcalf_rotation[0] = 0.0;
	vec_Rcalf_rotation[1] = 0.0;
	vec_Rcalf_rotation[2] = -shanklen;
	vec_Rthigh_rotation[0] = 0.0;
	vec_Rthigh_rotation[1] = 0.0;
	vec_Rthigh_rotation[2] = -thighlen;


	double accsq_thredH = 120;//���ٶ�ƽ���͵���ֵ������stance�׶ε��ж�
	double accsq_thredL = 80;//���ٶ�ƽ���͵���ֵ������stance�׶ε��ж�

	double gyrsq_thred = 2; //�ҽŽ��ٶ���ֵ������stance�׶ε��ж�

	double pos_KneeL[3] = {0.0, 0.0,-shanklen};//��ϥ�ؽڵ�λ��
	double pos_KneeR[3] = {0.0, 0.0,-shanklen};//��ϥ�ؽڵ�λ��
	double pos_Hip[3] = {0.0, 0.0, -(thighlen+shanklen)};//�Źؽڵ�λ��

	double accSqL = 0.0; //��ż��ٶ�ƽ����
	double accSqR = 0.0; //�ҽż��ٶ�ƽ����
	double gyrSqL = 0.0; //��Ž��ٶ�ƽ����
	double gyrSqR = 0.0; //�ҽŽ��ٶ�ƽ����

	int Rslogical_acc =-1;//���ٶ���õ��ҽ��߼���־
	int Lslogical_acc =-1;//���ٶ���õ�����߼���־
	int Rslogical_gyr =-1;//���ٶ���õ��ҽ��߼���־
	int Lslogical_gyr =-1;//���ٶ���õ�����߼���־


	//////////////////////////////////////////////////////////////////////
	//��ǰ���Ҵ�С�ȵ���ת֮�������,��body����ϵ��global����ϵ
	///////////////////////////////////////////////////////////////////////

	Quaternion_Rotation(cur_quat_Lthigh,vec_Lthigh_rotation);//�����
	Quaternion_Rotation(cur_quat_Rthigh,vec_Rthigh_rotation);//�Ҵ���
	Quaternion_Rotation(cur_quat_Lcalf,vec_Lcalf_rotation);//��С��
	Quaternion_Rotation(cur_quat_Rcalf,vec_Rcalf_rotation);//��С��

	//////////////////////////////////////// ���stance�׶�/////////////////////////////////////////
	//����ֻ��������ٶ�ƽ���ͣ�������ٶ�ƽ���ͣ����stance�׶Σ�����stance�׶νŵ��׹ؽڴ�����͵�
	////////////////////////////////////////////////////////////////////////////////////////////////
	accSqL = SQ_vector(acc_Lfoot);//������ż��ٶ�ƽ����
	accSqR = SQ_vector(acc_Rfoot);//�����ҽż��ٶ�ƽ����

	gyrSqL = SQ_vector(angular_Lfoot);//������Ž��ٶ�ƽ����
	gyrSqR = SQ_vector(angular_Rfoot);//�����ҽŽ��ٶ�ƽ����

	//int firstmove = 0; //��һ���˶����

	////////////////////////////////////////////////////////////////////////////////////
	//           ���ٶȼ��stance,����ֵ������Ϊ0��stance;֮����Ϊ1��swing
	////////////////////////////////////////////////////////////////////////////////////
	if(accSqR > accsq_thredL && accSqR < accsq_thredH) //�ҽ�
	{	
		Rslogical_acc = 0;//Stance
	}
	else 
	{
		Rslogical_acc = 1;//swing
	}
	if(accSqL > accsq_thredL && accSqL < accsq_thredH) //���
	{	
		Lslogical_acc = 0;//stance
	}
	else 
	{
		Lslogical_acc = 1;//swing
	}
	//////////////////////////////////////////////////////////////////////////////////
	//       ���ٶȼ��stance,����ֵ������Ϊ0��stance;֮����Ϊ1��swing.
	//////////////////////////////////////////////////////////////////////////////////
	if(gyrSqR < gyrsq_thred)//�ҽ�
	{	
		Rslogical_gyr = 0;//stance
	}
	else 
	{
		Rslogical_gyr = 1;//swing
	}
	if(gyrSqL < gyrsq_thred)//���
	{	
		Lslogical_gyr = 0;//stance
	}
	else 
	{
		Lslogical_gyr = 1;//swing
	}

	////////////////////////////////////////////////////////////////////////////////////
	//                 �ɼ��ٶȺͽ��ٶȹ�ͬ����Rslogical��Lslogical
	////////////////////////////////////////////////////////////////////////////////////

	// �ҽţ�1&&1==1��1&&0==1��0&&1==1��0&&0=0�����accΪ0��gyrΪ1���Ҽ�������20���Լ��ٶȵ�Ϊ׼
	if(Rslogical_acc==0)
	{
		countR_acc = countR_acc+1;
		if(Rslogical_gyr==0) 
		{
			countR_gyr = countR_gyr+1;
			curRslogical = 0;
		}
		else if(Rslogical_gyr==1) 
		{
			if(countR_acc>20 || countR_gyr>20) 
			{
				curRslogical = 0;
			}
			else 
			{
				curRslogical = 1;
			}
			countR_gyr = 0;
		}
	}
	else if(Rslogical_acc==1)
	{
		countR_acc = 0;
		if(Rslogical_gyr==0) 
		{
			countR_gyr = countR_gyr+1;
			curRslogical = 1;
		}
		else if(Rslogical_gyr==1)
		{
			countR_gyr = 0;
			curRslogical = 1;
		}
	}     

	//��ţ�1&&1==1��1&&0==1��0&&1==1��0&&0=0 ���accΪ0��gyrΪ1���Ҽ�������20���Լ��ٶȵ�Ϊ׼
	if(Lslogical_acc==0)
	{
		countL_acc = countL_acc+1;
		if(Lslogical_gyr==0)
		{
			countL_gyr = countL_gyr+1;
			curLslogical = 0;
		}
		else if(Lslogical_gyr==1)
		{
			if(countL_acc>20 || countL_gyr>20) 
			{
				curLslogical = 0;
			}
			else 
			{
				curLslogical = 1;
			}
			countL_gyr = 0;
		}
	}
	else if(Lslogical_acc==1)
	{
		countL_acc = 0;
		if(Lslogical_gyr==0)
		{
			countL_gyr = countL_gyr+1;
			curLslogical = 1;
		}
		else if(Lslogical_gyr==1) 
		{
			countL_gyr = 0;
			curLslogical = 1;
		}   
	}
	///////////////////////////////////////////////////////////////////////////////
	//                              �ж�support_leg
	///////////////////////////////////////////////////////////////////////////////
    //0&&0; 0&&1; 1&&0; 1&&1 
	if (curLslogical == 0)
	{
		counterL = counterL+1;        
		if (curRslogical == 0)
		{
			counterR = counterR+1;
			if (counterR < counterL)
			{
				curSupportLeg = 1;
			}
			else if (counterR > counterL)
			{
				curSupportLeg = 2;
			}
			else if (counterR == counterL)
			{
				if(preSupportLeg == 1)
				{
					curSupportLeg = 2;
				}
				else if(preSupportLeg==2)
				{
					curSupportLeg = 1;
				}
			}
		}
		else if (curRslogical == 1)
		{
			counterR = 0;
			curSupportLeg = 1;
		}
	}
	else if (curLslogical == 1)    
	{
		if (curRslogical == 0)
		{
			counterR = counterR+1;
			curSupportLeg = 2;
		}
		else if (curRslogical == 1) 
		{
			if (preRslogical == 0 && preLslogical == 1)
			{
				if (counterR<15)
				{
					curSupportLeg = preSupportLeg;
				}
				else
				{
					curSupportLeg = 1;
				}
			}
			else if (preLslogical == 0 && preRslogical == 1)
			{
				if (counterL<15)
				{
					curSupportLeg = preSupportLeg;
				}
				else
				{
					curSupportLeg = 2;
				}
			}
			else if (preLslogical == 1 && preRslogical == 1)
			{
				curSupportLeg = preSupportLeg;   
			}
			else if (preLslogical == 0 && preRslogical == 0)
			{
				if(preSupportLeg == 1)
				{
					curSupportLeg = 2;
				}
				else if(preSupportLeg == 2)
				{
					curSupportLeg = 1;
				}
			}                    
			counterR =0;
		}
		counterL=0;
	}


	////////////////////////////////////////////////////////////////////////////////////////
	//   �����Źؽڡ�����ϥ�ؽں������׹ؽڸ����ؽڵĵ�λ�ã����ݲ�������õ�ǰ����λ�ã�
	//   Ȼ�������ڵ��λ��	
	///////////////////////////////////////////////////////////////////////////////////////
	if (curSupportLeg == 1)
	{	
		for (int i=0; i<3; i++)
		{
			pos_KneeL[i] = vec_Lcalf_rotation[i];
			pos_Hip[i] = pos_KneeL[i] + vec_Lthigh_rotation[i];
			pos_KneeR[i] = pos_Hip[i] - vec_Rthigh_rotation[i]; 
			cur_Ranklepos[i] = pos_KneeR[i] - vec_Rcalf_rotation[i];
		}

		if(preSupportLeg == 2)
		{
			for(int i=0; i<3; i++)
			{
				cur_aPos[i] = pre_aPos[i] + pre_Lanklepos[i];// - pre_Ranklepos[i];
				if(i == 2)
					cur_aPos[i] = 0.0;					
				Root_pos[i] = pos_Hip[i] + cur_aPos[i];
			}	
		}
		else
		{
			for(int j=0; j<3; j++)
			{
				cur_aPos[j] = pre_aPos[j];
				Root_pos[j] = pos_Hip[j] + cur_aPos[j];
			}
		}

	}
	else if (curSupportLeg == 2)
	{
		for (int i=0; i<3; i++)
		{
			pos_KneeR[i] = vec_Rcalf_rotation[i];
			pos_Hip[i] = pos_KneeR[i] + vec_Rthigh_rotation[i];
			pos_KneeL[i] = pos_Hip[i] - vec_Lthigh_rotation[i];
			cur_Lanklepos[i] = pos_KneeL[i] - vec_Lcalf_rotation[i];
		}
		if(preSupportLeg == 1)
		{
			for(int i=0; i<3; i++)
			{
				cur_aPos[i] = pre_aPos[i] + pre_Ranklepos[i];// - pre_Lanklepos[i];
				if(i == 2)
					cur_aPos[i] = 0.0;					
				Root_pos[i] = pos_Hip[i] + cur_aPos[i];
			}	
		}
		else
		{
			for(int j=0; j<3; j++)
			{
				cur_aPos[j] = pre_aPos[j];
				Root_pos[j] = pos_Hip[j] + cur_aPos[j];
			}
		}
	}		
	else if (curSupportLeg == 0) 
	{
		for (int i=0; i<3; i++)
		{
			pos_KneeR[i] += 0.0;
			pos_Hip[i] += 0.0;
			pos_KneeL[i] += 0.0;
			cur_Lanklepos[i] = pre_Lanklepos[i];
			cur_Ranklepos[i] = pre_Ranklepos[i];
			cur_aPos[i] = pre_aPos[i];
			Root_pos[i] = pos_Hip[i] + cur_aPos[i]; 
		}
	}

	//���������result��
	result[0] = Root_pos[0];
	result[1] = Root_pos[1];
	result[2] = Root_pos[2];

	//result[2] += 0.93;//��ֱ������λ�Ƶı仯��

	//CST_GB2MA_Loc(result);//λ����ת��ģ������ϵ

	////////////////////���±���//////////////////////////////
	for (int i=0; i<3; i++)
	{
		pre_aPos[i] = cur_aPos[i];//����λ��
		pre_Ranklepos[i] = cur_Ranklepos[i];//�ҽ��׵�λ��
		pre_Lanklepos[i] = cur_Lanklepos[i];//����׵�λ��
	}
	preSupportLeg = curSupportLeg;//support_leg
	preRslogical = curRslogical;//�ҽ��߼�
	preLslogical = curLslogical;//����߼�

	return 1;//������
}	

//quaternion multiple
void DisplacementFusion::Quaternion_Multiple(double *a,double *b)
{
	double temp1=a[3]*b[3]-a[0]*b[0]-a[1]*b[1]-a[2]*b[2];
	double temp2=a[3]*b[0]+b[3]*a[0]+a[1]*b[2]-b[1]*a[2];
	double temp3=a[3]*b[1]+b[3]*a[1]+a[2]*b[0]-b[2]*a[0];
	double temp4=a[3]*b[2]+b[3]*a[2]+a[0]*b[1]-b[0]*a[1];
	a[0]=temp2;
	a[1]=temp3;
	a[2]=temp4;
	a[3]=temp1;

}
//quaternion rotation from BCS to GCS
void DisplacementFusion::Quaternion_Rotation(double *a, double *vctors)
{
	double aug_vc[]={vctors[0],vctors[1],vctors[2],0};
	double aa[] = {a[0],a[1],a[2],a[3]};
	double inva[]={-a[0],-a[1],-a[2],a[3]};
	Quaternion_Multiple(aug_vc,inva);
	Quaternion_Multiple(aa,aug_vc);
	for (int i=0;i<3;i++)
	{
		vctors[i]=aa[i];
	}

}
//Get acceleration square sum
double DisplacementFusion::SQ_vector(double *object)
{
	double SQ;
	SQ = object[0]*object[0]+object[1]*object[1]+object[2]*object[2];
	return(SQ);
}

void DisplacementFusion::CST_MA2GB_W(double *result)
// The classname is the own class name of position estimation.
// CST = Coordinate System Transfer
// MA = The CS defined by model display system, where M means model, A means avatar 
// GB = The CS of NED.
// W = Waist.
{
	double temp[4] = {result[0],result[1],result[2],result[3]};
	double temp2[4];
	double quatMG[4];
	double quat_temp1[4]={0,0,0,1}, quat_temp2[4]={0,0,0,1};
	int i,j;

	//ax = -pi/4; % angle x
	//ay = -pi/4; % angle x
	// qx = cos(ax)*[0;0;0;1]+sin(az)*[1;0;0;0];
	// qy = cos(ay)*[0;0;0;1]+sin(ay)*[0;1;0;0];
	// quat_temp1 = qx, quat_temp2 = qy.
	quat_temp1[0] = sin(-PI_/4);
	quat_temp1[1] = 0;
	quat_temp1[2] = 0;
	quat_temp1[3] = cos(-PI_/4);

	quat_temp2[0] = 0;
	quat_temp2[1] = sin(-PI_/4);
	quat_temp2[2] = 0;
	quat_temp2[3] = cos(-PI_/4);

	// qMG = qmult(qx,qy).
	Quaternion_Multiple(quat_temp1,quat_temp2);

	// qMG
	for(j=0; j<4; j++)
	{
		quatMG[j] = quat_temp1[j];
	} 

	// inv_qMG
	double inv_quatMG[4] = {-quatMG[0],-quatMG[1],-quatMG[2],quatMG[3]};

	// temp
	for(i=0; i<4; i++)
	{
		temp2[i] = inv_quatMG[i];
	}

	// qGB=qGM*qMA*qMG
	Quaternion_Multiple(temp, quatMG); // qMA*qMG
	Quaternion_Multiple(temp2,temp); // qGM*(qMA*qMG)

	for(i=0; i<4; i++)
	{
		result[i] = temp2[i];
	}
}

void DisplacementFusion::CST_MA2GB_LF(double *result)
// The classname is the own class name of position estimation.
// CST = Coordinate System Transfer
// MA = The CS defined by model display system, where M means model, A means avatar 
// GB = The CS of NED.
// LF = Leg and Foot.
{
	double temp[4] = {result[0],result[1],result[2],result[3]};
	double temp2[4];
	double quatMG[4];
	double quat_temp1[4]={0,0,0,1}, quat_temp2[4]={0,0,0,1};
	int i,j;

	//az = pi/4; % angle z
	//ax = pi/4; % angle x
	// qz = cos(az)*[0;0;0;1]+sin(az)*[0;0;1;0];
	// qx = cos(ax)*[0;0;0;1]+sin(ax)*[1;0;0;0];
	// quat_temp1 = qz, quat_temp2 = qx.
	quat_temp1[0] = 0;
	quat_temp1[1] = 0;
	quat_temp1[2] = sin(PI_/4);
	quat_temp1[3] = cos(PI_/4);

	quat_temp2[0] = sin(PI_/4);
	quat_temp2[1] = 0;
	quat_temp2[2] = 0;
	quat_temp2[3] = cos(PI_/4);

	// qMG = qmult(qz,qx).
	Quaternion_Multiple(quat_temp1,quat_temp2);

	// qMG
	for(j=0; j<4; j++)
	{
		quatMG[j] = quat_temp1[j];
	} 

	// inv_qMG
	double inv_quatMG[4] = {-quatMG[0],-quatMG[1],-quatMG[2],quatMG[3]};

	// temp
	for(i=0; i<4; i++)
	{
		temp2[i] = inv_quatMG[i];
	}

	// qGB=qGM*qMA*qMG
	Quaternion_Multiple(temp, quatMG); // qMA*qMG
	Quaternion_Multiple(temp2,temp); // qGM*(qMA*qMG)

	for(i=0; i<4; i++)
	{
		result[i] = temp2[i];
	}
}

void DisplacementFusion::CST_GB2MA_Loc(double *result)
// The classname is the own class name of position estimation.
// CST = Coordinate System Transfer
// MA = The CS defined by model display system, where M means model, A means avatar 
// GB = The CS of NED.
// Loc = Localization, position, displacement.
{
	double temp[4] = {result[0],result[1],result[2],result[3]};
	double quatMG[4];
	double quat_temp1[4]={0,0,0,1}, quat_temp2[4]={0,0,0,1};
	int j;

	//ax = pi/2; % angle x
	//az = pi/4; % angle z
	// qx = cos(ax)*[0;0;0;1]+sin(az)*[1;0;0;0];
	// qz = cos(az)*[0;0;0;1]+sin(az)*[0;0;1;0];
	// quat_temp1 = qx, quat_temp2 = qz.
	quat_temp1[0] = sin(PI_/2);
	quat_temp1[1] = 0;
	quat_temp1[2] = 0;
	quat_temp1[3] = cos(PI_/2);

	quat_temp2[0] = 0;
	quat_temp2[1] = 0;
	quat_temp2[2] = sin(PI_/4);
	quat_temp2[3] = cos(PI_/4);

	// qMG = qmult(qx,qz).
	Quaternion_Multiple(quat_temp1,quat_temp2);

	// qMG
	for(j=0; j<4; j++)
	{
		quatMG[j] = quat_temp1[j];
	} 

	// Displacement_new = qMG*Dis_old*qGM
	Quaternion_Rotation(quatMG,result);
}