#ifndef CVARS_H
#define CVARS_H

	#include <windows.h>
	#include <mmsystem.h>
	#include <vector>
	#include <assert.h>
	#include <time.h>
	#include <stdio.h>
	#include <Winbase.h>
	#include <tlhelp32.h>

	typedef unsigned long DWORD;

	class CVARS // cvars (of course ;P)
	{
	public:
		void Init();
		void Save();
		void Load();

	public:

		float radar; 
		float radar_x; 
		float radar_y; 
		float radar_size; 
		float floke;

		float chase_cam;
		float reload_meter;
		float sprites;

		float recoil;
		float menukey;
		float consolekey;
		float KillKey;

		// menu sub shit
		float Menu1,Menu2;

		float FarSound;
		//float Bones;
		float BigHead;
		
		//AIMBOT cvars
		float fov;
		float FirstKill_Mode;
		char zHAi_Check[15], zHAi_CheckQQ[15], zHAi_OrgQQ[15];

		float disaim;
	};

	extern CVARS cvar;

	void log(const char *fmt, ... );
	inline std::string getHackDirFile(const char* basename);
#endif
