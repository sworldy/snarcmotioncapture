#include "stdafx.h"
#include "qkf.h"

QKF::QKF(double *gintial, double *minitial){
	
	int i,j;
	dimension=23;

	for(i=0;i<dimension;i++){
		for (j=0;j<dimension;j++)
		{
			Augmented_P[i][j]=0;
		}	
	}

	for(i=0;i<(2*dimension+1);i++){
		for (j=0;j<dimension;j++)
		{
			point[i][j]=0;
			weight_m[i][j]=0;
			weight_c[i][j]=0;
		}	
	}

	for(i=0;i<(2*dimension+1);i++){
		for (j=0;j<7;j++)
		{
			statePredPts[i][j]=0;
			statePredPts_innovation[i][j]=0;
		}
		for (j=0;j<9;j++)
		{
			meaPredPts[i][j]=0;
			meaPredPts_innovation[i][j]=0;
		}
	}


	alpha=1;
	beta=0;
	kappa=2;
//	delta_t=0.02;
  delta_t=0.02;
	for(i=0;i<7;i++){
		for (j=0;j<7;j++)
		{
			Q[i][j]=0;
			P_0[i][j]=0;
			P_estimate[i][j]=0;
			P_predict[i][j]=0;
		}
		state_estimate[i]=0;
		state_predict[i]=0;
	}
	for (i=0;i<9;i++)
	{
		for (j=0;j<9;j++)
		{
			R[i][j]=0;
		}
	}
	Q[0][0]=0.001;
	Q[1][1]=0.001;
	Q[2][2]=0.001;
	Q[3][3]=0.001;
	Q[4][4]=0.51;
	Q[5][5]=0.51;
	Q[6][6]=0.51;

	R[0][0]=2;
	R[1][1]=2;
	R[2][2]=2;
	R[3][3]=0.5;
	R[4][4]=0.5;
	R[5][5]=0.5;
	R[6][6]=2;
	R[7][7]=2;
	R[8][8]=2;
	
	for (i=0;i<7;i++)
	{
		P_0[i][i]=0.1;
		P_estimate[i][i]=0.1;
	}
	state_estimate[0]=0;
	state_estimate[1]=0;
	state_estimate[2]=0;
	state_estimate[3]=1;
	state_estimate[4]=0.001;
	state_estimate[5]=0.001;
	state_estimate[6]=0.001;
	
	gravity_initial[0]=gintial[0];
	gravity_initial[1]=gintial[1];
	gravity_initial[2]=gintial[2];
	mag_intitial[0]=minitial[0];
	mag_intitial[1]=minitial[1];
	mag_intitial[2]=minitial[2];

}

QKF::~QKF(){
/*
		free(point);
		free(weight_c);
		free(weight_m);
		free(Augmented_P);
		free(statePredPts);
		free(meaPredPts);*/
	
}

void QKF::QKF_Filtering(double *measure){

	Augmented_Variance();
	cholesky(dimension);
	SigmaPoint(dimension);
	predict();
	Update(measure);

/*		
			int i,j;
		for (i=0;i<41;i++)
		{
			for (j=0;j<dimension;j++)
			{
				cout<<point[i][j]<<" ";
			}
			cout<<endl;
		}
	
		cout<<"aaa";*/
	
	
}
void QKF::Augmented_Variance(){
	int i,j;
	for (i=0;i<dimension;i++)
	{
		for (j=0;j<dimension;j++)
		{
			Augmented_P[i][j]=0;
		}
	}
	for (i=0;i<7;i++)
	{
		for (j=0;j<7;j++)
		{
			Augmented_P[i][j]=P_estimate[i][j];
		}
	}
	for (i=0;i<7;i++)
	{
		for (j=0;j<7;j++)
		{
			Augmented_P[7+i][7+j]=Q[i][j];
		}
	}
	for (i=0;i<9;i++)
	{
		for (j=0;j<9;j++)
		{
			Augmented_P[7+7+i][7+7+j]=R[i][j];
		}
	}
}
	

void QKF::cholesky(int n) {  
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
		
			free(L);
/*
				for (i=0;i<n;i++){
					for(j=0;j<n;j++)
						cout<<L[i][j]<<" ";
					cout<<endl;
				}*/		
		
} 

void QKF::SigmaPoint(int dimension){
	int i,j;
	double weight1,weight2,weight3;
	weight1=kappa/(dimension+kappa);
	weight2=weight1+(1-alpha*alpha) + beta;
	weight3=1.0/(2*(dimension+kappa));
	for (i=0;i<(2*dimension+1);i++)
	{
		for (j=0;j<7;j++)
		{
			point[i][j]=state_estimate[j];
		}
		for (j=7;j<dimension;j++)
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
}

	
void QKF::predict(){
	double **tmp_varaianc;	
	int i,j,k,l;


	tmp_varaianc=(double  **)malloc(9*sizeof(double));  
	for(i=0;i<9;i++)  
		tmp_varaianc[i]=(double *)malloc(9*sizeof(double));
	double stat_inno_vec[7],meas_inno_vec[9];

	ProcssModel();
	MeasureModel();
	

	


	//State prediction;
	for (j=0;j<7;j++)
	{
		state_predict[j]=0;
		for (i=0;i<(2*dimension+1);i++)
		{
			state_predict[j]+=statePredPts[i][j]*weight_m[i][j];
		}
	}

	//Measurement Prediction
	for (j=0;j<9;j++)
	{
		mesure_predict[j]=0;
		for (i=0;i<(2*dimension+1);i++)
		{
			mesure_predict[j]+=meaPredPts[i][j]*weight_m[i][j];
		}
	}

	//The innovation among state prediction and state prediction sigma points
	//The innovation among measurement prediction and measurement prediction sigma points

	for (i=0;i<(2*dimension+1);i++)
	{
		for (j=0;j<7;j++)
		{
			statePredPts_innovation[i][j]=statePredPts[i][j]-state_predict[j];
		}
		for (j=0;j<9;j++)
		{
			meaPredPts_innovation[i][j]=meaPredPts[i][j]-mesure_predict[j];
		}
	}
	
	for (i=0;i<7;i++)
	{
		for (j=0;j<7;j++)
		{
			P_predict[i][j]=0;
		}
		for (j=0;j<9;j++)
		{
			XZ_Cov[i][j]=0;
		}
	}

	for (i=0;i<9;i++)
	{
		for (j=0;j<9;j++)
		{
			Z_Cov[i][j]=0;
		}
	}


	for (i=0;i<(2*dimension+1);i++)
	{
		for (j=0;j<7;j++)
		{
			stat_inno_vec[j]=statePredPts_innovation[i][j];
		}
		for (j=0;j<9;j++)
		{
			meas_inno_vec[j]=meaPredPts_innovation[i][j];
		}
		Vector_Multiple(stat_inno_vec,stat_inno_vec,tmp_varaianc,7,7);
		for (k=0;k<7;k++)
		{
			for (l=0;l<7;l++)
			{
				P_predict[k][l]+=tmp_varaianc[k][l]*weight_c[i][0];
			}
		}


		Vector_Multiple(meas_inno_vec,meas_inno_vec,tmp_varaianc,9,9);

		for (k=0;k<9;k++)
		{
			for (l=0;l<9;l++)
			{
				Z_Cov[k][l]+=tmp_varaianc[k][l]*weight_c[i][0];
			}
		}

		Vector_Multiple(stat_inno_vec,meas_inno_vec,tmp_varaianc,7,9);
		for (k=0;k<7;k++)
		{
			for (l=0;l<9;l++)
			{
				XZ_Cov[k][l]+=tmp_varaianc[k][l]*weight_c[i][0];
			}
		}
	}

	free(tmp_varaianc);

}

void QKF::Update(double *measurment){

	double measurement_innovation[9];
	int i,j;
	double tmp;

	double **gain, **Z_Cov_inv,**XZ_Cov_point, **cov_error,**gaintrans;

	gain=(double  **)malloc(7*sizeof(double));  
	for(i=0;i<7;i++)  
		gain[i]=(double *)malloc(9*sizeof(double));

	XZ_Cov_point=(double  **)malloc(7*sizeof(double));  
	for(i=0;i<7;i++)  
		XZ_Cov_point[i]=(double *)malloc(9*sizeof(double));

	cov_error=(double  **)malloc(7*sizeof(double));  
	for(i=0;i<7;i++)  
		cov_error[i]=(double *)malloc(7*sizeof(double));

	Z_Cov_inv=(double  **)malloc(9*sizeof(double));  
	for(i=0;i<9;i++)  
		Z_Cov_inv[i]=(double *)malloc(9*sizeof(double));

	gaintrans=(double  **)malloc(9*sizeof(double));  
	for(i=0;i<9;i++)  
		gaintrans[i]=(double *)malloc(7*sizeof(double));

	
	

	double Z_Cov_back[9][9];

	for (i=0;i<9;i++)
	{
		measurement_innovation[i]=measurment[i]-mesure_predict[i];
	}

	for (i=0;i<9;i++)
	{
		for (j=0;j<9;j++)
		{
			Z_Cov_back[i][j]=Z_Cov[i][j];
		}
	}

	Matrix_inv(&Z_Cov_back[0][0],9);

	for (i=0;i<9;i++)
	{
		for (j=0;j<9;j++)
		{
			Z_Cov_inv[i][j]=Z_Cov_back[i][j];
		}
	}
	for (i=0;i<7;i++)
	{
		for (j=0;j<9;j++)
		{
			XZ_Cov_point[i][j]=XZ_Cov[i][j];
		}
	}
	Matrix_Multiple(XZ_Cov_point,Z_Cov_inv,7,9,9,gain);

	
	//update the state;
	for (i=0;i<7;i++)
	{
		tmp=0;
		for (j=0;j<9;j++)
		{
			tmp+=gain[i][j]*measurement_innovation[j];
		}
		state_estimate[i]=state_predict[i]+tmp;
	}

	double qnorm=0;
	for (i=0;i<4;i++)
	{
		qnorm+=state_estimate[i]*state_estimate[i];
	}
	qnorm=sqrt(qnorm);
	for (i=0;i<4;i++)
	{
		state_estimate[i]=state_estimate[i]/qnorm;
	}
	
	//update the covariance.

	for (i=0;i<7;i++)
	{
		
		for (j=0;j<9;j++)
		{
			gaintrans[j][i]=gain[i][j];
		}
	}
	Matrix_Multiple(XZ_Cov_point,gaintrans,7,9,7,cov_error);

	for (i=0;i<7;i++)
	{
		
		for (j=0;j<7;j++)
		{
			P_estimate[i][j]=P_predict[i][j]-cov_error[i][j];
		}
	}

	free(gain);
	free(Z_Cov_inv);
	free(XZ_Cov_point);
	free(cov_error);
	free(gaintrans);


}


void QKF::ProcssModel(){
	double quaternion[4],quaternion_angular[4],angular[3];
	double angular_norm;
	int i,j;
	for (i=0;i<(2*dimension+1);i++)
	{
		angular_norm=0;
		for (j=0;j<4;j++)
		{
			quaternion[j]=point[i][j];
		}
		for (j=0;j<3;j++)
		{
			angular[j]=point[i][j+4];
			statePredPts[i][j+4]=angular[j]+point[i][j+4+7];
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
		for (j=0;j<4;j++)
		{
			statePredPts[i][j]=quaternion[j]+point[i][j+7];
		}
	}

}


void QKF::MeasureModel(){
	double quaternion[4]; 
	double vectors[3],vectorm[3];
	int i,j;
	for (i=0;i<(2*dimension+1);i++)
	{
		for (j=0;j<4;j++)
		{
			quaternion[j]=statePredPts[i][j];
		}
		for(j=0;j<3;j++){
			vectors[j]=gravity_initial[j];
			vectorm[j]=mag_intitial[j];
			meaPredPts[i][j+3]=statePredPts[i][j+4]+point[i][j+7+7+3];
		}
		Quaternion_Rotation(quaternion,vectors);
		Quaternion_Rotation(quaternion,vectorm);
		for (j=0;j<3;j++)
		{
			meaPredPts[i][j]=vectors[j]+point[i][j+7+7];
			meaPredPts[i][j+6]=vectorm[j]+point[i][j+7+7+6];
		}

	}


}

void QKF::Quaternion_Multiple(double *a,double *b){
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

void QKF::Quaternion_Rotation(double *a, double *vctors){
	double inva[]={-a[0],-a[1],-a[2],a[3]};
	double aug_vc[]={vctors[0],vctors[1],vctors[2],0};
	Quaternion_Multiple(aug_vc,a);
	Quaternion_Multiple(inva,aug_vc);
	for (int i=0;i<3;i++)
	{
		vctors[i]=inva[i];
	}

}

void QKF::Vector_Multiple(double *col, double *row, double **retrunback,int col_length,int row_length){
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

int QKF::Matrix_inv(double a[], int n)
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

void QKF::Matrix_Multiple(double **a, double **b, int m, int n,int p, double **c)
{
	int i,j,k;
	for(i=0;i<m;i++)
		for(j=0;j<p;j++){
			c[i][j]=0;
			for(k=0;k<n;k++)
				c[i][j]+=a[i][k]*b[k][j];
	}
}