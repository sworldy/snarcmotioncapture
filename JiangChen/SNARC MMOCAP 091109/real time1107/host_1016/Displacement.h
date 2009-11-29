#include "math.h"
#include <stdlib.h>
#include "stdio.h"

class Displacement
{
public:
	Displacement();
	~Displacement();
	void ComputeDisplacementbyQuaternion();//������Ԫ������λ��
	void UpdateQuaternion();//����ǰһ����Ԫ��
	void Quaternion_Multiple(double *,double *);
	void Quaternion_Rotation(double *,double *);

	double pre_quater_Lthigh[4];//ǰһ�������
	double pre_quater_Lshank[4];//ǰһ����С�� 
	double pre_quater_Lfeet[4];//ǰһ�����
	double pre_quater_Rthigh[4];//ǰһ���Ҵ���
	double pre_quater_Rshank[4];//ǰһ����С��
	double pre_quater_Rfeet[4];	//ǰһ���ҽ�	


	double cur_quater_Lthigh[4];//�����
	double cur_quater_Lshank[4];//��С��
	double cur_quater_Lfeet[4];//���	
	double cur_quater_Rthigh[4];//�Ҵ���
	double cur_quater_Rshank[4];//��С��
	double cur_quater_Rfeet[4];//�ҽ�

	double cur_quater_wist[4];//����
    double last_wist_angle;//������ת�Ƕ�


	double acc_Lthigh[3];//����ȵļ��ٶ�
	double acc_Lshank[3];//��С�ȵļ��ٶ�
	double acc_Lfeet[3];//��ŵļ��ٶ�	
	double acc_Rthigh[3];//�Ҵ��ȵļ��ٶ�
	double acc_Rshank[3];//��С�ȵļ��ٶ�
	double acc_Rfeet[3];//�ҽŵļ��ٶ�	
	double acc_wist[3];//�����ļ��ٶ�

	double angular_Lthigh[3];//����ȵĽ��ٶ�
	double angular_Lshank[3];//��С�ȵĽ��ٶ�
	double angular_Lfeet[3];//��ŵĽ��ٶ�	
	double angular_Rthigh[3];//�Ҵ��ȵĽ��ٶ�
	double angular_Rshank[3];//��С�ȵĽ��ٶ�
	double angular_Rfeet[3];//�ҽŵĽ��ٶ�
	double angular_wist[3];//�����Ľ��ٶ�


	double displacement[3];//���ڵ��λ��
};