#include "stdafx.h"
#include "Displacement.h"

Displacement::Displacement()
{
	pre_quater_Lthigh[0] = 0.0;
	pre_quater_Lthigh[1] = 0.0;
	pre_quater_Lthigh[2] = 0.0;
	pre_quater_Lthigh[3] = 1.0;//ǰһ�������

	pre_quater_Rthigh[0] = 0.0;
	pre_quater_Rthigh[1] = 0.0;
	pre_quater_Rthigh[2] = 0.0;
	pre_quater_Rthigh[3] = 1.0;//ǰһ���Ҵ���

	pre_quater_Lshank[0] = 0.0;
	pre_quater_Lshank[1] = 0.0;  
	pre_quater_Lshank[2] = 0.0;  
	pre_quater_Lshank[3] = 1.0;//ǰһ����С�� 

	pre_quater_Rshank[0] = 0.0;
	pre_quater_Rshank[1] = 0.0;  
	pre_quater_Rshank[2] = 0.0;  
	pre_quater_Rshank[3] = 1.0;//ǰһ����С�� 

	pre_quater_Lfeet[0] = 0.0;
	pre_quater_Lfeet[1] = 0.0;  
	pre_quater_Lfeet[2] = 0.0;  
	pre_quater_Lfeet[3] = 1.0;//ǰһ����� 

	pre_quater_Rfeet[0] = 0.0;
	pre_quater_Rfeet[1] = 0.0;  
	pre_quater_Rfeet[2] = 0.0;  
	pre_quater_Rfeet[3] = 1.0;//ǰһ���ҽ� 

	cur_quater_wist[0] = 0.0;
	cur_quater_wist[1] = 0.0;
	cur_quater_wist[2] = 0.0;
	cur_quater_wist[3] = 1.0;//����

	cur_quater_Lthigh[0] = 0.0;
	cur_quater_Lthigh[1] = 0.0;
	cur_quater_Lthigh[2] = 0.0;
	cur_quater_Lthigh[3] = 1.0;//�����

	cur_quater_Rthigh[0] = 0.0;
	cur_quater_Rthigh[1] = 0.0;
	cur_quater_Rthigh[2] = 0.0;
	cur_quater_Rthigh[3] = 1.0;//�Ҵ���

	cur_quater_Lshank[0] = 0.0;
	cur_quater_Lshank[1] = 0.0;
	cur_quater_Lshank[2] = 0.0;
	cur_quater_Lshank[3] = 1.0;//��С��

	cur_quater_Rshank[0] = 0.0;
	cur_quater_Rshank[1] = 0.0;
	cur_quater_Rshank[2] = 0.0;
	cur_quater_Rshank[3] = 1.0;//��С��

	cur_quater_Lfeet[0] = 0.0;
	cur_quater_Lfeet[1] = 0.0;
	cur_quater_Lfeet[2] = 0.0;
	cur_quater_Lfeet[3] = 1.0;//���

	cur_quater_Rfeet[0] = 0.0;
	cur_quater_Rfeet[1] = 0.0;
	cur_quater_Rfeet[2] = 0.0;
	cur_quater_Rfeet[3] = 1.0;//�ҽ�

	acc_Lthigh[0] = 0.0;//����ȵļ��ٶ�
	acc_Lthigh[1] = 0.0;
	acc_Lthigh[2] = 0.0;
	acc_Lshank[0] = 0.0;//��С�ȵļ��ٶ�
	acc_Lshank[1] = 0.0;
	acc_Lshank[2] = 0.0;
	acc_Rthigh[0] = 0.0;//�Ҵ��ȵļ��ٶ�
	acc_Rthigh[1] = 0.0;
	acc_Rthigh[2] = 0.0;
	acc_Rshank[0] = 0.0;//��С�ȵļ��ٶ�
	acc_Rshank[1] = 0.0;
	acc_Rshank[2] = 0.0;
	acc_Lfeet[0] = 0.0;//��ŵļ��ٶ�
	acc_Lfeet[1] = 0.0;
	acc_Lfeet[2] = 0.0;
	acc_Rfeet[0] = 0.0;//�ҽŵļ��ٶ�
	acc_Rfeet[1] = 0.0;
	acc_Rfeet[2] = 0.0;
	acc_wist[0] = 0.0;//�����ļ��ٶ�
	acc_wist[1] = 0.0;
	acc_wist[2] = 0.0;

	angular_Lthigh[0] = 0.0;//����ȵĽ��ٶ�
	angular_Lthigh[1] = 0.0;
	angular_Lthigh[2] = 0.0;
	angular_Lshank[0] = 0.0;//��С�ȵĽ��ٶ�
	angular_Lshank[1] = 0.0;
	angular_Lshank[2] = 0.0;
	angular_Rthigh[0] = 0.0;//�Ҵ��ȵĽ��ٶ�
	angular_Rthigh[1] = 0.0;
	angular_Rthigh[2] = 0.0;
	angular_Rshank[0] = 0.0;//�Ҵ��ȵĽ��ٶ�
	angular_Rshank[1] = 0.0;
	angular_Rshank[2] = 0.0;
	angular_Lfeet[0] = 0.0;//��ŵĽ��ٶ�
	angular_Lfeet[1] = 0.0;
	angular_Lfeet[2] = 0.0;
	angular_Rfeet[0] = 0.0;//�ҽŵĽ��ٶ�
	angular_Rfeet[1] = 0.0;
	angular_Rfeet[2] = 0.0;	
	angular_wist[0] = 0.0;//�����Ľ��ٶ�
	angular_wist[1] = 0.0;
	angular_wist[2] = 0.0;

	displacement[0] = 0.0;//���ڵ��λ�� 
	displacement[1] = 0.0;
	displacement[2] = 0.0;

	for(int i=0;i<BUFFER_SIZE;i++)
	{
		Rfeet_buffer[i] = 0.0;;//�ҽŵ�buffer
		Lfeet_buffer[i] = 0.0;//��ŵ�buffer
	}

	counter = 0;//buffer�ļ���
	stance_or_swing_L = -2;
	stance_or_swing_R = -2;
	isStance_L = false;
	isSwing_L = false;
	isStance_L = false;
	isSwing_L = false;
	isfirst = false;

}

Displacement::~Displacement()
{
}


void Displacement::ComputeDisplacementbyQuaternion()
{
	//////////compute the dispalcement in tridimensional space///////
	// position estimation
	//���ý��ٶ��ж�stance�׶Σ�stance������Ҫ��֧�Ž�
	// �������ʱ�̵�����
	// ������·ʱ����ֻ�ڻ�ʸƽ�����˶�

	//������������ʼ��
	//////////////////////////////////////////////////////////////////////////////////////////
	double thighlen = 0.4; // ���ȳ���
	double shanklen = 0.47;//С�ȳ���

	double change_angle = 0.0;//��λ�ǵı仯
	double separate_angle = 0.0;//�������ȵļн�
	double separate_angleL = 0.0;//������봹ֱ����ļн�
	double separate_angleR = 0.0;//�Ҵ����봹ֱ����ļн�

	double delta_displacement[3], uint_wist_vec[3], vec_shank[3], vec_thigh[3];
	double pre_vec_Lthigh_rotation[3], pre_vec_Rthigh_rotation[3], pre_vec_Lshank_rotation[3], pre_vec_Rshank_rotation[3];
	double vec_Lthigh_rotation[3], vec_Rthigh_rotation[3], vec_Lshank_rotation[3], vec_Rshank_rotation[3];

	//λ�Ʊ仯��
	delta_displacement[0] = 0.0;
	delta_displacement[1] = 0.0;
	delta_displacement[2] = 0.0;

	//������λ���������ڼ��㷽λ��	
	uint_wist_vec[0] = 0.0;
	uint_wist_vec[1] = 0.0;
	uint_wist_vec[2] = -1.0;

	//С�ȳ�ʼλ������
	vec_shank[0] = shanklen;
	vec_shank[1] = 0.0;
	vec_shank[2] = 0.0;

	//���ȳ�ʼλ������	
	vec_thigh[0] = thighlen;
	vec_thigh[1] = 0.0;
	vec_thigh[2] = 0.0;

	//��ǰ���С����ת֮�������	
	vec_Lshank_rotation[0] = shanklen;
	vec_Lshank_rotation[1] = 0.0;
	vec_Lshank_rotation[2] = 0.0;
	vec_Lthigh_rotation[0] = thighlen;
	vec_Lthigh_rotation[1] = 0.0;
	vec_Lthigh_rotation[2] = 0.0;

	//��ǰ�Ҵ�С����ת֮�������
	vec_Rshank_rotation[0] = shanklen;
	vec_Rshank_rotation[1] = 0.0;
	vec_Rshank_rotation[2] = 0.0;
	vec_Rthigh_rotation[0] = thighlen;
	vec_Rthigh_rotation[1] = 0.0;
	vec_Rthigh_rotation[2] = 0.0;

	//ǰһ�����С����ת֮�������
	pre_vec_Lshank_rotation[0] = shanklen;
	pre_vec_Lshank_rotation[1] = 0.0;
	pre_vec_Lshank_rotation[2] = 0.0;	
	pre_vec_Lthigh_rotation[0] = thighlen;
	pre_vec_Lthigh_rotation[1] = 0.0;
	pre_vec_Lthigh_rotation[2] = 0.0;	

	//ǰһ���Ҵ�С����ת֮�������	
	pre_vec_Rshank_rotation[0] = shanklen;
	pre_vec_Rshank_rotation[1] = 0.0;
	pre_vec_Rshank_rotation[2] = 0.0;	
	pre_vec_Rthigh_rotation[0] = thighlen;
	pre_vec_Rthigh_rotation[1] = 0.0;
	pre_vec_Rthigh_rotation[2] = 0.0;	

	double delta_offset = 0.0;//�˶�ʸƽ����λ�Ʊ仯��
	double delta_Loffset = 0.0;//�˶�ʸƽ��������λ�Ʊ仯��
	double delta_Roffset = 0.0;//�˶�ʸƽ��������λ�Ʊ仯��

	double delta_s_knee = 0.0;//С�ȵ���ǰ������ı仯��
	double delta_s_coxa = 0.0;//���ȵ���ǰ������ı仯��
	double delta_s_Lknee = 0.0;//��С�ȵ���ǰ������ı仯��
	double delta_s_Lcoxa = 0.0;//����ȵ���ǰ������ı仯��
	double delta_s_Rknee = 0.0;//��С�ȵ���ǰ������ı仯�� 
	double delta_s_Rcoxa = 0.0;//�Ҵ��ȵ���ǰ������ı仯��

	double delta_vertical = 0.0;//��ֱ�����ϵ�λ��
	double delta_Lvertical = 0.0;//�����ڴ�ֱ�����ϵ�λ��
	double delta_Rvertical = 0.0;//�����ڴ�ֱ�����ϵ�λ��


	double thresh_theta = 0.1745;//�����ȼнǵ���ֵ����������վ������


	double angulerrate_thred = 0.2;//���ٶȵ���ֵ������stance�׶ε��ж�
	double posi_shankR = 0.0;//���׹ؽ���xzƽ���ڵ�λ��
	double posi_shankL = 0.0;//���׹ؽ���xzƽ���ڵ�λ��
	double posi_thighL = shanklen;//��ϥ�ؽ���xzƽ���ڵ�λ��
	double posi_thighR = shanklen;//��ϥ�ؽ���xzƽ���ڵ�λ��
	double posi_coxaL = shanklen+thighlen;//���Źؽ���xzƽ���ڵ�λ��
	double posi_coxaR = shanklen+thighlen;//���Źؽ���xzƽ���ڵ�λ��

	double anguler_thredR = 0.2; //�ҽŽ��ٶ���ֵ
	double anguler_thredL = 0.2; //��Ž��ٶ���ֵ

	double angle_thredR = 0.25; //�ҽŽ��ٶ���ֵ
	double angle_thredL = 0.25; //��Ž��ٶ���ֵ

	int support_leg = -1;//֧���ȵı�־
	int gyroRslogical = -1;//����stance�׶εı�־
	int gyroLslogical = -1;//����stance�׶εı�־

	////////////////////////////////////////////////////////////////////////////////////////////
	//ǰһ�����Ҵ�С�ȵ���ת֮�������
	Quaternion_Rotation(pre_quater_Lthigh,pre_vec_Lthigh_rotation);
	Quaternion_Rotation(pre_quater_Rthigh,pre_vec_Rthigh_rotation);
	Quaternion_Rotation(pre_quater_Lshank,pre_vec_Lshank_rotation);
	Quaternion_Rotation(pre_quater_Rshank,pre_vec_Rshank_rotation);
	//��ǰ���Ҵ�С�ȵ���ת֮�������
	Quaternion_Rotation(cur_quater_Lthigh,vec_Lthigh_rotation);
	Quaternion_Rotation(cur_quater_Rthigh,vec_Rthigh_rotation);
	Quaternion_Rotation(cur_quater_Lshank,vec_Lshank_rotation);
	Quaternion_Rotation(cur_quater_Rshank,vec_Rshank_rotation);
	//�������Ҵ����봹ֱ����ļн�
	separate_angleL = acos(abs(vec_Lthigh_rotation[0])/thighlen);
	separate_angleR = acos(abs(vec_Rthigh_rotation[0])/thighlen);
	//�����������ȼ�ļн�              
	separate_angle = separate_angleL+ separate_angleR; 

	////////////////////////////////////////////////////////////////////////////////////////
	//�����Źؽڡ�ϥ�ؽں��׹ؽڵ�λ��	
	///////////////////////////////////////////////////////////////////////////////////////
	// ���������š�ϥ���׹ؽڵ�λ��

	posi_shankR = shanklen+thighlen-vec_Rshank_rotation[0] - vec_Rthigh_rotation[0];
	posi_thighR = shanklen+thighlen-vec_Rthigh_rotation[0];

	posi_shankL = shanklen+thighlen-vec_Lshank_rotation[0] - vec_Lthigh_rotation[0];
	posi_thighL = shanklen+thighlen-vec_Lthigh_rotation[0];

	posi_coxaL = vec_Lshank_rotation[0] + vec_Lthigh_rotation[0];
	posi_coxaR = vec_Rshank_rotation[0] + vec_Rthigh_rotation[0];	
	////////////////////////////////////////////////////////////////////////////////////////


    if(counter<BUFFER_SIZE && counter>2)
	{
 	   stance_or_swing_R = StanceDetection(Rfeet_buffer,counter-2);//
	   stance_or_swing_L = StanceDetection(Lfeet_buffer,counter-2);//
	}
	else if(counter>=BUFFER_SIZE)
	{
	   stance_or_swing_R = StanceDetection(Rfeet_buffer,BUFFER_SIZE-2);//
	   stance_or_swing_L = StanceDetection(Lfeet_buffer,BUFFER_SIZE-2);//
	}
	///////for the first time////////////////////
	if(stance_or_swing_R>0 && !isfirst)

	{
		isfirst =true;
		stance_or_swing_L = 0;
		isStance_L = true;
		isSwing_L = false;

	}
	if(stance_or_swing_L>0 && !isfirst)
	{
		isfirst =true;
		stance_or_swing_R = 0;
		isStance_R = true;
		isSwing_R = false;

	}
	///////////////////////////////////////////
	if(0==stance_or_swing_R)
	{
		isStance_R = true;
		isSwing_R = false;
	}
	if(1==stance_or_swing_R)
	{
		isStance_R = false;
		isSwing_R = true;
	}

	if(0==stance_or_swing_L)
	{
		isStance_L = true;
		isSwing_L = false;
	}
	if(1==stance_or_swing_L)
	{
		isStance_L = false;
		isSwing_L = true;
	}

	if(isStance_R)
	{
		gyroRslogical = 0;
	}
	if(isSwing_R)
	{
		gyroRslogical = 1;
	}
	if(isStance_L)
	{	 
		gyroLslogical = 0;
	}
	if(isSwing_L)
	{
		gyroLslogical = 1;
	}

	////////////////////////////////////////

	/*

	//����ֻ�Ž��ٶȼ��stance�׶�

	if(abs(angular_Rfeet[1]) > anguler_thredR )
	{
	gyroLslogical = 0;gyroRslogical = 1;
	}      
	if(abs(angular_Lfeet[1]) > anguler_thredL )
	{
	gyroLslogical = 1;gyroRslogical = 0;
	}    

	if(abs(angular_Rfeet[1])> 0.15 && abs(angular_Lfeet[1])> 0.15 && separate_angleL >= 0.15&&separate_angleR >= 0.15 && separate_angle >= 0.3 )

	{
	gyroLslogical = 0;gyroRslogical = 0;
	}

	*/        
	//////////////////////////////////////////////////////////////////////////////

	//֧���ȵ�ѡ��
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

	if(gyroRslogical == 0 && gyroLslogical == 0 && separate_angle >= 0.3)
	{

		support_leg = 3;
	}


	//////////////////////////////////////////////////////////////////////

	//����λ��
	//////////////////////////////////////////////////////////////////////
	//������Ⱥ�С�����˶�ʸƽ��ı仯��
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

		break;

	default:
		delta_offset = 0.0;
		delta_Lvertical = shanklen+thighlen-vec_Lshank_rotation[0] - vec_Lthigh_rotation[0];
		delta_Rvertical = shanklen+thighlen-vec_Rshank_rotation[0] - vec_Rthigh_rotation[0];
		delta_vertical = (delta_Lvertical+delta_Rvertical)/2;

		break;
	}

	//���㷽��� 
	Quaternion_Rotation(cur_quater_wist,uint_wist_vec);  //��������·����

	change_angle = acos(abs(uint_wist_vec[2])/sqrt(pow(uint_wist_vec[2],2) + pow(uint_wist_vec[1],2))); 

	//��sensor ����ϵ�£����˶�ʸƽ���λ��ͶӰ��yz��	
	delta_displacement[2] = delta_offset*cos(change_angle);
	delta_displacement[1] = delta_offset*sin(change_angle);
	//�ۻ�����λ��
	displacement[0] = delta_vertical;//��ֱλ��
	//ƽ����λ��
	if(delta_displacement[1]>=0)
		displacement[1] = displacement[1] + delta_displacement[1];
	if(delta_displacement[2]>=0)
		displacement[2] = displacement[2] + delta_displacement[2];
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
	double temp2=a[3]*b[0]+b[3]*a[0]+b[1]*a[2]-a[1]*b[2];
	double temp3=a[3]*b[1]+b[3]*a[1]+b[2]*a[0]-a[2]*b[0];
	double temp4=a[3]*b[2]+b[3]*a[2]+b[0]*a[1]-a[0]*b[1];
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


/****************************************************
**** b:�������ݣ�
**** i:��������λ��
****************************************************/
int Displacement::StanceDetection(double* b,int i)
{

	double initialTh = -0.5; //��ʼ����
	double eita = 0.2;   //�������Խ�����޷�ӦԽ����
	double TP_PS = 0.3;  //��⵽�������޺���µ�PS��ϵ��
	double TP_PN = 0.7;  //�������Խ�����������µķ�ֵ��ø���
	double FP_PS = 0.3;
	double FP_PN = 0.7;
	double PS = initialTh;
	double PN = initialTh;
	double Th = initialTh; //����

	int PeakLoc=-1; //��ֵλ�ã�0��ʾ��λ��stance��ʼ��1��ʾswing��ʼ
	//int FindZeroCross[80];  //�������һ������㵽��ֵ�ļ����ʵ����Ҫ�������ݣ����80����
	//double Thre[80];

	int interval = 0;
	int cur_position = 0;
	//int k = 1;
	//int j = 1;
	//int m = 1;


	if((b[i] <= Th) && (b[i] < b[i+1]) && (b[i] < b[i-1]))
	{
		PS = TP_PS * b[i] + TP_PN * PS; //�ҵ���ļ�ֵ������PS
		Th = PN + eita*(PS - PN);       //��������
		//Thre[k] = Th;  //������ֵ���ڷ�����죬©��

		for(int n = i; n>0;n--)
		{
			if(b[n]>0)
			{
				interval = i - n;  //����һ�������ļ��
				// FindZeroCross[m] = interval;
				// m = m +1;
				cur_position = n;
				break;

			}	
		}
        
		if(interval > 20)
		{
			PeakLoc = 1;  //swing
		}
		else
		{
			PeakLoc = 0;  //stance
		}
		// k = k +1;

	}else if(b[i] > Th && b[i] <= b[i+1] && b[i] <= b[i-1] && b[i] < -0.1) //����0.1��Ϊ�˷�ֹ���޹���
	{

		PN = FP_PS * b[i] + FP_PN * PN;  //������ֵ������������ֵ
		Th = PN + eita*(PS - PN);        //�������� 
		// Thre[k] = Th;
		//k = k + 1;
		PeakLoc = -1;

	}else{
		PeakLoc = -1;

	}
	//j = j + 1;
	return PeakLoc;

}
