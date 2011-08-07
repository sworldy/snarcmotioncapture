#include "math.h"
#include <stdlib.h>
#include "stdio.h"
#include "FilterAbstract.h"

#define states 7
#define max_observations  7
#define max_Rnoise  7
#define max_dimension  21
#define Qnoise 7
#define PI 3.1415926535
#define NPI -3.1415926535

class QuaternionFusion:public FilterAbstract
{

public:

//	static const int states;       //NUMBER of states
//	static const int Qnoise = 7; //NUMBER of Qnoise for states
//	static const int max_observations = 7;
//	static const int max_Rnoise = 7;
//	static const int max_dimension = 21;
	int observations; //NUMBER of observations
	int Rnoise; //NUMBER of Rnoise for observations
	int dimension;   //dimension = states + Qnoise + Rnoise

	double alpha,beta,kappa,delta_t,Q[Qnoise][Qnoise],R[max_Rnoise][max_Rnoise],standard_gravity,standard_magnetic;
	double gravity_initial[3]; //The initial gravity vector;
	double mag_initial[3]; //The initial magnetic vector
	double angular_initial[3];//The initial angular rate vector
	double quat_initial[4];
	
	double state_estimate[states],state_predict[states], mesure_predict[max_observations];
	double P_0[states][states],P_predict[states][states],P_estimate[states][states];
	double Z_Cov[max_observations][max_observations],XZ_Cov[states][max_observations];
	double Augmented_P[max_dimension][max_dimension];
	//weight_m:Weights for mean calculation weight_c: Weights for covariance calculation
	double point[2*max_dimension+1][max_dimension],weight_m[2*max_dimension+1][max_dimension],weight_c[2*max_dimension+1][max_dimension]; //

	double statePredPts[2*max_dimension+1][states],statePredPts_innovation[2*max_dimension+1][states]; //2*dimension+1 by states;
	double meaPredPts[2*max_dimension+1][max_observations],meaPredPts_innovation[2*max_dimension+1][max_observations];  //2*dimension+1 by max_observations;
	double pre_obs_quat[4],measurement[max_observations],norm_measure[9];
	double obs_quat[4],attitude_body[4];
	int first_flag,err_flag;
	int counter;
///////////////////////////
	//for debug
//	double gaingain[states][max_dimension];
	////////////////
	//int dimension;

	QuaternionFusion(int);
	~QuaternionFusion();
	void QuaternionFusion_Filtering(double *,double *);
	void Augmented_Variance();
	void cholesky(int);
	void SigmaPoint(int);
	void predict();
	void Update(double *);
	void ProcssModel();
	void MeasureModel();
	void Quaternion_Multiple(double *,double *);
	void Quaternion_VecXform(double *,double *);      //qvxform
	void Quaternion_VecRot(double *,double *);      //qvrot
	void Vector_Multiple(double *, double*, double **,int,int);
	void Matrix_Multiple(double **, double **, int, int, int, double**);
	int Matrix_inv(double [], int);
	void FQA(double *);

	void FQA_to_Bodyframe(double *);
	void Normal_quaternion(double *);
	double Normal_vector(double *);
	void Adaptive_noise(double *);
	void AdaptiveR(double *);
	void Bias_Removed(double *);
	void multi_measurement();
	void normalization(double *);
	int sensorID;
	//double *Normalize_measure(double *);
};