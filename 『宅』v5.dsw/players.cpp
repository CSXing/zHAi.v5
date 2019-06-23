/**********************************************************
◆┅┣ zHA! H00k vEr:5.0 ┫┅◆
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
◆┅┣      (c)2018      ┫┅◆
    File Name: Players.cpp
************************************************************/
#include <windows.h>
#include <math.h>
#include <memory.h>
#include "misc/defs.h"
#include "players.h"
#include "drawing.h"
#include "font.h"
#include "cvars.h"
//===========================================================
local_player_info me;
_Players vPlayers;

//===========================================================
static bool bWait=false;
static DWORD Timer_Tick=0,Now_Tick=0, Now_ckTick=0;
static bool PressLMB=false,inRecoil=false;

//===========================================================
	extern float displayCenterX, displayCenterY;
	extern SCREENINFO screeninfo;
	extern bool oglSubtractive;
//===========================================================
inline float VectorLength(const vec3_t v)
{
  return (float)sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}
//===========================================================
#define M_PI 3.14159265358979323846
#define BOUND_VALUE(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}
typedef float TransformMatrix[MAXSTUDIOBONES][3][4];
//===========================================================
int Cstrike_SequenceInfo[] = 
{
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, // 0..9   
	0,	1,	2,	0,	1,	2,	0,	1,	2,	0, // 10..19 
	1,	2,	0,	1,	1,	2,	0,	1,	1,	2, // 20..29 
	0,	1,	2,	0,	1,	2,	0,	1,	2,	0, // 30..39 
	1,	2,	0,	1,	2,	0,	1,	2,	0,	1, // 40..49 
	2,	0,	1,	2,	0,	0,	0,	8,	0,	8, // 50..59 
	0, 16,	0, 16,	0,	0,	1,	1,	2,	0, // 60..69 
	1,	1,	2,	0,	1,	0,	1,	0,	1,	2, // 70..79 
	0,	1,	2, 	32, 40, 32, 40, 32, 32, 32, // 80..89
   	33, 64, 33, 34, 64, 65, 34, 32, 32, 4, // 90..99
	4,	4,	4,	4,	4,	4,	4,	4,	4,	4, // 100..109
	4                                      	// 110
};

//===========================================================
char* gModeName( int ModeId )
{
	static char weapon[50];
	weapon[0]=0;
	model_s* mdl = oEngStudio.GetModelByIndex( ModeId );
	if( !mdl ){ return weapon; }
	char* name = mdl->name;  if( !name )          { return weapon; }
	int len = strlen(name);  if( len>48 || len<10){ return weapon; }
	strcpy(weapon,name+9); len -=9;
	if(len>4)weapon[len-4]=(char)0;
	return weapon;
}
//===========================================================
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
//===========================================================
static inline void CalcOffsets(float* _location, float* origin, float* _offsets)
{
	_offsets[0] = _location[0] - origin[0];
	_offsets[1] = _location[1] - origin[1];
	_offsets[2] = _location[2] - origin[2];
}

//===========================================================
void VectorTransform(float *in1, float in2[3][4], float *out)
{
    out[0] = DotProduct(in1, in2[0]) + in2[0][3];
    out[1] = DotProduct(in1, in2[1]) + in2[1][3];
    out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}

#pragma warning (disable:4800)
bool pathFree(float* xfrom,float* xto)
{
	int pathtest; 
	pmtrace_t tr; 
	oEngfuncs.pEventAPI->EV_SetTraceHull( 2 ); 
	oEngfuncs.pEventAPI->EV_PlayerTrace( xfrom, xto, PM_GLASS_IGNORE, me.m_Index, &tr ); 
	pathtest = (tr.fraction == 1.0); 
	return pathtest; 
}


/*===========================================================
    Player Item Class[玩家]
===========================================================*/
bool __fastcall _Player::isEnemy()
{
	bool m_bEnemy=false;
	if (me.m_Team==2 && m_Team==1) m_bEnemy=true;
	if (me.m_Team==1 && m_Team==2) m_bEnemy=true;
	return m_bEnemy;
}
//===========================================================
vec3_t _Player::SuspectNextOrigin()
{
	float absFraction = (float) ClientTime::current - dw_SoundTime;
	vec3_t pflOutOrigin;
	pflOutOrigin[0] = f_SoundPos[0] + (f_SoundOffset[0] * absFraction);
	pflOutOrigin[1] = f_SoundPos[1] + (f_SoundOffset[1] * absFraction);
	pflOutOrigin[2] = f_SoundPos[2] + (f_SoundOffset[2] * absFraction); 

	if (fUpdatedTime()>0.4) Clear();

	return pflOutOrigin;
}

//===========================================================
void _Player::Quick_Update( const float*const origin )// call this func in the soundhook 
{		
	float absFraction = (float)(ClientTime::current - dw_SoundTime);
	dw_SoundTime = (DWORD)ClientTime::current;

	f_SoundOffset[0] = (origin[0] - f_SoundPos[0]) / absFraction;
	f_SoundOffset[1] = (origin[1] - f_SoundPos[1]) / absFraction;
	f_SoundOffset[2] = (origin[2] - f_SoundPos[2]) / absFraction;	

	VectorCopy(flLastSoundpos,f_SoundPos);
	VectorCopy(origin,f_SoundPos);

	if(m_Alive)
	{
		//if( m_lastUpdateType!=UPDATE_ADDENT || timeSinceLastUpdate()>0.3 )
		//{
			m_State     = enumPS_Sound;  
			m_StateTime = ClientTime::current; 
			VectorCopy(origin,m_origin);
		//}
	}
}

//===========================================================
void _Player::AddEntity(vec3_t neworg)
{ 
	if(m_Alive)
	{
		m_State     = enumPS_Entity; 
		m_StateTime = ClientTime::current; 
		VectorCopy(neworg,m_origin); 
	} 
}
//===========================================================
void _Player::CalcHitbox()
{
	if( !bGotHead )
	{
		vec3_t vMin, vMax, tmp_Bones, tmp_Hitbox;
		int i=0;
		model_s		    *pModel         = oEngStudio.SetupPlayerModel( m_Index );
		studiohdr_t     *pStudioHeader  = ( studiohdr_t* )oEngStudio.Mod_Extradata( pModel );
		mstudiobbox_t   *pStudioBox     = ( mstudiobbox_t* )( ( byte* )pStudioHeader + pStudioHeader->hitboxindex );
		TransformMatrix *pBoneTransform = ( TransformMatrix* )oEngStudio.StudioGetBoneTransform();	
		
		me.iMaxBones    = pStudioHeader->numbones-1;
		me.iMaxHitboxes = pStudioHeader->numhitboxes-1;

		for (i=0; i<pStudioHeader->numbones; i++)
		{
			tmp_Bones[0] = (*pBoneTransform)[i][0][3];
			tmp_Bones[1] = (*pBoneTransform)[i][1][3];
			tmp_Bones[2] = (*pBoneTransform)[i][2][3];
			VectorCopy(tmp_Bones,vBones[i]); 
			//CalcOffsets(tmp_Bones, getEnt()->origin, vBones[i]);
		}

		//Head 11 bone 7 | Low Head 9 bone 5 | Chest 8 bone 4 | Stomach 7 bone 3
		for(i=0; i<pStudioHeader->numhitboxes; i++)
		{
			VectorTransform(pStudioBox[i].bbmin, (*pBoneTransform)[pStudioBox[i].bone], vMin);
			VectorTransform(pStudioBox[i].bbmax, (*pBoneTransform)[pStudioBox[i].bone], vMax);
			tmp_Hitbox    = ( vMin + vMax ) * 0.5f;
            VectorCopy(tmp_Hitbox, vAimPosi[i]);
			CalcOffsets(tmp_Hitbox, getEnt()->origin, vHitbox[i]);
			bGotHead = true;
		}
	}
}
void _Player::DrawHitBox(ColorEntry *color2)
{
	if ( !m_Alive ) return;
	if ( !isEnemy() ) return;

	int boxSize =1, text_distbone = 1;

	vec3_t Ent_Angles = getEnt()->angles;
	
	if (m_FirstKill) { color2 = colorList.get(8); }

	if ( isUpdatedAddEnt() ){ //
		if (bGotHead && !m_Visible)
		{
			vec3_t _Up, _Right, _Forward, _PlayerAngles;

			_PlayerAngles[0]=0;
			_PlayerAngles[1]=Ent_Angles[1];
			_PlayerAngles[2]=0;
			oEngfuncs.pfnAngleVectors (_PlayerAngles, _Forward, _Right, _Up);
			_Forward[2]=-_Forward[2];

			//draw line
			//back bone
			DrawVectorLine(vBones[1] ,vBones[2] ,text_distbone,color2);
			DrawVectorLine(vBones[2] ,vBones[3] ,text_distbone,color2);
			DrawVectorLine(vBones[3] ,vBones[4] ,text_distbone,color2);
			DrawVectorLine(vBones[4] ,vBones[5] ,text_distbone,color2);
			DrawVectorLine(vBones[5] ,vBones[6] ,text_distbone,color2);
			DrawVectorLine(vBones[6] ,vBones[7] ,text_distbone,color2);
			DrawVectorLine(vBones[7] ,vBones[8] ,text_distbone,color2);
			//left arm
			DrawVectorLine(vBones[6] ,vBones[9] ,text_distbone,color2);
			DrawVectorLine(vBones[9] ,vBones[10],text_distbone,color2);
			DrawVectorLine(vBones[10],vBones[23],text_distbone,color2);
			DrawVectorLine(vBones[23],vBones[22],text_distbone,color2);
			DrawVectorLine(vBones[22],vBones[21],text_distbone,color2);
			DrawVectorLine(vBones[21],vBones[11],text_distbone,color2);
			DrawVectorLine(vBones[11],vBones[20],text_distbone,color2);
			DrawVectorLine(vBones[20],vBones[19],text_distbone,color2);
			DrawVectorLine(vBones[19],vBones[18],text_distbone,color2);
			DrawVectorLine(vBones[18],vBones[12],text_distbone,color2);
			DrawVectorLine(vBones[12],vBones[13],text_distbone,color2);
			DrawVectorLine(vBones[13],vBones[14],text_distbone,color2);
			DrawVectorLine(vBones[12],vBones[15],text_distbone,color2);
			DrawVectorLine(vBones[15],vBones[16],text_distbone,color2);
			DrawVectorLine(vBones[16],vBones[17],text_distbone,color2);
			//right arm
			DrawVectorLine(vBones[6] ,vBones[24],text_distbone,color2);
			DrawVectorLine(vBones[24],vBones[25],text_distbone,color2);
			DrawVectorLine(vBones[25],vBones[26],text_distbone,color2);
			DrawVectorLine(vBones[26],vBones[27],text_distbone,color2);
			DrawVectorLine(vBones[27],vBones[28],text_distbone,color2);
			DrawVectorLine(vBones[27],vBones[29],text_distbone,color2);
			DrawVectorLine(vBones[27],vBones[30],text_distbone,color2);
			DrawVectorLine(vBones[27],vBones[31],text_distbone,color2);
			DrawVectorLine(vBones[31],vBones[32],text_distbone,color2);
			DrawVectorLine(vBones[32],vBones[33],text_distbone,color2);
			DrawVectorLine(vBones[33],vBones[34],text_distbone,color2);
			DrawVectorLine(vBones[34],vBones[35],text_distbone,color2);
			DrawVectorLine(vBones[35],vBones[36],text_distbone,color2);
			//waistline
			DrawVectorLine(vBones[37],vBones[37],text_distbone,color2);
			DrawVectorLine(vBones[37],vBones[38],text_distbone,color2);
			DrawVectorLine(vBones[1] ,vBones[39],text_distbone,color2);
			DrawVectorLine(vBones[46],vBones[46],text_distbone,color2);
			DrawVectorLine(vBones[48],vBones[48],text_distbone,color2);
			//left foot
			DrawVectorLine(vBones[39],vBones[40],text_distbone,color2);
			DrawVectorLine(vBones[40],vBones[41],text_distbone,color2);
			DrawVectorLine(vBones[41],vBones[42],text_distbone,color2);
			DrawVectorLine(vBones[42],vBones[43],text_distbone,color2);
			DrawVectorLine(vBones[43],vBones[44],text_distbone,color2);
			DrawVectorLine(vBones[44],vBones[45],text_distbone,color2);
			//right foot
			DrawVectorLine(vBones[46],vBones[47],text_distbone,color2);
			DrawVectorLine(vBones[48],vBones[49],text_distbone,color2);
			DrawVectorLine(vBones[49],vBones[50],text_distbone,color2);
			DrawVectorLine(vBones[50],vBones[51],text_distbone,color2);
		//	DrawVectorLine(vBones[51],vBones[52],text_distbone,color2);
		//	DrawVectorLine(vBones[52],vBones[53],text_distbone,color2);//bug
		//	DrawVectorLine(vBones[53],vBones[54],text_distbone,color2);//not wanted
		}
	}
	
	if ( isUpdated() ){
		vec3_t EntViewOrg;
		float vecScreen[2];
		VectorCopy(m_origin,EntViewOrg);
		EntViewOrg = EntViewOrg + vHitbox[11];

		if( !CalcScreen(EntViewOrg, vecScreen) ) { return; }
		DrawBox(vecScreen[0]-boxSize/2, vecScreen[1]-boxSize/2, color2->r,color2->g,color2->b,234, boxSize);
		
		/*if (ax == Aimbot.iTarget){
			char Temp[255];
			sprintf( Temp,"Fov:%0.2f;  Hitbox:%i", m_Fovangle, m_AutoHitbox);
			DrawConsoleStringEx(vecScreen[0]+10, vecScreen[1]-10, color2->r,color2->g,color2->b,Temp);
		}*/
	}
}
//===========================================================
void _Player::CalcViewAngles()
{
	if (!(0<=m_AutoHitbox && m_AutoHitbox<=me.iMaxHitboxes)) return;

	float view[3];
	vec3_t EntViewOrg;
	VectorCopy(m_origin,EntViewOrg);
	EntViewOrg = EntViewOrg + vHitbox[m_AutoHitbox];

	view[0] = EntViewOrg[0] - me.pmEyePos[0];
	view[1] = EntViewOrg[1] - me.pmEyePos[1];
	view[2] = EntViewOrg[2] - me.pmEyePos[2];

	VectorAngles(view,m_AimbotAngles);
	m_AimbotAngles[0] *= -1;

	if (m_AimbotAngles[0]>180) m_AimbotAngles[0]-=360;
	if (m_AimbotAngles[1]>180) m_AimbotAngles[1]-=360;
}
//===========================================================
//===========================================================
float _Player::CalcFovAngle(int hBox)
{
	float view[3];
	vec3_t EntViewOrg, _offsets;
	VectorCopy(m_origin,EntViewOrg);

	VectorCopy(vHitbox[hBox], _offsets);

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

bool _Player::TargetRegion()
{
	vec3_t EntViewSrc,EntViewOrg;
	VectorCopy(m_origin,EntViewSrc);

	//Head 11 bone 7 | Low Head 9 bone 5 | Chest 8 bone 4 | Stomach 7 bone 3
    int i, _ackHitbox, ackHitbox = -1;
    float _ackFov, ackFov = 360.0f;
    _ackHitbox = ackHitbox;
    _ackFov    = ackFov;
	
    for (i=me.iMaxHitboxes; i>0; i--){
        VectorCopy(EntViewSrc + vHitbox[i],EntViewOrg);
        if(pathFree(me.pmEyePos,EntViewOrg)){
            ackHitbox = i;
            ackFov    = CalcFovAngle(i);
            if (ackFov<_ackFov){
                _ackHitbox = i;
                _ackFov   = ackFov;
            }
        }
    }

    m_AutoHitbox = _ackHitbox;
    m_Fovangle   = _ackFov;
    return (ackHitbox!=-1);

	return false;
}
void _Player::calcFovangleAndVisibility()
{
	float PlyFov = 0.0f, DistFov=0.0f; //1=10 , 2=30 , 3=90 , 4=360 , 5=1

	if      (cvar.fov == 1) PlyFov = 1.0f;
	else if (cvar.fov == 2) PlyFov = 2.0f;
	else if (cvar.fov == 3) PlyFov = 4.0f;
	else if (cvar.fov == 4) PlyFov = 8.0f;
	else if (cvar.fov == 5) PlyFov = 16.0f;

	if ( me.zGuns.IsRecoil() ) PlyFov+=1.2f;
	if ( me.zGuns.IsSecond() ) PlyFov+=0.5f;
	//if ( me.zGuns.IsPower() )  PlyFov+=0.5f;

	DistFov = m_Distance/3000.0f;
	PlyFov-=DistFov;

	if ( PlyFov<0.768f ) PlyFov=0.768f;

//fov=360;
	m_Visible = TargetRegion();
	
	if (m_FirstKill) {
		m_AutoHitbox = 11;
		m_Fovangle  = CalcFovAngle(11);
	}

	if (   !isEnemy()
		|| m_Fovangle>PlyFov
		|| ( 0<cvar.disaim && m_Distance < cvar.disaim )
		|| !isAlive()
		){
		m_CalcOk = false; 
	}else{
		m_CalcOk = m_Visible;
	}
}
//===========================================================
//===========================================================
void _Player::Glows()
{
	if (!isAlive() || !isEnemy() ) return;

	ColorEntry* color;
	if (m_Team == 1)	  { color = colorList.get(2); }
	else if (m_Team == 2) { color = colorList.get(3); }
	else				  { color = colorList.get(8); }
	getEnt()->curstate.renderamt = 9;
	getEnt()->curstate.rendercolor.r= color->r;
	getEnt()->curstate.rendercolor.g= color->g;
	getEnt()->curstate.rendercolor.b= color->b;
	getEnt()->curstate.renderfx |= kRenderFxGlowShell;
}

//===========================================================
void _Player::Refresh()
{	
	hud_player_info_t entinfo;
	oEngfuncs.pfnGetPlayerInfo(m_Index, &entinfo);

	if( !entinfo.name ) {
		entinfo.name = "";
		m_Name[0] = '\0';
	} else {
		int len = 31;
		strncpy(m_Name,entinfo.name,len);
		m_Name[len] = 0;
	}

	m_SeqInfo=Cstrike_SequenceInfo[getEnt()->curstate.sequence];

	if(!entinfo.model) { entinfo.model = "unknown model"; }
	
	if( oEngStudio.GetModelByIndex )
	{
		char* newname = gModeName(getEnt()->curstate.weaponmodel);
		register int len = strlen(newname);
		if(len && len<30) { strcpy(m_WeaponName,newname); }
	}
}
/*===========================================================
    Player Container Calss
===========================================================*/
void _Players::UpdateLocalPlayer()
{
	me.m_Index = 0;
	me.m_Entity = oEngfuncs.GetLocalPlayer();

	cl_entity_s *itsme = oEngfuncs.GetLocalPlayer();
	if(itsme)
	{
		me.m_Index = itsme->index;
	}
}

/*===========================================================
	First Kill Function[优先击杀]
===========================================================*/
void __fastcall _Players::FirstKill_Refresh(){
	int nId=1;
	for(int i=0; i<Count(); i++)
	{
		_Player& r = PlyItems[i];
		r.Refresh();
		if( r.isEnemy() && strcmp(r.Name(),"\\missing-name\\") && r.Name()[0]!='\0'){
			r.setFirstKill_List(true);
			nId++;
			PlyKillsId[nId]=i;
		} else {
			r.setFirstKill_List(false);
		}
	}
	FirstKill_Count=nId;
}

void _Players::FirstKill_Show()
{
	if ( !FirstKill_Active ) return;

	enum{ TEXT_H = 20 };

	char cTitle[32], cTitleSub[32], cTitleClear[32], cPlayerName[32];

	sprintf(cTitle,"『宅』◇┣优先击杀┅列表┫◇　　vEr:1.0");
	sprintf(cTitleSub,"功能：开[%.1f]", cvar.FirstKill_Mode);
	sprintf(cTitleClear,"清空列表[%d]", FirstKill_Count-1 );

	int cW = screeninfo.iWidth/2.3;
	int x, y, w, top=30;
	x = displayCenterX - cW/2;
	y = top;
	w = 350;

	int frame_h = (FirstKill_Count+1)*TEXT_H;
	if (frame_h>TEXT_H) frame_h -= TEXT_H;

	oglSubtractive = true;

	DrawWindow(x,y,w,frame_h, cTitle);  //标题

	int nSelId=FirstKill_SelId;

	if ( nSelId==0 ){
		whiteBorder(x+2,y+2,(w-150),TEXT_H-2);
	} else if ( nSelId==1 ){
		whiteBorder(x+2+(w-150),y+2,(w-200)-3,TEXT_H-2);
	} else if ( nSelId>=2 ){
		y += (nSelId-1)*TEXT_H;
		whiteBorder(x+2,y+2,w-3,TEXT_H-2);
	}

	y = top;
	x+=4;
	ColorEntry* clr;
	clr = colorList.get(cvar.FirstKill_Mode); // 1="red" 0="white"
	DrawConsoleStringEx(x,y, clr->r,clr->g,clr->b ,cTitleSub);

	clr = colorList.get(0); // "white"
	DrawConsoleStringEx(x+(w-100),y, clr->r,clr->g,clr->b ,cTitleClear);

	y+=TEXT_H;
	for(int i=2; i<=FirstKill_Count; i++)
	{
		sprintf(cPlayerName,"");
		_Player& r = PlyItems[PlyKillsId[i]];
		if (r.m_Team==1){
			clr = colorList.get(2); // "土匪"
			if ( r.bFirstKill() ){
				sprintf(cPlayerName,"★匪%.2d　%s", i-1, r.Name());
			} else {
				sprintf(cPlayerName,"　匪%.2d　%s", i-1, r.Name());
			}
		} else {
			clr = colorList.get(3); // "警察"
			if ( r.bFirstKill() ){
				sprintf(cPlayerName,"★警%.2d　%s", i-1, r.Name());
			} else {
				sprintf(cPlayerName,"　警%.2d　%s", i-1, r.Name());
			}
		}
		DrawConsoleStringEx(x,y, clr->r,clr->g,clr->b ,cPlayerName);
		if(strstr(cPlayerName,"★")){
			clr = colorList.get(0); // 专杀
			DrawConsoleStringEx(x+w-45,y, clr->r,clr->g,clr->b ,"优先");
		}
		y+=TEXT_H;
	}
	oglSubtractive = false;
}

int _Players::FirstKill_KeyEvent(int keynum)
{
	if ( keynum == cvar.KillKey ) //Del 打开/关闭菜单
	{
		FirstKill_Active=!FirstKill_Active;
		return 0;
	}

	if (FirstKill_Active){
		if ( keynum == 128 || keynum == 240 ) //uparrow || mwheelup
		{
			FirstKill_SelId--;
			if (FirstKill_SelId<0) { FirstKill_SelId=FirstKill_Count; }
			return 0;
		}
		else if ( keynum == 129 || keynum == 239 ) //downarrow || mwheeldown
		{
			FirstKill_SelId++;
			if (FirstKill_SelId>FirstKill_Count) { FirstKill_SelId=0; }
			return 0;
		}
		else if ( keynum == 130 || keynum == 241 ) //leftarrow || leftbutton(mouse1)
		{
			if (FirstKill_SelId>=2){
				_Player& r = PlyItems[PlyKillsId[FirstKill_SelId]];
				r.setFirstKill();
			} else {
				if ( FirstKill_SelId==0 ) {
					if (cvar.FirstKill_Mode!=0){
						cvar.FirstKill_Mode=0;
					} else {
						cvar.FirstKill_Mode=1;
					}
				} else {
					FirstKill_Clear();
				}
			}
			return 0;
		}
		/*
		else if ( keynum == 131 || keynum == 242 ) //rightarrow || rightbutton(mouse2)
		{
			if (FirstKill_SelId>=2){
				_Player& r = PlyItems[PlyKillsId[FirstKill_SelId]];
				r.setFirstKill();
			} else {
				if ( FirstKill_SelId==0 ) {
					cvar.id_mode++;
					if (cvar.id_mode!=1) cvar.id_mode=0;
				} else {
					FirstKill_Clear();
				}
			}
			return 0;
		}*/
	}
	return 1;
}
/*===========================================================
	Aimbot Function
===========================================================*/
int _Players::Aimbot_KeyEvents(int eventcode){
	if ( eventcode==1 ){
		Now_Tick=GetTickCount();
		if( !PressLMB ){
			PressLMB    = true;
			CanAimbot   = true;
			inRecoil    = false;
			cvar.recoil = 0;
			Timer_Tick  = Now_Tick;
			if( !bWait )
			{
				//if (me.zGuns.IsPower()) { oEngfuncs.pfnClientCmd("wait"); }
				bWait = true;
			}
			return 0;
		}

		if( !inRecoil && (Now_Tick-Timer_Tick)>=99 ) {
			inRecoil    = me.zGuns.IsRecoil();
			Now_ckTick  = Now_Tick;
		}

		if((Now_Tick-Timer_Tick)<=me.zGuns.AimbotTimer() ) { return 0; }
		CanAimbot   = false;
		if ( inRecoil ){
			if ( (Now_Tick-Now_ckTick)<=me.zGuns.RecoilTimer() )
				cvar.recoil = me.zGuns.Recoil();
			else
				cvar.recoil = 0;
		}
		//if ((Now_Tick-Timer_Tick)>=1000) { PressLMB=false; bWait=false;}
		return 0;
	}

	CanAimbot   = false;
	cvar.recoil = 0;
	bWait       = false;
	PressLMB    = false;
	inRecoil    = false;

	return 0;
}
//===========================================================
void _Players::doAimbot(struct usercmd_s *uCMD){
	if ( CanAimbot )
	if ( me.InAttack() )
	if ( iTarget!=-1 )
	{
        float fMyCmdAngles[3], fAimAngles[3], fSmoothAngles[3];

        VectorCopy(uCMD->viewangles,fMyCmdAngles);

        _Player& r = PlyItems[iTarget];
        cl_entity_s* pEnt=r.getEnt();

        if( r.isAlive() && pEnt != NULL)
        {
            Vector vAngles, vF, vR, vU, fInOut;
			int nAimId=11;
            vAngles=Vector(0.0f,pEnt->angles[1],0.0f);
            oEngfuncs.pfnAngleVectors(vAngles,vF,vR,vU);

			nAimId = (int)r.m_AutoHitbox;
			
			VectorCopy(r.vAimPosi[nAimId],fInOut);

            float *forward=(fInOut-me.pmEyePos);
            float tmp, yaw, pitch;

            if (forward[1] == 0 && forward[0] == 0){
                yaw = 0;
                if (forward[2] > 0)
                    pitch = 270;
                else
                    pitch = 90;
            } else {
                yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
                if (yaw < 0) yaw += 360;

                tmp = sqrt (forward[0]*forward[0] + forward[1]*forward[1]);
                pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
                if (pitch < 0) pitch += 360;
            }

            fAimAngles[0] = pitch;
            fAimAngles[1] = yaw;
            fAimAngles[2] = 0;
            while(fAimAngles[0]<-89) {fAimAngles[0]+=180;fAimAngles[1]+=180;}
            while(fAimAngles[0]>89)  {fAimAngles[0]-=180;fAimAngles[1]+=180;}
            while(fAimAngles[1]<-180){fAimAngles[1]+=360;}
            while(fAimAngles[1]>180) {fAimAngles[1]-=360;}

            //SmoothAimAngles(fMyCmdAngles,fAimAngles,fSmoothAngles,1.0f);
            VectorSubtract(fAimAngles, fMyCmdAngles, fSmoothAngles);
            if(fSmoothAngles[1]>180.0f)  {fSmoothAngles[1] -= 360.0f;}
            if(fSmoothAngles[1]<-180.0f) {fSmoothAngles[1] += 360.0f;}

            fSmoothAngles[0] = fSmoothAngles[0] / 1.0f + fMyCmdAngles[0];
            fSmoothAngles[1] = fSmoothAngles[1] / 1.0f + fMyCmdAngles[1];

            if(fSmoothAngles[1]>360.0f) {fSmoothAngles[1] -= 360.0f;}
            if(fSmoothAngles[1]<0.0f)   {fSmoothAngles[1] += 360.0f;}

            //ApplySilentAngles(fSmoothAngles,uCMD);
            Vector _oF, _oR, _oU, _aF, _aR, _aU, vTemp;
            float _nF, _nR, _nU;
            float _vF = uCMD->forwardmove;
            float _vR = uCMD->sidemove;
            float _vU = uCMD->upmove;

            if(me.m_Entity->curstate.movetype == MOVETYPE_WALK){
                oEngfuncs.pfnAngleVectors(Vector(0.0f, uCMD->viewangles.y, 0.0f), _oF, _oR, _oU);
            }else{
                oEngfuncs.pfnAngleVectors(uCMD->viewangles, _oF, _oR, _oU);
            }

            int iHasShiftHeld = GetAsyncKeyState(VK_LSHIFT);
            if(me.m_Entity->curstate.movetype == MOVETYPE_WALK && !iHasShiftHeld && !(me.pmMoveType == 5)){
                uCMD->viewangles.x = fSmoothAngles[0];
                uCMD->viewangles.y = fSmoothAngles[1];    
            }

            if(me.m_Entity->curstate.movetype == MOVETYPE_WALK){
                oEngfuncs.pfnAngleVectors(Vector(0.0f, uCMD->viewangles.y, 0.0f), _aF, _aR, _aU);
            }else{
                oEngfuncs.pfnAngleVectors(uCMD->viewangles, _aF, _aR, _aU);
            }

            _nF = DotProduct(_vF * _oF.Normalize(), _aF) + DotProduct(_vR * _oR.Normalize(), _aF) + DotProduct(_vU * _oU.Normalize(), _aF);
            _nR = DotProduct(_vF * _oF.Normalize(), _aR) + DotProduct(_vR * _oR.Normalize(), _aR) + DotProduct(_vU * _oU.Normalize(), _aR);
            _nU = DotProduct(_vF * _oF.Normalize(), _aU) + DotProduct(_vR * _oR.Normalize(), _aU) + DotProduct(_vU * _oU.Normalize(), _aU);
            
            if (fSmoothAngles[0]>81 ){
                uCMD->forwardmove = -_nF;
            } else {
                uCMD->forwardmove = _nF;
            }
            uCMD->sidemove = _nR;
            uCMD->upmove   = _nU;

            oEngfuncs.SetViewAngles((float *)fSmoothAngles);
            r.m_CalcOk=false;
        }
    }
}
//===========================================================
void __fastcall _Players::FindTarget()
{
	int actTarget = -1;
	iTarget       = actTarget;
	if ( !me.isAlive() ) return;

	for (int ax=0;ax<Count();ax++)
	{
		if( PlyItems[ax].isUpdatedAddEnt() && PlyItems[ax].m_CalcOk )
		{
			//id_mode==1 先杀视野中的专杀对象
			if ( cvar.FirstKill_Mode==1 && PlyItems[ax].bFirstKill() )     { PlyItems[ax].m_Fovangle -=2.5f; }
			if( iTarget==-1 )                                              { actTarget=ax; continue; }
			if( PlyItems[ax].m_Fovangle < PlyItems[actTarget].m_Fovangle ) { actTarget=ax; }
		}
	}
	iTarget = actTarget;
	if( cvar.FirstKill_Mode==1 && iTarget!=-1 ) {
		bToAttack=PlyItems[actTarget].bFirstKill();
	}
}

//===========================================================
void __fastcall _Players::DrawHitBox(){
	if ( !me.isAlive() ) return;
	for(int i=0;i<Count();i++) {
		PlyItems[i].DrawHitBox(Color(i));
	}
}

//===========================================================
void _Players::SetViewAngles(float *toAngles)
{
	if ( CanAimbot )
	if ( me.InAttack() )
	if ( iTarget!=-1 )
	{
		PlyItems[iTarget].CalcViewAngles();
		VectorCopy(PlyItems[iTarget].m_AimbotAngles,toAngles);
	}
}
//===========================================================
void _Players::SetViewAngles(float *toAnglesA, float *toAnglesB)
{
	if ( CanAimbot )
	if ( me.InAttack() )
	if ( iTarget!=-1 )
	{
		PlyItems[iTarget].CalcViewAngles();
		VectorCopy(PlyItems[iTarget].m_AimbotAngles,toAnglesA);
		VectorCopy(PlyItems[iTarget].m_AimbotAngles,toAnglesB);
	}
}
void __fastcall _Players::PistolCombo(struct usercmd_s *uCMD)
{
	/*
	if ( bToAttack && VectorLength(me.vPunchangle) == 0 ){
		uCMD->buttons |= IN_ATTACK;
		bToAttack = false;
	}*/

	if ( (uCMD->buttons&IN_ATTACK) && me.zGuns.IsSecond() && iTarget!=-1)
	{
		static bool flipFlop = true;
		if(flipFlop){ uCMD->buttons |= IN_ATTACK;    }
		else        { uCMD->buttons &= (~IN_ATTACK); }
 
		flipFlop = !flipFlop;
	}
}
/*===========================================================
	Guns Class[枪支]
===========================================================*/
void _zHAiGuns::SetDatas(struct Guns_Types *ItGuns)
{
	switch (ItGuns->Id)
	{
		case Guns_P228:
			ItGuns->ClipCap = ClipCap_P228;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 4096.0f;
			ItGuns->wallpierce1 = 0.8f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 32;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_P228;
			ItGuns->recoil = 0.011f;
			ItGuns->recoiltimer = 300.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_SCOUT:
			ItGuns->ClipCap = ClipCap_SCOUT;
			ItGuns->penetrate = WALL_PEN2;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.98f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 75;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_SCOUT;
			ItGuns->recoil = 1.42f;
			ItGuns->recoiltimer = 10.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_HEGRENADE:
			ItGuns->ClipCap = 0;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 0.0f;
			ItGuns->wallpierce1 = 0.0f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 0;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_NONE;
			ItGuns->recoil = 0.00f;
			ItGuns->recoiltimer = 0.0f;
			ItGuns->aimbottimer = 0.0f;
			break;
		case Guns_XM1014:
			ItGuns->ClipCap = ClipCap_XM1014;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 0.0f;
			ItGuns->wallpierce1 = 0.0f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 0;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_XM1014;
			ItGuns->recoil = 1.00f;
			ItGuns->recoiltimer = 300.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_C4:
			ItGuns->ClipCap = 0;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 0.0f;
			ItGuns->wallpierce1 = 0.0f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 0;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_NONE;
			ItGuns->recoil = 0.00f;
			ItGuns->recoiltimer = 0.0f;
			ItGuns->aimbottimer = 0.0f;
			break;
		case Guns_MAC10:
			ItGuns->ClipCap = ClipCap_MAC10;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.82f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 29;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_MAC10;
			ItGuns->recoil = 1.50f;
			ItGuns->recoiltimer = 300.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_AUG:
			ItGuns->ClipCap = ClipCap_AUG;
			ItGuns->penetrate = WALL_PEN1;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.96f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 32;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_AUG;
			ItGuns->recoil = 1.62f;
			ItGuns->recoiltimer = 300.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_SMOKEGRENADE:
			ItGuns->ClipCap = 0;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 0.0f;
			ItGuns->wallpierce1 = 0.0f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 0;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_NONE;
			ItGuns->recoil = 0.00f;
			ItGuns->recoiltimer = 0.0f;
			ItGuns->aimbottimer = 0.0f;
			break;
		case Guns_ELITE:
			ItGuns->ClipCap = ClipCap_ELITE;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.75f; // left
			ItGuns->wallpierce2 = 0.75f; // right
			ItGuns->damage1 = 20; // left
			ItGuns->damage2 = 27; // right
			ItGuns->bullettype = Bullet_ELITE;
			ItGuns->recoil = 0.029f;
			ItGuns->recoiltimer = 0.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_FIVESEVEN:
			ItGuns->ClipCap = ClipCap_FIVESEVEN;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 4096.0f;
			ItGuns->wallpierce1 = 0.885f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 14;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_FIVESEVEN;
			ItGuns->recoil = 0.0105f;
			ItGuns->recoiltimer = 300.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_UMP45:
			ItGuns->ClipCap = ClipCap_UMP45;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.82f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 30;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_UMP45;
			ItGuns->recoil = 1.60f;
			ItGuns->recoiltimer = 300.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_SG550:
			ItGuns->ClipCap = ClipCap_SG550;
			ItGuns->penetrate = WALL_PEN1;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.98f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 40;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_SG550;
			ItGuns->recoil = 0.10f;
			ItGuns->recoiltimer = 10.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_GALIL:
			ItGuns->ClipCap = ClipCap_GALIL;
			ItGuns->penetrate = WALL_PEN1;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.96f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 33;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_GALIL;
			ItGuns->recoil = 0.85f;
			ItGuns->recoiltimer = 300.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_FAMAS:
			ItGuns->ClipCap = ClipCap_FAMAS;
			ItGuns->penetrate = WALL_PEN1;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.96f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 33;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_FAMAS;
			ItGuns->recoil = 1.65f;
			ItGuns->recoiltimer = 300.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_USP:
			ItGuns->ClipCap = ClipCap_USP;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 4096.0f;
			ItGuns->wallpierce1 = 0.79f; // unsilenced
			ItGuns->wallpierce2 = 0.79f; // silenced
			ItGuns->damage1 = 34; // unsilenced
			ItGuns->damage2 = 30; // silenced
			ItGuns->bullettype = Bullet_USP;
			ItGuns->recoil = 0.015f;
			ItGuns->recoiltimer = 50.0f;
			ItGuns->aimbottimer = 100.0f;
			break;
		case Guns_GLOCK18:
			ItGuns->ClipCap = ClipCap_GLOCK18;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.75f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 20;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_GLOCK18;
			ItGuns->recoil = 0.015f;
			ItGuns->recoiltimer = 50.0f;
			ItGuns->aimbottimer = 100.0f;
			break;
		case Guns_AWP:
			ItGuns->ClipCap = ClipCap_AWP;
			ItGuns->penetrate = WALL_PEN2;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.99f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 115;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_AWP;
			ItGuns->recoil = 0.80f;
			ItGuns->recoiltimer = 10.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_MP5:
			ItGuns->ClipCap = ClipCap_MP5;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.84f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 26;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_MP5;
			ItGuns->recoil = 1.60f;
			ItGuns->recoiltimer = 2000.0f;
			ItGuns->aimbottimer = 300.0f;
			break;
		case Guns_M249:
			ItGuns->ClipCap = ClipCap_M249;
			ItGuns->penetrate = WALL_PEN1;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.97f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 32;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_M249;
			ItGuns->recoil = 1.72f;
			ItGuns->recoiltimer = 300.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_M3:
			ItGuns->ClipCap = ClipCap_M3;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 0.0f;
			ItGuns->wallpierce1 = 0.0f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 0;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_M3;
			ItGuns->recoil = 1.67f;
			ItGuns->recoiltimer = 300.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_M4A1:
			ItGuns->ClipCap = ClipCap_M4A1;
			ItGuns->penetrate = WALL_PEN1;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.97f; // unsilenced
			ItGuns->wallpierce2 = 0.95f; // silenced
			ItGuns->damage1 = 32; // unsilenced
			ItGuns->damage2 = 33; // silenced
			ItGuns->bullettype = Bullet_M4A1;
			ItGuns->recoil = 1.65f;
			ItGuns->recoiltimer = 300.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_TMP:
			ItGuns->ClipCap = ClipCap_TMP;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.85f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 20;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_TMP;
			ItGuns->recoil = 1.50f;
			ItGuns->recoiltimer = 200.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_G3SG1:
			ItGuns->ClipCap = ClipCap_G3SG1;
			ItGuns->penetrate = WALL_PEN1;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.98f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 60;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_G3SG1;
			ItGuns->recoil = 0.20f;
			ItGuns->recoiltimer = 10.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_FLASHBANG:
			ItGuns->ClipCap = 0;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 0.0f;
			ItGuns->wallpierce1 = 0.0f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 0;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_NONE;
			ItGuns->recoil = 0.00f;
			ItGuns->recoiltimer = 0.0f;
			ItGuns->aimbottimer = 0.0f;
			break;
		case Guns_DEAGLE:
			ItGuns->ClipCap = ClipCap_DEAGLE;
			ItGuns->penetrate = WALL_PEN1;
			ItGuns->distance = 4096.0f;
			ItGuns->wallpierce1 = 0.81f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 54;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_DEAGLE;
			ItGuns->recoil = 0.019f;
			ItGuns->recoiltimer = 50.0f;
			ItGuns->aimbottimer = 100.0f;
			break;
		case Guns_SG552:
			ItGuns->ClipCap = ClipCap_SG552;
			ItGuns->penetrate = WALL_PEN1;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.955f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 33;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_SG552;
			ItGuns->recoil = 1.67f;
			ItGuns->recoiltimer = 300.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		case Guns_AK47:
			ItGuns->ClipCap = ClipCap_AK47;
			ItGuns->penetrate = WALL_PEN1;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.98f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 36;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_AK47;
			ItGuns->recoil = 1.923f;
			ItGuns->recoiltimer = 300.0f;
			ItGuns->aimbottimer = 300.0f;
			break;
		case Guns_KNIFE:
			ItGuns->ClipCap = 0;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 0.0f;
			ItGuns->wallpierce1 = 0.0f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 0;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_NONE;
			ItGuns->recoil = 1.50f;
			ItGuns->recoiltimer = 0.0f;
			ItGuns->aimbottimer = 0.0f;
			break;
		case Guns_P90:
			ItGuns->ClipCap = ClipCap_P90;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 8192.0f;
			ItGuns->wallpierce1 = 0.885f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 21;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_P90;
			ItGuns->recoil = 1.50f;
			ItGuns->recoiltimer = 300.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
		default:
			ItGuns->ClipCap = 0;
			ItGuns->penetrate = WALL_PEN0;
			ItGuns->distance = 0.0f;
			ItGuns->wallpierce1 = 0.0f;
			ItGuns->wallpierce2 = 0.0f;
			ItGuns->damage1 = 0;
			ItGuns->damage2 = 0;
			ItGuns->bullettype = Bullet_NONE;
			ItGuns->recoil = 0.302f;
			ItGuns->recoiltimer = 5.0f;
			ItGuns->aimbottimer = 200.0f;
			break;
	}
}
//===========================================================
void _zHAiGuns::MsgAdds(const char *weaponname, int ammo1type, int max1, int ammo2type, int max2, int slot, int slotpos, int id, int flags)
{
	struct Guns_Types dummy;
	int index, len;

	len = strlen(weaponname);
	if (len > MAX_WEAPON_NAME)
		return;
	
	index=-1;
	for (int i=0; i<GunsItems.size(); i++)
	{
		if (GunsItems[i].Id == id) {
			index=i;
			break;
		}
	}

	if (index==-1) {
		GunsItems.push_back(dummy);
		index = GunsItems.size() - 1;
		if (index>MAX_WEAPON_NAME) return;

		strcpy( GunsItems[index].command, weaponname );
		if(strstr(weaponname,"weapon_")==weaponname) { weaponname += 7;}
		strcpy( GunsItems[index].name, weaponname );

		GunsItems[index].AmmoType = ammo1type;
		if (max1 == 255)
			GunsItems[index].Max1 = -1;
		else
			GunsItems[index].Max1 = max1;

		GunsItems[index].Ammo2Type = ammo2type;
		if (max2 == 255)
			GunsItems[index].Max2 = -1;
		else
			GunsItems[index].Max2 = max2;

		GunsItems[index].Slot = slot;
		GunsItems[index].SlotPos = slotpos;

		GunsItems[index].Id = id;
		GunsItems[index].Flags = flags;

		GunsItems[index].CAmmo = 0;
		GunsItems[index].XAmmo = 0;

		GunsItems[index].Active = false;

		SetDatas(&GunsItems[index]);

		memset(&GunsItems[index].Datas, 0, sizeof(weapon_data_t));
	}
}

//===========================================================
void _zHAiGuns::MsgSetAmmo(int isCurs, int Id, int Ammo)
{
	for (int i=0; i<GunsItems.size(); i++)
	{
		if (GunsItems[i].Id == Id) {
			GunsItems[i].Active  = true;
			GunsItems[i].CAmmo  = Ammo;
		} else {
			if ( GunsItems[i].Active ){
				GunsItems[i].Active = false;
			}
		}
	}
}
//===========================================================
void _zHAiGuns::MsgSetAmmoX(int Id, int Count)
{
	for (int i=0; i<GunsItems.size(); i++)
	{
		if (GunsItems[i].AmmoType == Id)
			GunsItems[i].XAmmo = Count;
	}
}

//===========================================================
Guns_Types* _zHAiGuns::CurrentGuns(void)
{
	//if ( !bGoted ){
		for (int i=0; i<GunsItems.size(); i++)
		{
			if ( GunsItems[i].Active ){
				nGotId = i;
	//			bGoted = true;
				return &GunsItems[nGotId];
			}
		}
	//} else {
	//	return &GunsItems[nGotId];
	//}
	return NULL;
}

bool _zHAiGuns::IsPower(void)
{
	Guns_Types *TempIt = CurrentGuns();
	if ( TempIt ){
		int Id = TempIt->Id;
		if (Id == Guns_DEAGLE ||
			Id == Guns_M4A1 ||
			Id == Guns_AK47)
		return true;
	}
	return false;
}
//===========================================================
bool _zHAiGuns::IsRecoil(int Id)
{
	if (   Id == Guns_AUG
		|| Id == Guns_M249
		|| Id == Guns_M4A1
		|| Id == Guns_MP5
		|| Id == Guns_DEAGLE
		|| Id == Guns_SG552
		|| Id == Guns_AK47
		|| Id == Guns_FAMAS
		|| Id == Guns_GALIL
		|| Id == Guns_P90
		)
		return true;

	return false;
}
bool _zHAiGuns::IsRecoil(void)
{
	Guns_Types *TempIt = CurrentGuns();
	if ( TempIt ) 
		return IsRecoil( TempIt->Id );

	return false;
}
//===========================================================
bool _zHAiGuns::IsSecond(void)
{
	Guns_Types *TempIt = CurrentGuns();
	if ( TempIt )
		return (TempIt->Slot == 1);

	return false;
}
//===========================================================
bool _zHAiGuns::InAttack(void)
{
	Guns_Types *TempIt = CurrentGuns();
	if ( TempIt ) {
		if ( TempIt->Id == Guns_KNIFE || 
			 TempIt->Id == Guns_HEGRENADE || 
			 TempIt->Id == Guns_FLASHBANG || 
			 TempIt->Id == Guns_C4 || 
			 TempIt->Id == Guns_SMOKEGRENADE || 
			 TempIt->Datas.m_fInReload
			 // ||TempIt->Datas.m_flNextPrimaryAttack>0
			 )
			return false;
		else 
			return true;
	}
		
	return false;
}

//===========================================================
bool _zHAiGuns::InCrosshair(int WeaponId)
{
	if ( WeaponId == Guns_SG550 ||
		 WeaponId == Guns_G3SG1 ||
		 WeaponId == Guns_SCOUT ||
		 WeaponId == Guns_AWP)
		return true;

	return false;
}
bool _zHAiGuns::InCrosshair(void)
{
	Guns_Types *TempIt = CurrentGuns();
	if ( TempIt ) 
		return InCrosshair( TempIt->Id );

	return false;
}

//===========================================================
void _zHAiGuns::SetState(int Id,weapon_data_t Data){
	Guns_Types *TempIt = CurrentGuns();
	if ( TempIt ) 
	if ( TempIt->Id==Id ){
		TempIt->Datas.m_iClip=Data.m_iClip;
		TempIt->Datas.m_fInZoom=Data.m_fInZoom;
		TempIt->Datas.m_fInReload=Data.m_fInReload;
		TempIt->Datas.m_iWeaponState=Data.m_iWeaponState;
		TempIt->Datas.m_flNextPrimaryAttack=Data.m_flNextPrimaryAttack;
	}
}

bool _zHAiGuns::InReload(void)
{
	Guns_Types *TempIt = CurrentGuns();
	if ( TempIt ) 
	if (TempIt->Datas.m_fInReload)
		return true;

	return false;
}
//===========================================================
void _zHAiGuns::UpdateBits(int wBits)
{
	static int Old_wBits = 0;

	if (wBits != Old_wBits)
	{
		Old_wBits = wBits;
		for (int i=0; i<GunsItems.size(); i++)
		{
			if (wBits & (1 << GunsItems[i].Id))
			{
				GunsItems[i].Active = true;
			}
			else
			{
				GunsItems[i].Active = false;
				GunsItems[i].CAmmo = 0;
				GunsItems[i].Datas.m_fInReload=-1;
			}
		}
	}
}
//===========================================================