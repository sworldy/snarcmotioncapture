#include "math.h"
#include <stdlib.h>
#include "DisplacementAbstract.h"
#include "stdio.h"

//using namespace std;

#define BUF_SIZE  80
#define PI_ (3.1418296535f)
class DisplacementFusion: public DisplacementAbstract
{
public:
	DisplacementFusion();//构造函数
	~DisplacementFusion();//析构函数
	bool SetPara(int id, double *quat, double *meas); //获得并设置初始参数
	bool EstimateDis(double *result);//采用四元数计算位移

	double SQ_vector(double *object);//calculate square of the vector 
	void Quaternion_Multiple(double *,double *);//quaternion multiple
	void Quaternion_Rotation(double *,double *);//qvrot q*V*q^(-1)
	void CST_MA2GB_W(double *);//腰部四元数转成NED坐标系
	void CST_MA2GB_LF(double *);//下肢四元数转成NED坐标系
	void CST_GB2MA_Loc(double *);//位移转到模型坐标系

	double cur_quat_Lthigh[4];//左大腿四元数
	double cur_quat_Lcalf[4];//左小腿四元数
	double cur_quat_Lfoot[4];//左脚四元数
	double cur_quat_Rthigh[4];//右大腿四元数
	double cur_quat_Rcalf[4];//右小腿四元数
	double cur_quat_Rfoot[4];//右脚四元数
	double cur_quat_pelvis[4];//腰部四元数


	double acc_Lthigh[3];//左大腿的加速度
	double acc_Lcalf[3];//左小腿的加速度
	double acc_Lfoot[3];//左脚的加速度	
	double acc_Rthigh[3];//右大腿的加速度
	double acc_Rcalf[3];//右小腿的加速度
	double acc_Rfoot[3];//右脚的加速度	
	double acc_pelvis[3];//腰部的加速度

	double angular_Lthigh[3];//左大腿的角速度
	double angular_Lcalf[3];//左小腿的角速度
	double angular_Lfoot[3];//左脚的角速度	
	double angular_Rthigh[3];//右大腿的角速度
	double angular_Rcalf[3];//右小腿的角速度
	double angular_Rfoot[3];//右脚的角速度
	double angular_pelvis[3];//腰部的角速度


	double Root_pos[3];//根节点的位移
	double cur_aPos[3];//当前absolute position of the anchor
	double pre_aPos[3];//前一刻absolute position of the anchor

	double pre_Lanklepos[3];//前一刻 position of Lankle
	double pre_Ranklepos[3];//前一刻absolute position of Rankle
	double cur_Lanklepos[3];//当前 position of Lankle
	double cur_Ranklepos[3];//当前 position of Rankle

	int counterL;//Llogical的计数
	int counterR;//Rlogical的计数

	int countR_acc; //右脚加速度求的Rlogical的计数
	int countR_gyr; //右脚角速度求的Rlogical的计数
	int countL_acc; //左脚加速度求的Llogical的计数
	int countL_gyr; //右脚角速度求的Llogical的计数

	int curRslogical;//当前时刻右腿stance阶段的标志
	int curLslogical;//当前时刻左腿stance阶段的标志

	int preRslogical;//前一时刻右腿stance阶段的标志
	int preLslogical;//前一时刻左腿stance阶段的标志


	int preSupportLeg;//前一时刻Support Leg
	int curSupportLeg;//当前时刻Support Leg

	bool isfirstL;//判断左脚第一次进入swing阶段
	bool isfirstR;//判断右脚第一次进入swing阶段

};