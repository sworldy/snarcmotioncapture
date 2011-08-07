#include "math.h"
#include <stdlib.h>
#include "stdio.h"
#include "QuaternionFusion.h"
#include "DisplacementFusion.h"
//#include "Math.hpp"
#define SENSOR_NUM 16
#define DIM_MEAS 9
#define DIM_VEC 3
#define DIM_QUAT 4
#define PI 3.1415926535
#define TRAINING_NUM 200
#define COF_METER2PIXEL 1
#define SENID_MAGREF 4 // Sensor ID of magnetic reference // Need_Check
//int num2QKFID[SENSOR_NUMBER];
//   int toCopy[16] = {19,20,21,1,3,4,6,15,16,17,8,9,10,12,13,14};
//using namespace std;
class Fusion
{
public:
	// Variables
	double Mag_Ref[DIM_VEC]; // Magnetic reference vector
	double MagRef_Training[DIM_MEAS]; // Training measurement, to get magnetic reference vector
	double Measure_Buf[SENSOR_NUM][DIM_MEAS];
	double Quaternions[SENSOR_NUM][DIM_QUAT];
	double Counter_Training; // Time steps for training
	double Quat_RS2GB[SENSOR_NUM][DIM_QUAT]; // Quaternion for frame transformation, from qGS to qGB 
	double Quat_GB2MA[SENSOR_NUM][DIM_QUAT]; // Quaternion for frame transformation, from qGB to qMA 
	double Quaternionnew[SENSOR_NUM][DIM_QUAT];     

	// Class and Obejects
	QuaternionFusion *QuaternionsFilter[SENSOR_NUM];
	DisplacementFusion *Displacement;
	
	// Functions
	Fusion();
	~Fusion();
	bool WholeBodyFusion(double QuatResult[][DIM_QUAT],double *DisResult);
	void WBQuatFusion_Filtering(); // Whole Body Quaternion Fusion Filtering
	int Get_Sign(double real_num); // Get = Get the positive or negtive sign of a real number. 
	void Quaternion_Multiple(double *quat_result,double *quat_input);
	void Quaternion_VecXform(double *Quat, double *Vec_Result); //qvxform
	void Quaternion_VecRot(double *Quat, double *Vec_Result); //qvrot
	void FrameTrans_RS2GB(double *Quat_Result,double *Quat_Input); // Frame Transformation, from qRS to qGB
	void FrameTrans_GB2MA(double *Quat_Result, double *Quat_Input); // Frame Transformation, from qGB to qMA 
	bool Get_Measure(int id, double *meas); //获得并设置初始参数
	void FQA(double *Mea_Normalized, double *Quat_Result);
	void Get_MagRef(double *Mea_Normalized);// Get the earth magnetic reference. 
	//void Get_QuatRS2GB(double *Mea_Normalized); // Get the quaternion for frame transformation, from qGS to qGB
	void Get_QuatRS2GB(); // Get the quaternion for frame transformation, from qGS to qGB
	void Get_QuatGB2MA(); // Get the quaternion for frame transformation, from qGB to qMA 
	double Get_VecNorm(double *object); // Get the norm of a vector
	void Quat_Normalization(double *Quat_Result); // Quaternion normalization
	void Meas_Normalization(double *Meas_Result); // Measurement normailization
	void Vect_Normalization(double *Vect_Result); // Vector normalization
	void Quaternion_Augment(double Quat_ResAug[][DIM_QUAT],double Quat_Result[][DIM_QUAT]);// The skeleton No is 22, while the sensor No is 16
	void Quaternion_Augment2(double Quat_Result[][DIM_QUAT]);
	void FrameTrans_RS2GB2(double *Quat_Result, double *Quat_Input,double *Quat_Output);
};