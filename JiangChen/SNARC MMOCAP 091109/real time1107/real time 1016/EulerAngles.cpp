/**** EulerAngles.c - Convert Euler angles to/from matrix or quat ****/
/* Ken Shoemake, 1993 */
#include "stdafx.h"
#include <math.h>
#include <float.h>
#include "EulerAngles.h"
#include <assert.h>

EulerAngles Eul_(float ai, float aj, float ah, int order)
{
	EulerAngles ea;
	ea.x = ai; ea.y = aj; ea.z = ah;
	ea.w = order;
	return (ea);
}
/* Construct quaternion from Euler angles (in radians). */
Quat Eul_ToQuat(EulerAngles ea)
{
	Quat qu;
	double a[3], ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
	int i,j,k,h,n,s,f;
	EulGetOrd(ea.w,i,j,k,h,n,s,f);
	if (f==EulFrmR) {float t = ea.x; ea.x = ea.z; ea.z = t;}
	if (n==EulParOdd) ea.y = -ea.y;
	ti = ea.x*0.5; tj = ea.y*0.5; th = ea.z*0.5;
	ci = cos(ti);  cj = cos(tj);  ch = cos(th);
	si = sin(ti);  sj = sin(tj);  sh = sin(th);
	cc = ci*ch; cs = ci*sh; sc = si*ch; ss = si*sh;
	if (s==EulRepYes) {
		a[i] = cj*(cs + sc);	/* Could speed up with */
		a[j] = sj*(cc + ss);	/* trig identities. */
		a[k] = sj*(cs - sc);
		qu.w = cj*(cc - ss);
	} else {
		a[i] = cj*sc - sj*cs;
		a[j] = cj*ss + sj*cc;
		a[k] = cj*cs - sj*sc;
		qu.w = cj*cc + sj*ss;
	}
	if (n==EulParOdd) a[j] = -a[j];
	qu.x = a[X]; qu.y = a[Y]; qu.z = a[Z];
	return (qu);
}

/* Construct matrix from Euler angles (in radians). */
void Eul_ToHMatrix(EulerAngles ea, HMatrix M)
{
	double ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
	int i,j,k,h,n,s,f;
	EulGetOrd(ea.w,i,j,k,h,n,s,f);
	if (f==EulFrmR) {float t = ea.x; ea.x = ea.z; ea.z = t;}
	if (n==EulParOdd) {ea.x = -ea.x; ea.y = -ea.y; ea.z = -ea.z;}
	ti = ea.x;	  tj = ea.y;	th = ea.z;
	ci = cos(ti); cj = cos(tj); ch = cos(th);
	si = sin(ti); sj = sin(tj); sh = sin(th);
	cc = ci*ch; cs = ci*sh; sc = si*ch; ss = si*sh;
	if (s==EulRepYes) {
		M[i][i] = cj;	  M[i][j] =  sj*si;    M[i][k] =  sj*ci;
		M[j][i] = sj*sh;  M[j][j] = -cj*ss+cc; M[j][k] = -cj*cs-sc;
		M[k][i] = -sj*ch; M[k][j] =  cj*sc+cs; M[k][k] =  cj*cc-ss;
	} else {
		M[i][i] = cj*ch; M[i][j] = sj*sc-cs; M[i][k] = sj*cc+ss;
		M[j][i] = cj*sh; M[j][j] = sj*ss+cc; M[j][k] = sj*cs-sc;
		M[k][i] = -sj;	 M[k][j] = cj*si;    M[k][k] = cj*ci;
	}
	M[W][X]=M[W][Y]=M[W][Z]=M[X][W]=M[Y][W]=M[Z][W]=0.0; M[W][W]=1.0;
}

/* Convert matrix to Euler angles (in radians). */
EulerAngles Eul_FromHMatrix(HMatrix M, int order)
{
	EulerAngles ea;
	int i,j,k,h,n,s,f;
	EulGetOrd(order,i,j,k,h,n,s,f);
	if (s==EulRepYes) {
		double sy = sqrt(M[i][j]*M[i][j] + M[i][k]*M[i][k]);
		if (sy > 16*FLT_EPSILON) {
			ea.x = atan2(M[i][j], M[i][k]);
			ea.y = atan2(sy, (double)M[i][i]);
			ea.z = atan2(M[j][i], -M[k][i]);
		} else {
			ea.x = atan2(-M[j][k], M[j][j]);
			ea.y = atan2(sy, (double)M[i][i]);
			ea.z = 0;
		}
	} else {
		double cy = sqrt(M[i][i]*M[i][i] + M[j][i]*M[j][i]);
		if (cy > 16*FLT_EPSILON) {
			ea.x = atan2(M[k][j], M[k][k]);
			ea.y = atan2((double)-M[k][i], cy);
			ea.z = atan2(M[j][i], M[i][i]);
		} else {
			ea.x = atan2(-M[j][k], M[j][j]);
			ea.y = atan2((double)-M[k][i], cy);
			ea.z = 0;
		}
	}
	if (n==EulParOdd) {ea.x = -ea.x; ea.y = - ea.y; ea.z = -ea.z;}
	if (f==EulFrmR) {float t = ea.x; ea.x = ea.z; ea.z = t;}
	ea.w = order;
	return (ea);
}

/* Convert quaternion to Euler angles (in radians). */
EulerAngles Eul_FromQuat(Quat q, int order)
{
	HMatrix M;
	double Nq = q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w;
	double s = (Nq > 0.0) ? (2.0 / Nq) : 0.0;
	double xs = q.x*s,	  ys = q.y*s,	 zs = q.z*s;
	double wx = q.w*xs,	  wy = q.w*ys,	 wz = q.w*zs;
	double xx = q.x*xs,	  xy = q.x*ys,	 xz = q.x*zs;
	double yy = q.y*ys,	  yz = q.y*zs,	 zz = q.z*zs;
	M[X][X] = 1.0 - (yy + zz); M[X][Y] = xy - wz; M[X][Z] = xz + wy;
	M[Y][X] = xy + wz; M[Y][Y] = 1.0 - (xx + zz); M[Y][Z] = yz - wx;
	M[Z][X] = xz - wy; M[Z][Y] = yz + wx; M[Z][Z] = 1.0 - (xx + yy);
	M[W][X]=M[W][Y]=M[W][Z]=M[X][W]=M[Y][W]=M[Z][W]=0.0; M[W][W]=1.0;
	return (Eul_FromHMatrix(M, order));
}

AxisAngle QuaternionToAxisAngle(Quat quater)
{
	AxisAngle axisangle;
	float normal;
	float scale;
	if (quater.w>1.0)
	{
		normal = sqrt(quater.x*quater.x+quater.y*quater.y+quater.z*quater.z+quater.w*quater.w);
		quater.x = quater.x/normal;
		quater.y = quater.y/normal;
		quater.z = quater.z/normal;
		quater.w = quater.w/normal;

	}
	axisangle.theta  = 2 * acos(quater.w);
	scale = sqrt(1-quater.w*quater.w);
	if (scale > 0.01)
	{
		axisangle.ax = quater.x/scale;
		axisangle.ay = quater.y/scale;
		axisangle.az = quater.z/scale;
		
	}
	else
	{
		axisangle.ax = quater.x;
		axisangle.ay = quater.y;
		axisangle.az = quater.z;
	}

	return axisangle;
}

double Norm(Quat quater)
{
	return(sqrt( SQR(quater.x) + SQR(quater.y) + SQR(quater.z) + SQR(quater.w)));
}	// end float Norm()


Quat Normalize(Quat quater)
{
	double norm = Norm(quater);

	assert(!FLOAT_EQ(0.0f, norm));		// norm should never be close to 0

	quater.x = float(quater.x / norm);
	quater.y = float(quater.y / norm);
	quater.z = float(quater.z / norm);
	quater.w = float(quater.w / norm);

	assert(FLOAT_EQ(1.0f, Norm(quater)));		// must be normalized, safe

	LIMIT_RANGE(-1.0f, quater.w, 1.0f);

	LIMIT_RANGE(-1.0f, quater.x, 1.0f);
	LIMIT_RANGE(-1.0f, quater.y, 1.0f);
	LIMIT_RANGE(-1.0f, quater.z, 1.0f);

	return quater;
}	// end void Normalize()
Quat EulerToQuat( float x,  float y,  float z)
{
	double	ex, ey, ez;		// temp half euler angles
	double	cr, cp, cy, sr, sp, sy, cpcy, spsy;		// temp vars in roll,pitch yaw

	Quat  temp_quater,result_quater;

	ex = DEGTORAD(x) / 2.0;	// convert to rads and half them
	ey = DEGTORAD(y) / 2.0;
	ez = DEGTORAD(z) / 2.0;

	cr = cos(ex);
	cp = cos(ey);
	cy = cos(ez);

	sr = sin(ex);
	sp = sin(ey);
	sy = sin(ez);

	cpcy = cp * cy;
	spsy = sp * sy;

	temp_quater.w = float(cr * cpcy + sr * spsy);

	temp_quater.x = float(sr * cpcy - cr * spsy);
	temp_quater.y = float(cr * sp * cy + sr * cp * sy);
	temp_quater.z = float(cr * cp * sy - sr * sp * cy);

	result_quater = Normalize(temp_quater);

	return result_quater;
}	// end void EulerToQuat(..)

Quat  QuatInverse(Quat quat)
{
	float norm, invNorm;
	Quat tempquat;

	norm = quat.x * quat.x + quat.y * quat.y + quat.z * quat.z+ quat.w * quat.w;

	invNorm = (float) (1.0 / norm);

	tempquat.x = -quat.x * invNorm;
	tempquat.y = -quat.y * invNorm;
	tempquat.z = -quat.z * invNorm;
	tempquat.w =  quat.w * invNorm;
	return tempquat;

}
///////////////////////////////////////////////////////////////////////////////
// Function:	SlerpQuat
// Purpose:		Spherical Linear Interpolation Between two Quaternions
// Arguments:	Two Quaternions, blend factor, result quaternion
// Notes:		The comments explain the handling of the special cases.
//				The comments in the magazine were wrong.  Adjust the
//				DELTA constant to play with the LERP vs. SLERP level
///////////////////////////////////////////////////////////////////////////////
Quat SlerpQuat(Quat quat1,Quat quat2)
{
/// Local Variables ///////////////////////////////////////////////////////////
	double omega,cosom,sinom,scale0,scale1;
    Quat   result;
    float  slerp=0.5;
///////////////////////////////////////////////////////////////////////////////
	// USE THE DOT PRODUCT TO GET THE COSINE OF THE ANGLE BETWEEN THE
	// QUATERNIONS
	cosom = quat1.x * quat2.x + 
			quat1.y * quat2.y + 
			quat1.z * quat2.z + 
			quat1.w * quat2.w; 

	// CHECK A COUPLE OF SPECIAL CASES. 
	// MAKE SURE THE TWO QUATERNIONS ARE NOT EXACTLY OPPOSITE? (WITHIN A LITTLE SLOP)
	if ((1.0 + cosom) > 0.01)
	{
		// ARE THEY MORE THAN A LITTLE BIT DIFFERENT? AVOID A DIVIDED BY ZERO AND LERP IF NOT
		if ((1.0 - cosom) > 0.01) {
			// YES, DO A SLERP
			omega = acos(cosom);
			sinom = sin(omega);
			scale0 = sin((1.0 - slerp) * omega) / sinom;
			scale1 = sin(slerp * omega) / sinom;
		} else {
			// NOT A VERY BIG DIFFERENCE, DO A LERP
			scale0 = 1.0 - slerp;
			scale1 = slerp;
		}
		result.x = scale0 * quat1.x + scale1 * quat2.x;
		result.y = scale0 * quat1.y + scale1 * quat2.y;
		result.z = scale0 * quat1.z + scale1 * quat2.z;
		result.w = scale0 * quat1.w + scale1 * quat2.w;
	} else {
		// THE QUATERNIONS ARE NEARLY OPPOSITE SO TO AVOID A DIVIDED BY ZERO ERROR
		// CALCULATE A PERPENDICULAR QUATERNION AND SLERP THAT DIRECTION
		result.x = -quat2.y;
		result.y = quat2.x;
		result.z = -quat2.w;
		result.w = quat2.z;
		scale0 = sin((1.0 - slerp) * 1.5708);
		scale1 = sin(slerp * 1.5708);
		result.x = scale0 * quat1.x + scale1 * result.x;
		result.y = scale0 * quat1.y + scale1 * result.y;
		result.z = scale0 * quat1.z + scale1 * result.z;
		result.w = scale0 * quat1.w + scale1 * result.w;
	}
return result;
}
// SlerpQuat  /////////////////////////////////////////////////////////////////