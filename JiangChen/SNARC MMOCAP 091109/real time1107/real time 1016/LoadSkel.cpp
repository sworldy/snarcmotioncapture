///////////////////////////////////////////////////////////////////////////////
//
// LoadSkel.cpp
//
// Purpose: implementation of the Custom Skeleton file Loader
//
// Created:
//		JL 9/12/99		
//
///////////////////////////////////////////////////////////////////////////////
//
//	Copyright 1999 Jeff Lander, All Rights Reserved.
//  For educational purposes only.
//  Please do not republish in electronic or print form without permission
//  Thanks - jeffl@darwin3d.com
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LoadSkel.h"
#include "EulerAngles.h"
#include "QuatTypes.h"


#define MAX_CHILDREN 8
typedef struct 
{
	void	*parent;
	void	*child[MAX_CHILDREN];
	int		childCnt;
	float	tx,ty,tz;
	float	rx,ry,rz;
	char	name[80];
	float   posi_x,posi_y,posi_z;
	int    identify;
} tSkeletonNode;


///////////////////////////////////////////////////////////////////////////////
// Fixes up the skeletal structure once it is loaded
///////////////////////////////////////////////////////////////////////////////
void AddDarwinBranch(t_Bone *root, tSkeletonNode *curNode,tSkeletonNode *baseNode)
{
/// Local Variables ///////////////////////////////////////////////////////////
	int i;
	t_Bone *child;
	Quat quater;
///////////////////////////////////////////////////////////////////////////////
	root->children = (t_Bone *)malloc((root->childCnt) * sizeof(t_Bone));
	for (i = 0; i < root->childCnt; i++)
	{
		child = &root->children[i];
		ResetBone(child,root);
		strcpy(child->name,baseNode[(int)curNode->child[i]].name);
		child->trans.x = baseNode[(int)curNode->child[i]].tx;
		child->trans.y = baseNode[(int)curNode->child[i]].ty;
		child->trans.z = baseNode[(int)curNode->child[i]].tz;
		child->rot.x = baseNode[(int)curNode->child[i]].rx;
		child->rot.y = baseNode[(int)curNode->child[i]].ry;
		child->rot.z = baseNode[(int)curNode->child[i]].rz;
		child->b_trans.x = baseNode[(int)curNode->child[i]].tx;
		child->b_trans.y = baseNode[(int)curNode->child[i]].ty;
		child->b_trans.z = baseNode[(int)curNode->child[i]].tz;
		child->b_rot.x = baseNode[(int)curNode->child[i]].rx;
		child->b_rot.y = baseNode[(int)curNode->child[i]].ry;
		child->b_rot.z = baseNode[(int)curNode->child[i]].rz;
		child->curMatrix = (tMatrix *)malloc(sizeof(tMatrix));
		child->childCnt = baseNode[(int)curNode->child[i]].childCnt;

		child->globalposi = (tVector *)malloc(sizeof(tVector));
		child->globalposi->x = baseNode[(int)curNode->child[i]].posi_x;
		child->globalposi->y = baseNode[(int)curNode->child[i]].posi_y;
		child->globalposi->z = baseNode[(int)curNode->child[i]].posi_z;

		child->localposi = (tVector *)malloc(sizeof(tVector));
		child->localposi->x = baseNode[(int)curNode->child[i]].tx;
		child->localposi->y = baseNode[(int)curNode->child[i]].ty;
		child->localposi->z = baseNode[(int)curNode->child[i]].tz;

		child->anim_matrix = (tMatrix *)malloc(sizeof(tMatrix));

		child->id = baseNode[(int)curNode->child[i]].identify;


		if (child->childCnt > 0)
		{
			AddDarwinBranch(child,&baseNode[(int)curNode->child[i]],baseNode);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// Function:	LoadSkeleton
// Purpose:		Load a Darwin Format Skeleton
// Arguments:	Name of the file to open and root skeleton to put it in
///////////////////////////////////////////////////////////////////////////////
BOOL LoadSkeleton(CString name,t_Bone *root)
{
/// Local Variables ///////////////////////////////////////////////////////////
	int i,j,boneCnt,parent;
	FILE *fp;		// I PREFER THIS STYLE OF FILE ACCESS
	tSkeletonNode *tempBone,*bonelist;
	t_Bone *child;
	char	temp[5];
  
	/////add by wz/////
	FILE *freadout;
	freadout = fopen("readout01","wb");
	char tempstr[80];
	///////////////////


///////////////////////////////////////////////////////////////////////////////
	if (fp = fopen((LPCTSTR)name,"rb")) {
		fread(temp,sizeof(char),4,fp);

        fwrite(temp,sizeof(char),4,freadout);//add by wz


		fread(&boneCnt,sizeof(int),1,fp);

	    sprintf(tempstr, "%d\r\n", boneCnt); //add by wz
		fwrite(tempstr,sizeof(char),4,freadout);//add by wz


		bonelist = (tSkeletonNode *)malloc((boneCnt) * sizeof(tSkeletonNode));
		for (i = 0; i < boneCnt; i++)
		{
			tempBone = &bonelist[i];
			fread(tempBone->name,sizeof(char),80,fp);
            
			fwrite(tempBone->name,sizeof(char),12,freadout);//add by wz

			fread(&tempBone->tx,sizeof(float),1,fp);

			sprintf(tempstr, "%f\r\n", tempBone->tx); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz

			fread(&tempBone->ty,sizeof(float),1,fp);

			sprintf(tempstr, "%f\r\n", tempBone->ty); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz

			fread(&tempBone->tz,sizeof(float),1,fp);

			sprintf(tempstr, "%f\r\n", tempBone->tz); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz


			fread(&tempBone->rx,sizeof(float),1,fp);

			sprintf(tempstr, "%f\r\n", tempBone->rx); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz


			fread(&tempBone->ry,sizeof(float),1,fp);

			sprintf(tempstr, "%f\r\n", tempBone->ry); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz


			fread(&tempBone->rz,sizeof(float),1,fp);

			sprintf(tempstr, "%f\r\n", tempBone->rz); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz
			
			// FIND THE PARENT
			fread(&parent,sizeof(int),1,fp);

			sprintf(tempstr, "%d\r\n", parent); //add by wz
			fwrite(tempstr,sizeof(char),6,freadout);//add by wz


			if (parent == -1)
				tempBone->parent = NULL;
			else
				tempBone->parent = (t_Bone *)parent;

			fread(&tempBone->childCnt,sizeof(int),1,fp);

			sprintf(tempstr, "%d\r\n", tempBone->childCnt); //add by wz
			fwrite(tempstr,sizeof(char),6,freadout);//add by wz


			for (j = 0; j < tempBone->childCnt; j++)
			{
				fread(&tempBone->child[j],sizeof(int),1,fp);

				sprintf(tempstr, "%d\r\n", tempBone->child[j]); //add by wz
				fwrite(tempstr,sizeof(char),6,freadout);//add by wz

			}
		}
		fclose(fp);

		fclose(freadout);//add by wz

		// SET UP ROOT
//		strcpy(root->name,bonelist[0].name);
		root->b_trans.x = bonelist[0].tx;
		root->b_trans.y = bonelist[0].ty;
		root->b_trans.z = bonelist[0].tz;
		root->b_rot.x = bonelist[0].rx;
		root->b_rot.y = bonelist[0].ry;
		root->b_rot.z = bonelist[0].rz;
		root->trans.x = bonelist[0].tx;
		root->trans.y = bonelist[0].ty;
		root->trans.z = bonelist[0].tz;
		root->rot.x = bonelist[0].rx;
		root->rot.y = bonelist[0].ry;
		root->rot.z = bonelist[0].rz;
		root->childCnt = bonelist[0].childCnt;
		root->children = (t_Bone *)malloc((root->childCnt) * sizeof(t_Bone));
		root->curMatrix = (tMatrix *)malloc(sizeof(tMatrix));
		for (i = 0; i < root->childCnt; i++)
		{
			child = &root->children[i];
			ResetBone(child,root);
			strcpy(child->name,bonelist[(int)bonelist[0].child[i]].name);
			child->trans.x = bonelist[(int)bonelist[0].child[i]].tx;
			child->trans.y = bonelist[(int)bonelist[0].child[i]].ty;
			child->trans.z = bonelist[(int)bonelist[0].child[i]].tz;
			child->rot.x = bonelist[(int)bonelist[0].child[i]].rx;
			child->rot.y = bonelist[(int)bonelist[0].child[i]].ry;
			child->rot.z = bonelist[(int)bonelist[0].child[i]].rz;
			child->b_trans.x = bonelist[(int)bonelist[0].child[i]].tx;
			child->b_trans.y = bonelist[(int)bonelist[0].child[i]].ty;
			child->b_trans.z = bonelist[(int)bonelist[0].child[i]].tz;
			child->b_rot.x = bonelist[(int)bonelist[0].child[i]].rx;
			child->b_rot.y = bonelist[(int)bonelist[0].child[i]].ry;
			child->b_rot.z = bonelist[(int)bonelist[0].child[i]].rz;
			child->childCnt = bonelist[(int)bonelist[0].child[i]].childCnt;
			child->curMatrix = (tMatrix *)malloc(sizeof(tMatrix));
			if (child->childCnt > 0)
			{
				AddDarwinBranch(child,&bonelist[(int)bonelist[0].child[i]],bonelist);
			}
		}

		free(bonelist);

		// Move the initial position so it can be seen
		root->b_trans.x = 0.0f;
		root->b_trans.y = -7.0f;
		root->b_trans.z = -40.0f;
		root->trans.x = 0.0f;
		root->trans.y = -7.0f;
		root->trans.z = -40.0f;

	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	LoadSkeleton
// Purpose:		Load a Darwin Format Skeleton
// Arguments:	Name of the file to open and root skeleton to put it in
///////////////////////////////////////////////////////////////////////////////
BOOL LoadSkeletonTest(CString name,t_Bone *root)
{
	/// Local Variables ///////////////////////////////////////////////////////////
	int i,j,boneCnt,parent;
	FILE *fp;		// I PREFER THIS STYLE OF FILE ACCESS
	tSkeletonNode *tempBone,*bonelist;
	t_Bone *child;
	char	temp[5];

	/////add by wz/////
	FILE *freadout;
	freadout = fopen("readout01","wb");
	char tempstr[80];
	Quat quater;
	///////////////////


	///////////////////////////////////////////////////////////////////////////////
	if (fp = fopen((LPCTSTR)name,"rb")) {
        fscanf(fp,"%s",temp);
		fwrite(temp,sizeof(char),4,freadout);//add by wz

		fscanf(fp,"%d",&boneCnt);

		sprintf(tempstr, "%d\r\n", boneCnt); //add by wz
		fwrite(tempstr,sizeof(char),4,freadout);//add by wz


		bonelist = (tSkeletonNode *)malloc((boneCnt) * sizeof(tSkeletonNode));
		for (i = 0; i < boneCnt; i++)
		{
			tempBone = &bonelist[i];
			fscanf(fp,"%s",tempBone->name);

			fwrite(tempBone->name,sizeof(char),12,freadout);//add by wz

			fscanf(fp,"%f",&(tempBone->tx));


			sprintf(tempstr, "%f\r\n", tempBone->tx); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz

			fscanf(fp,"%f",&(tempBone->ty));

			sprintf(tempstr, "%f\r\n", tempBone->ty); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz


			fscanf(fp,"%f",&(tempBone->tz));

			sprintf(tempstr, "%f\r\n", tempBone->tz); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz


			fscanf(fp,"%f",&(tempBone->rx));


			sprintf(tempstr, "%f\r\n", tempBone->rx); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz


			fscanf(fp,"%f",&(tempBone->ry));

			sprintf(tempstr, "%f\r\n", tempBone->ry); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz

			fscanf(fp,"%f",&(tempBone->rz));


			sprintf(tempstr, "%f\r\n", tempBone->rz); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz

			fscanf(fp,"%f",&(tempBone->posi_x));

			sprintf(tempstr, "%f\r\n", tempBone->posi_x); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz


			fscanf(fp,"%f",&(tempBone->posi_y));

			sprintf(tempstr, "%f\r\n", tempBone->posi_y); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz


			fscanf(fp,"%f",&(tempBone->posi_z));

			sprintf(tempstr, "%f\r\n", tempBone->posi_z); //add by wz
			fwrite(tempstr,sizeof(char),12,freadout);//add by wz


			// FIND THE PARENT
            fscanf(fp,"%d",&parent);

			sprintf(tempstr, "%d\r\n", parent); //add by wz
			fwrite(tempstr,sizeof(char),6,freadout);//add by wz


			if (parent == -1)
				tempBone->parent = NULL;
			else
			{
				tempBone->parent = (t_Bone *)parent;
				tempBone->identify = i;
			}

			fscanf(fp,"%d",&(tempBone->childCnt));

			sprintf(tempstr, "%d\r\n", tempBone->childCnt); //add by wz
			fwrite(tempstr,sizeof(char),6,freadout);//add by wz


			for (j = 0; j < tempBone->childCnt; j++)
			{

                fscanf(fp,"%d",&(tempBone->child[j]));

				sprintf(tempstr, "%d\r\n", tempBone->child[j]); //add by wz
				fwrite(tempstr,sizeof(char),6,freadout);//add by wz

			}
		}
		fclose(fp);

		fclose(freadout);//add by wz

		// SET UP ROOT
		//		strcpy(root->name,bonelist[0].name);
		root->b_trans.x = bonelist[0].tx;
		root->b_trans.y = bonelist[0].ty;
		root->b_trans.z = bonelist[0].tz;
		root->b_rot.x = bonelist[0].rx;
		root->b_rot.y = bonelist[0].ry;
		root->b_rot.z = bonelist[0].rz;
		root->trans.x = bonelist[0].tx;
		root->trans.y = bonelist[0].ty;
		root->trans.z = bonelist[0].tz;
		root->rot.x = bonelist[0].rx;
		root->rot.y = bonelist[0].ry;
		root->rot.z = bonelist[0].rz;
		root->childCnt = bonelist[0].childCnt;
		root->children = (t_Bone *)malloc((root->childCnt) * sizeof(t_Bone));
		root->curMatrix = (tMatrix *)malloc(sizeof(tMatrix));

		root->globalposi = (tVector *)malloc(sizeof(tVector));

		root->globalposi->x = bonelist[0].posi_x;
		root->globalposi->y = bonelist[0].posi_y;
		root->globalposi->z = bonelist[0].posi_z;

		root->localposi = (tVector *)malloc(sizeof(tVector));
		root->localposi->x = bonelist[0].tx;
		root->localposi->y = bonelist[0].ty;
		root->localposi->z = bonelist[0].tz;


	    root->anim_matrix = (tMatrix *)malloc(sizeof(tMatrix));

		root->id = bonelist[0].identify;


		for (i = 0; i < root->childCnt; i++)
		{
			child = &root->children[i];
			ResetBone(child,root);
			strcpy(child->name,bonelist[(int)bonelist[0].child[i]].name);
			child->trans.x = bonelist[(int)bonelist[0].child[i]].tx;
			child->trans.y = bonelist[(int)bonelist[0].child[i]].ty;
			child->trans.z = bonelist[(int)bonelist[0].child[i]].tz;
			child->rot.x = bonelist[(int)bonelist[0].child[i]].rx;
			child->rot.y = bonelist[(int)bonelist[0].child[i]].ry;
			child->rot.z = bonelist[(int)bonelist[0].child[i]].rz;
			child->b_trans.x = bonelist[(int)bonelist[0].child[i]].tx;
			child->b_trans.y = bonelist[(int)bonelist[0].child[i]].ty;
			child->b_trans.z = bonelist[(int)bonelist[0].child[i]].tz;
			child->b_rot.x = bonelist[(int)bonelist[0].child[i]].rx;
			child->b_rot.y = bonelist[(int)bonelist[0].child[i]].ry;
			child->b_rot.z = bonelist[(int)bonelist[0].child[i]].rz;
			child->childCnt = bonelist[(int)bonelist[0].child[i]].childCnt;
			child->curMatrix = (tMatrix *)malloc(sizeof(tMatrix));
		    child->globalposi = (tVector *)malloc(sizeof(tVector));
			child->globalposi->x = bonelist[(int)bonelist[0].child[i]].posi_x;
			child->globalposi->y = bonelist[(int)bonelist[0].child[i]].posi_y;
			child->globalposi->z = bonelist[(int)bonelist[0].child[i]].posi_z;

			child->localposi = (tVector *)malloc(sizeof(tVector));
			child->localposi->x = bonelist[(int)bonelist[0].child[i]].tx;
			child->localposi->y = bonelist[(int)bonelist[0].child[i]].ty;
			child->localposi->z = bonelist[(int)bonelist[0].child[i]].tz;

            child->anim_matrix = (tMatrix *)malloc(sizeof(tMatrix));

			child->id = bonelist[(int)bonelist[0].child[i]].identify;


			if (child->childCnt > 0)
			{
				AddDarwinBranch(child,&bonelist[(int)bonelist[0].child[i]],bonelist);
			}
		}

		free(bonelist);

		// Move the initial position so it can be seen
		root->b_trans.x = 0.0f;
		root->b_trans.y = -7.0f;
		root->b_trans.z = -40.0f;
		root->trans.x = 0.0f;
		root->trans.y = -7.0f;
		root->trans.z = -40.0f;

	}
	return TRUE;
}