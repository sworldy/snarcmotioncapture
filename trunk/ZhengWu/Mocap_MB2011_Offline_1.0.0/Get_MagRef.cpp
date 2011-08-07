//void FQKF::Get_MagRef(double *norm_measure, double *mag_ref)
//// The classname is the own class name of fusion program.
//// Get the earth magnetic reference.
//// Get = Get.
//// Mag = Magnetic field. 
//// Ref = Reference.
//{
//	int i;
//	double quat_pitch[4] ={0,1,0,1}; // quat_pitch -> y axis;
//	double quat_roll[4] ={1,0,0,1}; // quat_roll -> x axis;
//	double sin_p,sin_r,sin_z,cos_p,cos_r,cos_z;
//	double sin_ph,sin_rh,sin_zh,cos_ph,cos_rh,cos_zh;
//	double Acc[3],Mag[3];
//	
//	double quat_temp1[4]={0,0,0,1}, quat_temp2[4]={0,0,0,1};
//	double quat_GS[4];
//	
//	//
//	for( i=0;i<3;i++)
//	{
//		Acc[i] = norm_measure[i];
//		Mag[i] = norm_measure[i+6];
//	}
//
//	// -------------------------
//	// Frame Transformation
//    // qx = cos(pi/2)*[0;0;0;1;]+sin(pi/2)*[1;0;0;0;];
//	quat_temp1[0] = sin(PI/2);
//	quat_temp1[1] = 0;
//	quat_temp1[2] = 0;
//	quat_temp1[3] = cos(PI/2);
//	
//	// qy = cos(-pi/4)*[0;0;0;1;]+sin(-pi/4)*[0;1;0;0];
//	quat_temp2[0] = 0;
//	quat_temp2[1] = sin(-PI/4);
//	quat_temp2[2] = 0;
//	quat_temp2[3] = cos(-PI/4);
//	
//	// qGS = qmult(qx,qy);
//	Quaternion_Multiple(quat_temp1,quat_temp2);
//	for(i=0; i<4; i++)
//	{
//		quat_GS[i] = quat_temp1[i];
//	} 
//	Quaternion_VecRot(quat_GS,Acc);
//	Quaternion_VecRot(quat_GS,Mag);
//	// -------------------------
//
//	// -------------------------
//	// FQA to determine the pitch and roll angle
//	// Pitch/elevation quaternion calculation
//	sin_p = -Acc[0];
//	cos_p = sqrt(1-sin_p*sin_p);
//	sin_ph = Get_Sign(sin_p)*(sqrt(0.5*(1-cos_p)));
//	cos_ph = sqrt((0.5*(1+cos_p)));
//	quat_pitch[1] = sin_ph;
//	quat_pitch[3] = cos_ph;
//
//    //--- Roll/bank quaternion calculation
//	 sin_r = Acc[1]/cos_p;
//     cos_r = Acc[2]/cos_p;
//
//	if(Get_Sign(sin_r) ~= 0)
//		sin_rh = Get_Sign(sin_r)*(sqrt(0.5*(1-cos_r)));
//    else
//        sin_rh = sqrt((0.5*(1-cos_r)));
//    cos_rh = sqrt(0.5*(1+cos_r));
//	quat_roll[0] = sin_rh;
//	quat_roll[3] = cos_rh;
//	// -------------------------
//
//	// -------------------------
//	// Magnetism direction unit vector in global frame.
//	Quaternion_Multiple(quat_pitch,quat_roll);
//	Quaternion_VecRot(quat_pitch,mag_ref);
//	// -------------------------
//}
//
//int FQKF::Get_Sign(double real_num)
//// The classname is the own class name of fusion program.
//// Get = Get.
//// Sign = The positive or negtive sign of a real number. 
//{
//	if(real_num == 0)
//		return 0;
//	else if (real_num < 0)
//		return -1;
//	else if (real_num > 0)
//		return 1;
//}