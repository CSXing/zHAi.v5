
	//===========================================================
	#include "engine/wrect.h"
	#include "engine/cl_dll.h"
	#include "engine/cdll_int.h"
	#include "engine/const.h"
	#include "engine/progdefs.h"
	#include "engine/eiface.h"
	#include "engine/edict.h"
	#include "engine/studio_event.h"
	#include "engine/entity_types.h"
	#include "engine/r_efx.h"
	#include "engine/pmtrace.h"
	#include "engine/studio.h"
	#include "common/ref_params.h"
	#include "common/screenfade.h"
	#include "common/event_api.h"
	#include "common/com_model.h"
	#include "misc/parsemsg.h"
	#include "misc/sprites.h"
	#include "misc/r_studioint.h"
	#include "engine/triangleapi.h"
	#include "engine/pm_defs.h"
	#include "engine/pm_movevars.h"

	#include <windows.h>
	#include <mmsystem.h>
	#include <memory.h>
	#include <vector>
	#include <string>
	#include <tlhelp32.h>
	#include <fstream>
	#include <iostream>
	#include "color.h"

	using namespace std;

	//===========================================================
	extern struct cl_enginefuncs_s oEngfuncs;
	extern struct engine_studio_api_s oEngStudio;
	extern struct event_api_s oEventApi;

	extern struct cl_enginefuncs_s  gHookedEngfuncs;

	extern double * globalTime;
	extern cl_enginefuncs_s    *pEngfuncs;
	extern engine_studio_api_s *pEngStudio;

	extern bool oglSubtractive;
	extern float displayCenterX, displayCenterY;

	extern SCREENINFO screeninfo;
	extern char hackdir[256],DllCfg[256],ExeCfg[256];

	//===========================================================
	#ifndef CDLL_INT_H
	typedef struct
	{
		char *name;
		char *model;

	} hud_player_info_t;
	#endif

	//===========================================================
	static void AddEntityPlayer(struct cl_entity_s *ent);
	static void AddEntityWeapon (struct cl_entity_s *ent, const char *mdlname, bool special);
	static void AddEntityHostage(struct cl_entity_s *ent);
	static void drawPlayerEsp(int ax);
	static void drawEsp(int ab);
	int	HookUserMsg (char *szMsgName, pfnUserMsgHook pfn);
	void hlexec (const char *fmt, ... );
	void AtMapChange();
	void Con_Echo(const char *fmt, ... );
	void HandleCvarFloat(char* name, float* value);
	void HlEngineCommand(const char* command);
	bool isHlCvar(char* name);
	void HandleHlCvar(char* name);

	client_sprite_t* __stdcall hookpfnSPR_GetList(char *psz, int *piCount);
	void exec(const char* filename);
	bool __fastcall CalcScreen(float *origin, float *vecScreen);
	void CommandInit();

	//===========================================================
