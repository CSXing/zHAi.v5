/**********************************************************
◆┅┣ zHA! H00k vEr:5.0 ┫┅◆
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
◆┅┣      (c)2018      ┫┅◆
    File Name: cVars.cpp
************************************************************/
#pragma warning (disable:4786)
#include <windows.h>
#include <stdio.h>
#include <fstream>
#include "cvars.h"
#include "DefineCmds.h"
#include "client.h"
#include "xorstr.h"

CVARS cvar;

//===========================================================
inline std::string getHackDirFile(const char* basename)
{
	if(strstr(basename,"..")){ return ":*?\\/<>\""; }
	string ret = hackdir;
	return (ret+basename);
}

//===========================================================
bool fileExists(const char* filename)
{
	WIN32_FIND_DATA finddata;
	HANDLE handle = FindFirstFile(filename,&finddata);
	return (handle!=INVALID_HANDLE_VALUE);
}

//===========================================================
void log(const char *fmt, ... ) 
{ 	
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	ofstream fout; 
	string filepath = getHackDirFile("log.txt");
	fout.open(filepath.c_str(), ios::app); 
	fout << buf << endl; 
	fout.close(); // save & close file		
}

//===========================================================
void CVARS::Init()
{
	#define Init_Var(name,value) defCMD.AddCvarFloat(#name, &##name );##name=##value;
	
	Init_Var(radar,2)	Init_Var(radar_x,90)	Init_Var(radar_y,105)	Init_Var(radar_size,80)
	Init_Var(floke,1)
	Init_Var(fov,1)

	Init_Var(BigHead,0)       //大头模式
	Init_Var(FarSound,1)      //远透方框
	Init_Var(chase_cam,0)     //第三人称
	Init_Var(reload_meter,0)  //动作提示
	Init_Var(sprites,0)       //人物枪模
	Init_Var(disaim,256)      //自瞄距离
	Init_Var(recoil,0)        //反后座力--实际数值
	Init_Var(menukey,147)     //菜单按键   Ins
	Init_Var(consolekey,152)  //控制台按键 End
	Init_Var(KillKey,148)      //专杀按键  Del
	Init_Var(FirstKill_Mode,0) //专杀模式  0/1
	//防截图  10秒
}

//===========================================================
void CVARS::Save()
{
	ofstream ofs( getHackDirFile(DllCfg).c_str() );
	defCMD.save_cvars(ofs);
	ofs.close();
}

//===========================================================
void CVARS::Load()
{
	if( !fileExists(getHackDirFile(DllCfg).c_str()) ) return;
	defCMD.load_cvars();
}
