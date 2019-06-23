#ifndef SoundEHook_H
#define SoundEHook_H
	#pragma warning(disable:4091)
	#include <windows.h>
	#include "timehandling.h"
	#include "color.h"
	//===========================================================
	void S_StartDynamicSound( int entnum, int entchannel, char* sample, float* origin, float volume, float attenuation, int timeofs , int pitch );
	void ApplySoundHook( void );
	typedef void (*sound_t)(int,int,char*,float*,float,float,int,int);
	//===========================================================
	typedef struct svc_entry_s
	{
		DWORD dwId;			// +0 - id :) should be unique
		char  *szName;		// +4 - pointer to the name string
		DWORD pfnHandler;	// +C - the parser itself
	};

	class cSoundEsp
	{
		private:

			inline float GetPseudoDistance(const float* const pos1, const float* const pos2);
			inline void strcpy_x(char* dest, const char* pos);	

		public:

			void HUD_Redraw();
			void PreS_DynamicSound(const DWORD a,const DWORD b,const DWORD c,const char*const sample, const float*const origin,DWORD f,const DWORD g,const DWORD h);
	};

	extern cSoundEsp gSoundEsp;
//===========================================================
#endif