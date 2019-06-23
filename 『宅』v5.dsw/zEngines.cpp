/**********************************************************
¡ô©©©Ç zHA! H00k vEr:5.0 ©Ï©©¡ô
    Create by: X!ing,cHEn
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
¡ô©©©Ç      (c)2018      ©Ï©©¡ô
    File Name: zEngines.cpp
************************************************************/

#pragma warning ( disable: 4786 )
#include <windows.h>
#include <tlhelp32.h>

#include "DefineCmds.h"
#include "bind.h"
#include "console.h"
#include "client.h"
#include "players.h"
#include "eventhook.h"
#include "misc/sprites.h"
#include "zEngines.h"

void log(const char *fmt, ... );
int DrawConsoleStringEx(int x, int y,int r, int g, int b, char *str);
//==========================================================
	typedef struct cmd_s
	{
		struct cmd_s	*pNext;
		PCHAR			pszName;
		DWORD			pfnFunc;
		BYTE			bCrap[20];
	}	cmd_t, *pcmd_t;

zHAi_AntiScreen zHAi;

//==========================================================

	void CL_CreateMove(float frametime, struct usercmd_s *UserCMD, int active);
	void V_CalcRefdef (struct ref_params_s *pparams);
	int HUD_AddEntity(int type, struct cl_entity_s *ent, const char *modelname);
	void HUD_Frame( double time );
	int HUD_GetStudioModelInterface( int version, struct r_studio_interface_s **ppinterface, struct engine_studio_api_s *pstudio );
	int HUD_Init(void);
	int HUD_Key_Event(int eventcode, int keynum, const char *pszCurrentBinding);
	void HUD_PlayerMove(struct playermove_s *ppmove, int server);
	void HUD_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *usercmd, int runfuncs, double time, unsigned int random_seed);
	void HUD_Redraw(float x, int y);
	void HUD_TempEntUpdate(double frametime,double client_time,double cl_gravity,TEMPENTITY **ppTempEntFree,TEMPENTITY **ppTempEntActive,int( *Callback_AddVisibleEntity )( cl_entity_t *pEntity ),void( *Callback_TempEntPlaySound )( TEMPENTITY *pTemp, float damp));
	int HUD_UpdateClientData(client_data_t *cdata, float flTime);
//==========================================================

	typedef void( *fCAM_Think )( void );
	typedef void( *fCL_CameraOffset )( float * );
	typedef void( *fCL_CreateMove )( float, struct usercmd_s *, int );
	typedef int( *fCL_IsThirdPerson )( void );
	typedef void( *fDemo_ReadBuffer )( int, unsigned char * );
	typedef int( *fHUD_AddEntity )( int, struct cl_entity_s *, const char * );
	typedef int( *fHUD_ConnectionlessPacket )( struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );
	typedef void( *fHUD_CreateEntities )( void );
	typedef int( *fHUD_DirectorEvent )( unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags );
	typedef void( *fHUD_DrawNormalTriangles )( void );
	typedef void( *fHUD_DrawTransparentTriangles )( void );
	typedef void( *fHUD_Frame )( double time );
	typedef int( *fHUD_GetHullBounds )( int hullnumber, float *mins, float *maxs );
	typedef struct cl_entity_s *( *fHUD_GetUserEntity )( int index );
	typedef int( *fHUD_Init )( void );
	typedef int( *fHUD_Key_Event )( int, int, const char * );
	typedef void( *fHUD_PlayerMove )( struct playermove_s *, int );
	typedef void( *fHUD_PlayerMoveInit )( struct playermove_s * );
	typedef char( *fHUD_PlayerMoveTexture )( char * );
	typedef void( *fHUD_PostRunCmd )( struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed );
	typedef void( *fHUD_ProcessPlayerState )( struct entity_state_s *, const struct entity_state_s * );
	typedef int( *fHUD_Redraw )( float, int );
	typedef int( *fHUD_Reset )( void );
	typedef void( *fHUD_Shutdown )( void );
	typedef void( *fHUD_StudioEvent )( const struct mstudioevent_s *, const struct cl_entity_s * );
	typedef void( *fHUD_TempEntUpdate )( double, double, double, struct tempent_s **, struct tempent_s **, int( *Callback_AddVisibleEntity )( struct cl_entity_s *fEntity ), void( *Callback_TempEntPlaySound )( struct tempent_s *fTemp, float damp ) );
	typedef void( *fHUD_TxferLocalOverrides )( struct entity_state_s *, const struct clientdata_s * );
	typedef void( *fHUD_TxferPredictionData )( struct entity_state_s *, const struct entity_state_s *, struct clientdata_s *, const struct clientdata_s *, struct weapon_data_s *, const struct weapon_data_s * );
	typedef int( *fHUD_UpdateClientData )( client_data_t *, float );
	typedef int( *fHUD_VidInit )( void );
	typedef int( *fHUD_VoiceStatus )( int entindex, qboolean bTalking );
	typedef void( *fIN_Accumulate )( void );
	typedef void( *fIN_ActivateMouse )( void );
	typedef void( *fIN_ClearStates )( void );
	typedef void( *fIN_DeactivateMouse )( void );
	typedef void( *fIN_MouseEvent )( int mstate );
	typedef int( *fInitialize )( cl_enginefunc_t *, int );
	typedef struct kbutton_s *( *fKB_Find )( const char *name );
	typedef void( *fV_CalcRefdef )( struct ref_params_s *fparams ); 
	typedef void (*fHUD_DirectorMessage)(unsigned char, unsigned int, unsigned int, unsigned int);
	typedef int  (*fHUD_GetStudioModelInterface)( int, struct r_studio_interface_s **, struct engine_studio_api_s *);

	zHAi_v15_Client_t zHAi15;

//==========================================================
	_zHAiMemory Patcher;
	MODULEENTRY32 HLEntry;

	const BYTE HL_DecodeDLL_SIG[] = "\x51\x53\x55\x8B\x6C\x24\x10\x56\x8B\x74\x24\x20\x57";
	const DWORD HL_DecodeDLL_SIG_LEN = 13;

	const BYTE HW_DecodeDLL_SIG[] = "\x51\x55\x8B\x6C\x24\x0C\x56\x8B\x74\x24\x1C\x57";
	const DWORD HW_DecodeDLL_SIG_LEN = 12;

	int( *pHL_DecodeDLL )( DWORD dwBaseAddr, char **szDllName, DWORD *ExportTable, int iSize );
	int( *pHW_DecodeDLL )( DWORD dwBaseAddr, char **szDllName, DWORD *ExportTable, int iSize );
	int SPR_Load( const char* szPicName );

//==========================================================
int  SPR_Load( const char* szPicName );
int  SPR_Frames( int hPic );
int  SPR_Height( int hPic, int frame );
int  SPR_Width( int hPic, int frame );
void SPR_Set( int hPic, int r, int g, int b );
void SPR_Draw( int frame, int x, int y, const wrect_t* prc );
void SPR_DrawHoles( int frame, int x, int y, const wrect_t* prc );
void SPR_DrawAdditive( int frame, int x, int y, const wrect_t* prc );
void SPR_EnableScissor( int x, int y, int width, int height );
void SPR_DisableScissor( void );
client_sprite_t* SPR_GetList( char* psz, int* piCount );

//==========================================================
//==========================================================
glReadPixels_t glReadPixels_s;

BOOL ScreenFirst = TRUE;
bool bAntiSSTemp = true;
//bool DrawVisuals;
PBYTE BufferScreen;
int temp;
DWORD dwSize, time_scr;

void __stdcall m_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
	if (ScreenFirst)
	{
		ScreenFirst = FALSE;
		dwSize = ( width * height ) * 3;
		BufferScreen = (PBYTE)malloc(dwSize);
		glReadPixels_s(x, y, width, height, format, type, pixels);
		__try
		{
			memcpy(BufferScreen, pixels, dwSize);
		}
		__except (EXCEPTION_EXECUTE_HANDLER){};
		zHAi.bShotMaked = false;
		return;
	}
	__try
	{
		memcpy(pixels, BufferScreen, dwSize);
	}
	__except (EXCEPTION_EXECUTE_HANDLER){};
}

void Snapshot()
{
	if (bAntiSSTemp)
	{
		time_scr = GetTickCount();
		temp = 0;
		
		bAntiSSTemp = false;
	}

	if (GetTickCount() - time_scr > 5*1000)
	{
		zHAi.bShotMaked = true;
		temp++;
		
		if (temp > 10)
		{
			bAntiSSTemp = true;
			ScreenFirst = TRUE;
			DWORD sz = ( screeninfo.iWidth * screeninfo.iHeight ) * 3;
			free( (PBYTE)BufferScreen );
			PBYTE buf = (PBYTE)malloc( sz );
			glReadPixels( 0 , 0 , screeninfo.iWidth , screeninfo.iHeight , GL_RGB , GL_UNSIGNED_BYTE , buf );
			free( (PBYTE)buf );
		}
	}

	static bool FirstFrame = true;

	if (FirstFrame)
	{
		DWORD sz = screeninfo.iWidth * screeninfo.iHeight * 3;
		PBYTE buf = (PBYTE)malloc( sz );
		glReadPixels( 0 , 0 , screeninfo.iWidth , screeninfo.iHeight , GL_RGB , GL_UNSIGNED_BYTE , buf );
		free( (PBYTE)buf );

		FirstFrame = false;
	}
}

//==========================================================
void CB_KeyManager_ConType(char ch)
{
	gConsole.key(ch);
}
//==========================================================
void CB_KeyManager_Exec(const string& commandlist)
{
	defCMD.exec( const_cast<char*>( commandlist.c_str() ) );
}
//==========================================================

//==========================================================
void AutoOffsets() 
{
	static bool NotPatched = true; 
	if (NotPatched) 
	{ 
		int a; 
		for(a=0x01D00000;a<0x01D0FFFF;a++) 
		{ 
			if(*(PBYTE)(a) == 0x68 && *(PBYTE)(a + 0x05) == 0xE8 && 
			*(PBYTE)(a + 0x0A) == 0x6A && *(PBYTE)(a + 0x0B) == 0x07 && 
			*(PBYTE)(a + 0x0C) == 0x68 && *(PBYTE)(a + 0x11) == 0xFF && 
			*(PBYTE)(a + 0x12) == 0x15 && *(PBYTE)(a + 0x17) == 0x68) 
			{ 
				pEngfuncs = (cl_enginefunc_t*)*(DWORD*)(a+0x0D); 
			}
		}
		/*int b; 
		for(b=0x01D00000;b<0x01D0FFFF;b++) 
		{ 
			if(*(PBYTE)(b) == 0xE8 && *(PBYTE)(b + 0x05) == 0x6A && 
			*(PBYTE)(b + 0x06) == 0x07 && *(PBYTE)(b + 0x07) == 0x68 && 
			*(PBYTE)(b + 0x0C) == 0xFF && *(PBYTE)(b + 0x0D) == 0x15 && 
			*(PBYTE)(b + 0x12) == 0x68 && *(PBYTE)(b + 0x1C) == 0x81) 
			{ 
				pPMove = (playermove_s*)*(DWORD*)(b+0x13); 
				break; 
			}
		}*/
		int c; 
		for(c=0x01D00000;c<0x01D0FFFF;c++) 
		{ 
			if(*(PBYTE)(c) == 0x68 && *(PBYTE)(c + 0x05) == 0x68 && 
			*(PBYTE)(c + 0x0A) == 0x6A && *(PBYTE)(c + 0x0B) == 0x01 && 
			*(PBYTE)(c + 0x0C) == 0xFF && *(PBYTE)(c + 0x0D) == 0xD0 && 
			*(PBYTE)(c + 0x0E) == 0x83 && *(PBYTE)(c + 0x27) == 0xC3) 
			{ 
				pEngStudio = (engine_studio_api_s*)*(DWORD*)(c+0x01); 
			}
		}
		/*int d; 
		for(d=0x01D60000;d<0x01D6FFFF;d++) 
		{ 
			if(*(PBYTE)(d) == 0x68 && *(PBYTE)(d + 0x05) == 0xFF && 
			*(PBYTE)(d + 0x06) == 0x15 && *(PBYTE)(d + 0x0B) == 0x68 && 
			*(PBYTE)(d + 0x10) == 0xFF && *(PBYTE)(d + 0x11) == 0x15 && 
			*(PBYTE)(d + 0x16) == 0xA1 && *(PBYTE)(d + 0x1B) == 0x83) 
			{ 
				ltfxslots = (DWORD*)(*(DWORD*)(d+0x01)); 
				ltfx_loc = (DWORD)ltfxslots;
			}
		}

		if( ((DWORD*)pEngfuncs <= (DWORD*)(0x01EB52B8)) )//RetartPtr
		{
			RetartPtr = (DWORD*)0x01A179D0;
		} else {
			RetartPtr = (DWORD*)0x01A179F0;
		} 
	*/
		NotPatched = false; 
	}
}
//==========================================================

void Hud_StudioEntityLight( struct alight_s *plight );
void Hud_StudioDrawPoints(void);

int Initialize (cl_enginefunc_t* pEnginefuncs, int iVersion)
{
	int nResult;
	AutoOffsets();

    assert(pEnginefuncs);
	
	static bool NotPatched = true;
	if (NotPatched){
		pEngfuncs = pEnginefuncs;
		NotPatched = false; 
	}

	memcpy(&oEngfuncs, pEnginefuncs, sizeof(cl_enginefunc_t));
    memcpy (&gHookedEngfuncs, pEnginefuncs, sizeof(cl_enginefunc_t));
	
    gHookedEngfuncs.pfnHookUserMsg = HookUserMsg;
	gHookedEngfuncs.pfnHookEvent   = &HookEvent;

    gHookedEngfuncs.pfnSPR_Load         = &SPR_Load;
    gHookedEngfuncs.pfnSPR_Set          = &SPR_Set;
    gHookedEngfuncs.pfnSPR_Draw         = &SPR_Draw;
    gHookedEngfuncs.pfnSPR_DrawAdditive = &SPR_DrawAdditive;
    gHookedEngfuncs.pfnSPR_DrawHoles    = &SPR_DrawHoles;
/*
	pEngfuncs->pfnHookUserMsg = &HookUserMsg;
	//pEngfuncs->pfnHookEvent   = &HookEvent;
	
    pEngfuncs->pfnSPR_Load = &SPR_Load;
    pEngfuncs->pfnSPR_Set = &SPR_Set;
*/
	nResult = zHAi15.pInitialize(&gHookedEngfuncs,iVersion);
	/*if(optimizeNum < 3)
	{
		defCMD.execFile(ExeCfg);
		optimizeNum++;
	}*/

	static bool bFirstTime=false;
	if (!bFirstTime)
	{
		//zHAi.bHasShotSnap=((DWORD)pEngStudio==0x1dff340);

		keyBindManager.init();
		keyBindManager.CallBack_Execute = CB_KeyManager_Exec;
		keyBindManager.CallBack_ConType = CB_KeyManager_ConType;
		keyBindManager.con_visible = oEngfuncs.Con_IsVisible;

		gConsole.setcolortag( 'b', 128,128,255);
		gConsole.setcolortag( 'r', 255,128,128);
		gConsole.setcolortag( 'g',	80,222,140);
		gConsole.setcolortag( 'w', 255,255,255);
		gConsole.setcolortag( 'y', 255,255, 50);
		gConsole.setcolortag( 'o', 255,132,	0);

        gConsole.echo(" &b|[ &wzHAi HacK vER:&r5.0  &b]|");
        gConsole.echo(" &b|[ &wCoded By: &rzHAi    &b]|");
        gConsole.echo(" &b|[ &wQQ: &r45524562      &b]|");
        gConsole.echo("");
        gConsole.echo(" &r   &wIns &y==> &rMenu");
        gConsole.echo(" &r   &wDel &y==> &rConsole");
        gConsole.echo(" &r   &wEnd &y==> &rFirst Kill List");
		//==========================================================

		CommandInit();

		bFirstTime=true;
	}

	if (bFirstTime){
		static bool bStudioFirst=false;
		if (pEngStudio->GetModelByIndex && !bStudioFirst)
		{
			memcpy(&oEngStudio, pEngStudio, sizeof(oEngStudio));
			pEngStudio->StudioDrawPoints  =&Hud_StudioDrawPoints;
			pEngStudio->StudioEntityLight =&Hud_StudioEntityLight;
			bStudioFirst=false;
		}
	}
	return nResult;
}
//==========================================================

void CheckClientHook(FARPROC* pProc,LPCTSTR lpProcName)
{
	if (!strcmp(lpProcName,"Initialize")) 
	{
		zHAi15.pInitialize = (fInitialize)*pProc;
		*pProc = (FARPROC)Initialize;
	}
	else if (!strcmp(lpProcName, "HUD_Init"))
	{
		zHAi15.pHUD_Init = (fHUD_Init)*pProc;
		*pProc = (FARPROC)HUD_Init;
	}

	else if (!strcmp(lpProcName, "HUD_Redraw"))
	{
		zHAi15.pHUD_Redraw = (fHUD_Redraw)*pProc;
		*pProc = (FARPROC)HUD_Redraw;
	}
	else if (!strcmp(lpProcName, "HUD_UpdateClientData"))
	{
		zHAi15.pHUD_UpdateClientData = (fHUD_UpdateClientData)*pProc;
		*pProc = (FARPROC)HUD_UpdateClientData;
	}
	else if (!strcmp(lpProcName, "HUD_PlayerMove"))
	{
		zHAi15.pHUD_PlayerMove = (fHUD_PlayerMove)*pProc;
		*pProc = (FARPROC)HUD_PlayerMove;
	}
/*	else if (!strcmp(lpProcName, "HUD_PlayerMoveInit"))
	{
		zHAi15.pHUD_PlayerMoveInit = (fHUD_PlayerMoveInit)*pProc;
		*pProc = (FARPROC)HUD_PlayerMoveInit;
	}*/
	else if (!strcmp(lpProcName, "CL_CreateMove"))
	{
		zHAi15.pCL_CreateMove = (fCL_CreateMove)*pProc;
		*pProc = (FARPROC)CL_CreateMove;
	}
	else if (!strcmp(lpProcName, "V_CalcRefdef"))
	{
		zHAi15.pV_CalcRefdef = (fV_CalcRefdef)*pProc;
		*pProc = (FARPROC)V_CalcRefdef;
	}
	else if (!strcmp(lpProcName, "HUD_AddEntity"))
	{
		zHAi15.pHUD_AddEntity = (fHUD_AddEntity)*pProc;
		*pProc = (FARPROC)HUD_AddEntity;
	}
	else if (!strcmp(lpProcName, "HUD_Key_Event"))
	{
		zHAi15.pHUD_Key_Event = (fHUD_Key_Event)*pProc;
		*pProc = (FARPROC)HUD_Key_Event;
	}
	else if (!strcmp(lpProcName, "HUD_PostRunCmd"))
	{
		zHAi15.pHUD_PostRunCmd = (fHUD_PostRunCmd)*pProc;
		*pProc = (FARPROC)HUD_PostRunCmd;
	}
	else if (!strcmp(lpProcName, "HUD_Frame"))
	{
		zHAi15.pHUD_Frame = (fHUD_Frame)*pProc;
		*pProc = (FARPROC)HUD_Frame;
	}

	/*else if (!strcmp(lpProcName, "HUD_ProcessPlayerState"))
	{
		zHAi15.pHUD_ProcessPlayerState = (fHUD_ProcessPlayerState)*pProc;
		*pProc = (FARPROC)HUD_ProcessPlayerState;
	}*/
	else if (!strcmp(lpProcName, "HUD_TempEntUpdate"))
	{
		zHAi15.pHUD_TempEntUpdate = (fHUD_TempEntUpdate)*pProc;
		*pProc = (FARPROC)HUD_TempEntUpdate;
	}
	else if (!strcmp(lpProcName, "HUD_GetStudioModelInterface"))
	{
		zHAi15.pHUD_GetStudioModelInterface = (fHUD_GetStudioModelInterface)*pProc;
		*pProc = (FARPROC)HUD_GetStudioModelInterface;
	}
}

//--------------------------------------------------------------------
void PatchClientFunc( DWORD *ClientFunc_ADDR )
{
	//031364E0
	//logfile << ClientFunc_ADDR << endl;
	*( DWORD * )&zHAi15.pInitialize = *ClientFunc_ADDR;

	*ClientFunc_ADDR = ( DWORD )&Initialize;

	*( ( DWORD * )&zHAi15.pHUD_Init ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x04 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x04 ) ) = ( DWORD )&HUD_Init;

/*
	*( ( DWORD * )&zHAi15.pHUD_VidInit ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x08 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x08 ) ) = ( DWORD )&HUD_VidInit;
*/
	*( ( DWORD * )&zHAi15.pHUD_Redraw ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x0C ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x0C ) ) = ( DWORD )&HUD_Redraw;

	*( ( DWORD * )&zHAi15.pHUD_UpdateClientData ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x10 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x10 ) ) = ( DWORD )&HUD_UpdateClientData;
/*
	*( ( DWORD * )&zHAi15.pHUD_Reset ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x14 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x14 ) ) = ( DWORD )&HUD_Reset;
*/
	*( ( DWORD * )&zHAi15.pHUD_PlayerMove ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x18 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x18 ) ) = ( DWORD )&HUD_PlayerMove;
/*
	*( ( DWORD * )&zHAi15.pHUD_PlayerMoveInit ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x1C ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x1C ) ) = ( DWORD )&HUD_PlayerMoveInit;

	*( ( DWORD * )&zHAi15.pHUD_PlayerMoveTexture ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x20 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x20 ) ) = ( DWORD )&HUD_PlayerMoveTexture;

	*( ( DWORD * )&zHAi15.pIN_ActivateMouse ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x24 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x24 ) ) = ( DWORD )&IN_ActivateMouse;

	*( ( DWORD * )&zHAi15.pIN_DeactivateMouse ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x28 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x28 ) ) = ( DWORD )&IN_DeactivateMouse;

	*( ( DWORD * )&zHAi15.pIN_MouseEvent ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x2C ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x2C ) ) = ( DWORD )&IN_MouseEvent;

	*( ( DWORD * )&zHAi15.pIN_ClearStates ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x30 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x30 ) ) = ( DWORD )&IN_ClearStates;

	*( ( DWORD * )&zHAi15.pIN_Accumulate ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x34 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x34 ) ) = ( DWORD )&IN_Accumulate;
*/
	*( ( DWORD * )&zHAi15.pCL_CreateMove ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x38 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x38 ) ) = ( DWORD )&CL_CreateMove;
/*
	*( ( DWORD * )&zHAi15.pCL_IsThirdPerson ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x3C ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x3C ) ) = ( DWORD )&CL_IsThirdPerson;

	*( ( DWORD * )&zHAi15.pCL_CameraOffset ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x40 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x40 ) ) = ( DWORD )&CL_CameraOffset;
	
	*( ( DWORD * )&zHAi15.pKB_Find ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x44 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x44 ) ) = ( DWORD )&KB_Find;

	*( ( DWORD * )&zHAi15.pCAM_Think ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x48 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x48 ) ) = ( DWORD )&CAM_Think;
*/
	*( ( DWORD * )&zHAi15.pV_CalcRefdef ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x4C ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x4C ) ) = ( DWORD )&V_CalcRefdef;

	*( ( DWORD * )&zHAi15.pHUD_AddEntity ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x50 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x50 ) ) = ( DWORD )&HUD_AddEntity;
/*
	*( ( DWORD * )&zHAi15.pHUD_CreateEntities ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x54 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x54 ) ) = ( DWORD )&HUD_CreateEntities;

	*( ( DWORD * )&zHAi15.pHUD_DrawNormalTriangles ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x58 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x58 ) ) = ( DWORD )&HUD_DrawNormalTriangles;

	*( ( DWORD * )&zHAi15.pHUD_DrawTransparentTriangles ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x5C ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x5C ) ) = ( DWORD )&HUD_DrawTransparentTriangles;

	*( ( DWORD * )&zHAi15.pHUD_StudioEvent ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x60 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x60 ) ) = ( DWORD )&HUD_StudioEvent;
*/
	*( ( DWORD * )&zHAi15.pHUD_PostRunCmd ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x64 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x64 ) ) = ( DWORD )&HUD_PostRunCmd;
/*
	*( ( DWORD * )&zHAi15.pHUD_Shutdown ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x68 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x68 ) ) = ( DWORD )&HUD_Shutdown;

	*( ( DWORD * )&zHAi15.pHUD_TxferLocalOverrides ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x6C ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x6C ) ) = ( DWORD )&HUD_TxferLocalOverrides;

	*( ( DWORD * )&zHAi15.pHUD_ProcessPlayerState ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x70 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x70 ) ) = ( DWORD )&HUD_ProcessPlayerState;

	*( ( DWORD * )&zHAi15.pHUD_TxferPredictionData ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x74 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x74 ) ) = ( DWORD )&HUD_TxferPredictionData;

	*( ( DWORD * )&zHAi15.pDemo_ReadBuffer ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x78 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x78 ) ) = ( DWORD )&Demo_ReadBuffer;

	*( ( DWORD * )&zHAi15.pHUD_ConnectionlessPacket ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x7C ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x7C ) ) = ( DWORD )&HUD_ConnectionlessPacket;

	*( ( DWORD * )&zHAi15.pHUD_GetHullBounds ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x80 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x80 ) ) = ( DWORD )&HUD_GetHullBounds;
*/

	*( ( DWORD * )&zHAi15.pHUD_Frame ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x84 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x84 ) ) = ( DWORD )&HUD_Frame;

	*( ( DWORD * )&zHAi15.pHUD_Key_Event ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x88 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x88 ) ) = ( DWORD )&HUD_Key_Event;
	
	*( ( DWORD * )&zHAi15.pHUD_TempEntUpdate ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x8C ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x8C ) ) = ( DWORD )&HUD_TempEntUpdate;
/*
	*( ( DWORD * )&zHAi15.pHUD_GetUserEntity ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x90 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x90 ) ) = ( DWORD )&HUD_GetUserEntity;
	
	*( ( DWORD * )&zHAi15.pHUD_VoiceStatus ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x94 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x94 ) ) = ( DWORD )&HUD_VoiceStatus;

	*( ( DWORD * )&zHAi15.pHUD_DirectorMessage ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x98 ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x98 ) ) = ( DWORD )&HUD_DirectorMessage;
*/
	*( ( DWORD * )&zHAi15.pHUD_GetStudioModelInterface ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x9C ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x9C ) ) = ( DWORD )&HUD_GetStudioModelInterface;	
}

//===========================================================


//===========================================================

const unsigned long PAGE_SIZE = 4096;
_zHAiMemory::_zHAiMemory()
{
	hKernel32 = 0;
	pfnCreateToolhelp32Snapshot = 0;
	pfnProcess32First = 0;
	pfnProcess32Next = 0;
	pfnModule32First = 0;
	pfnModule32Next = 0;
	pfnCloseToolhelp32Snapshot = 0;
}

//===========================================================

_zHAiMemory::~_zHAiMemory()
{
	if ( hKernel32 )
		FreeLibrary (hKernel32);
}

//===========================================================

void _zHAiMemory::Error (const char* fmt, ...)
{
	va_list va_alist;
	char buf[512];
	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	szErrorMsg = buf;
}

//===========================================================

bool _zHAiMemory::Init()
{
	if ( (hKernel32 = LoadLibrary("kernel32.dll")) == 0 )
	{
		Error ("Could not open kernel32.dll: %s", strerror(errno));
		return false;
	}
	if ( (pfnCreateToolhelp32Snapshot = (HANDLE	(WINAPI*) (DWORD, DWORD)) GetProcAddress(hKernel32, "CreateToolhelp32Snapshot")) == 0 )
	{
		Error ("Could not get CreateToolhelp32Snapshot address\nIf you use Windows NT 4, you need libloader for NT.");
		return false;
	}
	if ( (pfnProcess32First = (BOOL (WINAPI*) (HANDLE, LPPROCESSENTRY32)) GetProcAddress(hKernel32, "Process32First")) == 0 )
	{
		Error ("Could not get Process32First address");
		return false;
	}
	if ( (pfnProcess32Next = (BOOL (WINAPI*) (HANDLE, LPPROCESSENTRY32)) GetProcAddress(hKernel32, "Process32Next")) == 0 )
	{
		Error ("Could not get Process32Next address");
		return false;
	}
	if ( (pfnModule32First = (BOOL (WINAPI*) (HANDLE, LPMODULEENTRY32)) GetProcAddress(hKernel32, "Module32First")) == 0 )
	{
		Error ("Could not get Module32First address");
		return false;
	}
	if ( (pfnModule32Next = (BOOL (WINAPI*) (HANDLE, LPMODULEENTRY32)) GetProcAddress(hKernel32, "Module32Next")) == 0 )
	{
		Error ("Could not get Process32Next address");
		return false;
	}
	pfnVirtualAllocEx = (LPVOID (WINAPI *) (HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect)) GetProcAddress(hKernel32, "VirtualAllocEx");
	pfnCloseToolhelp32Snapshot = (void (WINAPI*) (HANDLE)) GetProcAddress(hKernel32, "CloseToolhelp32Snapshot");
	return true;
}

//===========================================================

void _zHAiMemory::FreeProcessHandle(HANDLE hSnapShot)
{
	if ( hSnapShot != INVALID_HANDLE_VALUE )
	{
		if ( pfnCloseToolhelp32Snapshot )
			(*pfnCloseToolhelp32Snapshot)(hSnapShot);
		else
			CloseHandle(hSnapShot);
	}
}

//===========================================================
BOOL FindExe_Name( CONST CHAR *Target)
{
	CONST CHAR *index, *i, *j;
	CONST CHAR *srcName="cstrike";
	CONST CHAR *srcExt="exe";
	
    for( index=i=Target; *index; index++ )
	{
        if ( *index == '.')
			j=index+1;
        if ( *index == *srcName )
			i=index;
	}

    for ( ; *i && *srcName ; i++,srcName++ )
    {
        if ( tolower( *i ) != tolower( *srcName ) )
            return FALSE;
    }

    for ( ; *j && *srcExt ; j++,srcExt++ )
    {
        if ( tolower( *j ) != tolower( *srcExt ) )
            return FALSE;
    }

	return ( *srcName == 0 && *srcExt==0);
}
//===========================================================
bool _zHAiMemory::ExeLookup (DWORD pid, DWORD addr, MODULEENTRY32 * modentry)
{
	HANDLE hAllModules = INVALID_HANDLE_VALUE;
	bool fFound = false;
	if ( (hAllModules = (*pfnCreateToolhelp32Snapshot) (TH32CS_SNAPMODULE, pid)) == INVALID_HANDLE_VALUE )
		return false;
    modentry->dwSize = sizeof(MODULEENTRY32);
	if ( (*pfnModule32First) (hAllModules, modentry) )
	{
		for ( ; fFound == false; )
		{
			modentry->dwSize = sizeof(MODULEENTRY32);
			if ( FindExe_Name(modentry->szModule) )
			{
				FreeProcessHandle (hAllModules);
				return true;
			}
			else
			{
				if ( addr >= (DWORD) modentry->modBaseAddr 
				&&  addr <= ((DWORD) modentry->modBaseAddr + modentry->modBaseSize) )
				{
					FreeProcessHandle (hAllModules);
					return true;
				}
			}
			if ( !(*pfnModule32Next) (hAllModules, modentry) )
	        	break;
		}
	}
	FreeProcessHandle (hAllModules);    
	return false;
}

bool _zHAiMemory::FindExeInProcess ( DWORD pid, MODULEENTRY32 * modentry)
{
	if ( pid )
		return ExeLookup(pid, 0, modentry);

	HANDLE hAllProcesses = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 hProcEntry;
	hProcEntry.dwSize = sizeof(PROCESSENTRY32);
	if ( (hAllProcesses = (*pfnCreateToolhelp32Snapshot) (TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE )
	{
		Error ("CreateToolhelp32Snapshot: %s", strerror(errno));
        return false;
	}
	if ( (*pfnProcess32First) (hAllProcesses, &hProcEntry) )
	{
		for ( ; ; )
		{
			if ( hProcEntry.th32ProcessID != 0 )
			{
				if ( ExeLookup (hProcEntry.th32ProcessID, 0, modentry) )
				{
					FreeProcessHandle (hAllProcesses);
					return true;
				}
			}
			hProcEntry.dwSize = sizeof(PROCESSENTRY32);
            if ( !(*pfnProcess32Next) (hAllProcesses, &hProcEntry) )
        		break;
		}
	}
	FreeProcessHandle (hAllProcesses);
	return false;
}

/*
bool _zHAiMemory::FindModuleInProcess (const char * module_name, DWORD pid, MODULEENTRY32 * modentry)
{
	if ( pid )
		return ModulesLookup(pid, module_name, 0, modentry);

	HANDLE hAllProcesses = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 hProcEntry;
	hProcEntry.dwSize = sizeof(PROCESSENTRY32);
	if ( (hAllProcesses = (*pfnCreateToolhelp32Snapshot) (TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE )
	{
		Error ("CreateToolhelp32Snapshot: %s", strerror(errno));
        return false;
	}
	if ( (*pfnProcess32First) (hAllProcesses, &hProcEntry) )
	{
		for ( ; ; )
		{
			if ( hProcEntry.th32ProcessID != 0 )
			{
				if ( ModulesLookup (hProcEntry.th32ProcessID, module_name, 0, modentry) )
				{
					FreeProcessHandle (hAllProcesses);
					return true;
				}
			}
			hProcEntry.dwSize = sizeof(PROCESSENTRY32);
            if ( !(*pfnProcess32Next) (hAllProcesses, &hProcEntry) )
        		break;
		}
	}
	FreeProcessHandle (hAllProcesses);
	return false;
}

bool _zHAiMemory::ModulesLookup (DWORD pid, const char * name, DWORD addr, MODULEENTRY32 * modentry)
{
	char needed_name[256], module_name[256];
	HANDLE hAllModules = INVALID_HANDLE_VALUE;
	bool fFound = false;
	if ( name )
	{
		strcpy (needed_name, name);
		strupr (needed_name);
	}
	if ( (hAllModules = (*pfnCreateToolhelp32Snapshot) (TH32CS_SNAPMODULE, pid)) == INVALID_HANDLE_VALUE )
		return false;
    modentry->dwSize = sizeof(MODULEENTRY32);
	if ( (*pfnModule32First) (hAllModules, modentry) )
	{
		for ( ; fFound == false; )
		{
			modentry->dwSize = sizeof(MODULEENTRY32);
			if ( name )
			{
				strcpy (module_name, modentry->szModule);
				strupr (module_name);
				if ( strstr(module_name, needed_name) )
				{
					FreeProcessHandle (hAllModules);
					return true;
				}
			}
			else
			{
				if ( addr >= (DWORD) modentry->modBaseAddr 
				&&  addr <= ((DWORD) modentry->modBaseAddr + modentry->modBaseSize) )
				{
					FreeProcessHandle (hAllModules);
					return true;
				}
			}
			if ( !(*pfnModule32Next) (hAllModules, modentry) )
	        	break;
		}
	}
	FreeProcessHandle (hAllModules);    
	return false;
}
*/
//===========================================================

DWORD _zHAiMemory::FindPattern (DWORD pid, BYTE * pattern, DWORD pattern_length, DWORD start_addr, DWORD process_len)
{
	DWORD found_addr = 0;
	HANDLE hProcess = OpenProcess (PROCESS_VM_OPERATION|PROCESS_VM_READ|PROCESS_VM_WRITE, FALSE, pid);
	BYTE * tempbuf = new BYTE [PAGE_SIZE];
	if ( hProcess )
	{
		for ( DWORD current = 0; current < process_len && found_addr == 0; )
		{
			DWORD amount = process_len - current > PAGE_SIZE ? PAGE_SIZE : process_len - current;
			if ( !ReadFromProcessToBuf(hProcess, (void*) (start_addr + current), tempbuf, amount) )
				break;
			for ( int i = 0; i < amount - pattern_length; i++ )
			{
				if ( !memcmp (tempbuf+i, pattern, pattern_length) )
				{
					found_addr = start_addr + current + i;
					break;
				}
			}
			current += amount;
		}
		CloseHandle (hProcess);
	}
	else
		Error ("OpenProcess error: %s", strerror(errno));
	delete tempbuf;
	return found_addr;
}
//===========================================================
DWORD _zHAiMemory::ReadFromProcessToBuf (HANDLE hProcess, void * addr, BYTE * buf, DWORD length)
{
	DWORD returnlen = 0, readlen, old_attributes, old_attributes2;
	if ( VirtualProtectEx(hProcess, addr, length, PAGE_READWRITE, &old_attributes) )
	{
		if ( ReadProcessMemory (hProcess, addr, buf, length, &readlen) )
		{
			if ( VirtualProtectEx(hProcess, addr, length, old_attributes, &old_attributes2) )
			{
				returnlen = readlen;
			}
			else
				Error ("VirtualProtectEx error: %d", GetLastError());
		}
		else
			Error ("ReadProcessMemory error: %d", GetLastError());
	}
	else
		Error ("VirtualProtectEx error: %d", GetLastError());
	return returnlen;
}

//===========================================================
//===========================================================
void* RedirectFunction( char* pSource, int iNbBytesToCopy, DWORD pDestination ) 
{ 
	DWORD old_attributes;

	char *pTrampoline = new char[ iNbBytesToCopy + 5 ];
	memcpy( pTrampoline, pSource, iNbBytesToCopy );

	*( ( char * )( pTrampoline + iNbBytesToCopy ) ) = '\xE9';
	*( ( DWORD * )( pTrampoline + iNbBytesToCopy + 1 ) ) = (DWORD)pSource - ( (DWORD)pTrampoline + 5 );

	VirtualProtect( pSource, 5, PAGE_READWRITE, &old_attributes ); 
	*pSource = '\xE9';
	*( ( DWORD * )( pSource + 1 ) ) = pDestination - ( (DWORD)pSource + 5 ); 
	VirtualProtect( pSource, 5, old_attributes, &old_attributes ); 

	return ( void * )pTrampoline; 
}
int HW_DecodeDLL( DWORD dwBaseAddr, char **szDllName, DWORD *ExportTable, int iSize ) 
{ 
	//MessageBox(NULL,*szDllName,"HWText",MB_OK);
	DWORD retvalue; 
	static bool bAlreadyPatched = false; 

	retvalue = pHW_DecodeDLL( dwBaseAddr, szDllName, ExportTable, iSize ); 

	if( !bAlreadyPatched ) 
	{ 
		PatchClientFunc( ExportTable ); 
		bAlreadyPatched = true; 
	} 
	
	return( retvalue ); 
} 
int HL_DecodeDLL( DWORD dwBaseAddr, char **szDllName, DWORD *ExportTable, int iSize )
{
	//MessageBox(NULL,*szDllName,"Text",MB_OK);
	DWORD retvalue;
	DWORD dwSegmentAddr;
	DWORD HW_DecodeDLL_ADDR;

	retvalue = pHL_DecodeDLL( dwBaseAddr, szDllName, ExportTable, iSize );

	dwSegmentAddr = *( ( DWORD * )( dwBaseAddr + 0x5C ) );
	
	HW_DecodeDLL_ADDR = Patcher.FindPattern( HLEntry.th32ProcessID, ( BYTE * )HW_DecodeDLL_SIG, HW_DecodeDLL_SIG_LEN, ( DWORD )HLEntry.modBaseAddr, HLEntry.modBaseSize );

	pHW_DecodeDLL = ( int( * )( DWORD, char **, DWORD *, int ) )RedirectFunction( (char *)HW_DecodeDLL_ADDR, HW_DecodeDLL_SIG_LEN, ( DWORD )&HW_DecodeDLL );

	return( retvalue );
}
bool PatchHL( void )
{
	DWORD HL_DecodeDLL_ADDR;
	bool PathOk =false;
	if (!Patcher.Init())
		return (false);
	
	PathOk=Patcher.FindExeInProcess( GetCurrentProcessId(), &HLEntry);

	if (!PathOk) return (false);

	HL_DecodeDLL_ADDR = Patcher.FindPattern( HLEntry.th32ProcessID, ( BYTE * )HL_DecodeDLL_SIG, HL_DecodeDLL_SIG_LEN, ( DWORD )HLEntry.modBaseAddr, HLEntry.modBaseSize );
	
	if( !( pHL_DecodeDLL = ( int( * )( DWORD, char **, DWORD *, int ) )RedirectFunction( (char *)HL_DecodeDLL_ADDR, HL_DecodeDLL_SIG_LEN, ( DWORD )&HL_DecodeDLL ) ) )
		return( false );

	return( true );
}
//===========================================================
//===========================================================

const char * SPR_TOP        = "sprites/top";
const char * SPR_BOTTOM     = "sprites/bottom";
const char * SPR_LEFT       = "sprites/left";
const char * SPR_RIGHT      = "sprites/right";
const char * SPR_VERTICAL   = "sprites/vertical";
const char * SPR_HORIZONTAL = "sprites/horizontal";
    //const char * sprite_name    =  sprite_lookup(hPic); 
bool Sniper=false, Sniper_V=false, Sniper_H=false;

int SPR_Load( const char* szPicName )//Needed
{
	/*char AWPPath[256]="";
	HSPRITE spr;
    if( ((   !strncmp(szPicName, SPR_TOP,      strlen(SPR_TOP)    )
          || !strncmp(szPicName, SPR_BOTTOM,   strlen(SPR_BOTTOM) )
          || !strncmp(szPicName, SPR_LEFT,     strlen(SPR_LEFT)   )
          || !strncmp(szPicName, SPR_RIGHT,    strlen(SPR_RIGHT)  )
	     ) && (strstr(szPicName,"3.spr")!=NULL)
		)
    ||  !strncmp(szPicName, SPR_VERTICAL,   strlen(SPR_VERTICAL)  )
    ||  !strncmp(szPicName, SPR_HORIZONTAL, strlen(SPR_HORIZONTAL))
	)
    {
		//char *zPicName ="";
		//strncpy(zPicName, szPicName+8, strlen(szPicName)-7);
		sprintf(AWPPath,"zhai/%s",szPicName);
		//log("%s",AWPPath);
		spr = oEngfuncs.pfnSPR_Load(AWPPath);
    } else {
		spr = oEngfuncs.pfnSPR_Load(szPicName);
	}*/

    HSPRITE spr = oEngfuncs.pfnSPR_Load(szPicName);
	add_sprite(spr, szPicName);
	return spr;
}

int SPR_Frames( int hPic )
{
	return oEngfuncs.pfnSPR_Frames(hPic);
}

int SPR_Height( int hPic, int frame )
{
	return oEngfuncs.pfnSPR_Height(hPic,frame);
}

int SPR_Width( int hPic, int frame )
{
	return oEngfuncs.pfnSPR_Width(hPic,frame);
}

void SPR_Set( int hPic, int r, int g, int b )//Needed
{
    const char * szPicName =  sprite_lookup(hPic);
	/*!strncmp(szPicName, SPR_TOP,        strlen(SPR_TOP)       )
     || !strncmp(szPicName, SPR_BOTTOM,     strlen(SPR_BOTTOM)    )
     || !strncmp(szPicName, SPR_LEFT,       strlen(SPR_LEFT)      )
     || !strncmp(szPicName, SPR_RIGHT,      strlen(SPR_RIGHT)     )
     || */
	Sniper_V=!strncmp(szPicName, SPR_VERTICAL, strlen(SPR_VERTICAL));  //¸ß
	Sniper_H=!strncmp(szPicName, SPR_HORIZONTAL, strlen(SPR_HORIZONTAL));  //³¤
    if( Sniper_V || Sniper_H)
	{
		Sniper = true ;
	} else {
		Sniper = false;
	}
	oEngfuncs.pfnSPR_Set (hPic,r,g,b);
}

void SPR_Draw( int frame, int x, int y, const wrect_t* prc )
{
	if (Sniper){
		if(!zHAi.bShotMaked && zHAi.bGameActive){
			int iW_L=0, iW_R=0, iH_T=0, iH_B=0, iW=displayCenterX*2, iH=displayCenterY*2
				, iW_Ba=0, iH_Ba=0, iSize=256*3,iAlpha=255*0.4;
			if (x==0 && y==0 && iW>=800) {
				if (iH>=800) iAlpha=255*0.22;

				iW_R  = ((iW-iSize)/2);
				iW_L  = iW_R;

				iH_B  = ((iH-iSize)/2);
				iH_T  = iH_B;

				oglSubtractive = true;
				if (iW_L>0) oEngfuncs.pfnFillRGBA(0         , 0         , iW_L  , iH  , 0, 0, 0, iAlpha); //Left
				if (iW_R>0) oEngfuncs.pfnFillRGBA(iW_L+iSize, 0         , iW_R  , iH  , 0, 0, 0, iAlpha); //Right
				if (iH_T>0) oEngfuncs.pfnFillRGBA(iW_L      , 0         , iSize , iH_T, 0, 0, 0, iAlpha); //Top
				if (iH_B>0) oEngfuncs.pfnFillRGBA(iW_L      , iH_T+iSize, iSize , iH_B, 0, 0, 0, iAlpha); //Bottom
				oglSubtractive = false;
			}
			return;
		}
	}
	oEngfuncs.pfnSPR_Draw (frame, x, y, prc);
}

void SPR_DrawHoles( int frame, int x, int y, const wrect_t* prc )
{
	if(!zHAi.bShotMaked && zHAi.bGameActive && Sniper){
		return;
	} else {
		oEngfuncs.pfnSPR_DrawHoles (frame, x, y, prc);
	}
}

void SPR_DrawAdditive( int frame, int x, int y, const wrect_t* prc )
{
	oEngfuncs.pfnSPR_DrawAdditive(frame,x,y,prc);
}

void SPR_EnableScissor( int x, int y, int width, int height )
{
	oEngfuncs.pfnSPR_EnableScissor( x, y, width, height );
}

void SPR_DisableScissor( void )
{
	oEngfuncs.pfnSPR_DisableScissor();
}

client_sprite_t* SPR_GetList( char* psz, int* piCount )
{
	return oEngfuncs.pfnSPR_GetList(psz,piCount);
}
