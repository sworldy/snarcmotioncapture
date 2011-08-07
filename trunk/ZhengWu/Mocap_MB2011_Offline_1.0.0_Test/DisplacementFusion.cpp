//#include "stdafx.h"
#include "DisplacementFusion.h"

//using namespace std;

DisplacementFusion::DisplacementFusion()
//构造函数
{
	cur_quat_pelvis[0] = 0.0;
	cur_quat_pelvis[1] = 0.0;
	cur_quat_pelvis[2] = 0.0;
	cur_quat_pelvis[3] = 1.0;//腰部四元数初始化

	cur_quat_Lthigh[0] = 0.0;
	cur_quat_Lthigh[1] = 0.0;
	cur_quat_Lthigh[2] = 0.0;
	cur_quat_Lthigh[3] = 1.0;//左大腿四元数初始化

	cur_quat_Rthigh[0] = 0.0;
	cur_quat_Rthigh[1] = 0.0;
	cur_quat_Rthigh[2] = 0.0;
	cur_quat_Rthigh[3] = 1.0;//右大腿四元数初始化

	cur_quat_Lcalf[0] = 0.0;
	cur_quat_Lcalf[1] = 0.0;
	cur_quat_Lcalf[2] = 0.0;
	cur_quat_Lcalf[3] = 1.0;//左小腿四元数初始化

	cur_quat_Rcalf[0] = 0.0;
	cur_quat_Rcalf[1] = 0.0;
	cur_quat_Rcalf[2] = 0.0;
	cur_quat_Rcalf[3] = 1.0;//右小腿四元数初始化

	cur_quat_Lfoot[0] = 0.0;
	cur_quat_Lfoot[1] = 0.0;
	cur_quat_Lfoot[2] = 0.0;
	cur_quat_Lfoot[3] = 1.0;//左脚四元数初始化

	cur_quat_Rfoot[0] = 0.0;
	cur_quat_Rfoot[1] = 0.0;
	cur_quat_Rfoot[2] = 0.0;
	cur_quat_Rfoot[3] = 1.0;//右脚四元数初始化

	acc_Lthigh[0] = 0.0;//左大腿的加速度初始化
	acc_Lthigh[1] = 0.0;
	acc_Lthigh[2] = 0.0;
	acc_Lcalf[0] = 0.0;//左小腿的加速度初始化
	acc_Lcalf[1] = 0.0;
	acc_Lcalf[2] = 0.0;
	acc_Rthigh[0] = 0.0;//右大腿的加速度初始化
	acc_Rthigh[1] = 0.0;
	acc_Rthigh[2] = 0.0;
	acc_Rcalf[0] = 0.0;//右小腿的加速度初始化
	acc_Rcalf[1] = 0.0;
	acc_Rcalf[2] = 0.0;
	acc_Lfoot[0] = 0.0;//左脚的加速度初始化
	acc_Lfoot[1] = 0.0;
	acc_Lfoot[2] = 0.0;
	acc_Rfoot[0] = 0.0;//右脚的加速度初始化
	acc_Rfoot[1] = 0.0;
	acc_Rfoot[2] = 0.0;
	acc_pelvis[0] = 0.0;//腰部的加速度初始化
	acc_pelvis[1] = 0.0;
	acc_pelvis[2] = 0.0;

	angular_Lthigh[0] = 0.0;//左大腿的角速度初始化
	angular_Lthigh[1] = 0.0;
	angular_Lthigh[2] = 0.0;
	angular_Lcalf[0] = 0.0;//左小腿的角速度初始化
	angular_Lcalf[1] = 0.0;
	angular_Lcalf[2] = 0.0;
	angular_Rthigh[0] = 0.0;//右大腿的角速度初始化
	angular_Rthigh[1] = 0.0;
	angular_Rthigh[2] = 0.0;
	angular_Rcalf[0] = 0.0;//右大腿的角速度初始化
	angular_Rcalf[1] = 0.0;
	angular_Rcalf[2] = 0.0;
	angular_Lfoot[0] = 0.0;//左脚的角速度初始化
	angular_Lfoot[1] = 0.0;
	angular_Lfoot[2] = 0.0;
	angular_Rfoot[0] = 0.0;//右脚的角速度初始化
	angular_Rfoot[1] = 0.0;
	angular_Rfoot[2] = 0.0;	
	angular_pelvis[0] = 0.0;//腰部的角速度初始化
	angular_pelvis[1] = 0.0;
	angular_pelvis[2] = 0.0;

	Root_pos[0] = 0.0;//根节点的位移初始化 
	Root_pos[1] = 0.0;
	Root_pos[2] = -0.93;

	pre_aPos[0] = 0.0;//前一刻absolute position of the anchor初始化
	pre_aPos[1] = 0.0;
	pre_aPos[2] = 0.0;

	cur_aPos[0] = 0.0;//当前absolute position of the anchor初始化
	cur_aPos[1] = 0.0;
	cur_aPos[2] = 0.0;

	pre_Lanklepos[0] = 0.0;//前一刻 position of Lankle初始化
	pre_Lanklepos[1] = 0.0;//前一刻 position of Lankle初始化
	pre_Lanklepos[2] = 0.0;//前一刻 position of Lankle初始化

	pre_Ranklepos[0] = 0.0;//前一刻 position of Rankle初始化
	pre_Ranklepos[1] = 0.0;//前一刻 position of Rankle初始化
	pre_Ranklepos[2] = 0.0;//前一刻 position of Rankle初始化

	cur_Lanklepos[0] = 0.0;//当前 position of Lankle初始化
	cur_Lanklepos[1] = 0.0;//当前 position of Lankle初始化
	cur_Lanklepos[2] = 0.0;//当前 position of Lankle初始化

	cur_Ranklepos[0] = 0.0;//当前 position of Rankle初始化
	cur_Ranklepos[1] = 0.0;//当前 position of Rankle初始化
	cur_Ranklepos[2] = 0.0;//当前 position of Rankle初始化

	counterL = 0;//Llogical的计数初始化
	counterR = 0;//Rlogical的计数初始化

	countR_acc = 0; //右脚加速度求的Rlogical的计数初始化
	countR_gyr = 0; //右脚角速度求的Rlogical的计数初始化
	countL_acc = 0; //左脚加速度求的Llogical的计数初始化
	countL_gyr = 0; //右脚角速度求的Llogical的计数初始化

	curRslogical = -1;//右腿stance阶段的标志
	curLslogical = -1;//左腿stance阶段的标志

	preRslogical = -1;//右腿stance阶段的标志
	preLslogical = -1;//左腿stance阶段的标志

	preSupportLeg = -1;//前一时刻support leg
	curSupportLeg = -1;//当前时刻support leg

	isfirstL = true; //左脚第一次进入swing阶段
	isfirstR = true; //右脚第一次进入swing阶段

}

DisplacementFusion::~DisplacementFusion()
//析构函数
{
}

bool DisplacementFusion::SetPara(int id, double *quat, double *meas)
//set the parameters need in the DisplacementFusion estimation
{
	switch(id)
	{
	case 1://获得腰部四元数、加速度和角速度

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

		//CST_MA2GB_W(cur_quat_pelvis);//四元数转换到NED坐标系

		break;

	case 19://获得右大腿四元数、加速度和角速度

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

		//CST_MA2GB_LF(cur_quat_Rthigh);//四元数转换到NED坐标系

		break;

	case 20://右小腿四元数、加速度和角速度

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

		//CST_MA2GB_LF(cur_quat_Rcalf);//四元数转换到NED坐标系

		break;	

	case 21://右脚四元数、加速度和角速度

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

		//CST_MA2GB_LF(cur_quat_Rfoot);//四元数转换到NED坐标系

		break;		


	case 15://左大腿四元数、加速度和角速度

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

		//CST_MA2GB_LF(cur_quat_Lthigh);//四元数转换到NED坐标系

		break;

	case 16://左小腿四元数、加速度和角速度

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

		//CST_MA2GB_LF(cur_quat_Lcalf);//四元数转换到NED坐标系

		break;

	case 17://左脚四元数、加速度和角速度

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

		//CST_MA2GB_LF(cur_quat_Lfoot);//四元数转换到NED坐标系

		break;	

	default:
		break;

	}

	return 1;//返回真

}

//estimate the displacement
bool DisplacementFusion::EstimateDis(double *result)
//////////compute the dispalcement in tridimensional space////////////////////////
/*	//position estimation
/*  // 1.坐标系为NED，X向前，y向右，z向下。以初始时刻脚踝关节做为坐标原点.
/*	// 2.利用加速度和角速度共同判断stance阶段,处于stance阶段的踝关节处于最低点.
/*	// 3.该脚stance结束,换支撑脚,另只脚处于stance阶段,此时这只脚的踝关节处于最低点.
/*	// 4.计算各个时刻各关节相对于根节点的位置 
/*  // 5.最后位移需要转换到模型坐标系
////////////////////////////////////////////////////////////////////////////////*/
{
	//变量声明及初始化
	//////////////////////////////////////////////////////////////////////////////////////////
	double thighlen = 0.43; // 大腿长度
	double shanklen = 0.50; //小腿长度

	double separate_angle = 0.0;//两个大腿的夹角
	double separate_angleL = 0.0;//左大腿与垂直方向的夹角
	double separate_angleR = 0.0;//右大腿与垂直方向的夹角
	//左、右腿走路过程中旋转向量
	double vec_Lthigh_rotation[3], vec_Rthigh_rotation[3], vec_Lcalf_rotation[3], vec_Rcalf_rotation[3];

	//当前左大小腿旋转之后的向量初始化
	vec_Lcalf_rotation[0] = 0.0;
	vec_Lcalf_rotation[1] = 0.0;
	vec_Lcalf_rotation[2] = -shanklen;
	vec_Lthigh_rotation[0] = 0.0;
	vec_Lthigh_rotation[1] = 0.0;
	vec_Lthigh_rotation[2] = -thighlen;

	//当前右大小腿旋转之后的向量初始化
	vec_Rcalf_rotation[0] = 0.0;
	vec_Rcalf_rotation[1] = 0.0;
	vec_Rcalf_rotation[2] = -shanklen;
	vec_Rthigh_rotation[0] = 0.0;
	vec_Rthigh_rotation[1] = 0.0;
	vec_Rthigh_rotation[2] = -thighlen;


	double accsq_thredH = 120;//加速度平方和的阈值，用于stance阶段的判断
	double accsq_thredL = 80;//加速度平方和的阈值，用于stance阶段的判断

	double gyrsq_thred = 2; //右脚角速度阈值，用于stance阶段的判断

	double pos_KneeL[3] = {0.0, 0.0,-shanklen};//左膝关节的位置
	double pos_KneeR[3] = {0.0, 0.0,-shanklen};//右膝关节的位置
	double pos_Hip[3] = {0.0, 0.0, -(thighlen+shanklen)};//髋关节的位置

	double accSqL = 0.0; //左脚加速度平方和
	double accSqR = 0.0; //右脚加速度平方和
	double gyrSqL = 0.0; //左脚角速度平方和
	double gyrSqR = 0.0; //右脚角速度平方和

	int Rslogical_acc =-1;//加速度求得的右脚逻辑标志
	int Lslogical_acc =-1;//加速度求得的左脚逻辑标志
	int Rslogical_gyr =-1;//角速度求得的右脚逻辑标志
	int Lslogical_gyr =-1;//角速度求得的左脚逻辑标志


	//////////////////////////////////////////////////////////////////////
	//当前左右大小腿的旋转之后的向量,从body坐标系到global坐标系
	///////////////////////////////////////////////////////////////////////

	Quaternion_Rotation(cur_quat_Lthigh,vec_Lthigh_rotation);//左大腿
	Quaternion_Rotation(cur_quat_Rthigh,vec_Rthigh_rotation);//右大腿
	Quaternion_Rotation(cur_quat_Lcalf,vec_Lcalf_rotation);//左小腿
	Quaternion_Rotation(cur_quat_Rcalf,vec_Rcalf_rotation);//右小腿

	//////////////////////////////////////// 检测stance阶段/////////////////////////////////////////
	//用两只脚三轴加速度平方和，三轴角速度平方和，检测stance阶段，处于stance阶段脚的踝关节处于最低点
	////////////////////////////////////////////////////////////////////////////////////////////////
	accSqL = SQ_vector(acc_Lfoot);//计算左脚加速度平方和
	accSqR = SQ_vector(acc_Rfoot);//计算右脚加速度平方和

	gyrSqL = SQ_vector(angular_Lfoot);//计算左脚角速度平方和
	gyrSqR = SQ_vector(angular_Rfoot);//计算右脚角速度平方和

	//int firstmove = 0; //第一次运动检测

	////////////////////////////////////////////////////////////////////////////////////
	//           加速度检测stance,在阈值区间内为0，stance;之外则为1，swing
	////////////////////////////////////////////////////////////////////////////////////
	if(accSqR > accsq_thredL && accSqR < accsq_thredH) //右脚
	{	
		Rslogical_acc = 0;//Stance
	}
	else 
	{
		Rslogical_acc = 1;//swing
	}
	if(accSqL > accsq_thredL && accSqL < accsq_thredH) //左脚
	{	
		Lslogical_acc = 0;//stance
	}
	else 
	{
		Lslogical_acc = 1;//swing
	}
	//////////////////////////////////////////////////////////////////////////////////
	//       角速度检测stance,在阈值区间内为0，stance;之外则为1，swing.
	//////////////////////////////////////////////////////////////////////////////////
	if(gyrSqR < gyrsq_thred)//右脚
	{	
		Rslogical_gyr = 0;//stance
	}
	else 
	{
		Rslogical_gyr = 1;//swing
	}
	if(gyrSqL < gyrsq_thred)//左脚
	{	
		Lslogical_gyr = 0;//stance
	}
	else 
	{
		Lslogical_gyr = 1;//swing
	}

	////////////////////////////////////////////////////////////////////////////////////
	//                 由加速度和角速度共同决定Rslogical和Lslogical
	////////////////////////////////////////////////////////////////////////////////////

	// 右脚，1&&1==1；1&&0==1；0&&1==1；0&&0=0，如果acc为0，gyr为1并且计数大于20，以加速度的为准
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

	//左脚，1&&1==1；1&&0==1；0&&1==1；0&&0=0 如果acc为0，gyr为1并且计数大于20，以加速度的为准
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
	//                              判断support_leg
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
	//   计算髋关节、左右膝关节和左右踝关节各个关节的的位置，根据不动点求得当前绝对位置，
	//   然后计算根节点的位置	
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

	//最后结果存于result中
	result[0] = Root_pos[0];
	result[1] = Root_pos[1];
	result[2] = Root_pos[2];

	//result[2] += 0.93;//垂直方向上位移的变化量

	//CST_GB2MA_Loc(result);//位移旋转到模型坐标系

	////////////////////更新变量//////////////////////////////
	for (int i=0; i<3; i++)
	{
		pre_aPos[i] = cur_aPos[i];//绝对位置
		pre_Ranklepos[i] = cur_Ranklepos[i];//右脚踝的位置
		pre_Lanklepos[i] = cur_Lanklepos[i];//左脚踝的位置
	}
	preSupportLeg = curSupportLeg;//support_leg
	preRslogical = curRslogical;//右脚逻辑
	preLslogical = curLslogical;//左脚逻辑

	return 1;//返回真
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