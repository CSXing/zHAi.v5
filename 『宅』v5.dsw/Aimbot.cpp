/**********************************************************
◆┅┣ zHA! H00k vEr:5.0 ┫┅◆
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
◆┅┣      (c)2018      ┫┅◆
    File Name: Aimbot.cpp
************************************************************/

#include <windows.h>
#include <math.h>
#include "idhook.h"
#include "players.h"
#include "Aimbot.h"
#include "Arms.h"
#include "misc/defs.h"
#include "calcscreen.h"
#include "drawing.h"
#include "font.h"
#include "cvars.h"

using namespace std;

typedef float TransformMatrix[MAXSTUDIOBONES][3][4];

CAimbot Aimbot;
extern bool oglSubtractive;
#define M_PI 3.14159265358979323846
#define BOUND_VALUE(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}

//==============================================================================

void VectorAngles( const float *forward, float *angles )
{
	float tmp, yaw, pitch;
	
	if( forward[1] == 0 && forward[0] == 0 )
	{
		yaw = 0;
		if( forward[2] > 0 )
			pitch = 90.0f;
		else
			pitch = 270.0f;
	}
	else
	{
		yaw = ( float )( ( atan2( forward[1], forward[0] ) * 180 / M_PI ) );
		if( yaw < 0 )
			yaw += 360.0f;

		tmp = sqrt( forward[0] * forward[0] + forward[1] * forward[1] );
		pitch = ( float )( ( atan2( forward[2], tmp ) * 180 / M_PI ) );
	}
	
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

//==============================================================================

void VectorTransform (float *in1, float in2[3][4], float *out)
{
    out[0] = DotProduct(in1, in2[0]) + in2[0][3];
    out[1] = DotProduct(in1, in2[1]) + in2[1][3];
    out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}
//=====================================================================================================>>

void CalAimOffsets(float* aim_location, float* origin, float* aim_offsets)
{
	aim_offsets[0] = aim_location[0] - origin[0];
	aim_offsets[1] = aim_location[1] - origin[1];
	aim_offsets[2] = aim_location[2] - origin[2];
}
//=====================================================================================================>>

static void PredictTarget(int index,float *pred)
{
	VectorCopy(vPlayers[index].origin(),pred);
}
//==============================================================================

void zHAi_VectorAngles(const float *forward,float *angles)
{
	float tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt (forward[0]*forward[0] + forward[1]*forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
	while(angles[0]<-89){angles[0]+=180;angles[1]+=180;}
	while(angles[0]>89){angles[0]-=180;angles[1]+=180;}
	while(angles[1]<-180){angles[1]+=360;}
	while(angles[1]>180){angles[1]-=360;}
}

void ApplySilentAngles(float *angles,usercmd_s *cmd)
{
	Vector viewforward, viewright, viewup, aimforward, aimright, aimup, vTemp;
	float newforward, newright, newup;
	float forward = cmd->forwardmove;
	float right = cmd->sidemove;
	float up = cmd->upmove;

	if(!me.alive)return;
	if(!me.ent) return;

	if(me.ent->curstate.movetype == MOVETYPE_WALK){oEngfuncs.pfnAngleVectors(Vector(0.0f, cmd->viewangles.y, 0.0f), viewforward, viewright, viewup);}
	else{oEngfuncs.pfnAngleVectors(cmd->viewangles, viewforward, viewright, viewup);}

	int iHasShiftHeld = GetAsyncKeyState(VK_LSHIFT);
	if(me.ent->curstate.movetype == MOVETYPE_WALK && !iHasShiftHeld && !(me.pmMoveType == 5))
	{
		cmd->viewangles.x = angles[0];
		cmd->viewangles.y = angles[1];	
	}

	if(me.ent->curstate.movetype == MOVETYPE_WALK){oEngfuncs.pfnAngleVectors(Vector(0.0f, cmd->viewangles.y, 0.0f), aimforward, aimright, aimup);}
	else{oEngfuncs.pfnAngleVectors(cmd->viewangles, aimforward, aimright, aimup);}

	newforward = DotProduct(forward * viewforward.Normalize(), aimforward) + DotProduct(right * viewright.Normalize(), aimforward) + DotProduct(up * viewup.Normalize(), aimforward);
	newright = DotProduct(forward * viewforward.Normalize(), aimright) + DotProduct(right * viewright.Normalize(), aimright) + DotProduct(up * viewup.Normalize(), aimright);
	newup = DotProduct(forward * viewforward.Normalize(), aimup) + DotProduct(right * viewright.Normalize(), aimup) + DotProduct(up * viewup.Normalize(), aimup);
	
	if (angles[0]>81 ){cmd->forwardmove = -newforward;} 
	else {cmd->forwardmove = newforward;}

	cmd->sidemove = newright;
	cmd->upmove = newup;
}

//==============================================================================
void SmoothAimAngles(float *MyViewAngles, float *AimAngles, float *OutAngles, float Smoothing)
{
	VectorSubtract(AimAngles, MyViewAngles, OutAngles);

	if(OutAngles[1]>180.0f) OutAngles[1] -= 360.0f;
	if(OutAngles[1]<-180.0f) OutAngles[1] += 360.0f;

	OutAngles[0] = OutAngles[0] / Smoothing + MyViewAngles[0];
	OutAngles[1] = OutAngles[1] / Smoothing + MyViewAngles[1];

	if(OutAngles[1]>360.0f)OutAngles[1] -= 360.0f;
	if(OutAngles[1]<0.0f)OutAngles[1] += 360.0f;
}

//==============================================================================
vec3_t CalcOffset(int Id)
{
	PlayerInfo& r = vPlayers[Id];
	cl_entity_s* pEnt=r.getEnt();
	
	if( r.isAlive() && pEnt != NULL)
	{
		Vector vAngles,vF,vR,vU,fInOut;
		float oS_Stand, oS_Duck, oS_Jump;
		if ( (cvar.hitbox ==1 || idhook.Plys[Id]==1 || cArms.isPower()) ) {
			oS_Stand = 22.0f*0.90f;  //22
			oS_Duck  = 25.0f*0.90f;  //25
			oS_Jump  = 13.0f*0.90f;  //13
		} else {
			oS_Stand = 22.0f*0.82f;
			oS_Duck  = 25.0f*0.85f;
			oS_Jump  = 13.0f*0.82f;
		}

		vAngles=Vector(0.0f,pEnt->angles[1],0.0f);
		oEngfuncs.pfnAngleVectors(vAngles,vF,vR,vU);

		if(pEnt->curstate.gaitsequence == 2 || pEnt->curstate.gaitsequence == 5)
		{
			fInOut = pEnt->origin+vF*5.5f+vR*2.5f+vU*oS_Duck;  //蹲下
		}
		else if(pEnt->curstate.gaitsequence == 6)
		{
			fInOut = pEnt->origin+vF*2.0f+vR*5.0f+vU*oS_Jump;  //跳起
		}
		else
		{
			fInOut = pEnt->origin+vF*5.5f+vR*2.5f+vU*oS_Stand; //站在
		}
		return fInOut;
	}
	return NULL;
}


//==============================================================================
void CalcALL_Hitbox( int ax, TransformMatrix* pbonetransform )
{
	for (int i=0;i<55;i++)
	{
		vPlayers[ax].vBones[i][0] = (*pbonetransform)[i][0][3];
		vPlayers[ax].vBones[i][1] = (*pbonetransform)[i][1][3];
		vPlayers[ax].vBones[i][2] = (*pbonetransform)[i][2][3];
	}
}

//==============================================================================
void HitBox_Draw(int ax)
{
	float distance,boxSize;
	
	boxSize=2;
	if ( vPlayers[ax].isUpdated() ){
		if ( vPlayers[ax].isUpdatedAddEnt() ){
			distance = vPlayers[ax].distance/22.0f;
			boxSize = (300.0*90.0) / (distance*fCurrentFOV);
		}
	}
	BOUND_VALUE(boxSize,2,4);

	int text_distbone = 1;

	//color
	vec3_t Ent_Angles = vPlayers[ax].getEnt()->angles;
	ColorEntry *color2;
	if (idhook.Plys[ax]==1){
		color2 = colorList.get(8);;
	}else{
		color2 = PlayerColor(vPlayers[ax].entindex);
	}

	if ( vPlayers[ax].isUpdatedAddEnt() ){ //
		if (vPlayers[ax].bGotHead && !vPlayers[ax].visible) //&& (!vPlayers[ax].visible || Aimbot.iTarget==ax)
		{
			vec3_t _Up, _Right, _Forward, _PlayerAngles;

			_PlayerAngles[0]=0;
			_PlayerAngles[1]=Ent_Angles[1];
			_PlayerAngles[2]=0;
			oEngfuncs.pfnAngleVectors (_PlayerAngles, _Forward, _Right, _Up);
			_Forward[2]=-_Forward[2];

		//if (aPly.bGotHead && !aPly.visible)
		//{
			//draw line
			//back bone
			DrawVectorLine(vPlayers[ax].vBones[1] ,vPlayers[ax].vBones[2] ,text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[2] ,vPlayers[ax].vBones[3] ,text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[3] ,vPlayers[ax].vBones[4] ,text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[4] ,vPlayers[ax].vBones[5] ,text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[5] ,vPlayers[ax].vBones[6] ,text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[6] ,vPlayers[ax].vBones[7] ,text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[7] ,vPlayers[ax].vBones[8] ,text_distbone,color2);
			//left arm
			DrawVectorLine(vPlayers[ax].vBones[6] ,vPlayers[ax].vBones[9] ,text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[9] ,vPlayers[ax].vBones[10],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[10],vPlayers[ax].vBones[23],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[23],vPlayers[ax].vBones[22],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[22],vPlayers[ax].vBones[21],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[21],vPlayers[ax].vBones[11],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[11],vPlayers[ax].vBones[20],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[20],vPlayers[ax].vBones[19],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[19],vPlayers[ax].vBones[18],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[18],vPlayers[ax].vBones[12],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[12],vPlayers[ax].vBones[13],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[13],vPlayers[ax].vBones[14],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[12],vPlayers[ax].vBones[15],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[15],vPlayers[ax].vBones[16],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[16],vPlayers[ax].vBones[17],text_distbone,color2);
			//right arm
			DrawVectorLine(vPlayers[ax].vBones[6] ,vPlayers[ax].vBones[24],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[24],vPlayers[ax].vBones[25],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[25],vPlayers[ax].vBones[26],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[26],vPlayers[ax].vBones[27],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[27],vPlayers[ax].vBones[28],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[27],vPlayers[ax].vBones[29],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[27],vPlayers[ax].vBones[30],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[27],vPlayers[ax].vBones[31],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[31],vPlayers[ax].vBones[32],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[32],vPlayers[ax].vBones[33],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[33],vPlayers[ax].vBones[34],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[34],vPlayers[ax].vBones[35],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[35],vPlayers[ax].vBones[36],text_distbone,color2);
			//waistline
			DrawVectorLine(vPlayers[ax].vBones[37],vPlayers[ax].vBones[37],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[37],vPlayers[ax].vBones[38],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[1] ,vPlayers[ax].vBones[39],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[46],vPlayers[ax].vBones[46],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[48],vPlayers[ax].vBones[48],text_distbone,color2);
			//left foot
			DrawVectorLine(vPlayers[ax].vBones[39],vPlayers[ax].vBones[40],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[40],vPlayers[ax].vBones[41],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[41],vPlayers[ax].vBones[42],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[42],vPlayers[ax].vBones[43],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[43],vPlayers[ax].vBones[44],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[44],vPlayers[ax].vBones[45],text_distbone,color2);
			//right foot
			DrawVectorLine(vPlayers[ax].vBones[46],vPlayers[ax].vBones[47],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[48],vPlayers[ax].vBones[49],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[49],vPlayers[ax].vBones[50],text_distbone,color2);
			DrawVectorLine(vPlayers[ax].vBones[50],vPlayers[ax].vBones[51],text_distbone,color2);
		//	DrawVectorLine(vPlayers[ax].vBones[51],vPlayers[ax].vBones[52],text_distbone,color2);
		//	DrawVectorLine(vPlayers[ax].vBones[52],vPlayers[ax].vBones[53],text_distbone,color2);//bug
		//	DrawVectorLine(vPlayers[ax].vBones[53],vPlayers[ax].vBones[54],text_distbone,color2);//not wanted
		}
	}
	
	vec3_t EntViewOrg;
	float vecScreen[2];
	PredictTarget(ax, EntViewOrg);
	if (0<=vPlayers[ax].m_AimbotBox && vPlayers[ax].m_AimbotBox<=me.iMaxHitboxes){
		EntViewOrg = EntViewOrg + vPlayers[ax].vHitbox[vPlayers[ax].m_AimbotBox];
	}

	if( !CalcScreen(EntViewOrg, vecScreen) ) { return; }
	DrawBox(vecScreen[0]-boxSize/2, vecScreen[1]-boxSize/2, color2->r,color2->g,color2->b,234, boxSize);
	/*
	if (ax == Aimbot.iTarget){
		char Temp[255];
		sprintf( Temp,"Fov:%0.2f;  Hitbox:%i", vPlayers[ax].fovangle, vPlayers[ax].m_AimbotBox);
		DrawConsoleStringEx(vecScreen[0]+10, vecScreen[1]-10, color2->r,color2->g,color2->b,Temp);
	}*/
}

//==============================================================================
void CAimbot::CL_CreateMove(usercmd_s* usercmd)
{
	if ( iTarget==-1 ) return;

	//FindTarget();
	float fMyCmdAngles[3], fAimAngles[3], fSmoothAngles[3];

	VectorCopy(usercmd->viewangles,fMyCmdAngles);
	zHAi_VectorAngles(CalcOffset(iTarget)-me.pmEyePos,fAimAngles);
	SmoothAimAngles(fMyCmdAngles,fAimAngles,fSmoothAngles,1.0f);
	ApplySilentAngles(fSmoothAngles,usercmd);
	oEngfuncs.SetViewAngles((float *)fSmoothAngles);
}

//==============================================================================
void CAimbot::CalculateHitbox( cl_entity_s *pEnt )
{
	if( !vPlayers[pEnt->index].bGotHead )
	{
		int i, Id = pEnt->index;

		vec3_t vMin, vMax, tmp_Bones, tmp_Hitbox;
		model_s		    *pModel			= oEngStudio.SetupPlayerModel( Id );
		studiohdr_t     *pStudioHeader	= ( studiohdr_t* )oEngStudio.Mod_Extradata( pModel );
		mstudiobbox_t   *pStudioBox     = ( mstudiobbox_t* )( ( byte* )pStudioHeader + pStudioHeader->hitboxindex );
		TransformMatrix *pBoneTransform = ( TransformMatrix* )oEngStudio.StudioGetBoneTransform();	
		
		me.iMaxBones    = pStudioHeader->numbones-1;
		me.iMaxHitboxes = pStudioHeader->numhitboxes-1;

		//CalcALL_Hitbox(Id,pBoneTransform);
		for (i=0; i<pStudioHeader->numbones; i++)
		{
			tmp_Bones[0] = (*pBoneTransform)[i][0][3];
			tmp_Bones[1] = (*pBoneTransform)[i][1][3];
			tmp_Bones[2] = (*pBoneTransform)[i][2][3];
			VectorCopy(tmp_Bones,vPlayers[Id].vBones[i]);
			//CalAimOffsets(tmp_Bones,pEnt->origin,vPlayers[Id].vBones[i]);
		}

		//Head 11 bone 7 | Low Head 9 bone 5 | Chest 8 bone 4 | Stomach 7 bone 3
		for(i=0; i<pStudioHeader->numhitboxes; i++)
		{
			VectorTransform(pStudioBox[i].bbmin, (*pBoneTransform)[pStudioBox[i].bone], vMin);
			VectorTransform(pStudioBox[i].bbmax, (*pBoneTransform)[pStudioBox[i].bone], vMax);
			tmp_Hitbox    = ( vMin + vMax ) * 0.5f;
			CalAimOffsets(tmp_Hitbox,pEnt->origin,vPlayers[Id].vHitbox[i]);
			vPlayers[Id].bGotHead = true;
		}
	}
}

//==============================================================================

void CAimbot::CalculateAimingView( void )
{
	float view[3];
	vec3_t EntViewOrg ;
	if ( iTarget==-1) return;
	if (!(0<=vPlayers[iTarget].m_AimbotBox && vPlayers[iTarget].m_AimbotBox<=me.iMaxHitboxes)) return;

	PredictTarget(iTarget, EntViewOrg);

	EntViewOrg = EntViewOrg + vPlayers[iTarget].vHitbox[vPlayers[iTarget].m_AimbotBox];

	view[0] = EntViewOrg[0] - me.pmEyePos[0];
	view[1] = EntViewOrg[1] - me.pmEyePos[1];
	view[2] = EntViewOrg[2] - me.pmEyePos[2];

	VectorAngles(view,aim_viewangles);
	aim_viewangles[0] *= -1;

	if (aim_viewangles[0]>180) aim_viewangles[0]-=360;
	if (aim_viewangles[1]>180) aim_viewangles[1]-=360;
}

//==============================================================================

pmtrace_t pmtrace;
int CAimbot::CanPenetrate( float *start, float *end, int power ) 
{ 
	pmtrace_t* tr = (pmtrace_t*) &pmtrace;

	float view[3]; 
	float dir[3]; 

	view[0] = end[0] - start[0]; 
	view[1] = end[1] - start[1]; 
	view[2] = end[2] - start[2]; 

	float length = VectorLength(view); 

	dir[0] = view[0] / length; 
	dir[1] = view[1] / length; 
	dir[2] = view[2] / length; 

	float position[3]; 
	position[0] = start[0]; 
	position[1] = start[1]; 
	position[2] = start[2]; 
	tr->startsolid = true; 

	while( power ) 
	{ 
	   if( !tr->startsolid ) 
	      power--; 
	   tr = oEngfuncs.PM_TraceLine( position, end, PM_TRACELINE_PHYSENTSONLY, 2, -1); 
        
	   if( tr->fraction==1.0f ) 
	      return 1; 
	   if( tr->allsolid ) 
	      return 0; 

	   position[0] = tr->endpos[0] + dir[0] * 8.0f; 
	   position[1] = tr->endpos[1] + dir[1] * 8.0f; 
	   position[2] = tr->endpos[2] + dir[2] * 8.0f; 
	} 
	return 0; 
} 

//==============================================================================

int CAimbot::CorrectGun()
{
	int Id=cArms.Index();
	if (Id == Arms_SG550 ||
		Id == Arms_G3SG1 ||
		Id == Arms_SCOUT ||
		Id == Arms_AWP) 
	{
		return 3;
	} else if ( Id == Arms_AUG    ||
				Id == Arms_M249   ||
				Id == Arms_M4A1   ||
				Id == Arms_DEAGLE ||
				Id == Arms_SG552  ||
				Id == Arms_AK47   ||
				Id == Arms_FAMAS  ||
				Id == Arms_GALIL)  
	{
		return 2;
	}
	return 0; 
}

//==============================================================================

#pragma warning (disable:4800)
bool CAimbot::pathFree(float* xfrom,float* xto)
{
	int pathtest; 
	pmtrace_t tr; 
	oEngfuncs.pEventAPI->EV_SetTraceHull( 2 ); 
	oEngfuncs.pEventAPI->EV_PlayerTrace( xfrom, xto, PM_GLASS_IGNORE, me.entindex, &tr ); 
	pathtest = (tr.fraction == 1.0); 
	if (!pathtest && cvar.autowall && CorrectGun()) 
	{ 
	   pathtest = CanPenetrate(xfrom, xto, CorrectGun()); 
	} 
	return pathtest; 
}

//====================================================================================

bool CAimbot::TargetRegion(int ax)
{
	vec3_t EntViewSrc,EntViewOrg;

	PredictTarget(ax, EntViewSrc);

	//Head 11 bone 7 | Low Head 9 bone 5 | Chest 8 bone 4 | Stomach 7 bone 3
	int i, iBox = 9;
	if ( idhook.Plys[ax]==1 )  iBox = 11;
	else if (cvar.hitbox == 1) iBox = 11;
	else if (cvar.hitbox == 2) iBox = 9;
	else if (cvar.hitbox == 3) iBox = 8;
	//else if (cvar.hitbox == 0) iBox = vPlayers[Id].AimSite;

	vPlayers[ax].m_AimbotBox = iBox;
	vPlayers[ax].fovangle    = calcFovAngle(ax);
		VectorCopy(EntViewSrc + vPlayers[ax].vHitbox[iBox],EntViewOrg);
	if(pathFree(me.pmEyePos,EntViewOrg)){
		return true;
	}

	for (i=0; i<3; i++){
		VectorCopy(EntViewSrc + vPlayers[ax].vHitbox[9-i],EntViewOrg);
		if(pathFree(me.pmEyePos,EntViewOrg)){
			vPlayers[ax].m_AimbotBox = 9-i;
			vPlayers[ax].fovangle    = calcFovAngle(ax);
			return true;
		}
	}

	//if (cvar.hitbox == 0){
		for (i=me.iMaxHitboxes; i>=0; i--){
			VectorCopy(EntViewSrc + vPlayers[ax].vHitbox[i],EntViewOrg);
			if(pathFree(me.pmEyePos,EntViewOrg)){
				if ( cArms.InCrosshair() ){
					vPlayers[ax].m_AimbotBox = i;
					vPlayers[ax].fovangle    = calcFovAngle(ax);
				}
				return true;
			}
		}
	//}
	return false;
/*
	if(pathFree(me.pmEyePos,EntViewOrg)) return true;
	else
	return false;*/
}

//==============================================================================

void CAimbot::FindTarget( void )
{
	int actTarget=-1;
	SetTarget(-1);
	bToKill=false;
	if (!me.alive) return;

	for (int ax=0;ax<MAX_VPLAYERS;ax++)
	{
		if  ( vPlayers[ax].isUpdatedAddEnt() && vPlayers[ax].canAim )
		{
			if ( cvar.id_mode==1 && idhook.Plys[ax]==1 ) {vPlayers[ax].fovangle -=2.5f; }
			if( !HasTarget() )											{ actTarget=ax; continue; }	
			if( vPlayers[ax].fovangle < vPlayers[actTarget].fovangle )	{ actTarget=ax; }
			//if( vPlayers[ax].distance < vPlayers[iTarget].distance )	{ SetTarget(ax); }
		}
	}
	SetTarget(actTarget);
	if ( cvar.id_mode==1 && idhook.Plys[actTarget]==1 ) bToKill=true;
	//id_mode==1 先杀视野中的专杀对象
}

//==============================================================================

void CAimbot::Draw()
{
	if ( !me.alive ) return;
	for (int ax=0;ax<MAX_VPLAYERS;ax++)
		if ( vPlayers[ax].isUpdated() && vPlayers[ax].isAlive() && isEnemy(ax) ) {
			if ( cvar.FarSound || idhook.Plys[ax]==1 ){
				HitBox_Draw(ax);
				//Draw_Player_Vecs(ax);
			}
		}
}

//==============================================================================
float CAimbot::calcFovAngle(int ax)
{
	float view[3];
	vec3_t EntViewOrg, _offsets;

	PredictTarget(ax, EntViewOrg);
	VectorCopy(vPlayers[ax].vHitbox[vPlayers[ax].m_AimbotBox], _offsets);

	EntViewOrg = EntViewOrg + _offsets;

	view[0] = EntViewOrg[0] - me.pmEyePos[0];
	view[1] = EntViewOrg[1] - me.pmEyePos[1];
	view[2] = EntViewOrg[2] - me.pmEyePos[2];

	Vector a,b ;
	a=me.vForward;
	b=view;

	float l1=0.0f, l2=0.0f;
	l1=a.Length();
	l2=b.Length();
	float sc=a.x*b.x+a.y*b.y+a.z*b.z;
	return acos(sc/(l1*l2))*(180.0/M_PI);
}
/*
float CAimbot::calcFovAngle(int ax)
{
	float view[3];
	vec3_t EntViewOrg;

	PredictTarget(ax, EntViewOrg);

	EntViewOrg = EntViewOrg + vPlayers[ax].offsets;

	view[0] = EntViewOrg[0] - me.pmEyePos[0];
	view[1] = EntViewOrg[1] - me.pmEyePos[1];
	view[2] = EntViewOrg[2] - me.pmEyePos[2];

	Vector a,b ;
	a=me.vForward;
	b=view;

	float l1=0.0f, l2=0.0f;
	l1=a.Length();
	l2=b.Length();
	float sc=a.x*b.x+a.y*b.y+a.z*b.z;
	return acos(sc/(l1*l2))*(180.0/M_PI);
}*/
/*
float CAimbot::calcFovAngle(const float* origin_viewer, const float* angle_viewer, const float* origin_target)
{
	double vec[3], view[3];
	double dot;	
	
	view[0] = origin_target[0] - origin_viewer[0];
	view[1] = origin_target[1] - origin_viewer[1];
	view[2] = origin_target[2] - origin_viewer[2];

	dot = sqrt(view[0] * view[0] + view[1] * view[1] + view[2] * view[2]);
	dot = 1/dot;

	vec[0] = view[0] * dot;
	vec[1] = view[1] * dot;
	vec[2] = view[2] * dot;
	
	view[0] =  sin((angle_viewer[1] + 90) * (M_PI / 180));
	view[1] = -cos((angle_viewer[1] + 90) * (M_PI / 180));
	view[2] = -sin( angle_viewer[0] * (M_PI / 180));	
	
	dot = view[0] * vec[0] + view[1] * vec[1] + view[2] * vec[2];
	
	// dot to angle:
	return (float)((1.0-dot)*180.0); 
}*/

//==============================================================================

bool CAimbot::CheckTeam(int ax)
{
	return isEnemy(ax);
}

//==============================================================================

void CAimbot::calcFovangleAndVisibility(int ax)
{
	float PlyFov = 0.0f, DistFov=0.0f; //1=10 , 2=30 , 3=90 , 4=360 , 5=1
	PlayerInfo& r = vPlayers[ax];
	//r.fovangle    = calcFovAngle( ax );
	//r.fovangle    = calcFovAngle(me.pmEyePos, me.viewAngles, r.origin() );

	if      (cvar.fov == 1) PlyFov = 1.0f;
	else if (cvar.fov == 2) PlyFov = 1.5f;
	else if (cvar.fov == 3) PlyFov = 3.0f;
	else if (cvar.fov == 4) PlyFov = 5.0f;
	else if (cvar.fov == 5) PlyFov = 10.0f;
	
	if (cvar.autoFov>0) PlyFov+=cvar.autoFov;
	DistFov = vPlayers[ax].distance/3000.0f;
	PlyFov-=DistFov;
	if ( PlyFov<0.768f ) PlyFov=0.768f;

	if ( cvar.id_mode==1 && idhook.Plys[ax]==1 ) { PlyFov += 2.5f; }
//fov=360;
	r.visible = TargetRegion(ax);
	if (   !CheckTeam(ax)
		|| r.fovangle>PlyFov
		|| ( 0<cvar.disaim && r.distance < cvar.disaim )
		|| !r.isAlive()
		){
		 r.canAim = 0; 
	}else{
		r.canAim = r.visible;
	}
}

//==============================================================================
