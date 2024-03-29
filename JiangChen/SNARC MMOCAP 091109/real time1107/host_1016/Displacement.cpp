#include "stdafx.h"
#include "Displacement.h"


Displacement::Displacement()
{
	pre_quater_Lthigh[0] = 0.0;
	pre_quater_Lthigh[1] = 0.0;
	pre_quater_Lthigh[2] = 0.0;
	pre_quater_Lthigh[3] = 1.0;//前一次左大腿

	pre_quater_Rthigh[0] = 0.0;
	pre_quater_Rthigh[1] = 0.0;
	pre_quater_Rthigh[2] = 0.0;
	pre_quater_Rthigh[3] = 1.0;//前一次右大腿

	pre_quater_Lshank[0] = 0.0;
	pre_quater_Lshank[1] = 0.0;  
	pre_quater_Lshank[2] = 0.0;  
	pre_quater_Lshank[3] = 1.0;//前一次左小腿 

	pre_quater_Rshank[0] = 0.0;
	pre_quater_Rshank[1] = 0.0;  
	pre_quater_Rshank[2] = 0.0;  
	pre_quater_Rshank[3] = 1.0;//前一次右小腿 

	pre_quater_Lfeet[0] = 0.0;
	pre_quater_Lfeet[1] = 0.0;  
	pre_quater_Lfeet[2] = 0.0;  
	pre_quater_Lfeet[3] = 1.0;//前一次左脚 

	pre_quater_Rfeet[0] = 0.0;
	pre_quater_Rfeet[1] = 0.0;  
	pre_quater_Rfeet[2] = 0.0;  
	pre_quater_Rfeet[3] = 1.0;//前一次右脚 

	cur_quater_wist[0] = 0.0;
	cur_quater_wist[1] = 0.0;
	cur_quater_wist[2] = 0.0;
	cur_quater_wist[3] = 1.0;//腰部

	cur_quater_Lthigh[0] = 0.0;
	cur_quater_Lthigh[1] = 0.0;
	cur_quater_Lthigh[2] = 0.0;
	cur_quater_Lthigh[3] = 1.0;//左大腿

	cur_quater_Rthigh[0] = 0.0;
	cur_quater_Rthigh[1] = 0.0;
	cur_quater_Rthigh[2] = 0.0;
	cur_quater_Rthigh[3] = 1.0;//右大腿

	cur_quater_Lshank[0] = 0.0;
	cur_quater_Lshank[1] = 0.0;
	cur_quater_Lshank[2] = 0.0;
	cur_quater_Lshank[3] = 1.0;//左小腿

	cur_quater_Rshank[0] = 0.0;
	cur_quater_Rshank[1] = 0.0;
	cur_quater_Rshank[2] = 0.0;
	cur_quater_Rshank[3] = 1.0;//右小腿

	cur_quater_Lfeet[0] = 0.0;
	cur_quater_Lfeet[1] = 0.0;
	cur_quater_Lfeet[2] = 0.0;
	cur_quater_Lfeet[3] = 1.0;//左脚

	cur_quater_Rfeet[0] = 0.0;
	cur_quater_Rfeet[1] = 0.0;
	cur_quater_Rfeet[2] = 0.0;
	cur_quater_Rfeet[3] = 1.0;//右脚

	acc_Lthigh[0] = 0.0;//左大腿的加速度
	acc_Lthigh[1] = 0.0;
	acc_Lthigh[2] = 0.0;
	acc_Lshank[0] = 0.0;//左小腿的加速度
	acc_Lshank[1] = 0.0;
	acc_Lshank[2] = 0.0;
	acc_Rthigh[0] = 0.0;//右大腿的加速度
	acc_Rthigh[1] = 0.0;
	acc_Rthigh[2] = 0.0;
	acc_Rshank[0] = 0.0;//右小腿的加速度
	acc_Rshank[1] = 0.0;
	acc_Rshank[2] = 0.0;
	acc_Lfeet[0] = 0.0;//左脚的加速度
	acc_Lfeet[1] = 0.0;
	acc_Lfeet[2] = 0.0;
	acc_Rfeet[0] = 0.0;//右脚的加速度
	acc_Rfeet[1] = 0.0;
	acc_Rfeet[2] = 0.0;
	acc_wist[0] = 0.0;//腰部的加速度
	acc_wist[1] = 0.0;
	acc_wist[2] = 0.0;

	angular_Lthigh[0] = 0.0;//左大腿的角速度
	angular_Lthigh[1] = 0.0;
	angular_Lthigh[2] = 0.0;
	angular_Lshank[0] = 0.0;//左小腿的角速度
	angular_Lshank[1] = 0.0;
	angular_Lshank[2] = 0.0;
	angular_Rthigh[0] = 0.0;//右大腿的角速度
	angular_Rthigh[1] = 0.0;
	angular_Rthigh[2] = 0.0;
	angular_Rshank[0] = 0.0;//右大腿的角速度
	angular_Rshank[1] = 0.0;
	angular_Rshank[2] = 0.0;
	angular_Lfeet[0] = 0.0;//左脚的角速度
	angular_Lfeet[1] = 0.0;
	angular_Lfeet[2] = 0.0;
	angular_Rfeet[0] = 0.0;//右脚的角速度
	angular_Rfeet[1] = 0.0;
	angular_Rfeet[2] = 0.0;	
	angular_wist[0] = 0.0;//腰部的角速度
	angular_wist[1] = 0.0;
	angular_wist[2] = 0.0;

	displacement[0] = 0.0;//根节点的位移 
	displacement[1] = 0.0;
	displacement[2] = 0.0;

}

Displacement::~Displacement()
{
}


void Displacement::ComputeDisplacementbyQuaternion()
{
	//////////compute the dispalcement in tridimensional space///////
	// position estimation
	//利用角速度判断stance阶段，stance结束，要换支撑脚
	// 计算各个时刻的向量
	// 假设走路时人腿只在弧矢平面内运动

	//变量声明及初始化
	//////////////////////////////////////////////////////////////////////////////////////////
	double thighlen = 0.4; // 大腿长度
	double shanklen = 0.47;//小腿长度
    double PI = 3.1415926;
	double change_angle = 0.0;//方位角的变化
	double phi = 0.0;//方位角的变化
	double separate_angle = 0.0;//两个大腿的夹角
	double separate_angleL = 0.0;//左大腿与垂直方向的夹角
	double separate_angleR = 0.0;//右大腿与垂直方向的夹角

	double delta_displacement[3], uint_wist_vec[3], vec_shank[3], vec_thigh[3];
	double pre_vec_Lthigh_rotation[3], pre_vec_Rthigh_rotation[3], pre_vec_Lshank_rotation[3], pre_vec_Rshank_rotation[3];
	double vec_Lthigh_rotation[3], vec_Rthigh_rotation[3], vec_Lshank_rotation[3], vec_Rshank_rotation[3];

	//位移变化量
	delta_displacement[0] = 0.0;
	delta_displacement[1] = 0.0;
	delta_displacement[2] = 0.0;

	//腰部单位向量，用于计算方位角	
	uint_wist_vec[0] = 0.0;
	uint_wist_vec[1] = 0.0;
	uint_wist_vec[2] = -1.0;

	//小腿初始位置向量
	vec_shank[0] = shanklen;
	vec_shank[1] = 0.0;
	vec_shank[2] = 0.0;

	//大腿初始位置向量	
	vec_thigh[0] = thighlen;
	vec_thigh[1] = 0.0;
	vec_thigh[2] = 0.0;

	//当前左大小腿旋转之后的向量	
	vec_Lshank_rotation[0] = shanklen;
	vec_Lshank_rotation[1] = 0.0;
	vec_Lshank_rotation[2] = 0.0;
	vec_Lthigh_rotation[0] = thighlen;
	vec_Lthigh_rotation[1] = 0.0;
	vec_Lthigh_rotation[2] = 0.0;

	//当前右大小腿	旋转之后的向量
	vec_Rshank_rotation[0] = shanklen;
	vec_Rshank_rotation[1] = 0.0;
	vec_Rshank_rotation[2] = 0.0;
	vec_Rthigh_rotation[0] = thighlen;
	vec_Rthigh_rotation[1] = 0.0;
	vec_Rthigh_rotation[2] = 0.0;

	//前一次左大小腿旋转之后的向量
	pre_vec_Lshank_rotation[0] = shanklen;
	pre_vec_Lshank_rotation[1] = 0.0;
	pre_vec_Lshank_rotation[2] = 0.0;	
	pre_vec_Lthigh_rotation[0] = thighlen;
	pre_vec_Lthigh_rotation[1] = 0.0;
	pre_vec_Lthigh_rotation[2] = 0.0;	

	//前一次右大小腿旋转之后的向量	
	pre_vec_Rshank_rotation[0] = shanklen;
	pre_vec_Rshank_rotation[1] = 0.0;
	pre_vec_Rshank_rotation[2] = 0.0;	
	pre_vec_Rthigh_rotation[0] = thighlen;
	pre_vec_Rthigh_rotation[1] = 0.0;
	pre_vec_Rthigh_rotation[2] = 0.0;	

	double delta_offset = 0.0;//运动矢平面内位移变化量
	double delta_Loffset = 0.0;//运动矢平面内左腿位移变化量
	double delta_Roffset = 0.0;//运动矢平面内右腿位移变化量

	double delta_s_knee = 0.0;//小腿的在前进方向的变化量
	double delta_s_coxa = 0.0;//大腿的在前进方向的变化量
	double delta_s_Lknee = 0.0;//左小腿的在前进方向的变化量
	double delta_s_Lcoxa = 0.0;//左大腿的在前进方向的变化量
	double delta_s_Rknee = 0.0;//右小腿的在前进方向的变化量 
	double delta_s_Rcoxa = 0.0;//右大腿的在前进方向的变化量

	double delta_vertical = 0.0;//垂直方向上的位移
	double delta_Lvertical = 0.0;//左腿在垂直方向上的位移
	double delta_Rvertical = 0.0;//右腿在垂直方向上的位移


	double thresh_theta = 0.1745;//两大腿夹角的阈值，用于区分站立和走


	double angulerrate_thred = 0.2;//加速度的阈值，用于stance阶段的判断
	double posi_shankR = 0.0;//左踝关节在xz平面内的位置
	double posi_shankL = 0.0;//右踝关节在xz平面内的位置
	double posi_thighL = shanklen;//左膝关节在xz平面内的位置
	double posi_thighR = shanklen;//右膝关节在xz平面内的位置
	double posi_coxaL = shanklen+thighlen;//左髋关节在xz平面内的位置
	double posi_coxaR = shanklen+thighlen;//右髋关节在xz平面内的位置

    double anguler_thredR = 0.15; //右脚角速度阈值
    double anguler_thredL = 0.15; //左脚角速度阈值

    double angle_thredR = 0.15; //右脚角度阈值
    double angle_thredL = 0.15; //左脚角度阈值

	int support_leg = -1;//支撑腿的标志
	int gyroRslogical = -2;//右腿stance阶段的标志
	int gyroLslogical = -2;//左腿stance阶段的标志

	////////////////////////////////////////////////////////////////////////////////////////////
	//前一次左右大小腿的旋转之后的向量
	Quaternion_Rotation(pre_quater_Lthigh,pre_vec_Lthigh_rotation);
	Quaternion_Rotation(pre_quater_Rthigh,pre_vec_Rthigh_rotation);
	Quaternion_Rotation(pre_quater_Lshank,pre_vec_Lshank_rotation);
	Quaternion_Rotation(pre_quater_Rshank,pre_vec_Rshank_rotation);
	//当前左右大小腿的旋转之后的向量
	Quaternion_Rotation(cur_quater_Lthigh,vec_Lthigh_rotation);
	Quaternion_Rotation(cur_quater_Rthigh,vec_Rthigh_rotation);
	Quaternion_Rotation(cur_quater_Lshank,vec_Lshank_rotation);
	Quaternion_Rotation(cur_quater_Rshank,vec_Rshank_rotation);
	//计算左右大腿与垂直方向的夹角
	separate_angleL = acos(abs(vec_Lthigh_rotation[0])/thighlen);
	separate_angleR = acos(abs(vec_Rthigh_rotation[0])/thighlen);
	//计算两个大腿间的夹角              
	separate_angle = separate_angleL+ separate_angleR; 

    ////////////////////////////////////////////////////////////////////////////////////////
	//计算髋关节、膝关节和踝关节的位置	
	///////////////////////////////////////////////////////////////////////////////////////
	// 计算左右髋、膝和踝关节的位置

	posi_shankR = shanklen+thighlen-vec_Rshank_rotation[0] - vec_Rthigh_rotation[0];
	posi_thighR = shanklen+thighlen-vec_Rthigh_rotation[0];

	posi_shankL = shanklen+thighlen-vec_Lshank_rotation[0] - vec_Lthigh_rotation[0];
	posi_thighL = shanklen+thighlen-vec_Lthigh_rotation[0];

	posi_coxaL = vec_Lshank_rotation[0] + vec_Lthigh_rotation[0];
	posi_coxaR = vec_Rshank_rotation[0] + vec_Rthigh_rotation[0];	
	////////////////////////////////////////////////////////////////////////////////////////
	
    //用两只脚角速度检测stance阶段


    if(abs(angular_Rfeet[1]) > anguler_thredR && separate_angleL >= angle_thredL)
	{
	    gyroLslogical = 0;gyroRslogical = 1;
	}      
    if(abs(angular_Lfeet[1]) > anguler_thredL && separate_angleR >= angle_thredR)
	{
	     gyroLslogical = 1;gyroRslogical = 0;
	}    

    if(separate_angleL >= 0.1&&separate_angleR >= 0.1 && separate_angle >= 0.2)

	{
	    gyroLslogical = 0;gyroRslogical = 0;
	}

	//////////////////////////////////////////////////////////////////////////////

	//支撑腿的选择
	//////////////////////////////////////////////////////////////////////
	//sup_leg:1->left;2->right;3->both;0->no;
	if(gyroRslogical == 1 && gyroLslogical == 0)
	{
	
	    support_leg = 1;
	}
	
	
	if(gyroRslogical == 0 && gyroLslogical == 1)
	{
	
	    support_leg = 2;
	}
	
	if(gyroRslogical == 0 && gyroLslogical == 0 && separate_angle >= thresh_theta)
	{
	
	    support_leg = 3;
	}
	

	//////////////////////////////////////////////////////////////////////

	//计算位移
	//////////////////////////////////////////////////////////////////////
	//计算大腿和小腿在运动矢平面的变化量
	switch (support_leg)
	{

	case 1:

		delta_s_knee = abs(sin(acos(abs(vec_Lshank_rotation[0])/shanklen))*shanklen-sin(acos(abs(pre_vec_Lshank_rotation[0])/shanklen))*shanklen);
		delta_s_coxa = abs(sin(acos(abs(vec_Lthigh_rotation[0])/thighlen))*thighlen-sin(acos(abs(pre_vec_Lthigh_rotation[0])/thighlen))*thighlen); 
		delta_offset = delta_s_knee +delta_s_coxa;

        delta_vertical = shanklen+thighlen-vec_Lshank_rotation[0] - vec_Lthigh_rotation[0];
		break;

	case 2:

		delta_s_knee = abs(sin(acos(abs(vec_Rshank_rotation[0])/shanklen))*shanklen-sin(acos(abs(pre_vec_Rshank_rotation[0])/shanklen))*shanklen);
		delta_s_coxa = abs(sin(acos(abs(vec_Rthigh_rotation[0])/thighlen))*thighlen-sin(acos(abs(pre_vec_Rthigh_rotation[0])/thighlen))*thighlen); 
		delta_offset = delta_s_knee +delta_s_coxa;

        delta_vertical = shanklen+thighlen-vec_Rshank_rotation[0] - vec_Rthigh_rotation[0];
		break;

	case 3:
		if(separate_angle >= thresh_theta && abs(angular_Rfeet[1]) > anguler_thredR && abs(angular_Lfeet[1]) > anguler_thredL)
		{
			delta_s_Lknee = abs(sin(acos(abs(vec_Lshank_rotation[0])/shanklen))*shanklen-sin(acos(abs(pre_vec_Lshank_rotation[0])/shanklen))*shanklen);
			delta_s_Lcoxa = abs(sin(acos(abs(vec_Lthigh_rotation[0])/thighlen))*thighlen-sin(acos(abs(pre_vec_Lthigh_rotation[0])/thighlen))*thighlen); 
			delta_Loffset = delta_s_Lknee +delta_s_Lcoxa;

			delta_s_Rknee = abs(sin(acos(abs(vec_Rshank_rotation[0])/shanklen))*shanklen-sin(acos(abs(pre_vec_Rshank_rotation[0])/shanklen))*shanklen);
			delta_s_Rcoxa = abs(sin(acos(abs(vec_Rthigh_rotation[0])/thighlen))*thighlen-sin(acos(abs(pre_vec_Rthigh_rotation[0])/thighlen))*thighlen); 
			delta_Roffset = delta_s_Rknee +delta_s_Rcoxa;

			delta_offset = (abs(delta_Loffset)+ abs(delta_Roffset))/2;

            delta_Lvertical = shanklen+thighlen-vec_Lshank_rotation[0] - vec_Lthigh_rotation[0];
			delta_Rvertical = shanklen+thighlen-vec_Rshank_rotation[0] - vec_Rthigh_rotation[0];
			delta_vertical = (delta_Lvertical+delta_Rvertical)/2;


		}
		else
		{
			delta_offset = 0.0;
			delta_vertical = 0.0;
		}
		break;

	default:
		delta_offset = 0.0;
		delta_vertical = 0.0;

		break;
	}
	phi = 2*acos(cur_quater_wist[3])*180/PI;
	//计算方向角 
	Quaternion_Rotation(cur_quater_wist,uint_wist_vec);   
	change_angle = acos(abs(uint_wist_vec[2])/sqrt(pow(uint_wist_vec[2],2) + pow(uint_wist_vec[1],2))); 

	//在sensor 坐标系下，将运动矢平面的位移投影到yz轴	
	delta_displacement[2] = delta_offset*cos(change_angle);
	delta_displacement[1] = delta_offset*sin(change_angle);
	//累积更新位移
	displacement[0] = delta_vertical;
	if(abs(phi)<=30||abs(phi-360)<=30)
	{
		if(delta_displacement[1]>0)
			displacement[1] = displacement[1] + delta_displacement[1];
     	if(delta_displacement[2]>0)
			displacement[2] = displacement[2] + delta_displacement[2];
	}
	else if(abs(phi)>30&& abs(phi)<140)
	{
		displacement[1] = displacement[1];
		displacement[2] = displacement[2];
	}
	else if(abs(phi)>=140)
	{
		if(delta_displacement[1]>0)
			displacement[1] = displacement[1] - delta_displacement[1];
     	if(delta_displacement[2]>0)
			displacement[2] = displacement[2] - delta_displacement[2];
	}
	//return displacement;
}	

void Displacement::UpdateQuaternion()
{
	for(int i = 0; i< 4; i++)
	{
		pre_quater_Lthigh[i] = cur_quater_Lthigh[i]; 
		pre_quater_Rthigh[i] = cur_quater_Rthigh[i];
		pre_quater_Lshank[i] = cur_quater_Lshank[i];
		pre_quater_Rshank[i] = cur_quater_Rshank[i];

	}
}

void Displacement::Quaternion_Multiple(double *a,double *b){
	double temp1=a[3]*b[3]-a[0]*b[0]-a[1]*b[1]-a[2]*b[2];
	double temp2=a[3]*b[0]+b[3]*a[0]+a[1]*b[2]-b[1]*a[2];
	double temp3=a[3]*b[1]+b[3]*a[1]+a[2]*b[0]-b[2]*a[0];
	double temp4=a[3]*b[2]+b[3]*a[2]+a[0]*b[1]-b[0]*a[1];

	//cout<<temp2<<" "<<temp3<<" "<<temp4<<" "<<temp1<<" ";
	a[0]=temp2;
	a[1]=temp3;
	a[2]=temp4;
	a[3]=temp1;

}

void Displacement::Quaternion_Rotation(double *a, double *vctors){
	double inva[]={-a[0],-a[1],-a[2],a[3]};
	double aug_vc[]={vctors[0],vctors[1],vctors[2],0};
	Quaternion_Multiple(aug_vc,a);
	Quaternion_Multiple(inva,aug_vc);
	for (int i=0;i<3;i++)
	{
		vctors[i]=inva[i];
	}

}
