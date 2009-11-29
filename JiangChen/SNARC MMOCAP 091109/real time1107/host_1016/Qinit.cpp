#include "stdafx.h"
#include "Qinit.h"
using namespace std;

Qinit::Qinit(double * ref, double * gobs, double *mobs)
{
	g[0]= ref[0];
	g[1]= ref[1];
	g[2]= ref[2];
	m[0]= ref[3];
	m[1]= ref[4];
	m[2]= ref[5];
	gObs[0]= gobs[0];
	gObs[1]= gobs[1];
	gObs[2]= gobs[2];
	mObs[0]= mobs[0];
	mObs[1]= mobs[1];
	mObs[2]= mobs[2];
}
Qinit::~Qinit()
{

}
//计算两个向量的叉积，aXb->c
void Qinit::cross_product(double * a, double * b, double * c)   
{   
	double aX, aY, aZ;
	double bX, bY, bZ;

	aX=a[0];
	aY=a[1];
	aZ=a[2];
	bX=b[0];
	bY=b[1];
	bZ=b[2];

	c[0]=aY*bZ-aZ*bY;
	c[1]=aZ*bX-aX*bZ;
	c[2]=aX*bY-aY*bX;

} 
//Display a matrix on screen (for debugging only)
void Qinit::matPrint(double **m, int size1, int size2) 
{

	int i,j;
	for (i=0; i< size1; i++) {
		for (j=0; j< size2; j++) {
			cout << m[i][j] << ' ';
		}
		cout << '\n';
	}   
	cout << '\n';

}

//Display a vector on screen (for debugging only)
void Qinit::vecPrint(double *v,int size)
{

	int i;
	for (i=0; i< size; i++) {
		cout << v[i] << ' ';
	}
	cout << '\n';

}
//采用部分主元法的高斯消去法求方阵A的逆矩阵B    
//    
//A 方阵  (IN)    
//n 方阵的阶数 (IN)    
//B 方阵  (OUT)    
//返回true 说明正确计算出逆矩阵    
//返回false说明矩阵A不存在逆矩阵    

void Qinit::gaussj(double** MatInit, double **MatInv)   
{   
	int i,j,k;   
	double lMax,temp;   
	double B[3][3];
	double A[3][3];

	for(i=0;i<3;i++)   
	{   
		for(j=0;j<3;j++)   
		{ 
			A[i][j] = MatInit[i][j];
		}
	}
	//临时矩阵存放A    
	double tt[3][3];   
	for(i=0;i<3;i++)   
	{   
		for(j=0;j<3;j++)   
		{   
			tt[i][j] = A[i][j];   
		}   
	}   

	//初始化B为单位阵    
	for(i=0;i<3;i++)   
	{   
		for(j=0;j<3;j++)   
		{   
			if(i!=j)B[i][j] = 0;   
			else B[i][j] = 1;   
		}   
	}   
	for(i=0;i<3;i++)   
	{   
		//寻找主元    
		lMax = tt[i][i];   
		k = i;   
		for(j=i+1;j<3;j++) //扫描从i+1到n的各行    
		{   
			if( fabs(tt[j][i]) > fabs(lMax))   
			{   
				lMax = tt[j][i];   
				k = j;   
			}   
		}   
		//如果主元所在行不是第i行，进行行交换    
		if(k!=i)   
		{   
			for(j=0;j<3;j++)   
			{   
				temp = tt[i][j] ;   
				tt[i][j] = tt[k][j];   
				tt[k][j] = temp;   
				//B伴随计算    
				temp = B[i][j];   
				B[i][j] = B[k][j];   
				B[k][j] = temp;   
			}   
		}   
		//判断主元是否是0，如果是，则矩阵A不是满秩矩阵，不存在逆矩阵    
		if(tt[i][i] == 0) printf("matrix is not full rank!!\n");   
		//消去A的第i列除去i行以外的各行元素    
		temp = tt[i][i];   
		for(j=0;j<3;j++)   
		{   
			tt[i][j] = tt[i][j] / temp; //主对角线上元素变成1    
			B[i][j] = B[i][j] / temp; //伴随计算    
		}   

		for(j=0;j<3;j++) //行 0 -> n    
		{   
			if(j!=i)  //不是第i行    
			{   
				temp = tt[j][i];   
				for(k=0;k<3;k++) // j行元素 - i行元素* j行i列元素    
				{   
					tt[j][k] = tt[j][k] - tt[i][k] * temp;   
					B[j][k] = B[j][k] - B[i][k] * temp;   
				}   
			}   
		}   
	}   
	for(i=0;i<3;i++)   
	{   
		for(j=0;j<3;j++)   
		{ 
			MatInv[i][j] = B[i][j];
		}
	}

}   

//计算两个矩阵的乘积    
// AB -> C    
void Qinit::Multi(double** A_Mat, double** B_Mat, double **MatMul)   
{   

	int i,j,k; 
	double A[3][3];
	double B[3][3];
	double C[3][3];

	for(i=0;i<3;i++)   
	{   
		for(j=0;j<3;j++)   
		{ 
			A[i][j] = A_Mat[i][j];
		}
	}
	for(i=0;i<3;i++)   
	{   
		for(j=0;j<3;j++)   
		{ 
			B[i][j] = B_Mat[i][j];
		}
	}
	for(i=0;i<3;i++)   
	{   
		for(j=0;j<3;j++)   
		{   
			C[i][j] = 0;   
			for(k=0;k<3;k++)   
			{   
				C[i][j] += A[i][k] * B[k][j];   
			}   
		}   
	}   
	for(i=0;i<3;i++)   
	{   
		for(j=0;j<3;j++)   
		{ 
			MatMul[i][j] =C[i][j];
		}
	}

}   


//convert direction cosine matrices into quaternions
void Qinit::dcmtoq(double** RotMat, double * qInit)
{
	double R11,R22,R33,R23,R32,R31,R13,R12,R21;
	double Diag[3];

	R11 = RotMat[0][0];
	R22 = RotMat[1][1];
	R33 = RotMat[2][2];
	R23 = RotMat[1][2];
	R32 = RotMat[2][1];
	R31 = RotMat[2][0];
	R13 = RotMat[0][2];
	R12 = RotMat[0][1];
	R21 = RotMat[1][0];

	double trace = R11+R22+R33;
	double temp1,temp2;

	if (trace>0)
	{	
		temp1 = sqrt(trace+1.0);  
		qInit[0] = (R23-R32)/(2.0*temp1);
		qInit[1] = (R31-R13)/(2.0*temp1);
		qInit[2] = (R12-R21)/(2.0*temp1);
		qInit[3] = 0.5*temp1;
	}
	else 
	{
		Diag[0] = R11;
		Diag[1] = R22;
		Diag[2] = R33;
		if ((Diag[1] > Diag[0]) && (Diag[1] > Diag[2]))
		{
			// max value at dcm(1,1)
			temp2 = sqrt(Diag[1]-Diag[0]-Diag[2]+1.0);
			qInit[1] = 0.5*temp2;   
			if (temp2 != 0)
				temp2 = 0.5/temp2;
			qInit[0] = (R12+R21)*temp2;
			qInit[2] = (R23+R32)*temp2;
			qInit[3] = (R31-R13)*temp2;
		}
		else if(Diag[2]>Diag[0])
		{
			// max value at dcm(2,2)
			temp2 = sqrt(Diag[2]-Diag[0]-Diag[1]+1.0);
			qInit[2] = 0.5*temp2; 
			if ( temp2 != 0 )
				temp2 = 0.5/temp2;
			qInit[3] = (R12-R21)*temp2;
			qInit[0] = (R31+R13)*temp2;
			qInit[1] = (R23-R32)*temp2;
		}
		else
		{
			// max value at dcm(0,0)			
			temp2 = sqrt(Diag[0]-Diag[1]-Diag[2]+1.0);
			qInit[0] = 0.5*temp2; 
			if ( temp2 != 0 )
				temp2 = 0.5/temp2;
			qInit[3] = (R23-R32)*temp2;
			qInit[1] = (R12+R21)*temp2;
			qInit[2] = (R31+R13)*temp2; 
		}
	}

	double qnorm=0.0;
	for (int i=0;i<4;i++)
	{
		qnorm+=qInit[i]*qInit[i];
	}
	qnorm=sqrt(qnorm);
	for (int i=0;i<4;i++)
	{
		qInit[i]=qInit[i]/qnorm;
	}

}

//计算姿态初值，m，g是参考系初值，mSnr，gSnr是测量值
void Qinit::cmptQinit(double * RotMat)
{
	double *cross;
	cross=(double *)malloc(sizeof(double)*3);

	double *crossObs;
	crossObs=(double *)malloc(sizeof(double)*3);

	cross_product(m,g,cross);
	cross_product(mObs,gObs,crossObs);

	double **aMtrx;
	int i;
	aMtrx=(double **)malloc(3*sizeof(double *));
	for (i=0;i<3;i++)
		aMtrx[i]=(double *)malloc(3*sizeof(double));

	double **aMtrxObs;
	aMtrxObs=(double **)malloc(3*sizeof(double *));
	for (i=0;i<3;i++)
		aMtrxObs[i]=(double *)malloc(3*sizeof(double));

	double **aMtrxInv;
	aMtrxInv=(double **)malloc(3*sizeof(double *));
	for (i=0;i<3;i++)
		aMtrxInv[i]=(double *)malloc(3*sizeof(double));

	double **rotMat;
	rotMat=(double **)malloc(3*sizeof(double *));
	for (i=0;i<3;i++)
		rotMat[i]=(double *)malloc(3*sizeof(double));

	for(int j=0;j<3;j++)
	{
		aMtrx[j][0] = g[j];
		aMtrx[j][1] = m[j];
		aMtrx[j][2] = cross[j];
	}

	for(int j=0;j<3;j++)
	{
		aMtrxObs[j][0] = gObs[j];
		aMtrxObs[j][1] = mObs[j];
		aMtrxObs[j][2] = crossObs[j];
	}

	matPrint(aMtrx,3,3);//输出
	matPrint(aMtrxObs,3,3);//输出

	gaussj(aMtrx,aMtrxInv);  
	Multi(aMtrxObs,aMtrxInv,rotMat);
	for(int i = 0;i<3;i++)
		for(int j=0;j<3;j++)
			RotMat[j+i*3] = rotMat[i][j];
	//dcmtoq(rotMat, qinit);
	//matPrint(RotMat,3,3);
	free(cross);
	free(crossObs);
	for(i=0;i<3; ++i)   
	{   
		free(aMtrx[i]);   
	}   
	free(aMtrx);

	for(i=0;i<3; ++i)   
	{   
		free(aMtrxObs[i]);   
	}
	free(aMtrxObs);

	for(i=0;i<3; ++i)   
	{   
		free(aMtrxInv[i]);   
	}
	free(aMtrxInv);

	for(i=0;i<3; ++i)   
	{   
		free(rotMat[i]);   
	}
	free(rotMat);

}