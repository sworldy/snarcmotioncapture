// Port number to use for server
const int  PORTNUMBER	= 8889;		// Port number for communication
const int  SIM_FPS		= 60;		// Tested for 30,60,120

const int ChannelCount = 30;

#if defined( WIN32 )
typedef unsigned __int64 nsTime;
#else
typedef unsigned long long nsTime;
#endif

struct sDataBuffer
{
	nsTime 	mTime;				//!< Time in Nanoseconds
	int		mCounter;
	struct  
	{
		double		mT[3];
		double		mR[3];
	} mChannel[ChannelCount];

	float rawdata[18][4];
	float displace[3];

	sDataBuffer()
	{
		mTime		  = 0;
		mCounter	  = 0;
	}

	void Simulate(nsTime pTime)
	{
		mTime = pTime;
	}

	void SetupBuffer()
	{
		//0:Hips
		mChannel[0].mT[0] = 0;
		mChannel[0].mT[1] = 91.1113919067;
		mChannel[0].mT[2] = -4.42;
		//mChannel[0].mT[0] = 5.52507944791e-15;
		//mChannel[0].mT[1] = 90.2313919067;
		//mChannel[0].mT[2] = 0.0;
		mChannel[0].mR[0] = 0.0;
		mChannel[0].mR[1] = 0.0;
		mChannel[0].mR[2] = 0.0;

		//1:LeftUpLeg
		mChannel[1].mT[0] = 9.2919000626;
		mChannel[1].mT[1] = 90.0113919067;
		mChannel[1].mT[2] = -2.28;
		//mChannel[1].mT[0] = 10.8919000626;
		//mChannel[1].mT[1] = 90.2313919067;
		//mChannel[1].mT[2] = 0.0;
		mChannel[1].mR[0] = 0.0;
		mChannel[1].mR[1] = 0.0;
		mChannel[1].mR[2] = 0.0;

		//2:LeftLeg
		mChannel[2].mT[0] = 9.3519000626;
		mChannel[2].mT[1] = 50.4167926025;
		mChannel[2].mT[2] = 0.53;
		//mChannel[2].mT[0] = 10.8919000626;
		//mChannel[2].mT[1] = 46.1967926025;
		//mChannel[2].mT[2] = 0.0;
		mChannel[2].mR[0] = 0.0;
		mChannel[2].mR[1] = 0.0;
		mChannel[2].mR[2] = 0.0;

		//3:LeftFoot
		mChannel[3].mT[0] = 9.4219000626;
		mChannel[3].mT[1] = 9.20909210205;
		mChannel[3].mT[2] = -6.58;
		//mChannel[3].mT[0] = 10.8919000626;
		//mChannel[3].mT[1] = 4.74909210205;
		//mChannel[3].mT[2] = 0.0;
		//mChannel[3].mR[0] = 14.9999777807;
		mChannel[3].mR[0] = 0.0;
		mChannel[3].mR[1] = 0.0;
		mChannel[3].mR[2] = 0.0;

		//4:LeftToeBase
		mChannel[4].mT[0] = 10.8919000626;
		mChannel[4].mT[1] = 1.05947666894;
		mChannel[4].mT[2] = 13.7698536165;
		mChannel[4].mR[0] = 0.0;
		mChannel[4].mR[1] = 0.0;
		mChannel[4].mR[2] = 0.0;

		//5:L_Toe_End
		mChannel[5].mT[0] = 10.8919000626;
		mChannel[5].mT[1] = 1.05947666894;
		mChannel[5].mT[2] = 21.2698536165;
		mChannel[5].mR[0] = 0.0;
		mChannel[5].mR[1] = 0.0;
		mChannel[5].mR[2] = 0.0;

		//6:RightUpLeg
		mChannel[6].mT[0] = -9.3919000626;
		mChannel[6].mT[1] = 90.0113919067;
		mChannel[6].mT[2] = -2.28;
		//mChannel[6].mT[0] = -10.8919000626;
		//mChannel[6].mT[1] = 90.2313919067;
		//mChannel[6].mT[2] = 0.0;
		mChannel[6].mR[0] = 0.0;
		mChannel[6].mR[1] = 0.0;
		mChannel[6].mR[2] = 0.0;

		//7:RightLeg
		mChannel[7].mT[0] = -9.3619000626;
		mChannel[7].mT[1] = 50.4167926025;
		mChannel[7].mT[2] = 0.53;
		//mChannel[7].mT[0] = -10.8919000626;
		//mChannel[7].mT[1] = 46.1967926025;
		//mChannel[7].mT[2] = 0.0;
		mChannel[7].mR[0] = 0.0;
		mChannel[7].mR[1] = 0.0;
		mChannel[7].mR[2] = 0.0;

		//8:RightFoot
		mChannel[8].mT[0] = -9.4219000626;
		mChannel[8].mT[1] = 9.2909210205;
		mChannel[8].mT[2] = -6.58;
		//mChannel[8].mT[0] = -10.8919000626;
		//mChannel[8].mT[1] = 4.74909210205;
		//mChannel[8].mT[2] = 0.0;
		//mChannel[8].mR[0] = 14.9999777807;
		mChannel[8].mR[0] = 0.0;
		mChannel[8].mR[1] = 0.0;
		mChannel[8].mR[2] = 0.0;

		//9:RightToeBase
		mChannel[9].mT[0] = -10.8919000626;
		mChannel[9].mT[1] = 1.05947666894;
		mChannel[9].mT[2] = 13.7698536165;
		mChannel[9].mR[0] = 0.0;
		mChannel[9].mR[1] = 0.0;
		mChannel[9].mR[2] = 0.0;

		//10:R_Toe_End
		mChannel[10].mT[0] = -10.8919000626;
		mChannel[10].mT[1] = 1.05947666894;
		mChannel[10].mT[2] = 21.2698536165;
		mChannel[10].mR[0] = 0.0;
		mChannel[10].mR[1] = 0.0;
		mChannel[10].mR[2] = 0.0;

		//11:Spine
		mChannel[11].mT[0] = 0.19;
		mChannel[11].mT[1] = 106.69;
		mChannel[11].mT[2] = -1.67;
		mChannel[11].mR[0] = 0.0;
		mChannel[11].mR[1] = 0.0;
		mChannel[11].mR[2] = 0.0;

		//12:Spine1
		mChannel[12].mT[0] = 0.19;
		mChannel[12].mT[1] = 118.198892137;
		mChannel[12].mT[2] = -1.55;
		mChannel[12].mR[0] = 0.0;
		mChannel[12].mR[1] = 0.0;
		mChannel[12].mR[2] = 0.0;

		//13:Neck
		mChannel[13].mT[0] = 0.19;
		mChannel[13].mT[1] = 149.915692751;
		mChannel[13].mT[2] = -1.22;
		mChannel[13].mR[0] = 0.0;
		mChannel[13].mR[1] = 0.0;
		mChannel[13].mR[2] = 0.0;

		//14:Head
		mChannel[14].mT[0] = 0.23;
		mChannel[14].mT[1] = 160.927693025;
		mChannel[14].mT[2] = 0.54;
		mChannel[14].mR[0] = 0.0;
		mChannel[14].mR[1] = 0.0;
		mChannel[14].mR[2] = 0.0;

		//15:Head_End
		mChannel[15].mT[0] = 0.29;
		mChannel[15].mT[1] = 175.987693025;
		mChannel[15].mT[2] = 2.95;
		mChannel[15].mR[0] = 0.0;
		mChannel[15].mR[1] = 0.0;
		mChannel[15].mR[2] = 0.0;

		//16:LeftShoulder
		mChannel[16].mT[0] = 4.86;
		mChannel[16].mT[1] = 146.625692751;
		mChannel[16].mT[2] = -1.07;
		mChannel[16].mR[0] = 0.0;
		mChannel[16].mR[1] = 0.0;
		mChannel[16].mR[2] = -90.0002104591;

		//17:LeftArm
		mChannel[17].mT[0] = 17.4577007292;
		mChannel[17].mT[1] = 144.195613014;
		mChannel[17].mT[2] = -2.56;
		mChannel[17].mR[0] = 0.0;
		mChannel[17].mR[1] = 0.0;
		mChannel[17].mR[2] = -90.0002104591;
	//	mChannel[17].mR[2] = 0.0;

		//18:LeftForeArm
		mChannel[18].mT[0] = 44.403015896;
		mChannel[18].mT[1] = 144.195516399;
		mChannel[18].mT[2] = -2.56;
		mChannel[18].mR[0] = 0.0;
		mChannel[18].mR[1] = 0.0;
		mChannel[18].mR[2] = -90.0002104591;
		//mChannel[18].mR[2] = 0.0;


		//19:LeftHand
		mChannel[19].mT[0] = 71.5387020106;
		mChannel[19].mT[1] = 144.195415281;
		mChannel[19].mT[2] = -1.24;
		mChannel[19].mR[0] = 0.0;
		mChannel[19].mR[1] = 0.0;
		mChannel[19].mR[2] = -90.0002104591;
		//mChannel[19].mR[2] = 0.0;


		//20:LeftHandThumb
		mChannel[20].mT[0] = 75.5387020106;
		mChannel[20].mT[1] = 133.325415281;
		mChannel[20].mT[2] = 0.0;
		mChannel[20].mR[0] = 0.0;
		mChannel[20].mR[1] = 0.0;
		mChannel[20].mR[2] = -90.0002104591;

		//21:L_Thumb_End
		mChannel[21].mT[0] = 75.5387020106;
		mChannel[21].mT[1] = 133.325415281;
		mChannel[21].mT[2] = 10.0;
		mChannel[21].mR[0] = 0.0;
		mChannel[21].mR[1] = 0.0;
		mChannel[21].mR[2] = -90.0002104591;

		//22:L_Wrist_End
		mChannel[22].mT[0] = 85.5387020105;
		mChannel[22].mT[1] = 133.325378549;
		mChannel[22].mT[2] = 0.0;
		mChannel[22].mR[0] = 0.0;
		mChannel[22].mR[1] = 0.0;
		mChannel[22].mR[2] = -90.0002104591;

		//23:RightShoulder
		mChannel[23].mT[0] = -4.98;
		mChannel[23].mT[1] = 146.625692751;
		mChannel[23].mT[2] = -1.07;
		mChannel[23].mR[0] = 0.0;
		mChannel[23].mR[1] = 0.0;
		mChannel[23].mR[2] = 90.0002104591;

		//24:RightArm
		mChannel[24].mT[0] = -17.5877007292;
		mChannel[24].mT[1] = 144.195613014;
		mChannel[24].mT[2] = -2.56;
		mChannel[24].mR[0] = 0.0;
		mChannel[24].mR[1] = 0.0;
		mChannel[24].mR[2] = 90.0002104591;

		//25:RightForeArm
		mChannel[25].mT[0] = -44.5203015896;
		mChannel[25].mT[1] = 144.125516399;
		mChannel[25].mT[2] = -2.54;
		mChannel[25].mR[0] = 0.0;
		mChannel[25].mR[1] = 0.0;
		mChannel[25].mR[2] = 90.0002104591;

		//26:RightHand
		mChannel[26].mT[0] = -71.6487020106;
		mChannel[26].mT[1] = 143.765415281;
		mChannel[26].mT[2] = -1.23;
		mChannel[26].mR[0] = 0.0;
		mChannel[26].mR[1] = 0.0;
		mChannel[26].mR[2] = 90.0002104591;

		//27:RightHandThumb
		mChannel[27].mT[0] = -75.5387020106;
		mChannel[27].mT[1] = 133.325415281;
		mChannel[27].mT[2] = 0.0;
		mChannel[27].mR[0] = 0.0;
		mChannel[27].mR[1] = 0.0;
		mChannel[27].mR[2] = 90.0002104591;

		//28:R_Thumb_End
		mChannel[28].mT[0] = -75.5387020106;
		mChannel[28].mT[1] = 133.325415281;
		mChannel[28].mT[2] = 10.0;
		mChannel[28].mR[0] = 0.0;
		mChannel[28].mR[1] = 0.0;
		mChannel[28].mR[2] = 90.0002104591;

		//29:R_Wrist_End
		mChannel[29].mT[0] = -89.2387018198;
		mChannel[29].mT[1] = 133.325364958;
		mChannel[29].mT[2] = 0.0;
		mChannel[29].mR[0] = 0.0;
		mChannel[29].mR[1] = 0.0;
		mChannel[29].mR[2] = 90.0002104591;
	}


};
