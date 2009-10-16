#include "stdafx.h"
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <strstream>  
#include <string> 
#include "Displacement.h"

using namespace std;


#define MAX_STRINGLENGTH	255


void ParseString(char *buffer,CStringArray *words,int *cnt);

int main(int argc, char* argv[])
{    
	FILE *fp_wist, *fp_Rthigh, *fp_Lthigh, *fp_Rshank, *fp_Lshank, *fp_Rfeet, *fp_Lfeet;
	char buffer[MAX_STRINGLENGTH];
	CStringArray words;
	CString name_wist, name_Rthigh, name_Lthigh, name_Rshank, name_Lshank, name_Rfeet, name_Lfeet;

	int cnt;

	int datanum = 2066; //读入文件数据行数
	const int input_datadimention = 13; //输入文件维数：加速度3 角速度3，磁3，四元数4
	const int output_datadimention = 7; //输出文件维数：位移3， 四元数4
	const int sensor_num = 7; //用到7个sensor的数据
	float *wist, *Rthigh, *Lthigh, *Rshank, *Lshank, *Rfeet, *Lfeet, *fptr_wist, *fptr_Rthigh, *fptr_Lthigh, *fptr_Rshank, *fptr_Lshank, *fptr_Rfeet, *fptr_Lfeet;
	float *fptr_displace, *displace;
	float QuartandDisplace[sensor_num][output_datadimention];
	for (int i =0;i <sensor_num; i++ )
	{
		for (int j  =0; j<output_datadimention; j++)
		{
			QuartandDisplace[i][j]=0.0;
		}
		QuartandDisplace[i][3]=1.0;
	}

	Displacement* cur_displacemet;
	cur_displacemet = new Displacement();


	wist = (float *)malloc(sizeof(float) * datanum * input_datadimention);
	Rthigh = (float *)malloc(sizeof(float) * datanum * input_datadimention);
	Lthigh = (float *)malloc(sizeof(float) * datanum * input_datadimention);
	Rshank = (float *)malloc(sizeof(float) * datanum * input_datadimention);
	Lshank = (float *)malloc(sizeof(float) * datanum * input_datadimention);
	Rfeet = (float *)malloc(sizeof(float) * datanum * input_datadimention);
	Lfeet = (float *)malloc(sizeof(float) * datanum * input_datadimention);

	displace = (float *)malloc(sizeof(float) * datanum * 3);

	fptr_wist = wist;
	fptr_Rthigh = Rthigh;
	fptr_Lthigh = Lthigh;
	fptr_Rshank = Rshank;
	fptr_Lshank = Lshank;
	fptr_Rfeet = Rfeet;
	fptr_Lfeet = Lfeet;
	fptr_displace = displace;

	name_Lthigh = "左大腿.txt";
	name_Lshank = "左小腿.txt";
	name_Lfeet = "左脚.txt";
	name_wist = "下腰.txt";
	name_Rthigh = "右大腿.txt";	
	name_Rshank = "右小腿.txt";	
	name_Rfeet = "右脚.txt";



	fp_wist = fopen((LPCTSTR)name_wist,"r");
	fp_Rthigh = fopen((LPCTSTR)name_Rthigh,"r");
	fp_Lthigh = fopen((LPCTSTR)name_Lthigh,"r");
	fp_Rshank = fopen((LPCTSTR)name_Rshank,"r");
	fp_Lshank = fopen((LPCTSTR)name_Lshank,"r");
	fp_Rfeet = fopen((LPCTSTR)name_Rfeet,"r");
	fp_Lfeet = fopen((LPCTSTR)name_Lfeet,"r");

	while (!feof(fp_wist))
	{

		fgets(buffer,MAX_STRINGLENGTH,fp_wist);
		ParseString(buffer,&words,&cnt);

		*fptr_wist = (float)atof(words.GetAt(9));
		fptr_wist++;
		*fptr_wist = (float)atof(words.GetAt(10));
		fptr_wist++;
		*fptr_wist = (float)atof(words.GetAt(11));
		fptr_wist++;
		*fptr_wist = (float)atof(words.GetAt(12));
		fptr_wist++;
		*fptr_wist = (float)atof(words.GetAt(0));
		fptr_wist++;
		*fptr_wist = (float)atof(words.GetAt(1));
		fptr_wist++;
		*fptr_wist = (float)atof(words.GetAt(2));
		fptr_wist++;
		*fptr_wist = (float)atof(words.GetAt(3));
		fptr_wist++;
		*fptr_wist = (float)atof(words.GetAt(4));
		fptr_wist++;
		*fptr_wist = (float)atof(words.GetAt(5));
		fptr_wist++;

		words.RemoveAll();

	}

	while (!feof(fp_Rthigh))
	{
		fgets(buffer,MAX_STRINGLENGTH,fp_Rthigh);
		ParseString(buffer,&words,&cnt);

		*fptr_Rthigh = (float)atof(words.GetAt(9));
		fptr_Rthigh++;
		*fptr_Rthigh = (float)atof(words.GetAt(10));
		fptr_Rthigh++;
		*fptr_Rthigh = (float)atof(words.GetAt(11));
		fptr_Rthigh++;
		*fptr_Rthigh = (float)atof(words.GetAt(12));
		fptr_Rthigh++;
		*fptr_Rthigh = (float)atof(words.GetAt(0));
		fptr_Rthigh++;
		*fptr_Rthigh = (float)atof(words.GetAt(1));
		fptr_Rthigh++;
		*fptr_Rthigh = (float)atof(words.GetAt(2));
		fptr_Rthigh++;
		*fptr_Rthigh = (float)atof(words.GetAt(3));
		fptr_Rthigh++;
		*fptr_Rthigh = (float)atof(words.GetAt(4));
		fptr_Rthigh++;
		*fptr_Rthigh = (float)atof(words.GetAt(5));
		fptr_Rthigh++;

		words.RemoveAll();
	}

	while (!feof(fp_Lthigh))
	{
		fgets(buffer,MAX_STRINGLENGTH,fp_Lthigh);
		ParseString(buffer,&words,&cnt);

		*fptr_Lthigh = (float)atof(words.GetAt(9));
		fptr_Lthigh++;
		*fptr_Lthigh = (float)atof(words.GetAt(10));
		fptr_Lthigh++;
		*fptr_Lthigh = (float)atof(words.GetAt(11));
		fptr_Lthigh++;
		*fptr_Lthigh = (float)atof(words.GetAt(12));
		fptr_Lthigh++;
		*fptr_Lthigh = (float)atof(words.GetAt(0));
		fptr_Lthigh++;
		*fptr_Lthigh = (float)atof(words.GetAt(1));
		fptr_Lthigh++;
		*fptr_Lthigh = (float)atof(words.GetAt(2));
		fptr_Lthigh++;
		*fptr_Lthigh = (float)atof(words.GetAt(3));
		fptr_Lthigh++;
		*fptr_Lthigh = (float)atof(words.GetAt(4));
		fptr_Lthigh++;
		*fptr_Lthigh = (float)atof(words.GetAt(5));
		fptr_Lthigh++;

		words.RemoveAll();
	}	


	while (!feof(fp_Rshank))
	{
		fgets(buffer,MAX_STRINGLENGTH,fp_Rshank);
		ParseString(buffer,&words,&cnt);

		*fptr_Rshank = (float)atof(words.GetAt(9));
		fptr_Rshank++;
		*fptr_Rshank = (float)atof(words.GetAt(10));
		fptr_Rshank++;
		*fptr_Rshank = (float)atof(words.GetAt(11));
		fptr_Rshank++;
		*fptr_Rshank = (float)atof(words.GetAt(12));
		fptr_Rshank++;
		*fptr_Rshank = (float)atof(words.GetAt(0));
		fptr_Rshank++;
		*fptr_Rshank = (float)atof(words.GetAt(1));
		fptr_Rshank++;
		*fptr_Rshank = (float)atof(words.GetAt(2));
		fptr_Rshank++;
		*fptr_Rshank = (float)atof(words.GetAt(3));
		fptr_Rshank++;
		*fptr_Rshank = (float)atof(words.GetAt(4));
		fptr_Rshank++;
		*fptr_Rshank = (float)atof(words.GetAt(5));
		fptr_Rshank++;

		words.RemoveAll();
	}

	while (!feof(fp_Lshank))
	{
		fgets(buffer,MAX_STRINGLENGTH,fp_Lshank);
		ParseString(buffer,&words,&cnt);

		*fptr_Lshank = (float)atof(words.GetAt(9));
		fptr_Lshank++;
		*fptr_Lshank = (float)atof(words.GetAt(10));
		fptr_Lshank++;
		*fptr_Lshank = (float)atof(words.GetAt(11));
		fptr_Lshank++;
		*fptr_Lshank = (float)atof(words.GetAt(12));
		fptr_Lshank++;
		*fptr_Lshank = (float)atof(words.GetAt(0));
		fptr_Lshank++;
		*fptr_Lshank = (float)atof(words.GetAt(1));
		fptr_Lshank++;
		*fptr_Lshank = (float)atof(words.GetAt(2));
		fptr_Lshank++;
		*fptr_Lshank = (float)atof(words.GetAt(3));
		fptr_Lshank++;
		*fptr_Lshank = (float)atof(words.GetAt(4));
		fptr_Lshank++;
		*fptr_Lshank = (float)atof(words.GetAt(5));
		fptr_Lshank++;

		words.RemoveAll();
	}	


	while (!feof(fp_Rfeet))
	{
		fgets(buffer,MAX_STRINGLENGTH,fp_Rfeet);
		ParseString(buffer,&words,&cnt);

		*fptr_Rfeet = (float)atof(words.GetAt(9));
		fptr_Rfeet++;
		*fptr_Rfeet = (float)atof(words.GetAt(10));
		fptr_Rfeet++;
		*fptr_Rfeet = (float)atof(words.GetAt(11));
		fptr_Rfeet++;
		*fptr_Rfeet = (float)atof(words.GetAt(12));
		fptr_Rfeet++;
		*fptr_Rfeet = (float)atof(words.GetAt(0));
		fptr_Rfeet++;
		*fptr_Rfeet = (float)atof(words.GetAt(1));
		fptr_Rfeet++;
		*fptr_Rfeet = (float)atof(words.GetAt(2));
		fptr_Rfeet++;
		*fptr_Rfeet = (float)atof(words.GetAt(3));
		fptr_Rfeet++;
		*fptr_Rfeet = (float)atof(words.GetAt(4));
		fptr_Rfeet++;
		*fptr_Rfeet = (float)atof(words.GetAt(5));
		fptr_Rfeet++;

		words.RemoveAll();
	}

	while (!feof(fp_Lfeet))
	{
		fgets(buffer,MAX_STRINGLENGTH,fp_Lfeet);
		ParseString(buffer,&words,&cnt);

		*fptr_Lfeet = (float)atof(words.GetAt(9));
		fptr_Lfeet++;
		*fptr_Lfeet = (float)atof(words.GetAt(10));
		fptr_Lfeet++;
		*fptr_Lfeet = (float)atof(words.GetAt(11));
		fptr_Lfeet++;
		*fptr_Lfeet = (float)atof(words.GetAt(12));
		fptr_Lfeet++;
		*fptr_Lfeet = (float)atof(words.GetAt(0));
		fptr_Lfeet++;
		*fptr_Lfeet = (float)atof(words.GetAt(1));
		fptr_Lfeet++;
		*fptr_Lfeet = (float)atof(words.GetAt(2));
		fptr_Lfeet++;
		*fptr_Lfeet = (float)atof(words.GetAt(3));
		fptr_Lfeet++;
		*fptr_Lfeet = (float)atof(words.GetAt(4));
		fptr_Lfeet++;
		*fptr_Lfeet = (float)atof(words.GetAt(5));
		fptr_Lfeet++;

		words.RemoveAll();
	}
	fptr_wist = wist;
	fptr_Rthigh = Rthigh;
	fptr_Lthigh = Lthigh;
	fptr_Rshank = Rshank;
	fptr_Lshank = Lshank;
	fptr_Rfeet = Rfeet;
	fptr_Lfeet = Lfeet;

	//////////////////////////////////////////////////////////	
	for(int i = 0; i< datanum; i++ )
	{
		cur_displacemet->cur_quater_wist[0] = *fptr_wist;
		fptr_wist++;
		cur_displacemet->cur_quater_wist[1] = *fptr_wist;
		fptr_wist++;
		cur_displacemet->cur_quater_wist[2] = *fptr_wist;
		fptr_wist++;
		cur_displacemet->cur_quater_wist[3] = *fptr_wist;
		fptr_wist++;
		cur_displacemet->acc_wist[0] = *fptr_wist;
		fptr_wist++;
		cur_displacemet->acc_wist[1] = *fptr_wist;
		fptr_wist++;
		cur_displacemet->acc_wist[2] = *fptr_wist;
		fptr_wist++;
		cur_displacemet->angular_wist[0] = *fptr_wist;
		fptr_wist++;
		cur_displacemet->angular_wist[1] = *fptr_wist;
		fptr_wist++;
		cur_displacemet->angular_wist[2] = *fptr_wist;
		fptr_wist++;


		cur_displacemet->cur_quater_Rthigh[0] = *fptr_Rthigh;
		fptr_Rthigh++;
		cur_displacemet->cur_quater_Rthigh[1] = *fptr_Rthigh;
		fptr_Rthigh++;
		cur_displacemet->cur_quater_Rthigh[2] = *fptr_Rthigh;
		fptr_Rthigh++;
		cur_displacemet->cur_quater_Rthigh[3] = *fptr_Rthigh;
		fptr_Rthigh++;
		cur_displacemet->acc_Rthigh[0] = *fptr_Rthigh;
		fptr_Rthigh++;
		cur_displacemet->acc_Rthigh[1] = *fptr_Rthigh;
		fptr_Rthigh++;
		cur_displacemet->acc_Rthigh[2] = *fptr_Rthigh;
		fptr_Rthigh++;
		cur_displacemet->angular_Rthigh[0] = *fptr_Rthigh;
		fptr_Rthigh++;
		cur_displacemet->angular_Rthigh[1] = *fptr_Rthigh;
		fptr_Rthigh++;
		cur_displacemet->angular_Rthigh[2] = *fptr_Rthigh;
		fptr_Rthigh++;


		cur_displacemet->cur_quater_Lthigh[0] = *fptr_Lthigh;
		fptr_Lthigh++;
		cur_displacemet->cur_quater_Lthigh[1] = *fptr_Lthigh;
		fptr_Lthigh++;
		cur_displacemet->cur_quater_Lthigh[2] = *fptr_Lthigh;
		fptr_Lthigh++;
		cur_displacemet->cur_quater_Lthigh[3] = *fptr_Lthigh;
		fptr_Lthigh++;
		cur_displacemet->acc_Lthigh[0] = *fptr_Lthigh;
		fptr_Lthigh++;
		cur_displacemet->acc_Lthigh[1] = *fptr_Lthigh;
		fptr_Lthigh++;
		cur_displacemet->acc_Lthigh[2] = *fptr_Lthigh;
		fptr_Lthigh++;
		cur_displacemet->angular_Lthigh[0] = *fptr_Lthigh;
		fptr_Lthigh++;
		cur_displacemet->angular_Lthigh[1] = *fptr_Lthigh;
		fptr_Lthigh++;
		cur_displacemet->angular_Lthigh[2] = *fptr_Lthigh;
		fptr_Lthigh++;

		cur_displacemet->cur_quater_Rshank[0] = *fptr_Rshank;
		fptr_Rshank++;
		cur_displacemet->cur_quater_Rshank[1] = *fptr_Rshank;
		fptr_Rshank++;
		cur_displacemet->cur_quater_Rshank[2] = *fptr_Rshank;
		fptr_Rshank++;
		cur_displacemet->cur_quater_Rshank[3] = *fptr_Rshank;
		fptr_Rshank++;
		cur_displacemet->acc_Rshank[0] = *fptr_Rshank;
		fptr_Rshank++;
		cur_displacemet->acc_Rshank[1] = *fptr_Rshank;
		fptr_Rshank++;
		cur_displacemet->acc_Rshank[2] = *fptr_Rshank;
		fptr_Rshank++;
		cur_displacemet->angular_Rshank[0] = *fptr_Rshank;
		fptr_Rshank++;
		cur_displacemet->angular_Rshank[1] = *fptr_Rshank;
		fptr_Rshank++;
		cur_displacemet->angular_Rshank[2] = *fptr_Rshank;
		fptr_Rshank++;


		cur_displacemet->cur_quater_Lshank[0] = *fptr_Lshank;
		fptr_Lshank++;
		cur_displacemet->cur_quater_Lshank[1] = *fptr_Lshank;
		fptr_Lshank++;
		cur_displacemet->cur_quater_Lshank[2] = *fptr_Lshank;
		fptr_Lshank++;
		cur_displacemet->cur_quater_Lshank[3] = *fptr_Lshank;
		fptr_Lshank++;
		cur_displacemet->acc_Lshank[0] = *fptr_Lshank;
		fptr_Lshank++;
		cur_displacemet->acc_Lshank[1] = *fptr_Lshank;
		fptr_Lshank++;
		cur_displacemet->acc_Lshank[2] = *fptr_Lshank;
		fptr_Lshank++;
		cur_displacemet->angular_Lshank[0] = *fptr_Lshank;
		fptr_Lshank++;
		cur_displacemet->angular_Lshank[1] = *fptr_Lshank;
		fptr_Lshank++;
		cur_displacemet->angular_Lshank[2] = *fptr_Lshank;
		fptr_Lshank++;

		cur_displacemet->cur_quater_Rfeet[0] = *fptr_Rfeet;
		fptr_Rfeet++;
		cur_displacemet->cur_quater_Rfeet[1] = *fptr_Rfeet;
		fptr_Rfeet++;
		cur_displacemet->cur_quater_Rfeet[2] = *fptr_Rfeet;
		fptr_Rfeet++;
		cur_displacemet->cur_quater_Rfeet[3] = *fptr_Rfeet;
		fptr_Rfeet++;
		cur_displacemet->acc_Rfeet[0] = *fptr_Rfeet;
		fptr_Rfeet++;
		cur_displacemet->acc_Rfeet[1] = *fptr_Rfeet;
		fptr_Rfeet++;
		cur_displacemet->acc_Rfeet[2] = *fptr_Rfeet;
		fptr_Rfeet++;
		cur_displacemet->angular_Rfeet[0] = *fptr_Rfeet;
		fptr_Rfeet++;
		cur_displacemet->angular_Rfeet[1] = *fptr_Rfeet;
		fptr_Rfeet++;
		cur_displacemet->angular_Rfeet[2] = *fptr_Rfeet;
		fptr_Rfeet++;

		cur_displacemet->cur_quater_Lfeet[0] = *fptr_Lfeet;
		fptr_Lfeet++;
		cur_displacemet->cur_quater_Lfeet[1] = *fptr_Lfeet;
		fptr_Lfeet++;
		cur_displacemet->cur_quater_Lfeet[2] = *fptr_Lfeet;
		fptr_Lfeet++;
		cur_displacemet->cur_quater_Lfeet[3] = *fptr_Lfeet;
		fptr_Lfeet++;
		cur_displacemet->acc_Lfeet[0] = *fptr_Lfeet;
		fptr_Lfeet++;
		cur_displacemet->acc_Lfeet[1] = *fptr_Lfeet;
		fptr_Lfeet++;
		cur_displacemet->acc_Lfeet[2] = *fptr_Lfeet;
		fptr_Lfeet++;
		cur_displacemet->angular_Lfeet[0] = *fptr_Lfeet;
		fptr_Lfeet++;
		cur_displacemet->angular_Lfeet[1] = *fptr_Lfeet;
		fptr_Lfeet++;
		cur_displacemet->angular_Lfeet[2] = *fptr_Lfeet;
		fptr_Lfeet++;

		//估计位移开始
		if(cur_displacemet->counter<BUFFER_SIZE)//计数器小于buffer_size，直接存入buffer
		{
			cur_displacemet->Rfeet_buffer[cur_displacemet->counter] = cur_displacemet->angular_Rfeet[1];//右脚y轴角速度
			cur_displacemet->Lfeet_buffer[cur_displacemet->counter] = cur_displacemet->angular_Lfeet[1];//左脚y轴角速度

		}
		else//大于时，依次往前移，只存最新80个数
		{
			for(int i=0;i<BUFFER_SIZE-1;i++)
			{
				cur_displacemet->Rfeet_buffer[i]=cur_displacemet->Rfeet_buffer[i+1];
				cur_displacemet->Lfeet_buffer[i]=cur_displacemet->Lfeet_buffer[i+1];
			}
			cur_displacemet->Rfeet_buffer[BUFFER_SIZE-1] = cur_displacemet->angular_Rfeet[1];//最后一个存最新获得的值
			cur_displacemet->Lfeet_buffer[BUFFER_SIZE-1] = cur_displacemet->angular_Lfeet[1];//同样
		}

		cur_displacemet->counter = cur_displacemet->counter+1;//改，计数器递增
		cur_displacemet->ComputeDisplacementbyQuaternion();//计算位移
		cur_displacemet->UpdateQuaternion();//保存本次四元数

		*fptr_displace = cur_displacemet->displacement[0];
		fptr_displace++;
		*fptr_displace = cur_displacemet->displacement[1];
		fptr_displace++;
		*fptr_displace = cur_displacemet->displacement[2];
		fptr_displace++;

	}


	FILE *data;
	data = fopen("data.txt","wb");
	////////////////reset the pointer to the original position//////////////////////////////
	fptr_displace = displace;
	///////////////////////////////////////////////////////////////////////////////////////////////
	//腰部的位移

	for(int i = 0; i < datanum; i++)
	{
		QuartandDisplace[0][0] = *fptr_displace;
		fptr_displace++;
		QuartandDisplace[0][1] = *fptr_displace;
		fptr_displace++;
		QuartandDisplace[0][2] = *fptr_displace;
		fptr_displace++;
		fprintf(data,"%f %f %f \r\n",QuartandDisplace[0][0], QuartandDisplace[0][1],QuartandDisplace[0][2]);   
	}
	fclose(data);


	fclose(fp_wist); 
	fclose(fp_Rthigh);
	fclose(fp_Lthigh);
	fclose(fp_Rshank);
	fclose(fp_Lshank);
	fclose(fp_Rfeet);
	fclose(fp_Lfeet);

	delete cur_displacemet;
	if (wist == NULL) {
	} 
	else 
	{
		free(wist); 
		wist = NULL; 
	}

	if (Rthigh == NULL) {
	} else {

		free(Rthigh); 
		Rthigh = NULL; 
	}
	if (Lthigh == NULL) {
	} else {

		free(Lthigh); 
		Lthigh = NULL; 
	}
	if (Rshank == NULL) {

	} else {

		free(Rshank); 
		Rshank = NULL; 
	}
	if (Lshank == NULL) {

	} else {

		free(Lshank); 
		Lshank = NULL; 
	}
	if (Rfeet == NULL) {

	} else {

		free(Rfeet); 
		Rfeet = NULL; 
	}
	if (Lfeet == NULL) {

	} else {

		free(Lfeet); 
		Lfeet = NULL; 
	}
	if (displace == NULL) {

	} else {

		free(displace); 
		displace = NULL; 
	}


	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	ParseString
// Purpose:		Actually breaks a string of words into individual pieces
// Arguments:	Source string in, array to put the words and the count
///////////////////////////////////////////////////////////////////////////////
void ParseString(char *buffer,CStringArray *words,int *cnt)
{
	/// Local Variables ///////////////////////////////////////////////////////////
	CString in = buffer, temp;
	///////////////////////////////////////////////////////////////////////////////

	in.TrimLeft();
	in.TrimRight();
	*cnt = 0;
	do 
	{
		temp = in.SpanExcluding(" \t");		// GET UP TO THE NEXT SPACE OR TAB
		words->Add(temp);
		if (temp == in) break;
		in = in.Right(in.GetLength() - temp.GetLength());
		in.TrimLeft();
		*cnt = *cnt + 1;			
	} while (1);
	*cnt = *cnt + 1;
}