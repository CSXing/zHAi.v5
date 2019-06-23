/**********************************************************
¡ô©©©Ç zHA! H00k vEr:5.0 ©Ï©©¡ô
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
¡ô©©©Ç      (c)2018      ©Ï©©¡ô
    File Name: cSoundHook.cpp
************************************************************/
#pragma warning(disable:4091)
#include <map>
#include <string>
#include "cPatch.h"
#include "cvars.h"
#include "cSoundHook.h"
#include "client.h"
#include "players.h"

static ColorEntry dummycolor;
class SoundMarker
{
public:
	SoundMarker() : color(&dummycolor),priority(0) {}
	float       origin[3];
	char        description[16];
	int         team;
	int         index;
	StopTimer   timer;
	ColorEntry* color;
	int         priority;
};
typedef vector<SoundMarker>  SoundMarkerVector;
SoundMarkerVector vecSoundMarker(65);
int soundMarkerCurrentIndex = 0;

DWORD g_dwHW_Base = 0x01D0D000,g_dwHW_Len = 0x000645a4; // check base for validity might be 
DWORD g_dwCL_ParseSound_Begin	= NULL;
DWORD g_dwCL_ParseSound_Call	= NULL;
DWORD g_dwCL_ParseSound_End		= NULL;
DWORD g_dwSvc_Table				= NULL;


svc_entry_s*	pSvcDispatch;
sound_t			pSound;
//extern		cCalculate *pCalc;
cSoundEsp		gSoundEsp;
//extern			cl_enginefuncs_s *pEngfuncs;

#define BOUND_VALUE(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}

//===========================================================
inline void cSoundEsp::strcpy_x(char* dest, const char* pos)
{
	do{ 
		while( *pos>='0' && *pos<='9' )  ++pos; // ignore numbers
		
		if(*pos=='.') {*dest=0;break; } // cut off ".wav"
		*dest=*pos;
		++dest;
		++pos; 
	}
	while(*pos);
}

//===========================================================
inline float cSoundEsp::GetPseudoDistance(const float* const pos1, const float* const pos2)
{
	return
		fabs(pos1[0]-pos2[0])+
		fabs(pos1[1]-pos2[1])+
		fabs(pos1[2]-pos2[2]);
}

//===========================================================
void cSoundEsp::PreS_DynamicSound(const DWORD a,const DWORD b,const DWORD c,const char*const sample, const float*const origin,DWORD f,const DWORD g,const DWORD h)
{
	//if( GetPseudoDistance(origin,me.pmEyePos)<cvar.disaim){ return; }

	BOUND_VALUE(soundMarkerCurrentIndex,0,64);
	SoundMarker& ref = vecSoundMarker[soundMarkerCurrentIndex];

	string theMessage;

	if(b>0 && b<=36) {
		vPlayers[b].m_Index = b;
		vPlayers[b].Refresh();
		theMessage = vPlayers[b].Name();
		ref.team = vPlayers[b].m_Team;
	} else return;
	
	if(strstr(sample,"player/pl_ladder")) {
		int len = theMessage.length();
		theMessage+=" is using ladder!";
	}

	if(sample[0]=='p' && sample[3]=='y' && sample[6]=='/') { // "player/pl_step1.wav"
		if( GetPseudoDistance(origin,me.pmEyePos)<cvar.disaim ){ return; }
		const char* pos = sample+7;
		if(pos[0]=='p' && pos[2]=='_') // pl_xx
		{
			if(pos[3]=='s' && pos[4]=='h' && pos[5]=='e' && pos[6]=='l' )  return; // pl_shell#      
	
			pos+=3;
			ref.color = colorList.get(1+ref.team);
			ref.priority = 0;
		}
		strcpy_x(ref.description,pos);
	} else if(sample[0]=='a' && sample[1]=='m' && sample[8]=='/') { // "ambience/3dmbridge.wav"
		if( GetPseudoDistance(origin,me.pmEyePos)<cvar.disaim ){ return; }
		if(!strcmp(sample+9,"wind.wav")) strcpy(ref.description,"");
		//ref.color = colorList.get(5); // snd_special
		//ref.priority = 2;
	//}
	//else
	//if(sample[0]=='a' && sample[1]=='m' && sample[8]=='/')  // "ambience/3dmbridge.wav"
	//{
		if(!strcmp(sample+9,"rain.wav")) strcpy(ref.description,"");
		//ref.color = colorList.get(5); // snd_special
		//ref.priority = 2;
	//}
	//else
	//if(sample[0]=='a' && sample[1]=='m' && sample[8]=='/')  // "ambience/3dmbridge.wav"
	//{
		if(!strcmp(sample+9,"3dmbridge.wav")) strcpy(ref.description,"");
		ref.color = colorList.get(5); // snd_special
		ref.priority = 2;
	} else if(sample[0]=='p' && sample[3]=='t' && sample[5]=='/') {// "plats/vehicle.."
		if( GetPseudoDistance(origin,me.pmEyePos)<cvar.disaim ){ return; }
		strcpy(ref.description,"");
		ref.color = colorList.get(5); // snd_special
		ref.priority = 2;
	} else if(sample[0]=='h' && sample[1]=='o' && sample[2]=='s') {// "hostage/hos1.."
		if( GetPseudoDistance(origin,me.pmEyePos)<cvar.disaim ){ return; }
		strcpy(ref.description,"");
		ref.color = colorList.get(5); // snd_special
		ref.priority = 1;
	} else if( sample[0]=='w' && sample[1]=='e' && sample[7]=='/' ) {// weapons/ric1.wav
		const char* pos = sample+8;
		if(   (pos[0]=='r' && pos[1]=='i')    // ricxxx
			||(pos[0]=='b' && pos[1]=='u') )  // bullet-hit...
		{
			return;
		}
		if( GetPseudoDistance(origin,me.pmEyePos)<cvar.disaim ){ return; }
		ref.color = colorList.get(1+ref.team);
		ref.priority = 1;
		strcpy_x(ref.description,pos);
		for(char *pos1=ref.description;*pos1;++pos1){ *pos1 = toupper(*pos1); }
	} else { return; }

	vPlayers[b].Quick_Update( origin );
	return;

	// activate sound marker
	VectorCopy( origin, ref.origin );
	++soundMarkerCurrentIndex;
	if(soundMarkerCurrentIndex>=32){ soundMarkerCurrentIndex=0;}

}

//===========================================================
void cSoundEsp::HUD_Redraw()
{
	static bool a = false;
	if(!a)
	{
		int b = 32;
		BOUND_VALUE  (b,0,64);
		for( int i=0;i<b;i++)
		{
			vecSoundMarker[i].timer.setExpired();
		}
		void ApplySoundHook( void );
		ApplySoundHook();
		a = true;
	}
}

//===========================================================
void S_StartDynamicSound( int entnum, int entchannel, char* sample, float* origin, float volume, float attenuation, int timeofs , int pitch )
{
	// process_sound( entnum , sample , origin );
	if( entnum != oEngfuncs.GetLocalPlayer()->index)
		gSoundEsp.PreS_DynamicSound(0,(DWORD)entnum,0,sample,origin,0,0,0);

	(*pSound)(entnum,entchannel,sample,origin,volume,attenuation,timeofs,pitch);
}

//===========================================================
DWORD dwHook_SvcEntry( DWORD dwBegin,	 // begin of the parser
					  DWORD dwEnd,		 // end of the parser
					  DWORD dwCall,	 // call to the original handler
					  DWORD dwHandler,  // our handler
					  int iPos			 // svc_table element ##x
					  )
{	
	//Make a copy of the original Handler
	DWORD dwMy_Handler			= pPatch->dwCopyRange( dwBegin , dwEnd );
	//Hook Svc_Table ( hw.dll )
	pSvcDispatch[iPos].pfnHandler = (DWORD)dwMy_Handler;
	//Redirect Call
	return pPatch->dwRedirectCall( ( dwMy_Handler + dwCall ), dwHandler );
}

//===========================================================
void ApplySoundHook( void )
{
	g_dwCL_ParseSound_Begin = pPatch->dwFindPattern( g_dwHW_Base,g_dwHW_Len,(BYTE*)"\x83\xEC\x68\x53\x55\x56\x57\x68\x00\x00\x00\x00\xE8","xxxxxxxx????x");
	g_dwCL_ParseSound_Call  = pPatch->dwFindPattern( g_dwHW_Base,g_dwHW_Len,(BYTE*)"\x50\x57\xE8\x00\x00\x00\x00\x83\xC4\x20\x5F\x5E\x5D\x5B\x83\xC4\x68\xC3","xxx????xxxxxxxxxxx");
	g_dwCL_ParseSound_End	= pPatch->dwFindPattern( g_dwHW_Base,g_dwHW_Len,(BYTE*)"\x50\x57\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x0C\x5F\x5E\x5D\x5B\x83\xC4\x68\xC3","xxx????x????xxxxxxxxxxx");
	g_dwSvc_Table			= pPatch->dwFindPattern( g_dwHW_Base,g_dwHW_Len,(BYTE*)"\x51\x53\x56\x57\x68\xFF\xFF\xFF\xFF\x33\xDB\xE8\xFF\xFF\xFF\xFF\x83\xC4\x04\x33\xF6\xBF","xxxxx????xxx????xxxxxx");

	//correct vals
	g_dwSvc_Table			 += 0x15;
	g_dwCL_ParseSound_End	 += 0x16;
	g_dwCL_ParseSound_Call	 += 2;
	g_dwCL_ParseSound_Call	 -= g_dwCL_ParseSound_Begin;
	//set locations
	pSvcDispatch			  = (svc_entry_s*)(*(DWORD*)(g_dwSvc_Table + 1 ) - 4 );

	pSound = (sound_t)dwHook_SvcEntry( g_dwCL_ParseSound_Begin ,
		g_dwCL_ParseSound_End ,
		g_dwCL_ParseSound_Call ,
		(DWORD)S_StartDynamicSound,
		6 );
}

//===========================================================