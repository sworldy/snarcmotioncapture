#ifndef DISPLACEMENT_ABSTRACT_H
#define DISPLACEMENT_ABSTRACT_H
//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//




/*****************************************************************************/
/** 
 * DisplacementAbstract defines the API that the algorithm of displacement
 * estimation relies on. A third party displacement algorithm must
 * implement this API in order to be integrated with Snarc 3D.
 *
 * Snarc 3D uses a displacement algorithm to handle the task of estimating the 
 * displacement of a moving object. If an algorithm proves to be insufficient for 
 * the applications needs, an alternate algorithm can be implemented and used 
 * without notifying Snarc 3D in any way. 
 *
 * Subclassing DisplacementAbstract when implementing an alternate 
 * algorithm therefore provides a better integration with Snarc 3D.
 *
 *****************************************************************************/

class DisplacementAbstract
{
public:
	DisplacementAbstract(){}
	virtual ~DisplacementAbstract() {}
   /*****************************************************************************/
   /**
   * Set the parameter of displacement algorithm.
   *
   * @param id The ID of the bone.
   *        quat the quaternion of the bone in the order of x y z w
   *        meas the measurement from the sensor in the order of acc_x acc_y acc_z angular_x angular_y angular_z
   * @return  true if SetPara executes right
   *          false if SetPara executes wrong
   *
   *****************************************************************************/
	virtual bool SetPara(int id, double *quat, double *meas) = 0;


   /*****************************************************************************/
   /**
   * Estimate the displacement.
   *
   * @param result the result of the algorithm in the order x y z
   * @return  true if the algorithm executes right
   *          false if the algorithm executes wrong
   *
   *****************************************************************************/
	//virtual bool EstimateDis(double &result) = 0;
	virtual bool EstimateDis(double *result) = 0;//采用四元数计算位移

};

#endif
//****************************************************************************//
