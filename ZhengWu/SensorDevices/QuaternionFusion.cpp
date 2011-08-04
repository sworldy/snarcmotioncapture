/*===================================================*\
|                                                     |
|  FQA + ADAPTIVE_UKF ATTITUDE ESTIMATE               |
|                                                     |
|  Copyright (c) 2010 - CHENJAING                     |
|      jiangch86@gmail.com                            |
|                                                     |
|  Created:                                           |
|         2010/03/24                                  |
|                                                     |
\*===================================================*/

// MainFrm.cpp : implementation of the CMainFrame class

#include "stdafx.h"
#include "QuaternionFusion.h"
#include "math.h"
//using namespace std;

QuaternionFusion::QuaternionFusion(int ID){
    // set the id
	sensorID = ID;
	int i,j;
	alpha=1;
	beta=0;
	kappa=2;
    delta_t=0.02;

	counter=1;
	for(i=0;i<states;i++){
		for (j=0;j<states;j++)
		{
			P_estimate[i][j]=0;
			P_predict[i][j]=0;
		}
		state_estimate[i]=0;
		state_predict[i]=0;
	}

	for(i=0;i<Qnoise;i++){
		for (j=0;j<Qnoise;j++)
		{
			Q[i][j]=0;
		}
	}

	for(i=0;i<3;i++)
	{
		gravity_initial[i] = 0;
		angular_initial[i] = 0;
		mag_initial[i] = 0;
	}
	Q[0][0]=0.5;
	Q[1][1]=0.5;
	Q[2][2]=0.5;
	Q[3][3]=0.001;
	Q[4][4]=0.001;
	Q[5][5]=0.001;
	Q[6][6]=0.001;

	for(i=0;i<max_Rnoise;i++){
		for (j=0;j<max_Rnoise;j++)
		{
			R[i][j]=0;
		}
	}
	
	for (i=0;i<states;i++)
	{
		P_estimate[i][i]=0.1;
	}
		
	state_estimate[0]=0.001;
	state_estimate[1]=0.001;
	state_estimate[2]=0.001;
	state_estimate[3]=0;
	state_estimate[4]=0;
	state_estimate[5]=0;
	state_estimate[6]=1;

}

QuaternionFusion::~QuaternionFusion(){
/*
		free(point);
		free(weight_c);
		free(weight_m);
		free(Augmented_P);
		free(statePredPts);
		free(meaPredPts);*/
	
}

void QuaternionFusion::QuaternionFusion_Filtering(double *measure,double *result){

	int i;
	
	if(counter<200){
		counter++;
		for(i=0;i<3;i++)
		{
			gravity_initial[i] += measure[i];
			angular_initial[i] += measure[i+3];
			mag_initial[i] += measure[i+6];
		}
		result[0]=0;
		result[1]=0;
		result[2]=0;
		result[3]=1;
	}
	else if(counter==200){
		counter++;
		for(i=0;i<3;i++)
		{
			gravity_initial[i] += measure[i];
			angular_initial[i] += measure[i+3];
			mag_initial[i] += measure[i+6];
			gravity_initial[i] = gravity_initial[i]/200;
			angular_initial[i] = angular_initial[i]/200;
			mag_initial[i] = mag_initial[i]/200;
		}

		standard_gravity =  Normal_vector(gravity_initial);
		standard_magnetic = Normal_vector(mag_initial);

		for(i=0;i<3;i++)
		{
			gravity_initial[i] = gravity_initial[i]/standard_gravity;
			mag_initial[i] = mag_initial[i]/standard_magnetic;
		}
		double initial_data[9]={0,0,0,0,0,0,0,0,0};
		for (i=0;i<3;i++)
		{
			initial_data[i]   = gravity_initial[i];
			initial_data[i+3] = angular_initial[i];
			initial_data[i+6] = mag_initial[i];
		}
		first_flag=0;          // the flag indicate first time
		FQA(initial_data);

		first_flag=1;

		quat_initial[0]=obs_quat[0];
		quat_initial[1]=obs_quat[1];
		quat_initial[2]=obs_quat[2];
		quat_initial[3]=obs_quat[3];

		result[0]=0;
		result[1]=0;
		result[2]=0;
		result[3]=1;		
	}
	else{

		Bias_Removed(measure);             //remobe the angular rate bias
		Adaptive_noise(measure);
		//Denoise_Filter(measure);         //remove the noise of rawdata
		normalization(measure);            //get norm_measure

		FQA(norm_measure);                      /// get obs_quat
		FQA_to_Bodyframe(obs_quat);        //change from NED to Body frame

		Augmented_Variance();
		cholesky(dimension);
		SigmaPoint(dimension);
		predict();

		multi_measurement();
		Update(measurement);	

		for(i=0;i<4;i++)
		{
			result[i]=state_estimate[i+3];
		}
		

		//Smooth(attitude_body);             //smooth the output of quaternion
	}	
}

void QuaternionFusion::Bias_Removed(double *measure)
{
	int i;
	for (i=0;i<3;i++)
	{
		measure[i+3] = measure[i+3]-angular_initial[i];
	}
}

void QuaternionFusion::Augmented_Variance(){
	int i,j;
	for (i=0;i<dimension;i++)
	{
		for (j=0;j<dimension;j++)
		{
			Augmented_P[i][j]=0;
		}
	}
	for (i=0;i<states;i++)
	{
		for (j=0;j<states;j++)
		{
			Augmented_P[i][j]=P_estimate[i][j];
		}
	}
	for (i=0;i<Qnoise;i++)
	{
		for (j=0;j<Qnoise;j++)
		{
			Augmented_P[states+i][states+j]=Q[i][j];
		}
	}
	for (i=0;i<Rnoise;i++)
	{
		for (j=0;j<Rnoise;j++)
		{
			Augmented_P[states+Qnoise+i][states+Qnoise+j]=R[i][j];
		}
	}
}
	

void QuaternionFusion::cholesky(int n) {  
	int   i,j,m,k;  
	double   **L;  
	L=(double   **)malloc(n*sizeof(double));  
	for(i=0;i<n;i++)  
		L[i]=(double   *)malloc(n*sizeof(double));  
	kappa = alpha*alpha*(n+kappa)-n;
	for(i=0;i<n;i++)  
		for(j=0;j<n;j++) 
			Augmented_P[i][j]=Augmented_P[i][j]*(n+kappa);
	for(i=0;i<n;i++)  
		for(j=0;j<n;j++)  
			L[i][j]=0;  
		for(k=0;k<n;k++)  
		{  
			L[k][k]=Augmented_P[k][k];  
			for(m=0;m<k;m++)  
				L[k][k]-=L[k][m]*L[k][m];  
			L[k][k]=sqrt(L[k][k]);  
			for(i=k+1;i<n;i++)  
			{  
				L[i][k]=Augmented_P[i][k];  
				for(m=0;m<k;m++)  
					L[i][k]-=L[i][m]*L[k][m];  
				L[i][k]/=L[k][k];  
			}  
		} 
		for(i=0;i<n;i++)  
			for(j=0;j<n;j++) 
			Augmented_P[i][j]=L[j][i];
		

	for(i=0;i<n; ++i)   
	{   
		free(L[i]);   
	}   
	free(L);
/*
				for (i=0;i<n;i++){
					for(j=0;j<n;j++)
						cout<<L[i][j]<<" ";
					cout<<endl;
				}*/		
		
} 

void QuaternionFusion::SigmaPoint(int dimension){
	int i,j;
	double weight1,weight2,weight3;
	weight1=kappa/(dimension+kappa);         //2/(2+19)
	weight2=weight1+(1-alpha*alpha) + beta;  //2/(2+19)+(1-1*1)+0
	weight3=1.0/(2*(dimension+kappa));       //2/(2+19)/2
	for (i=0;i<(2*dimension+1);i++)
	{
		for (j=0;j<states;j++)
		{
			point[i][j]=state_estimate[j];
		}
		for (j=states;j<dimension;j++)
		{
			point[i][j]=0;
		}
	}

	for (i=1;i<(dimension+1);i++)
	{
		for (j=0;j<dimension;j++)
		{
			point[i][j]-=Augmented_P[i-1][j];
			point[i+dimension][j]+=Augmented_P[i-1][j];
		}
	}

	///////////////////////////////////////////////
    ////BY CJ
	////////////////////////////////////////////
	for (i=0;i<(2*dimension+1);i++)
	{
		for (j=0;j<dimension;j++)
		{
			if(i==0){
				weight_m[i][j]=weight1;
				weight_c[i][j]=weight2;
			}
			else{
				weight_c[i][j]=weight3;
				weight_m[i][j]=weight3;
			}
		}
	}
	///////////////////////////////////
}

	
void QuaternionFusion::predict(){
	double **tmp_varaianc;	
	int i,j,k,l;


	tmp_varaianc=(double  **)malloc(10*sizeof(double));    //here set size as 10 because 10 is bigger than states and observations
	for(i=0;i<10;i++)  
		tmp_varaianc[i]=(double *)malloc(10*sizeof(double));
	double stat_inno_vec[states],meas_inno_vec[max_observations];

	ProcssModel();
	MeasureModel();
	

	
////////////////////right/////////////////////////////

	//State prediction;
	for (j=0;j<states;j++)
	{
		state_predict[j]=0;
		for (i=1;i<(2*dimension+1);i++)
		{
			state_predict[j]+=(statePredPts[i][j]-statePredPts[0][j])*weight_m[i][j];
		}
		state_predict[j]+=statePredPts[0][j];
	}

	//Measurement Prediction
	for (j=0;j<observations;j++)
	{
		mesure_predict[j]=0;
		for (i=1;i<(2*dimension+1);i++)
		{
			mesure_predict[j]+=(meaPredPts[i][j]-meaPredPts[0][j])*weight_m[i][j];
		}
		mesure_predict[j]+=meaPredPts[0][j];
	}

	//The innovation among state prediction and state prediction sigma points
	//The innovation among measurement prediction and measurement prediction sigma points

	for (i=0;i<(2*dimension+1);i++)
	{
		for (j=0;j<states;j++)
		{
			statePredPts_innovation[i][j]=statePredPts[i][j]-state_predict[j];
		}
		////////////////////right/////////////////////////////
		for (j=0;j<observations;j++)
		{
			meaPredPts_innovation[i][j]=meaPredPts[i][j]-mesure_predict[j];
		}
	}
	
	for (i=0;i<states;i++)
	{
		for (j=0;j<states;j++)
		{
			P_predict[i][j]=0;
		}
		for (j=0;j<observations;j++)
		{
			XZ_Cov[i][j]=0;
		}
	}

	for (i=0;i<observations;i++)
	{
		for (j=0;j<observations;j++)
		{
			Z_Cov[i][j]=0;
		}
	}


	for (i=0;i<(2*dimension+1);i++)
	{
		for (j=0;j<states;j++)
		{
			stat_inno_vec[j]=statePredPts_innovation[i][j];
		}
		for (j=0;j<observations;j++)
		{
			meas_inno_vec[j]=meaPredPts_innovation[i][j];
		}
		Vector_Multiple(stat_inno_vec,stat_inno_vec,tmp_varaianc,states,states);
		for (k=0;k<states;k++)
		{
			for (l=0;l<states;l++)
			{
				P_predict[k][l]+=tmp_varaianc[k][l]*weight_c[i][0];
			}
		}


		Vector_Multiple(meas_inno_vec,meas_inno_vec,tmp_varaianc,observations,observations);

		for (k=0;k<observations;k++)
		{
			for (l=0;l<observations;l++)
			{
				Z_Cov[k][l]+=tmp_varaianc[k][l]*weight_c[i][0];
			}
		}

		Vector_Multiple(stat_inno_vec,meas_inno_vec,tmp_varaianc,states,observations);
		for (k=0;k<states;k++)
		{
			for (l=0;l<observations;l++)
			{
				XZ_Cov[k][l]+=tmp_varaianc[k][l]*weight_c[i][0];
			}
		}
	}

	for(i=0;i<10; ++i)   
	{   
		free(tmp_varaianc[i]);   
	}
	free(tmp_varaianc);

}

void QuaternionFusion::Update(double *measurment){

	double measurement_innovation[max_observations];
	int i,j;
	double tmp;

	double **gain, **Z_Cov_inv,**XZ_Cov_point, **cov_error,**gaintrans;

	gain=(double  **)malloc(states*sizeof(double));  
	for(i=0;i<states;i++)  
		gain[i]=(double *)malloc(observations*sizeof(double));

	XZ_Cov_point=(double  **)malloc(states*sizeof(double));  
	for(i=0;i<states;i++)  
		XZ_Cov_point[i]=(double *)malloc(observations*sizeof(double));

	cov_error=(double  **)malloc(states*sizeof(double));  
	for(i=0;i<states;i++)  
		cov_error[i]=(double *)malloc(states*sizeof(double));

	Z_Cov_inv=(double  **)malloc(observations*sizeof(double));  
	for(i=0;i<observations;i++)  
		Z_Cov_inv[i]=(double *)malloc(observations*sizeof(double));

	gaintrans=(double  **)malloc(observations*sizeof(double));  
	for(i=0;i<observations;i++)  
		gaintrans[i]=(double *)malloc(states*sizeof(double));

	double Z_Cov_back0[7][7];
	double Z_Cov_back1[6][6];
	double Z_Cov_back2[6][6];
	double Z_Cov_back3[3][3];

	for (i=0;i<observations;i++)
	{
		measurement_innovation[i]=measurment[i]-mesure_predict[i];
	}

	
/////////////////////////////////////////////////
	if(err_flag==0)
	{
		for (i=0;i<observations;i++)
		{
			for (j=0;j<observations;j++)
			{
				Z_Cov_back0[i][j]=Z_Cov[i][j];
			}
		}

		Matrix_inv(&Z_Cov_back0[0][0],observations);

		for (i=0;i<observations;i++)
		{
			for (j=0;j<observations;j++)
			{
				Z_Cov_inv[i][j]=Z_Cov_back0[i][j];
			}
		}
	}
	if(err_flag==1)
	{
		for (i=0;i<observations;i++)
		{
			for (j=0;j<observations;j++)
			{
				Z_Cov_back1[i][j]=Z_Cov[i][j];
			}
		}

		Matrix_inv(&Z_Cov_back1[0][0],observations);

		for (i=0;i<observations;i++)
		{
			for (j=0;j<observations;j++)
			{
				Z_Cov_inv[i][j]=Z_Cov_back1[i][j];
			}
		}
	}
	if(err_flag==2)
	{
		for (i=0;i<observations;i++)
		{
			for (j=0;j<observations;j++)
			{
				Z_Cov_back2[i][j]=Z_Cov[i][j];
			}
		}

		Matrix_inv(&Z_Cov_back2[0][0],observations);

		for (i=0;i<observations;i++)
		{
			for (j=0;j<observations;j++)
			{
				Z_Cov_inv[i][j]=Z_Cov_back2[i][j];
			}
		}
	}
	if(err_flag==3)
	{
		for (i=0;i<observations;i++)
		{
			for (j=0;j<observations;j++)
			{
				Z_Cov_back3[i][j]=Z_Cov[i][j];
			}
		}

		Matrix_inv(&Z_Cov_back3[0][0],observations);

		for (i=0;i<observations;i++)
		{
			for (j=0;j<observations;j++)
			{
				Z_Cov_inv[i][j]=Z_Cov_back3[i][j];
			}
		}
	}



////////////////////////////////////////////////////////
	for (i=0;i<states;i++)
	{
		for (j=0;j<observations;j++)
		{
			XZ_Cov_point[i][j]=XZ_Cov[i][j];
		}
	}
	Matrix_Multiple(XZ_Cov_point,Z_Cov_inv,states,observations,observations,gain);

	//update the state;
	for (i=0;i<states;i++)
	{
		tmp=0;
		for (j=0;j<observations;j++)
		{
			tmp+=gain[i][j]*measurement_innovation[j];
		}
		state_estimate[i]=state_predict[i]+tmp;
	}

	double qnorm=0;
	for (i=0;i<4;i++)
	{
		qnorm+=state_estimate[i+3]*state_estimate[i+3];
	}
	qnorm=sqrt(qnorm);
	for (i=0;i<4;i++)
	{
		state_estimate[i+3]=state_estimate[i+3]/qnorm;
	}
	
	//update the covariance.

	for (i=0;i<states;i++)
	{
		
		for (j=0;j<observations;j++)
		{
			gaintrans[j][i]=gain[i][j];
		}
	}
	Matrix_Multiple(XZ_Cov_point,gaintrans,states,observations,states,cov_error);

	for (i=0;i<states;i++)
	{
		
		for (j=0;j<states;j++)
		{
			P_estimate[i][j]=P_predict[i][j]-cov_error[i][j];
		}
	}


	for(i=0;i<states; ++i)   
	{   
		free(gain[i]);   
	}  
	free(gain);

	for(i=0;i<observations; ++i)   
	{   
		free(Z_Cov_inv[i]);   
	}
	free(Z_Cov_inv);

	for(i=0;i<states; ++i)   
	{   
		free(XZ_Cov_point[i]);   
	}
	free(XZ_Cov_point);

	for(i=0;i<states; ++i)   
	{   
		free(cov_error[i]);   
	}
	free(cov_error);

	for(i=0;i<observations; ++i)   
	{   
		free(gaintrans[i]);   
	}
	free(gaintrans);


}


void QuaternionFusion::ProcssModel(){
	double quaternion[4],quaternion_angular[4],angular[3],quaternion_noise[4];//,delta_nn[4];
	double angular_norm;//,delta_n;
	int i,j;
	for (i=0;i<(2*dimension+1);i++)
	{

		angular_norm=0;
		for (j=0;j<4;j++)
		{
			quaternion[j]=point[i][j+3];       //quaternion of state
			quaternion_noise[j] = point[i][j+3+states];
		}
		for (j=0;j<3;j++)
		{
			angular[j]=point[i][j];           //angular rate of state
			statePredPts[i][j]=angular[j]+point[i][j+states];  //statePred = state + noise---angular rate
			angular_norm=angular_norm+angular[j]*angular[j];
			
		}
		angular_norm=sqrt(angular_norm);
		for (j=0;j<3;j++)
		{
			angular[j]=angular[j]/angular_norm*sin(angular_norm*delta_t/2);
			quaternion_angular[j]=angular[j];		
		}
		quaternion_angular[3]=cos(angular_norm*delta_t/2);
		Quaternion_Multiple(quaternion,quaternion_angular);

		Normal_quaternion(quaternion);
		for (j=0;j<4;j++)
		{
			statePredPts[i][j+3]=quaternion[j]+quaternion_noise[j];;
		}
	
	}

}


void QuaternionFusion::MeasureModel(){
	//double quaternion[4],delta_nn[4];//,quaternion_noise[3]; 
	double quaternion[4];//,quaternion_noise[3]; 
	double vectorg[3],vectorm[3];
	//double delta_n;
	int i,j;

	if(err_flag==0)
	{
		for (i=0;i<(2*dimension+1);i++)
		{
			for (j=0;j<observations;j++)
			{
				meaPredPts[i][j] = statePredPts[i][j]+point[i][j+states+Qnoise];
			}
		}
	}

	if(err_flag==1)
	{
		for (i=0;i<(2*dimension+1);i++)
		{
			for (j=0;j<3;j++)                          ///w = w
			{
				meaPredPts[i][j] = statePredPts[i][j];
			}

			for (j=0;j<4;j++)
			{
				quaternion[j] = statePredPts[i][j+3];
			}
			for(j=0;j<3;j++){

				vectorg[j]=gravity_initial[j];
				meaPredPts[i][j]=statePredPts[i][j]+point[i][j+states+Qnoise];   // w=w+n
			}

		//	Normal_quaternion(quaternion);
		    Quaternion_VecXform(quaternion,vectorg);
		    for (j=0;j<3;j++)
		    {
			    meaPredPts[i][j+3]=vectorg[j]+point[i][j+states+Qnoise+3];
		    }

		}
	}

	if(err_flag==2)
	{
		for (i=0;i<(2*dimension+1);i++)
		{
			for (j=0;j<3;j++)                          ///w = w
			{
				meaPredPts[i][j] = statePredPts[i][j];
			}

			for (j=0;j<4;j++)
			{
				quaternion[j] = statePredPts[i][j+3];
			}
			for(j=0;j<3;j++){

				vectorm[j]=mag_initial[j];
				meaPredPts[i][j]=statePredPts[i][j]+point[i][j+states+Qnoise];   // w=w+n
			}

		//	Normal_quaternion(quaternion);
		    Quaternion_VecXform(quaternion,vectorm);
		    for (j=0;j<3;j++)
		    {
			    meaPredPts[i][j+3]=vectorm[j]+point[i][j+states+Qnoise+3];
		    }
		}
	}

	if(err_flag==3)
	{
		for (i=0;i<(2*dimension+1);i++)
		{
			for (j=0;j<observations;j++)                          ///w = w
			{
				meaPredPts[i][j] = statePredPts[i][j]+point[i][j+states+Qnoise];
			}
		}
	}
	


}

void QuaternionFusion::Quaternion_Multiple(double *a,double *b){
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

void QuaternionFusion::Quaternion_VecRot(double *a, double *vctors){           ///qvrot
	
	double aug_vc[]={vctors[0],vctors[1],vctors[2],0};
	double aa[]={a[0],a[1],a[2],a[3]};
	double inva[]={-aa[0],-aa[1],-aa[2],aa[3]};

	/////////////////////////////////////////////
     ////BY cj
	//////////////////////////////////
	Quaternion_Multiple(aug_vc,inva);
	Quaternion_Multiple(aa,aug_vc);

   // Normal_quaternion(aa);
	
	////////////////////////////////////////
	for (int i=0;i<3;i++)
	{
		vctors[i]=aa[i];
	}

}

void QuaternionFusion::Quaternion_VecXform(double *a, double *vctors){            ///qvxform
	
	double aug_vc[]={vctors[0],vctors[1],vctors[2],0};
	double aa[]={a[0],a[1],a[2],a[3]};
	double inva[]={-aa[0],-aa[1],-aa[2],aa[3]};

	/////////////////////////////////////////////
     ////BY cj
	//////////////////////////////////
	Quaternion_Multiple(aug_vc,aa);
	Quaternion_Multiple(inva,aug_vc);

   // Normal_quaternion(aa);
	
	////////////////////////////////////////
	for (int i=0;i<3;i++)
	{
		vctors[i]=inva[i];
	}

}

void QuaternionFusion::Vector_Multiple(double *col, double *row, double **retrunback,int col_length,int row_length){
	int i,j;
	for (j=0;j<row_length;j++)
	{
		for (i=0;i<col_length;i++)
		{
			retrunback[i][j]=row[j]*col[i];
		}
	}

}


//This function was found in the Internet.

int QuaternionFusion::Matrix_inv(double a[], int n)
{
	int *is,*js,i,j,k,l,u,v;
	double d,p;
	is=(int*)malloc(n*sizeof(int));
	js=(int*)malloc(n*sizeof(int));
	for (k=0; k<=n-1; k++)
	{ d=0.0;
	for (i=k; i<=n-1; i++)
		for (j=k; j<=n-1; j++)
		{ l=i*n+j; p=fabs(a[l]);
		if (p>d) { d=p; is[k]=i; js[k]=j;}
		}
		if (d+1.0==1.0)
		{ free(is); free(js); printf("error... no invert exists\n");
		return(0);
		}
		if (is[k]!=k)
			for (j=0; j<=n-1; j++)
			{ u=k*n+j; v=is[k]*n+j;
			p=a[u]; a[u]=a[v]; a[v]=p;
			}
			if (js[k]!=k)
				for (i=0; i<=n-1; i++)
				{ u=i*n+k; v=i*n+js[k];
				p=a[u]; a[u]=a[v]; a[v]=p;
				}
				l=k*n+k;
				a[l]=1.0/a[l];
				for (j=0; j<=n-1; j++)
					if (j!=k)
					{ u=k*n+j; a[u]=a[u]*a[l];}
					for (i=0; i<=n-1; i++)
						if (i!=k)
							for (j=0; j<=n-1; j++)
								if (j!=k)
								{ u=i*n+j;
								a[u]=a[u]-a[i*n+k]*a[k*n+j];
								}
								for (i=0; i<=n-1; i++)
									if (i!=k)
									{ u=i*n+k; a[u]=-a[u]*a[l];}
	}
	for (k=n-1; k>=0; k--)
	{ if (js[k]!=k)
	for (j=0; j<=n-1; j++)
	{ u=k*n+j; v=js[k]*n+j;
	p=a[u]; a[u]=a[v]; a[v]=p;
	}
	if (is[k]!=k)
		for (i=0; i<=n-1; i++)
		{ u=i*n+k; v=i*n+is[k];
		p=a[u]; a[u]=a[v]; a[v]=p;
		}
	}
	free(is); free(js);
	return(1);
}

void QuaternionFusion::Matrix_Multiple(double **a, double **b, int m, int n,int p, double **c)
{
	int i,j,k;
	for(i=0;i<m;i++)
		for(j=0;j<p;j++){
			c[i][j]=0;
			for(k=0;k<n;k++)
				c[i][j]+=a[i][k]*b[k][j];
	}
}


void QuaternionFusion::FQA(double *norm_measure)
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

	Normal_quaternion(q_z);
	for (int ii=0;ii<4;ii++)
	{
		obs_quat[ii] = q_z[ii];
	//	obs_quat[i] = pre_obs_quat[i];
	}

}



void QuaternionFusion::FQA_to_Bodyframe(double *quaternion)
{
	double init_rotate[4];
	init_rotate[0] = -quat_initial[0];
	init_rotate[1] = -quat_initial[1];
	init_rotate[2] = -quat_initial[2];
	init_rotate[3] = quat_initial[3];

	double ErrPos[4],ErrNeg[4];

//	Normal_quaternion(init_rotate);
//	Normal_quaternion(init_rotate);
//	Normal_quaternion(quaternion);

	Quaternion_Multiple(init_rotate,quaternion);

	if (first_flag==1)
	{
		double temp = 0;
		for (int i=0;i<4;i++)
		{
			ErrPos[i] =  init_rotate[i]-state_estimate[i+3];
			ErrNeg[i] =  init_rotate[i]+state_estimate[i+3];
			temp = temp+ErrPos[i]*ErrPos[i]-ErrNeg[i]*ErrNeg[i];
		}
		if(temp>0)
		{
			init_rotate[0] = -init_rotate[0];
			init_rotate[1] = -init_rotate[1];
			init_rotate[2] = -init_rotate[2];
			init_rotate[3] = -init_rotate[3];
		}
	}
	for (int i=0;i<4;i++)
	{
		quaternion[i] = init_rotate[i];
	}
	//Normal_quaternion[i]

	//return(normal);
}

void QuaternionFusion::Normal_quaternion(double *quaternion)
{
	double normal;
	normal = quaternion[0]*quaternion[0]+quaternion[1]*quaternion[1]+quaternion[2]*quaternion[2]+quaternion[3]*quaternion[3];
	normal = sqrt(normal);

	for (int i=0;i<4;i++)
	{
		quaternion[i]=quaternion[i]/normal;
	}
	//return(normal);
}

double QuaternionFusion::Normal_vector(double *object)
{
	double normal;
	normal = object[0]*object[0]+object[1]*object[1]+object[2]*object[2];
	normal = sqrt(normal);
	return(normal);
}

void QuaternionFusion::Adaptive_noise(double *measure)
{
	int i;
	double norm_gravity,norm_magnetic;
	double Acc[3],Mag[3];
	double DOC_A,DOC_M;   //degree of confidence
	for(i=0;i<3;i++)
	{
		Acc[i] = measure[i];
		Mag[i] = measure[i+6];
	}
	norm_gravity = Normal_vector(Acc);
	norm_magnetic = Normal_vector(Mag);
	DOC_A = norm_gravity/standard_gravity;
	DOC_M = norm_magnetic/standard_magnetic;

	if(DOC_A<1)
	{
		DOC_A = 1/DOC_A;
	}
	if(DOC_M<1)
	{
		DOC_M = 1/DOC_M;
	}
	DOC_A = DOC_A-1;
	DOC_M = DOC_M-1;
	
	double threshold = 1000;

	err_flag = 0;
	if(DOC_A > threshold && DOC_M > threshold)
	{
		err_flag = 3;
		Rnoise = 3;
		observations = 3;
		dimension = 17;
		R[0][0] = 0.5;
		R[1][1] = 0.5;
		R[2][2] = 0.5;
	}
	else if(DOC_A > threshold && DOC_M < threshold)      ///magnetic is needed
	{
		err_flag = 2;
		Rnoise = 6;
		observations = 6;
		dimension = 20;
		R[0][0] = 0.5;
		R[1][1] = 0.5;
		R[2][2] = 0.5;
		R[3][3] = 0.1;
		R[4][4] = 0.1;
		R[5][5] = 0.1;
	}
	else if(DOC_A < threshold && DOC_M > threshold)    ///gravity is needed
	{
		err_flag = 1;
		Rnoise = 6;
		observations = 6;
		dimension = 20;
		R[0][0] = 0.5;
		R[1][1] = 0.5;
		R[2][2] = 0.5;
		R[3][3] = 0.1;
		R[4][4] = 0.1;
		R[5][5] = 0.1;
	}
	else if(DOC_A < threshold && DOC_M < threshold)
	{
		err_flag = 0;
		Rnoise = 7;
		observations = 7;
		dimension = 21;
		R[0][0] = 0.5;
		R[1][1] = 0.5;
		R[2][2] = 0.5;
		R[3][3] = 0.1;
		R[4][4] = 0.1;
		R[5][5] = 0.1;
		R[6][6] = 0.1;
	}
	
}

void QuaternionFusion::multi_measurement()
{
	int i;
//	double Acc[3],Mag[3];

	for(i=0;i<3;i++)
	{
		measurement[i] = norm_measure[i+3];
	}
	if(err_flag==0)
	{	for(i=0;i<4;i++)
		{
			measurement[i+3] = obs_quat[i];
		}
	}
	else if(err_flag==1)               ///ACC
	{
		for(i=0;i<3;i++)
		{
			measurement[i+3] = norm_measure[i];
		}
	}
	else if(err_flag==2)              //MAG
	{
		for(i=0;i<3;i++)
		{
			measurement[i+3] = norm_measure[i+6];
		}
	}
}

void QuaternionFusion::normalization(double *measure)
{
	int i;
	double Acc[3],Mag[3];
    for( i=0;i<3;i++)
	{
		Acc[i] = measure[i];
		Mag[i] = measure[i+6];
	}
	double norm_Acc = Normal_vector(Acc);
	double norm_Mag = Normal_vector(Mag);
	for( i=0; i<3; i++)
	{
		norm_measure[i]= Acc[i]/norm_Acc;
		norm_measure[i+6]= Mag[i]/norm_Mag;
		norm_measure[i+3] = measure[i+3];//½ÇËÙ¶ÈÈ¥Æ¯ÒÆ
	}
}