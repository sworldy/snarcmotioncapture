#ifndef FILTER_ABSTRACT_H
#define FILTER_ABSTRACT_H
//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//




/*****************************************************************************/
/** 
 * FilterAbstract defines the API that the algorithm of Kalman Filter
 * relies on. A third party Kalman Filter algorithm must
 * implement this API in order to be integrated with Snarc 3D.
 *
 * Snarc 3D uses a Kalman Filter algorithm to handle the task of producing the
 * estimates of the true values of measurements of the sensor. 
 * If an algorithm proves to be insufficient for 
 * the applications needs, an alternate algorithm can be implemented and used 
 * without notifying Snarc 3D in any way. 
 *
 * Subclassing FilterAbstract when implementing an alternate 
 * algorithm therefore provides a better integration with Snarc 3D.
 *
 *****************************************************************************/

class FilterAbstract
{
public:
	FilterAbstract(){}
	virtual ~FilterAbstract(){}
   /*****************************************************************************/
   /**
   * Kalman Filter process
   *
   * @param meas the measurement from the sensor in the order of 
   *             acc_x acc_y acc_z angular_x angular_y angular_z mag_x mag_y mag_z
   *
   *	    result the result of the algorithm in the order x y z w
   * @return  true if the algorithm executes right
   *          false if the algorithm executes wrong
   *
   *****************************************************************************/
	virtual void QuaternionFusion_Filtering(double *meas, double *result) = 0;

};

#endif
//****************************************************************************//
