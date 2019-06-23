/**********************************************************
◆┅┣ zHA! H00k vEr:5.0 ┫┅◆
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
◆┅┣      (c)2018      ┫┅◆
    File Name: Client.cpp
************************************************************/
#pragma warning (disable:4551)
#pragma warning (disable:4786)
#pragma warning(disable:4800)
#include <list>
#include <time.h>
#include "client.h"
#include "calcscreen.h"
#include "cvars.h"
#include "drawing.h"
#include "color.h"
#include "timehandling.h"
#include "radar.h"
#include "Font.h"
#include "console.h"
#include "DefineCmds.h"
#include "bind.h"
#include "common/net_api.h"
#include "players.h"
#include "menu.h"
#include "cSoundHook.h"
//#include "eventhook.h"
#include "misc\defs.h"
#include "misc\sprites.h"
#include "zEngines.h"
#include "xorstr.h"
#include <gl/gl.h>

//===========================================================
struct cl_enginefuncs_s  oEngfuncs;
struct cl_enginefuncs_s  gHookedEngfuncs;
struct engine_studio_api_s oEngStudio;
struct event_api_s oEventApi;
//extern cl_enginefuncs_s *pEngfuncs;
double * globalTime;
cl_enginefuncs_s    *pEngfuncs  = (cl_enginefuncs_s*)    0x1de5158;;
engine_studio_api_s *pEngStudio = (engine_studio_api_s*) 0x1dff340;
playermove_s        *pPMove     = (playermove_s*)        0x2f7d2a0;
DWORD *ltfxslots;
DWORD ltfx_loc=0x01DE52F0;
DWORD *RetartPtr=(DWORD*)0x2F7D1E0;
DWORD *tempslots=(DWORD*)0x01700000; 
DWORD *mempatcher=0;
//===========================================================
SCREENINFO screeninfo;
char hackdir[256] = "", DllCfg[256]="", ExeCfg[256]="";
extern bool oglSubtractive;
//===========================================================
// 85/86 == Grenade Throw with shield
// 91/92 == Shoot with shield
// 89/93/94/95/96 == Shield to side
// 97/98 == Shield in front

// esp Info Structure
struct esp_info
{
	esp_info() { memset(this,0,sizeof(*this)); }

	char        name[16];
	float       origin[3];
	bool        blink;
	ColorEntry* color;
};
// esp Info vector
enum { MAX_ESPINFO=32 };
static esp_info vEspInfo[MAX_ESPINFO];
static int      espInfoCount=0;

// Blinking for Radar/Esp
StopTimer blinkTimer;
bool blinkPhase = false;

// Bomb Timer Stuff
bool bBombIsPlanted=false;
float fBombPlantedTime=0;
int iC4Timer=0;

static char currentMap[100];
// Scheduled Commands (later x blabla)
struct ScheduledCommand
{
	string           cmd;
	RunningStopTimer timer;
};
typedef list<ScheduledCommand> ScheduleList;
ScheduleList scheduleList;

float chaseofs = 0;

#define BOUND_VALUE(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}

//===========================================================
bool __fastcall CalcScreen(float *origin, float *vecScreen)
{
	if(!cvar.chase_cam)
	{
		return NewCalcScreen(origin, vecScreen);
	}
	else if(cvar.chase_cam)
	{
		int cResult = 0;
		if( oEngfuncs.GetLocalPlayer() != NULL )
			cResult = oEngfuncs.pTriAPI->WorldToScreen( origin, vecScreen );
		if( vecScreen[0] < 1 && vecScreen[1] < 1 && vecScreen[0] > -1 && vecScreen [1] > -1 && !cResult )
		{
			vecScreen[0] = vecScreen[0] * displayCenterX + displayCenterX;
			vecScreen[1] = -vecScreen[1] * displayCenterY + displayCenterY;
			return true;
		}
		return NULL;
	}
	return NULL;
}

//===========================================================
void exec(const char* filename)
{
	string file = getHackDirFile(filename);
	defCMD.execFile( file.c_str() );
}
//===========================================================


//===========================================================
void drawRadarPoint(float* origin,int r,int g,int b,int a,bool blink=false,int boxsize=5)
{
	int screenx;
	int screeny;

	if (cvar.radar == 2 && mapLoaded)
	{
		overview_calcRadarPoint(origin, &screenx, &screeny);
		if ( screenx>(cvar.radar_x+cvar.radar_size-boxsize-1)) screenx = cvar.radar_x+cvar.radar_size-boxsize-1;
		else if (screenx<(cvar.radar_x-cvar.radar_size-1)) screenx = cvar.radar_x-cvar.radar_size-1;

		if (screeny>(cvar.radar_y+cvar.radar_size-boxsize-1)) screeny = cvar.radar_y+cvar.radar_size-boxsize-1;
		else if (screeny<(cvar.radar_y-cvar.radar_size-1)) screeny = cvar.radar_y-cvar.radar_size-1;
	}
	else
	{
		float dx  = origin[0] - me.pmEyePos[0];
		float dy  = origin[1] - me.pmEyePos[1];

		float yaw = mainViewAngles[1]*(3.1415/180.0);
		me.sin_yaw       =  sin(yaw); 
		me.minus_cos_yaw = -cos(yaw);

		// rotate
		float x =  dy*me.minus_cos_yaw + dx*me.sin_yaw;
		float y =  dx*me.minus_cos_yaw - dy*me.sin_yaw;

		float range = 2500.0f;
		if(fabs(x)>range || fabs(y)>range)
		{ 
			// clipping
			if(y>x)
			{
				if(y>-x) {
					x = range*x/y;
					y = range;
				}  else  {
					y = -range*y/x; 
					x = -range; 
				}
			} else {
				if(y>-x) {
					y = range*y/x; 
					x = range; 
				}  else  {
					x = -range*x/y;
					y = -range;
				}
			}
		}
		screenx = cvar.radar_x+int(x/range*float(cvar.radar_size));
		screeny = cvar.radar_y+int(y/range*float(cvar.radar_size));		
	}
		
	if(!blink || blinkPhase)
	{ 
		oglSubtractive = true;
		oEngfuncs.pfnFillRGBA(screenx-2,screeny-2,5,5,r,g,b,255);
		oEngfuncs.pfnFillRGBA(screenx-1,screeny-1,3,3,255,255,0,255);
		oglSubtractive = false;
		//oEngfuncs.pfnFillRGBA(screenx-1,screeny-1,boxsize,boxsize,r,g,b,a);
		//blackBorder(screenx-1,screeny-1,boxsize,boxsize+1);
	}
}

//===========================================================
void CL_CreateMove(float frametime, struct usercmd_s *cmd, int active)
{	
	zHAi15.pCL_CreateMove(frametime,cmd,active);

	if ( zHAi.bGameActive ) { vPlayers.FirstKill_Refresh(); } //Refresh First Kill [刷新]

	//oGL_ClearBackground(); //截图黑屏

	if( me.isAlive() )
	{
		//Handle HLH AIMING
		if ( (cmd->buttons&IN_ATTACK) ){
			vPlayers.Aimbot_KeyEvents(1);
		} else {
			vPlayers.Aimbot_KeyEvents(0);
		}

		//if ( me.zGuns.IsPower() ) 
			vPlayers.doAimbot(cmd);
		//else
		//	vPlayers.SetViewAngles(cmd->viewangles);
		
		// 自动开枪
		vPlayers.PistolCombo(cmd);

		/*if ( (cmd->buttons&IN_ATTACK) && cvar.recoil>0.0f ){
			float offset[3];
			me.zGuns.GetSpreadOffset(1, cmd->viewangles, me.pmVelocity, offset);
			cmd->viewangles[0] += offset[0];
			cmd->viewangles[1] += offset[1];
	//		cmd->viewangles[2] += offset[2];
		}*/
/*
		if( cvar.recoil>0.0f )
		{
			cmd->viewangles[0] -= (me.vPunchangle[0] * cvar.recoil);
			cmd->viewangles[1] -= (me.vPunchangle[1] * cvar.recoil);
		}
*/
		if( !(me.pmFlags&(FL_ONGROUND|FL_INWATER)) && me.pmGroundSpeed && GetAsyncKeyState(0x46)) //F键
		{
			static bool bStrafing=false;
			double strafe_speed=0;
			if(me.pmAirAccelerate==10)
				strafe_speed=18.3487423787845434132324324;//18.348742;
			else if(me.pmAirAccelerate==100)
				strafe_speed=17.75;
			else
				strafe_speed=17.92;
			cmd->forwardmove=strafe_speed*me.pmMaxSpeed/me.pmGroundSpeed;
			if(bStrafing)
				cmd->sidemove=-me.pmMaxSpeed;
			else
				cmd->sidemove=+me.pmMaxSpeed;
			bStrafing=!bStrafing;
		}

		//if ((cmd->buttons & IN_JUMP) && !(me.pmFlags & FL_ONGROUND)) cmd->buttons &= ~IN_JUMP;

		if( GetAsyncKeyState(0x46) ) //F键
		{
			if (me.pmFlags & FL_ONGROUND) cmd->buttons |= IN_DUCK;
			if (((me.pmFlags & FL_ONGROUND) && (me.pmInDuck)) || !(me.pmFlags & FL_ONGROUND)) cmd->buttons &= ~IN_DUCK;
		}
	}
}
//===========================================================
void PreV_CalcRefdef(struct ref_params_s *pparams)
{
	VectorCopy(pparams->punchangle,me.vPunchangle);	
	VectorCopy(pparams->forward,me.vForward);

	//if ( cvar.recoil>0.0f ) for (int i=0; i<3; i++) pparams->punchangle[i] = 0;

	if( pparams->nextView == 0 )
	{
		//if ( !me.zGuns.IsPower() ) {
		//	vPlayers.SetViewAngles(pparams->cl_viewangles, pparams->viewangles);
			//ss_cl_viewangles=(DWORD)pparams->cl_viewangles;
			//ss_viewangles=(DWORD)pparams->viewangles;
		//}
		for(int i=0;i<vPlayers.Count();i++) 
		{
			_Player& p = vPlayers[i];
			p.SuspectNextOrigin();
		}
    }
}

void PostV_CalcRefdef(struct ref_params_s *pparams)
{
	if( pparams->nextView == 0 )
	{
		// Primary Screen, below all other Viewports Sent
		// update vectors for CalcScreen
		//VectorCopy(pparams->cl_viewangles,f_cl_viewangles);
		VectorCopy(pparams->viewangles,mainViewAngles);
		VectorCopy(pparams->vieworg,mainViewOrigin);		

		if(chaseofs)
		{
			vec3_t vecEnd, up, right, forward, EntViewOrg;
			oEngfuncs.pfnAngleVectors (mainViewAngles, forward, right, up);

			mainViewOrigin[0] += chaseofs*forward[0];
			mainViewOrigin[1] += chaseofs*forward[1];
			mainViewOrigin[2] -= chaseofs*forward[2];
		}
	}
}
void V_CalcRefdef (struct ref_params_s *pparams)
{
	PreV_CalcRefdef(pparams);
	//if (pparams->nextView == 0 || !me.isAlive() ){
	    zHAi15.pV_CalcRefdef(pparams);
	//}
	PostV_CalcRefdef(pparams);
}
//===========================================================
int HUD_AddEntity(int type, struct cl_entity_s *ent, const char *modelname)
{
	if(0){}
	else if( ent->player)
	{ 
		AddEntityPlayer(ent);
	}
/*      
	char* pos;
	// 枪支瞄准方向
		ColorEntry *pClr = PlayerColor(ent->index);
		vec3_t vecBegin,vecEnd,up,right,forward;
		VectorCopy(ent->origin,vecBegin);
		if(ent->curstate.usehull)
			vecBegin[2]+=12;
		else
			vecBegin[2]+=17;
		oEngfuncs.pfnAngleVectors(ent->angles,forward,right,up);
		forward[2]=-forward[2];
		vecBegin[0]+=forward[0]*10;
		vecBegin[1]+=forward[1]*10;
		vecBegin[2]+=forward[2]*10;
		vecEnd=vecBegin+forward*999;
		// 血色
		int beamindex = 0;
		if(!beamindex)
			beamindex = oEngfuncs.pEventAPI->EV_FindModelIndex("sprites/laserbeam.spr");
		oEngfuncs.pEfxAPI->R_BeamPoints(vecBegin,vecEnd,beamindex,0.001f,1.2f,0,32,2,0,0,pClr->r/255,pClr->g/255,pClr->b/255);
*/
	int nResult = 1;
	nResult = zHAi15.pHUD_AddEntity(type,ent,modelname);
	return nResult;
}
//===========================================================
void HUD_Frame( double time )
{
	zHAi.iUpdateCurrent = timeGetTime();
	if(zHAi.iUpdateCurrent - zHAi.iUpdateLast >= 300)
		zHAi.bGameActive = false;

	Snapshot();
/*
	// 闪光过滤
	if (!zHAi.bShotMaked && zHAi.bGameActive && cvar.floke)
	{
		static screenfade_t sf = { 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0 }; 
		oEngfuncs.pfnSetScreenFade( &sf ); 
	}
*/
	zHAi15.pHUD_Frame(time);
}
//==========================================================
//==========================================================

void Hud_StudioEntityLight( struct alight_s *plight )
{
	cl_entity_t * pEnt; 
	pEnt = oEngStudio.GetCurrentEntity();
	if(pEnt && pEnt->player){
		vPlayers[pEnt->index].CalcHitbox();
	}
	oEngStudio.StudioEntityLight(plight);
}
/*
int Hud_StudioDrawPlayer ( int flags, struct entity_state_s *pplayer )
{
	int ret = zHAi15.pStudioDrawPlayer( flags, pplayer );
	//vPlayers[pplayer->number].CalcHitbox();
	return ret;
}
*/
// Draw mesh vertices
int iModelIndexAlreadyFat[512];
int iModelsMadeFat = 0;
void Hud_StudioDrawPoints(void)
{
	cl_entity_s* ent = oEngStudio.GetCurrentEntity();
	if( vPlayers[ent->index].isAlive() && vPlayers[ent->index].isUpdatedAddEnt() && cvar.BigHead)
	{
		int i, j, k;
		mstudiobodyparts_t *pBodyPart;
		mstudiomodel_t *pSubModel;
		model_t *pModel;
		studiohdr_t *pStudioHeader;
		byte *pvertbone;
		Vector *pstudioverts;
		cl_entity_t *pEnt;

		pEnt = oEngStudio.GetCurrentEntity(); // get the ent currently being rendered
		pModel = oEngStudio.SetupPlayerModel(pEnt->index); // then get the model of that ent
		pStudioHeader = (studiohdr_t*)oEngStudio.Mod_Extradata(pModel); // ...and the header of that model's data
		// then use the header to find the address of the first body part in the model data
		pBodyPart = (mstudiobodyparts_t*)((byte*)pStudioHeader + pStudioHeader->bodypartindex);

		// check this model hasn't already been made into a fatty -- simple and ugly pointer comparison that can easily be improved
		for(i = 0; i < iModelsMadeFat; i++)
		{
			if(iModelIndexAlreadyFat[i] == (int)pStudioHeader)
			{
				oEngStudio.StudioDrawPoints();
				return ;
			}
		}
		if(iModelsMadeFat >= 512)
		{
			oEngStudio.StudioDrawPoints();
			return ;
		}

		//add_log("making model with pointer %x big-headed!", pStudioHeader);
		iModelIndexAlreadyFat[iModelsMadeFat] = (int)pStudioHeader;
		iModelsMadeFat++;

		for(k = 0; k < pStudioHeader->numbodyparts; k++) // loop through all bodyparts of the model...
		{
			pSubModel = (mstudiomodel_t*)((byte*)pStudioHeader + pBodyPart[k].modelindex);
			for(i = 0; i < pBodyPart[k].nummodels; i++) // ...and all submodels of the bodyparts
			{
				pstudioverts = (Vector*)((byte*)pStudioHeader + pSubModel[i].vertindex);
				for(j = 0; j < pSubModel[i].numverts; j++)  // ...and all vertices in the submodel
				{
					pvertbone = ((byte*)pStudioHeader + pSubModel[i].vertinfoindex);
					if(pvertbone[j] == 7) // if this vertex is attached to the head bone (bighead only, remove this check for fatboy)...
					{
						pstudioverts[j] = pstudioverts[j] * 2.0f; // make all the vertices in the head 3x as far away from the head bone
					}
				}
			}
		}
	} else oEngStudio.StudioDrawPoints();
}
//==========================================================
//==========================================================
int HUD_GetStudioModelInterface( int version, struct r_studio_interface_s **ppinterface, struct engine_studio_api_s *pstudio )
{
    int nResult = zHAi15.pHUD_GetStudioModelInterface(version,ppinterface,pstudio);
	
	/*static bool bStudioFirst=false;
	//if (!bStudioFirst) //pEngStudio->GetModelByIndex && 
	{
		pstudio->StudioDrawPoints  =&Hud_StudioDrawPoints;
		pstudio->StudioEntityLight =&Hud_StudioEntityLight;
		memcpy(&oEngStudio, pstudio, sizeof(engine_studio_api_t));
		//bStudioFirst=false;
	}*/
/*
	if(ppinterface && *ppinterface)
	{
		zHAi15.pStudioDrawPlayer = (*ppinterface)->StudioDrawPlayer;
		(*ppinterface)->StudioDrawPlayer  =&Hud_StudioDrawPlayer;
	}
*/
    return nResult;
}
//===========================================================
int HUD_Init(void)
{
	int retval = NULL;
	retval = zHAi15.pHUD_Init();
	return retval;
}
//===========================================================
int HUD_Key_Event(int eventcode, int keynum, const char *pszCurrentBinding)
{
	int retval = 1;
	//Handle Menu on INS
	if (keynum == cvar.menukey && eventcode) {
		gMenu.Active = !gMenu.Active;
		if (!gMenu.Active) cvar.Save();
		return 0;
	}
	if( gMenu.Active && eventcode )
	{
		int iMenuKey = gMenu.KeyEvent(keynum);
		if (!iMenuKey) return 0;		
	}
	
	//Handle Id Hook on End
	if (eventcode) {
		int retFK = vPlayers.FirstKill_KeyEvent(keynum);
		if (!retFK) return 0;
	}

	//Handle Console on DEL
	if (keynum == cvar.consolekey && eventcode) {gConsole.active = !gConsole.active; return 0;}
	
	if (keynum >= 239 && keynum <= 244)
	{
		switch (keynum) {
			case 239:
				keyBindManager.notifyMouseEvent("mwheeldown", (eventcode == 1));
				break;
			case 240:
				keyBindManager.notifyMouseEvent("mwheelup", (eventcode ==1));
				break;
			case 241:
				keyBindManager.notifyMouseEvent("mouse1", (eventcode == 1));
				break;
			case 242:
				keyBindManager.notifyMouseEvent("mouse2", (eventcode == 1));
				break;
			case 243:
				keyBindManager.notifyMouseEvent("mouse3", (eventcode == 1));
				break;
			case 244:
				keyBindManager.notifyMouseEvent("mouse4", (eventcode == 1));
				break;
			case 245:
				keyBindManager.notifyMouseEvent("mouse5", (eventcode == 1));
				break;
		}
	}
	else
	{
		// don't pass event code straight in, for some reason it b0rks
		if (eventcode)
			keyBindManager.notifyKeyEvent(keynum, true, false);
		else
			keyBindManager.notifyKeyEvent(keynum, false, false);
	}

    if (!keyBindManager.keyBlocked(keynum, eventcode))
        retval = zHAi15.pHUD_Key_Event( eventcode, keynum, pszCurrentBinding );
    else
        if ( me.isAlive() ) {
			retval = 0;
		} else {
			retval = zHAi15.pHUD_Key_Event( eventcode, keynum, pszCurrentBinding );
		}

	return(retval);
}
// =============================================================================
void HUD_PlayerMove(struct playermove_s *ppmove, int server)
{
	zHAi15.pHUD_PlayerMove(ppmove,server);

	me.pmFlags = ppmove->flags;
	me.pmMoveType = ppmove->movetype;
    VectorCopy(ppmove->velocity,me.pmVelocity);
	
	me.pmMaxSpeed = ppmove->maxspeed;
	me.pmAirAccelerate = ppmove->movevars->airaccelerate;
	me.pmGroundSpeed = sqrt(ppmove->velocity[0]*ppmove->velocity[0]+ppmove->velocity[1]*ppmove->velocity[1]);
	
	me.pmInDuck = ppmove->bInDuck;

	// copy origin+angles
	oEngfuncs.pEventAPI->EV_LocalPlayerViewheight(me.pmEyePos);	

    me.pmEyePos[0] += ppmove->origin[0];
	me.pmEyePos[1] += ppmove->origin[1];
	me.pmEyePos[2] += ppmove->origin[2];

	VectorCopy(ppmove->angles,me.pmViewAngles);	
}
//===========================================================
void HUD_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *usercmd, int runfuncs, double time, unsigned int random_seed)
{
	//me.zGuns.HUD_PostRunCmd( from, to, usercmd, runfuncs, time, random_seed );
	zHAi15.pHUD_PostRunCmd( from, to, usercmd, runfuncs, time, random_seed );
}
//===========================================================
void HUD_Redraw(float x, int y)
{
	zHAi.bGameActive = true;
	zHAi.iUpdateLast = timeGetTime();

	zHAi15.pHUD_Redraw(x,y);

	char Temp[255];
	static bool bInit=false;
	if (!bInit)
	{
		screeninfo.iSize=sizeof(SCREENINFO);
		oEngfuncs.pfnGetScreenInfo(&screeninfo);
		displayCenterX=screeninfo.iWidth/2;
		displayCenterY=screeninfo.iHeight/2;
		//hlexec("hideradar");
		// init cvars here
		cvar.Init();
		cvar.Load();
		int o=1;
		hookpfnSPR_GetList("sprites/weapon_knife.txt",&o); 
		
		hlexec("unbind ins");
		hlexec("unbind f");
		hlexec("unbind end");
		hlexec("unbind del");
		bInit=true;
	}

	// 远透开关
	gSoundEsp.HUD_Redraw();

	// 菜单命令
	if(!zHAi.bShotMaked && zHAi.bGameActive) {
		gMenu.Init();
		gMenu.Draw();
	}

	vPlayers.UpdateLocalPlayer();
	ClientTime::reportMapTime(oEngfuncs.GetClientTime());

	//显示间隔
	if(blinkTimer.expired()) {
		blinkPhase = !blinkPhase;
		if(blinkPhase){ blinkTimer.countdown(0.2); }
		else          { blinkTimer.countdown(0.1); }
	}

	//延迟命令
	for( ScheduleList::iterator pos = scheduleList.begin();
		 pos != scheduleList.end();
		 ++pos)
	{
		if( (*pos).timer.expired() )
		{
			defCMD.exec( const_cast<char*>((*pos).cmd.c_str()) );
			scheduleList.erase(pos);
			break;
		}	
	}

	//AWP 光标
	if (!zHAi.bShotMaked && zHAi.bGameActive && me.zGuns.InCrosshair() ) {
		if ( me.isAlive() && !me.inZoomMode() ) {
			DrawCrosshair();
		}
	}
	
	//自身发光
	/*
	cl_entity_t *wpn = oEngfuncs.GetViewModel();
	if(wpn)
	{
		if (!zHAi.bShotMaked && zHAi.bGameActive && cvar.sprites) {
			if (wpn->curstate.renderfx != kRenderFxGlowShell){
				wpn->curstate.renderamt = 2;
				wpn->curstate.rendercolor.r = 254;
				wpn->curstate.rendercolor.g = 242;
				wpn->curstate.rendercolor.b = 58;
				wpn->curstate.renderfx = kRenderFxGlowShell;
			}
		} else {
			if (wpn->curstate.renderfx != kRenderFxNone){
				wpn->curstate.renderfx = kRenderFxNone;
			}
		}
	}*/

	//专杀菜单
	if (!zHAi.bShotMaked && zHAi.bGameActive){
		if ( vPlayers.FirstKill_Active ) { vPlayers.FirstKill_Show(); }
	}

	//控制台菜单
	if (!zHAi.bShotMaked && zHAi.bGameActive)
	{
		//Draw the console
		int cW = screeninfo.iWidth/2;
		int cH = screeninfo.iHeight/2;
		int cX = displayCenterX - cW/2;
		int cY = displayCenterY - cH/2 - 100;
		gConsole.draw(cX,cY,cW-20,cH);
	}
	
	//C4 已装时间
	if (!zHAi.bShotMaked && zHAi.bGameActive && me.isAlive() && me.m_Team==2)
	{
		if (bBombIsPlanted && blinkPhase)
		{
			int planted;
			if (!iC4Timer)  { planted = (ClientTime::current - fBombPlantedTime); }
			else            { planted = ((iC4Timer - (ClientTime::current - fBombPlantedTime))+1); }
			
			int planted_sec = planted % 60;
			int planted_min = planted / 60;

			sprintf (Temp, "[==> C4-Time: %01d:%02d <==]", planted_min, planted_sec);

			ColorEntry* clr = colorList.get(8);
			DrawHudString(10,screeninfo.iHeight-200, clr->r, clr->g, clr->b,Temp);
		}
	}

	// 雷达
	if ( !zHAi.bShotMaked && zHAi.bGameActive && me.isAlive()  && cvar.radar ){
		if ( cvar.radar == 1 || (!mapLoaded && cvar.radar == 2))	{DrawRadar();}
		else if ( cvar.radar == 2 && mapLoaded )					{overview_redraw();}
	}

	// 自瞄查找对象
	vPlayers.FindTarget();

	if( !zHAi.bShotMaked && zHAi.bGameActive ) { vPlayers.DrawHitBox(); }

/*
	if( !zHAi.bShotMaked && zHAi.bGameActive && me.alive && cvar.radar)
	{
		_Player& r = vPlayers[me.entindex];
		//"杀敌数: %d"
		sprintf( Temp,XorStr<0x42,11,0x09F8B047>("\x8B\xF2\xF1\x95\x8C\xBA\x72\x69\x6F\x2F"+0x09F8B047).s, r.frags );
		DrawConsoleStringEx(10, cvar.radar_y+80, 255,255,255,Temp);
		//死亡数: %d
		sprintf( Temp,XorStr<0x3F,11,0x11E0F969>("\xF4\x80\x8C\xB4\x89\xB9\x7F\x66\x62\x2C"+0x11E0F969).s, r.deaths );
		DrawConsoleStringEx(10, cvar.radar_y+95, 255,255,255,Temp);
		//爆头数: %d(%4.1f%%)
		sprintf( Temp,XorStr<0x1C,20,0xBF5CFD1A>("\xAD\xB1\xD3\xA8\xEA\xDC\x18\x03\x01\x41\x0E\x02\x1C\x07\x1B\x4D\x09\x08\x07"+0xBF5CFD1A).s, me.headshots ,me.hspercent );
		DrawConsoleStringEx(10, cvar.radar_y+110, 255,255,255,Temp);
	}
*/
}
//===========================================================
void HUD_TempEntUpdate(
		double frametime,
		double client_time,
		double cl_gravity,
		TEMPENTITY **ppTempEntFree,
		TEMPENTITY **ppTempEntActive,
		int		( *Callback_AddVisibleEntity )( cl_entity_t *pEntity ),
		void	( *Callback_TempEntPlaySound )( TEMPENTITY *pTemp, float damp ) ){

	zHAi15.pHUD_TempEntUpdate(
		frametime,
		client_time,
		cl_gravity,
		ppTempEntFree,
		ppTempEntActive,
		Callback_AddVisibleEntity,
		Callback_TempEntPlaySound
		);
}
//===========================================================
int HUD_UpdateClientData(client_data_t *cdata, float flTime)
{
	me.zGuns.UpdateBits(cdata->iWeaponBits);
	int retval = 0;
	retval = zHAi15.pHUD_UpdateClientData(cdata, flTime);
	return( retval );
}
//===========================================================

//Func Commands ================================================================
void func_quit()
{
	defCMD.exec("#quit");
}

void func_color()
{
	int arg=1;
	while(*defCMD.argC(arg))
	{
		colorList.set( defCMD.argC(arg), defCMD.argC(arg+1), 
			 defCMD.argC(arg+2),  defCMD.argC(arg+3),  defCMD.argC(arg+4));
		arg+=5;
	}
}

void func_later()
{
	if(defCMD.argS(1).empty())
	{
		list<ScheduledCommand>::iterator pos;
		for(pos=scheduleList.begin();pos!=scheduleList.end();++pos)
		{
			Con_Echo("%6.2f sec: \"%s\"", (*pos).timer.timeleft(), (*pos).cmd.c_str() );
		}

		return;
	}

	if(defCMD.argS(1)=="clear")
	{
		scheduleList.erase(scheduleList.begin(),scheduleList.end());
		return;
	}

	float time = defCMD.argF(1);

	ScheduledCommand tmp;
	defCMD.collectArguments( tmp.cmd, 2); 
	tmp.timer.countdown(time);
	if(scheduleList.size()<32) scheduleList.push_back(tmp);
}

void func_alias()
{
	const char* name = defCMD.argC(1);
	string& content = defCMD.argS(2);
	defCMD.AddAlias(name,content);
}

void func_bind()
{
	keyBindManager.addBind(defCMD.argC(1),defCMD.argC(2));
}

void func_unbind()
{
	keyBindManager.removeBind(defCMD.argC(1));
}

void func_load()
{
	exec( defCMD.argC(1) ); 
}

void func_txt()
{
	string s;
	defCMD.collectArguments(s);
}

//===========================================================
void CommandInit()
{
	defCMD.init();

	#define REG_COMMAND(name) defCMD.AddCommand( #name, func_##name);

	REG_COMMAND(quit)
	REG_COMMAND(color)
	REG_COMMAND(load)
	REG_COMMAND(alias)
	REG_COMMAND(bind)
	REG_COMMAND(unbind)
	REG_COMMAND(later)
	REG_COMMAND(txt)
}
//===========================================================

//===========================================================

//Entity Handling ==============================================================
static void AddEntityPlayer(struct cl_entity_s *ent)
{
	int ax = ent->index;
	_Player& aPly = vPlayers[ax];

    //ent->curstate.rendermode = kRenderTransTexture;
	if(ent->curstate.solid) { aPly.setAlive();}

	// filter local player out out and set me.alive
	if( me.isOwner(ent->index) ) 
	{ 
		aPly.m_Distance = 100000.0;
		aPly.m_Visible  = false;
		aPly.m_Fovangle = 360;
		//aPly.updateClear();
		aPly.SuspectNextOrigin();
		return;
	}

	if( aPly.isAlive() )
	{
		aPly.AddEntity(ent->origin);
		
		aPly.Refresh(); // avoid division by zero
		aPly.calcFovangleAndVisibility();

		aPly.bGotHead = false;

		if(!zHAi.bShotMaked && zHAi.bGameActive && me.isAlive() && aPly.isEnemy()) {
			aPly.Glows();
		}
	}
}

//===========================================================

#define MAX_SPRITES 212
typedef struct
{
   char szName[256];
   char szSprite[256];
   HSPRITE hspr;
   wrect_t rc;
   int iRes;
} spriteinfo_t;

//===========================================================

typedef struct
{
	int iSpritesLoaded;
	spriteinfo_t spriteinfo[MAX_SPRITES];
}spritedata_t;

spritedata_t spritedata;

//===========================================================

void LoadSprites(client_sprite_t *pList, int iCount)
{
   int i;
   client_sprite_t *p;
   char buf[512];

   spritedata.iSpritesLoaded = 0;

   if(!pList)
   {
       return;
   }

   p = pList;

   i = iCount;

   while(i-- && p->szName[0] != 0x00 && spritedata.iSpritesLoaded < MAX_SPRITES)
   {
       strncpy(spritedata.spriteinfo[spritedata.iSpritesLoaded].szName, p->szName,MAX_SPRITES);
       strncpy(spritedata.spriteinfo[spritedata.iSpritesLoaded].szSprite, p->szSprite,MAX_SPRITES);

       sprintf(buf, "sprites/%s.spr", p->szSprite);
       spritedata.spriteinfo[spritedata.iSpritesLoaded].hspr = oEngfuncs.pfnSPR_Load(buf);

       memcpy(&(spritedata.spriteinfo[spritedata.iSpritesLoaded].rc), &(p->rc), sizeof(p->rc));
       spritedata.spriteinfo[spritedata.iSpritesLoaded].iRes = p->iRes;

       p++;
       spritedata.iSpritesLoaded++;

   }
}

//===========================================================

client_sprite_t* __stdcall hookpfnSPR_GetList(char *psz, int *piCount)
{
	client_sprite_t *ret;

	ret = oEngfuncs.pfnSPR_GetList(psz, piCount);
	client_sprite_t *pList;
	if(!strcmp(psz, "sprites/weapon_knife.txt")) 
	{
		int iCount;
		pList = oEngfuncs.pfnSPR_GetList("sprites/hud.txt", &iCount);
		if(pList)
		{
	      LoadSprites(pList, iCount);
		}
	}
	return ret;
}

//===========================================================

int GetSpriteIndex(char *szName)
{
   int i;

   if(szName[0] == 0x00)
       return -1;

   for(i = 0; i < spritedata.iSpritesLoaded; i++)
   {
       if(spritedata.spriteinfo[i].iRes == 640)
       {
           if(!strcmp(spritedata.spriteinfo[i].szName, szName))
           {
               return i;
           }
       }
   }
	return 0;
}

//===========================================================

static void drawPlayerEsp(int ax)
{
	if ( !me.isAlive() || !vPlayers[ax].isEnemy()) return;
	if ( cvar.radar ) {
		ColorEntry* TeamColor;
		if (vPlayers[ax].m_Team == 1) { TeamColor = colorList.get(2); }
		else                        { TeamColor = colorList.get(3); }
		drawRadarPoint(vPlayers[ax].origin(),TeamColor->r,TeamColor->g,TeamColor->b,200); 
	}

	ColorEntry* pClr = vPlayers.Color(ax);
	// rest of code is only for players on screen:
	float vecScreen[2];
	if( !CalcScreen(vPlayers[ax].origin(),vecScreen) ) { return; }	

	// distance and boxradius also needed for esp offset
	float distance = vPlayers[ax].m_Distance/22.0f;
	int   boxradius = (300.0*90.0) / (distance*fCurrentFOV);
	BOUND_VALUE(boxradius,3,15);

	// ===== prepare text esp:
	enum{ CHAR_HEIGHT = 16 };
	int text_dist = (int)(boxradius);
	int ystep = -CHAR_HEIGHT;

	int x = vecScreen[0];
	int y = vecScreen[1];
	if ( me.isAlive() ) y = y-text_dist-CHAR_HEIGHT+10;
	
	if (cvar.reload_meter)
	{
		int seqinfo = vPlayers[ax].SeqInfo();
		if( seqinfo == 2)
		{
    		oglSubtractive=true;
    		oEngfuncs.pfnFillRGBA(x-26,y,53,12,0,0,0,250);
    		oEngfuncs.pfnFillRGBA(x-25,y+1,(vPlayers[ax].getEnt()->curstate.frame/255)*50,10,pClr->r,pClr->g,pClr->b,255);
    		oglSubtractive=false;
			y+=ystep;
		}
		else if( seqinfo == 16)
		{
    		oglSubtractive=true;
    		oEngfuncs.pfnFillRGBA(x-26,y,53,12,0,0,0,250);
    		oEngfuncs.pfnFillRGBA(x-25,y+1,(vPlayers[ax].getEnt()->curstate.frame/255)*101,10,255,255,0,255);
    		oglSubtractive=false;
    		y+=ystep;
		}
	}

	if ( GetAsyncKeyState(VK_MENU) & 0x8000) {
		DrawConStringCenter(x,y,254,242,58,vPlayers[ax].Name());
		y += ystep;
	}

	if (cvar.sprites)
	{
		dlight_t * pDl = oEngfuncs.pEfxAPI->CL_AllocDlight(0);
		pDl->color.r = pClr->r;
		pDl->color.g = pClr->g;
		pDl->color.b = pClr->b;
		pDl->origin = vPlayers[ax].origin();
		pDl->die = oEngfuncs.GetClientTime() + 0.1;
		pDl->radius = 100;

		char buf[255];
		int index;
		
		if (strstr(vPlayers[ax].WeaponName(),"nade"))
			sprintf(buf,"d_grenade"); 
		else if (strstr(vPlayers[ax].WeaponName(),"c4"))
			sprintf(buf,"c4"); 
		else if (strstr(vPlayers[ax].WeaponName(),"mp5"))
			sprintf(buf,"d_mp5navy"); 
		else if (strstr(vPlayers[ax].WeaponName(),"shield"))
			sprintf(buf,"d_skull"); 
		else
			sprintf(buf, "d_%s",vPlayers[ax].WeaponName()); 

		index = GetSpriteIndex(buf); 

		if (index != -1)
		{         
			oEngfuncs.pfnSPR_Set(spritedata.spriteinfo[index].hspr, pClr->r,pClr->g,pClr->b);
			oEngfuncs.pfnSPR_DrawAdditive(0, x-15, y, &(spritedata.spriteinfo[index].rc));
		}
	}
}

//===========================================================

//extern int QQ_Count;			//QQ句柄数量
//extern char QQ_Number[5][15];	//存放QQ号的
int roundcounter = 0;
void AtRoundStart(void)
{
	if( strcmp(currentMap,oEngfuncs.pfnGetLevelName()) )
	{
		strcpy(currentMap,oEngfuncs.pfnGetLevelName());
		AtMapChange();
	}
/*
	if (cvar.zHAi_Check[0]=='\0' || 
		cvar.zHAi_CheckQQ[0]=='\0' ||
	    !(strstr(cvar.zHAi_Check,cvar.zHAi_CheckQQ))
	){
		char zLice_QQ[15];
		int i=0, Id=0, intQQ,HigQQ,LowQQ;
		//45524562000000
		sprintf(zLice_QQ
			,XorStr<0xE6,3,0xA26FC4C5>("\xC3\x94"+0xA26FC4C5).s
			,XorStr<0xA1,15,0xB6EE8AB9>("\x95\x97\x96\x96\x91\x93\x91\x9A\x99\x9A\x9B\x9C\x9D\x9E"+0xB6EE8AB9).s);

		for (i = 0; i<QQ_Count;i++){
			for (Id=0;Id<15;Id++)
			{
				HigQQ=(int)(zLice_QQ[Id]-'0');
				LowQQ=(int)(QQ_Number[i][Id]-'0');
				intQQ=HigQQ+LowQQ;
				if (intQQ>9) intQQ-=10;
				cvar.zHAi_CheckQQ[Id]=intQQ+'0';
			}
			if (!(strstr(cvar.zHAi_Check,cvar.zHAi_CheckQQ)) ){
				zHAi.bHasShotSnap=false;
			}
		}
	}
*/
	me.isHeadshot=false;
	me.setAlive();
	me.zGuns.bGoted=false;

	vPlayers.Clear();
}

//USER MESSAGES HANDLING =======================================================

#define USER_MSG_INTERCEPT(name) \
	static pfnUserMsgHook name ## Org = NULL; \
	static int UserMsg ## name ( const char *pszName, int iSize, void *pbuf)
#define USER_MSG_CALL(name) (*name ## Org)(pszName,iSize,pbuf)

//===========================================================

USER_MSG_INTERCEPT(CurWeapon)
{
	BEGIN_READ( pbuf, iSize );
	int iState = READ_BYTE();
	int iID    = READ_CHAR();
	int iClip  = READ_CHAR();

	me.setAmmo(iState, iID, iClip);
	return USER_MSG_CALL(CurWeapon);
}

//===========================================================

USER_MSG_INTERCEPT(TeamInfo)
{
	static bool         done=false;
	static StringFinder teamNumber;
	if(!done)
	{
		teamNumber.add("terrorist"  ,1);teamNumber.add("ct"         ,2);
		teamNumber.add("unassigned" ,0);teamNumber.add("spectator"  ,0);
		done = true;
	}

	vPlayers.UpdateLocalPlayer();

	BEGIN_READ(pbuf,iSize);
	int px = READ_BYTE();
	char * teamtext = READ_STRING();

	if(!*teamtext) { vPlayers[px].m_Team = 0; }
	else
	{
		_strlwr(teamtext);
		if(teamNumber.find(teamtext))
		{
			vPlayers[px].m_Team = teamNumber.num;
		}
		else
		{
			vPlayers[px].m_Team = -1;
		}
		if (me.isOwner(px)) { me.m_Team=vPlayers[px].m_Team; }
	}	
	return USER_MSG_CALL(TeamInfo);
}

//===========================================================

USER_MSG_INTERCEPT(SetFOV)
{	
	BEGIN_READ( pbuf, iSize );
	me.iFov = READ_BYTE();
	if(!me.iFov) { me.iFov=90; }
	me.setZoomMode(me.iFov);
	fCurrentFOV = me.iFov;
	return USER_MSG_CALL(SetFOV);
}

//===========================================================

USER_MSG_INTERCEPT(ResetHUD)
{
	bBombIsPlanted=false;
	AtRoundStart();
	return USER_MSG_CALL(ResetHUD);
}

//===========================================================

USER_MSG_INTERCEPT(WeaponList)
{
	char *weaponname;
	int ammo1type, max1, ammo2type, max2, slot, slotpos, id, flags;
	BEGIN_READ(pbuf, iSize);
	weaponname = READ_STRING();
	ammo1type = READ_CHAR();
	max1 = READ_BYTE();
	ammo2type = READ_CHAR();
	max2 = READ_BYTE();
	slot = READ_CHAR();
	slotpos = READ_CHAR();
	id = READ_CHAR();
	flags = READ_BYTE();
	me.zGuns.MsgAdds(weaponname, ammo1type, max1, ammo2type, max2, slot, slotpos, id, flags);
	return USER_MSG_CALL(WeaponList);
}

//===========================================================

USER_MSG_INTERCEPT(DeathMsg)
{
	BEGIN_READ( pbuf, iSize );
	int killer = READ_BYTE();
	int victim = READ_BYTE();
	int headshot = READ_BYTE();
    char* weaponName = READ_STRING();	

	if( me.isOwner(killer) && headshot)
	{
		me.isHeadshot=true;
	}

	if ( me.isOwner(victim) ) {
		me.setDead();
		vPlayers.iTarget=-1;
		//cvar.ltfxspeed = 0.0;
	}	
	int px = victim;
	vPlayers[px].setDead();

	return USER_MSG_CALL(DeathMsg);
}

//===========================================================

USER_MSG_INTERCEPT(ScoreInfo)
{
	BEGIN_READ(pbuf, iSize);
	int idx = READ_BYTE();
	_Player& r = vPlayers[idx];

	r.frags  = READ_SHORT();
	r.deaths = READ_SHORT();

	if(r.frags<=0 )   { r.ratio=0; }
	else              { r.ratio = (double)r.frags / ((r.deaths<=0)?1:r.deaths); }

	// update bestplayer
	r.bestplayer = false;
	bool foundbetter = false;
	for(int i=0;i<vPlayers.Count() ;i++)
	{
		if     ( r.ratio < vPlayers[i].ratio ) foundbetter = true;
		else if( r.ratio > vPlayers[i].ratio ) vPlayers[i].bestplayer=false;
	}
	if(!foundbetter) r.bestplayer = true;

	
	// score handling:
	if(me.isOwner(idx))
	{
		static int lastfrags=0;
		static int lastdeaths=0;

		if(me.Headshot_Nums>r.frags || r.frags==0) 
		{ 
			// reset statistics
			me.isHeadshot    = false; 
			me.Headshot_Nums = 0; 
			me.Headshot_Rate = 0;
			lastfrags    = r.frags;
			lastdeaths   = r.deaths;
		}
		else
		{
			if(me.isHeadshot) { ++me.Headshot_Nums; }

			double den = r.frags ? (double)r.frags : 1.0;
			me.Headshot_Rate = (double)me.Headshot_Nums/den*100.0;
		}
		me.isHeadshot = false;
		lastfrags     = r.frags;
		lastdeaths    = r.deaths;
	}

	return USER_MSG_CALL(ScoreInfo);
}

//===========================================================

USER_MSG_INTERCEPT(TextMsg)
{

	BEGIN_READ(pbuf, iSize);
	int id = READ_BYTE();
	char* msg = READ_STRING();

	if( *msg )
	{
		if (!strcmp(msg, "#Bomb_Planted"))
		{
			fBombPlantedTime = ClientTime::current;
			bBombIsPlanted=true;
		}
		else if (!strcmp(msg, "#Target_Bombed"))
		{
			if (bBombIsPlanted)
			{
				iC4Timer = (int) ((ClientTime::current-fBombPlantedTime)+0.4999999);
				if(iC4Timer<0 || iC4Timer>120)  {  iC4Timer=0;  } 
				bBombIsPlanted=false;
			}
		}
		else if (!strcmp(msg, "#Bomb_Defused"))
		{
			bBombIsPlanted=false;
		}

	}
	return USER_MSG_CALL(TextMsg);
}

USER_MSG_INTERCEPT(AmmoX)
{
	int ID, Count;
	BEGIN_READ(pbuf, iSize);
	ID = READ_BYTE();
	Count = READ_BYTE();
	me.zGuns.MsgSetAmmoX(ID, Count);
	return USER_MSG_CALL(AmmoX);
}

//===========================================================

int	HookUserMsg (char *szMsgName, pfnUserMsgHook pfn)
{
	int retval = oEngfuncs.pfnHookUserMsg(szMsgName, pfn);

	#define REDIRECT_MESSAGE(name) \
		else if (!strcmp(szMsgName,#name)) \
		{ \
			name##Org = pfn; \
			return oEngfuncs.pfnHookUserMsg (szMsgName, UserMsg##name ); \
		}	

	if(0){}
	REDIRECT_MESSAGE( TeamInfo    ) REDIRECT_MESSAGE( CurWeapon   )
	REDIRECT_MESSAGE( SetFOV      ) REDIRECT_MESSAGE( DeathMsg    )
	REDIRECT_MESSAGE( ResetHUD    ) REDIRECT_MESSAGE( WeaponList  )
	REDIRECT_MESSAGE( ScoreInfo	  ) REDIRECT_MESSAGE( TextMsg     )
	REDIRECT_MESSAGE( AmmoX		  )
	return retval;
}

//===========================================================

//TimeHandling =================================================================

float  ClientTime::mapTime    = 1;
double ClientTime::current    = 1;
unsigned int ClientTime::current_ms = 1;

//===========================================================

void AtMapChange()
{
	bBombIsPlanted=false;
    fBombPlantedTime=0;
    iC4Timer=0;

	me.isHeadshot=false;

	roundcounter = 0;

	mapLoaded = false;
	overview_loadcurrent();
}

//===========================================================

void ClientTime::reportMapTime(float mapTimeNew)
{
	mapTime = mapTimeNew;
	updateCurrentTime();
}

//============================================================

void ClientTime::updateCurrentTime()
{
	// timer update
	current_ms = timeGetTime();

	current = (double)current_ms / 1000.0;
}

//============================================================

void hlexec (const char *fmt, ... )
{
	va_list va_alist;
	char buf[384];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	oEngfuncs.pfnClientCmd(buf);
}
//===========================================================

void Con_Echo(const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	// echo + scroll up:
	gConsole.echo("%s",buf);
}

//===========================================================

void HlEngineCommand(const char* command)
{
	if(!oEngfuncs.pfnClientCmd) { return; }
	oEngfuncs.pfnClientCmd( const_cast<char*>(command) );
}

//===========================================================

void HandleCvarFloat(char* name, float* value)
{
	char* arg1 = defCMD.argC(1);  
	if (!strcmp(arg1,"change")) 
	{
		if (*value) *value=0; else *value=1; 
		return;
	}
	if (!strcmp(arg1,"up"))     
	{ 
		*value += defCMD.argF(2); 
		return;
	}
    if (!strcmp(arg1,"down"))   
	{ 
		*value -= defCMD.argF(2); 
		return;
	}
	if (!strcmp(arg1,"hide")) 
    { 
        *value = defCMD.argF(2);
        return;
    }
	if (!*arg1) 
	{ 
		Con_Echo( "CVAR &w%s&r = %f\n",name,*value); 
		return;
	}
    *value = defCMD.argF(1);
}

//===========================================================

bool isHlCvar(char* name)
{
	if(!oEngfuncs.pfnGetCvarPointer) { return false; }
	
	cvar_s* test = oEngfuncs.pfnGetCvarPointer(name);
	return (test!=NULL);
}

//===========================================================

void HandleHlCvar(char* name)
{
	if(!oEngfuncs.pfnGetCvarPointer) { return; }

	cvar_s* ptr = oEngfuncs.pfnGetCvarPointer(name);
	if(!ptr) return;

	HandleCvarFloat(name,&ptr->value);
}

//===========================================================