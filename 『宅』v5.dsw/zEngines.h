#ifndef zEngines_H
#define zEngines_H
	#include <gl\gl.h>
	#include <gl\glu.h>
	#include <gl\glaux.h>
	#include <deque>
	#include <Lmcons.h>

	//==============================================================================
	typedef struct zHAi_v15_Client_s
	{
		void( *pCAM_Think )( void );
		void( *pCL_CameraOffset )( float * );
		void( *pCL_CreateMove )( float, struct usercmd_s *, int );
		int( *pCL_IsThirdPerson )( void );
		void( *pDemo_ReadBuffer )( int, unsigned char * );
		int( *pHUD_AddEntity )( int, struct cl_entity_s *, const char * );
		int( *pHUD_ConnectionlessPacket )( struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );
		void( *pHUD_CreateEntities )( void );
		int( *pHUD_DirectorEvent )( unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags );
		void( *pHUD_DrawNormalTriangles )( void );
		void( *pHUD_DrawTransparentTriangles )( void );
		void( *pHUD_Frame )( double time );
		int( *pHUD_GetHullBounds )( int hullnumber, float *mins, float *maxs );
		struct cl_entity_s *( *pHUD_GetUserEntity )( int index );
		int( *pHUD_Init )( void );
		int( *pHUD_Key_Event )( int, int, const char * );
		void( *pHUD_PlayerMove )( struct playermove_s *, int );
		void( *pHUD_PlayerMoveInit )( struct playermove_s * );
		char( *pHUD_PlayerMoveTexture )( char * );
		void( *pHUD_PostRunCmd )( struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed );
		void( *pHUD_ProcessPlayerState )( struct entity_state_s *, const struct entity_state_s * );
		int( *pHUD_Redraw )( float, int );
		int( *pHUD_Reset )( void );
		void( *pHUD_Shutdown )( void );
		void( *pHUD_StudioEvent )( const struct mstudioevent_s *, const struct cl_entity_s * );
		void( *pHUD_TempEntUpdate )( double, double, double, struct tempent_s **, struct tempent_s **, int( *Callback_AddVisibleEntity )( struct cl_entity_s *pEntity ), void( *Callback_TempEntPlaySound )( struct tempent_s *pTemp, float damp ) );
		void( *pHUD_TxferLocalOverrides )( struct entity_state_s *, const struct clientdata_s * );
		void( *pHUD_TxferPredictionData )( struct entity_state_s *, const struct entity_state_s *, struct clientdata_s *, const struct clientdata_s *, struct weapon_data_s *, const struct weapon_data_s * );
		int( *pHUD_UpdateClientData )( client_data_t *, float );
		int( *pHUD_VidInit )( void );
		int( *pHUD_VoiceStatus )( int entindex, qboolean bTalking );
		void( *pIN_Accumulate )( void );
		void( *pIN_ActivateMouse )( void );
		void( *pIN_ClearStates )( void );
		void( *pIN_DeactivateMouse )( void );
		void( *pIN_MouseEvent )( int mstate );
		int( *pInitialize )( cl_enginefunc_t *, int );
		struct kbutton_s *( *pKB_Find )( const char *name );
		void( *pV_CalcRefdef )( struct ref_params_s *pparams ); 
		void (*pHUD_DirectorMessage)(unsigned char, unsigned int, unsigned int, unsigned int);
		int  (*pHUD_GetStudioModelInterface)( int, struct r_studio_interface_s **, struct engine_studio_api_s *);

		int	 ( *pStudioDrawPlayer )( int flags, struct entity_state_s *pplayer );
	} zHAi_v15_Client_t;

	extern zHAi_v15_Client_t zHAi15;

	//==============================================================================
	void CheckClientHook(FARPROC* pProc,LPCTSTR lpProcName);
	void PatchClientFunc( DWORD *ClientFunc_ADDR );
	bool PatchHL( void );
	//==============================================================================
/*
	bool zHAi_AntiScreen_Hook();

	typedef void(*con_func)();

	class zHAi_AntiScreen
	{
		public:
			GLvoid * ClearPixels;
			DWORD iReadTimer;
			bool bHasShotSnap,bGameActive,IsHacked;
			static bool bShot_Snap, bShot_Screen, bShot_ReadPixels, bShotMaked;

			int iUpdateLast, iUpdateCurrent;
			static int iSnapTimer, iScreenTimer, iReadPixelsTimer;

			void AntiScreen(){
				if (!bHasShotSnap) return ;

				if(bShot_Snap){
					iSnapTimer--;
					if(iSnapTimer <= 0){
						if(RealShot_Snap)
							RealShot_Snap();

						iSnapTimer = 5;
						bShot_Snap = false;
					}
				}

				if(bShot_Screen){
					iScreenTimer--;
					if(iScreenTimer <= 0){
						if(RealShot_Screen)
							RealShot_Screen();

						iScreenTimer = 5;
						bShot_Screen = false;
					}
				}
				bShotMaked = ( bShot_Snap || bShot_Screen || bShot_ReadPixels );
			}
			static void Func_SnapShot(), Func_ScreenShot();
			con_func RealShot_Screen, RealShot_Snap;
			bool Hook(){ return zHAi_AntiScreen_Hook();}
	};
	extern zHAi_AntiScreen zHAi;
*/
	
	typedef void(__stdcall *glReadPixels_t)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid*);
	void __stdcall m_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
	extern glReadPixels_t glReadPixels_s;
	void Snapshot();

	class zHAi_AntiScreen
	{
	public:
		zHAi_AntiScreen(){ bGameActive=false; bShotMaked=true; }
		bool bShotMaked, bGameActive;
		int iUpdateLast, iUpdateCurrent;
	};
	extern zHAi_AntiScreen zHAi;
	//==============================================================================
	class _zHAiMemory
	{
		public:
    		_zHAiMemory();
			~_zHAiMemory();
			bool	Init();
			bool	FindExeInProcess (DWORD pid, MODULEENTRY32 * modentry);
			//bool	FindModuleInProcess (const char * module_name, DWORD pid, MODULEENTRY32 * modentry);
			DWORD	FindPattern (DWORD pid, BYTE * pattern, DWORD pattern_length, DWORD start_addr, DWORD process_len);
		private:
			void		Error (const char * fmt, ...);
			void		FreeProcessHandle(HANDLE hSnapShot);
			bool		ExeLookup (DWORD pid, DWORD addr, MODULEENTRY32 * modentry);
			//bool		ModulesLookup (DWORD pid, const char * name, DWORD addr, MODULEENTRY32 * modentry);
			DWORD		ReadFromProcessToBuf (HANDLE hProcess, void * addr, BYTE * buf, DWORD length);
			std::string	szErrorMsg;
			HMODULE		hKernel32;
			HANDLE		(WINAPI *pfnCreateToolhelp32Snapshot) (DWORD, DWORD);
			BOOL		(WINAPI *pfnProcess32First) (HANDLE, LPPROCESSENTRY32);
			BOOL		(WINAPI *pfnProcess32Next) 	(HANDLE, LPPROCESSENTRY32);
			BOOL 		(WINAPI *pfnModule32First)  (HANDLE, LPMODULEENTRY32);
			BOOL 		(WINAPI *pfnModule32Next)   (HANDLE, LPMODULEENTRY32);
			void 		(WINAPI *pfnCloseToolhelp32Snapshot) (HANDLE);
			LPVOID 		(WINAPI *pfnVirtualAllocEx) (HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
	};

#endif