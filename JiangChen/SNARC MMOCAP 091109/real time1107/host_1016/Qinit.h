#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

class Qinit
{
public:
	double g[3], m[3], gObs[3], mObs[3];
	Qinit(double * ref, double * gobs, double *mobs);
	~Qinit();
	void cross_product(double * a, double * b, double * c);
	void matPrint(double **m, int size1, int size2);
	void vecPrint(double *v,int size);
	void gaussj(double** MatInit, double **MatInv);
	void Multi(double** A_Mat, double** B_Mat, double **MatMul);
	void dcmtoq(double** RotMat, double * qInit);
	void cmptQinit(double * qinit);
    
	

};