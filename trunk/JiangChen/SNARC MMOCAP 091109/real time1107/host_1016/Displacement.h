#include "math.h"
#include <stdlib.h>
#include "stdio.h"

class Displacement
{
public:
	Displacement();
	~Displacement();
	void ComputeDisplacementbyQuaternion();//采用四元数计算位移
	void UpdateQuaternion();//保存前一次四元数
	void Quaternion_Multiple(double *,double *);
	void Quaternion_Rotation(double *,double *);

	double pre_quater_Lthigh[4];//前一次左大腿
	double pre_quater_Lshank[4];//前一次左小腿 
	double pre_quater_Lfeet[4];//前一次左脚
	double pre_quater_Rthigh[4];//前一次右大腿
	double pre_quater_Rshank[4];//前一次右小腿
	double pre_quater_Rfeet[4];	//前一次右脚	


	double cur_quater_Lthigh[4];//左大腿
	double cur_quater_Lshank[4];//左小腿
	double cur_quater_Lfeet[4];//左脚	
	double cur_quater_Rthigh[4];//右大腿
	double cur_quater_Rshank[4];//右小腿
	double cur_quater_Rfeet[4];//右脚

	double cur_quater_wist[4];//腰部
    double last_wist_angle;//腰部旋转角度


	double acc_Lthigh[3];//左大腿的加速度
	double acc_Lshank[3];//左小腿的加速度
	double acc_Lfeet[3];//左脚的加速度	
	double acc_Rthigh[3];//右大腿的加速度
	double acc_Rshank[3];//右小腿的加速度
	double acc_Rfeet[3];//右脚的加速度	
	double acc_wist[3];//腰部的加速度

	double angular_Lthigh[3];//左大腿的角速度
	double angular_Lshank[3];//左小腿的角速度
	double angular_Lfeet[3];//左脚的角速度	
	double angular_Rthigh[3];//右大腿的角速度
	double angular_Rshank[3];//右小腿的角速度
	double angular_Rfeet[3];//右脚的角速度
	double angular_wist[3];//腰部的角速度


	double displacement[3];//根节点的位移
};