/**********************************************************
¡ô©©©Ç zHA! H00k vEr:5.0 ©Ï©©¡ô
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
¡ô©©©Ç      (c)2018      ©Ï©©¡ô
    File Name: CalcScreen.cpp
************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#define BOUND_VALUE(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}
#include "common/mathlib.h"
//#include "engine/eiface.h"
#include "calcscreen.h"

//===========================================================
float mainViewOrigin[3],mainViewAngles[3],TempNewAim[3],_inOrigin[3];
float displayCenterX=500, displayCenterY=350;

// extern float fCurrentFOV;
// add this var in your user-message hook
// temp var for compiling (std fov, not in zoom!!!)
float fCurrentFOV=90;
//===========================================================
inline float VectorLength(const vec3_t v)
{
  return (float)sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

//===========================================================
inline float VectorAngle(const vec3_t a, const vec3_t b)
{
  float length_a=VectorLength(a);
	float length_b=VectorLength(b);
	float length_ab=length_a*length_b;
	if (length_ab==0.0)
  { 
    return 0.0; 
  }
	else                
  { 
    return (float)(acos(DotProduct(a,b)/length_ab)*(180.f/M_PI)); 
  }
}

//===========================================================
void MakeVector(const vec3_t ain, vec3_t vout)
{
	float pitch;
	float yaw;
	float tmp;		
	
	pitch=(float)(ain[0]*M_PI/180);
	yaw=(float)(ain[1]*M_PI/180);
	tmp=(float)cos(pitch);
	
	vout[0]=(float)(-tmp*-cos(yaw));
	vout[1]=(float)(sin(yaw)*tmp);
	vout[2]=(float)-sin(pitch);
}

//===========================================================
void VectorRotateX(const vec3_t in, float angle, vec3_t out)
{
	float a, c, s;

	a=(float)(angle*M_PI/180);
	c=(float)cos(a);
	s=(float)sin(a);
	out[0]=in[0];
	out[1]=c*in[1]-s*in[2];
	out[2]=s*in[1]+c*in[2];	
}

//===========================================================
void VectorRotateY(const vec3_t in, float angle, vec3_t out)
{
	float a, c, s;

	a=(float)(angle*M_PI/180);
	c=(float)cos(a);
	s=(float)sin(a);
	out[0]=c*in[0]+s*in[2];
	out[1]=in[1];
	out[2]=-s*in[0]+c*in[2];
}

//===========================================================
void VectorRotateZ(const vec3_t in, float angle, vec3_t out)
{
	float a, c, s;

	a=(float)(angle*M_PI/180);
	c=(float)cos(a);
	s=(float)sin(a);
	out[0]=c*in[0]-s*in[1];
	out[1]=s*in[0]+c*in[1];
	out[2]=in[2];
}

//===========================================================
int NewCalcScreen(float* in, float* out)
{
	vec3_t aim;
	vec3_t newaim;
	vec3_t view;
	vec3_t tmp;
	float num;

	if (!in || !out)
	{ 
		return false; 
	}

	VectorSubtract(in, mainViewOrigin, aim);
	MakeVector(mainViewAngles, view);

	if (VectorAngle(view, aim)>(fCurrentFOV/1.8))
	{
		return false;
	}		

	VectorRotateZ(aim, -mainViewAngles[1], newaim); // yaw
	VectorRotateY(newaim, -mainViewAngles[0], tmp); // pitch
	VectorRotateX(tmp, -mainViewAngles[2], newaim); // roll
	
	if (   newaim[0]<=0   ) { return false;	}
	if ( fCurrentFOV==0.0 ) { return false; }

	num=(float)((displayCenterX/newaim[0])*(120.0/fCurrentFOV-1.0/3.0));

	out[0]=displayCenterX-num*newaim[1];
	out[1]=displayCenterY-num*newaim[2];


	BOUND_VALUE(out[0], 0, displayCenterX*2);
	BOUND_VALUE(out[1], 0, displayCenterY*2);

	return true;
}

//===========================================================
int NewCalcScreen1(float* in, float* out)
{
	vec3_t aim;
	vec3_t newaim;
	vec3_t view;
	vec3_t tmp;
	float num;

	if (!in || !out)
  { 
    return false; 
  }

	VectorSubtract(in, mainViewOrigin, aim);	
	MakeVector(mainViewAngles, view);		
	
	VectorRotateZ(aim, -mainViewAngles[1], newaim); // yaw
	VectorRotateY(newaim, -mainViewAngles[0], tmp); // pitch
	VectorRotateX(tmp, -mainViewAngles[2], newaim); // roll
	
	num=(float)((displayCenterX/newaim[0])*(120.0/fCurrentFOV-1.0/3.0));

	out[0]=displayCenterX-num*newaim[1];
	out[1]=displayCenterY-num*newaim[2];


	BOUND_VALUE(out[0], 0, displayCenterX*2);
	BOUND_VALUE(out[1], 0, displayCenterY*2);

	return true;
}
//===========================================================
