#include "math.h"
#include <stdlib.h>
#include "stdio.h"

class QKF
{
public:
	double alpha,beta,kappa,delta_t,Q[7][7],R[9][9],P_0[7][7];
	double gravity_initial[3]; //The initial gravity vector;
	double mag_intitial[3]; //The initial magnetic vector
	double state_estimate[7],state_predict[7], mesure_predict[9],P_predict[7][7],P_estimate[7][7],Z_Cov[9][9],XZ_Cov[7][9];
	double Augmented_P[23][23];
	double point[47][23],weight_m[47][23],weight_c[47][23]; //They are all 47 by 20
	double statePredPts[47][7],statePredPts_innovation[47][7]; //47 by 7;
	double meaPredPts[47][9],meaPredPts_innovation[47][9];  //47 by 9;

	int dimension;

	QKF(double *, double *);
	~QKF();
	void QKF_Filtering(double *);
	void Augmented_Variance();
	void cholesky(int);
	void SigmaPoint(int);
	void predict();
	void Update(double *);
	void ProcssModel();
	void MeasureModel();
	void Quaternion_Multiple(double *,double *);
	void Quaternion_Rotation(double *,double *);
	void Vector_Multiple(double *, double*, double **,int,int);
	void Matrix_Multiple(double **, double **, int, int, int, double**);
	int Matrix_inv(double [], int);
};