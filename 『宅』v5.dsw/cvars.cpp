/**********************************************************
������ zHA! H00k vEr:5.0 �ϩ���
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
������      (c)2018      �ϩ���
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

	Init_Var(BigHead,0)       //��ͷģʽ
	Init_Var(FarSound,1)      //Զ͸����
	Init_Var(chase_cam,0)     //�����˳�
	Init_Var(reload_meter,0)  //������ʾ
	Init_Var(sprites,0)       //����ǹģ
	Init_Var(disaim,256)      //�������
	Init_Var(recoil,0)        //��������--ʵ����ֵ
	Init_Var(menukey,147)     //�˵�����   Ins
	Init_Var(consolekey,152)  //����̨���� End
	Init_Var(KillKey,148)      //רɱ����  Del
	Init_Var(FirstKill_Mode,0) //רɱģʽ  0/1
	//����ͼ  10��
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
