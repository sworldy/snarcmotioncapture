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
	DisplacementFusion();//���캯��
	~DisplacementFusion();//��������
	bool SetPara(int id, double *quat, double *meas); //��ò����ó�ʼ����
	bool EstimateDis(double *result);//������Ԫ������λ��

	double SQ_vector(double *object);//calculate square of the vector 
	void Quaternion_Multiple(double *,double *);//quaternion multiple
	void Quaternion_Rotation(double *,double *);//qvrot q*V*q^(-1)
	void CST_MA2GB_W(double *);//������Ԫ��ת��NED����ϵ
	void CST_MA2GB_LF(double *);//��֫��Ԫ��ת��NED����ϵ
	void CST_GB2MA_Loc(double *);//λ��ת��ģ������ϵ

	double cur_quat_Lthigh[4];//�������Ԫ��
	double cur_quat_Lcalf[4];//��С����Ԫ��
	double cur_quat_Lfoot[4];//�����Ԫ��
	double cur_quat_Rthigh[4];//�Ҵ�����Ԫ��
	double cur_quat_Rcalf[4];//��С����Ԫ��
	double cur_quat_Rfoot[4];//�ҽ���Ԫ��
	double cur_quat_pelvis[4];//������Ԫ��


	double acc_Lthigh[3];//����ȵļ��ٶ�
	double acc_Lcalf[3];//��С�ȵļ��ٶ�
	double acc_Lfoot[3];//��ŵļ��ٶ�	
	double acc_Rthigh[3];//�Ҵ��ȵļ��ٶ�
	double acc_Rcalf[3];//��С�ȵļ��ٶ�
	double acc_Rfoot[3];//�ҽŵļ��ٶ�	
	double acc_pelvis[3];//�����ļ��ٶ�

	double angular_Lthigh[3];//����ȵĽ��ٶ�
	double angular_Lcalf[3];//��С�ȵĽ��ٶ�
	double angular_Lfoot[3];//��ŵĽ��ٶ�	
	double angular_Rthigh[3];//�Ҵ��ȵĽ��ٶ�
	double angular_Rcalf[3];//��С�ȵĽ��ٶ�
	double angular_Rfoot[3];//�ҽŵĽ��ٶ�
	double angular_pelvis[3];//�����Ľ��ٶ�


	double Root_pos[3];//���ڵ��λ��
	double cur_aPos[3];//��ǰabsolute position of the anchor
	double pre_aPos[3];//ǰһ��absolute position of the anchor

	double pre_Lanklepos[3];//ǰһ�� position of Lankle
	double pre_Ranklepos[3];//ǰһ��absolute position of Rankle
	double cur_Lanklepos[3];//��ǰ position of Lankle
	double cur_Ranklepos[3];//��ǰ position of Rankle

	int counterL;//Llogical�ļ���
	int counterR;//Rlogical�ļ���

	int countR_acc; //�ҽż��ٶ����Rlogical�ļ���
	int countR_gyr; //�ҽŽ��ٶ����Rlogical�ļ���
	int countL_acc; //��ż��ٶ����Llogical�ļ���
	int countL_gyr; //�ҽŽ��ٶ����Llogical�ļ���

	int curRslogical;//��ǰʱ������stance�׶εı�־
	int curLslogical;//��ǰʱ������stance�׶εı�־

	int preRslogical;//ǰһʱ������stance�׶εı�־
	int preLslogical;//ǰһʱ������stance�׶εı�־


	int preSupportLeg;//ǰһʱ��Support Leg
	int curSupportLeg;//��ǰʱ��Support Leg

	bool isfirstL;//�ж���ŵ�һ�ν���swing�׶�
	bool isfirstR;//�ж��ҽŵ�һ�ν���swing�׶�

};