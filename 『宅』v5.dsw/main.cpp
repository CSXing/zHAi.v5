/**********************************************************
◆┅┣ zHA! H00k vEr:5.0 ┫┅◆
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
◆┅┣      (c)2018      ┫┅◆
    File Name: Main.cpp
************************************************************/
#pragma warning (disable:4786)

#include <windows.h>
#include <tlhelp32.h>
#include "apihook.h"
#include "detours.h"
#include "client.h"
#include "opengl.h"
//#include "bind.h"
#include "cvars.h"
#include "xorstr.h"
//#include "utils.h"
#include "zEngines.h"

//==============================================================================
//==================================================================================
int exit_par=0;
DWORD exit_zhai;
DWORD exit_bye = (DWORD)&ExitProcess;
void zHAi_Sale(int* shit, bool fuck)
{
	__asm mov ebx, ExitProcess
	__asm mov exit_zhai, ebx
	__asm push exit_par
	__asm call exit_bye
	__asm xor ebx,ebx
	__asm mov edx, 1
	__asm push edx
	__asm call exit
	ExitProcess(0);
}

float zHAi_Sale2(int* shit, bool fuck)
{
	float fv = 0.1f;
	double* da=(double*)malloc(sizeof(double));
	*da=fv;
	float vf[3];
	vf[0]=0;vf[1]=0;vf[2]=fv;
	free(da);
	ExitProcess(0);
	vf[0]=fv*2;vf[1]=fv/2;
	return vf[2];
}
//==============================================================================
//==============================================================================
int isFindQQ=0;
int QQ_MaxCount=5;     //QQ句柄最大数量
int QQ_Count = 0;      //QQ句柄数量
HWND QQ_hWnd[5];       //存放QQ句柄
char QQ_Number[5][15]; //存放QQ号的
bool bLicensed;
//==============================================================================
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	if (QQ_Count>=QQ_MaxCount) {return 1;}
	char Name[37],ckName[37]= "5B3838F5-0C81-46D9-A4C0-6EA28CA3E942";//存放获取句柄的窗口类名
	GetClassName(hwnd, Name, sizeof(char) * sizeof(Name));           //获取窗口类名存入到“Name”中
	if (strcmp(Name, ckName)==0)                                     //比较“Name”是不是想要的类名
	{
		QQ_hWnd[QQ_Count] = hwnd;
		QQ_Count++;
	}
	return 1;
}

//==============================================================================
void GetQQ_Number()
{
	QQ_Count=0;
	EnumWindows(EnumWindowsProc,0);
	for (int i = 0; i<QQ_Count;i++)
	{
		char temp[255];
		int Tlong = 0,y=0, n;                         //获取返回的字符长度
		Tlong=GetWindowText(QQ_hWnd[i],temp, 255); //获取窗口标题
		y = Tlong - strlen("qqexchangewnd_shortcut_prefix_");
		char*p[10];                                //字符串指针，最大限制为10  即本地QQ号最多只能取10个
		p[i] = (char*)malloc(sizeof(char)*y);      //申请动态内存
		for (n = 0;y>n ; n++)
		{
			QQ_Number[i][n] = temp[30 + n];        //从第 30个起，后面的都是QQ号
		}
		QQ_Number[i][n] ='\0';
	}
}

bool xLice_Checker(char *Message)
{
	char zLice_QQ[6][15] ;//, *pBuf=new char[64];
	int i=0, Id=0, intQQ,HigQQ,LowQQ;
	bLicensed=false;
	isFindQQ=0;
	/*45524562000000*/
	sprintf(zLice_QQ[0],XorStr<0xE6,3,0xA26FC4C5>("\xC3\x94"+0xA26FC4C5).s,XorStr<0x87,15,0xC1CEEC92>("\xB3\xBD\xBC\xB8\xBF\xB9\xBB\xBC\xBF\xA0\xA1\xA2\xA3\xA4"+0xC1CEEC92).s);

	/*45524562000000*/ /*xor*/ /* QQ: 91095595100000*/
	/*36519057100000*/
	sprintf(zLice_QQ[3],XorStr<0x3F,15,0x11E0F969>("\x0C\x76\x74\x73\x7A\x74\x70\x71\x76\x78\x79\x7A\x7B\x7C"+0x11E0F969).s);
	
	/*45524562000000*/ /*xor*/ /* QQ: 38182835800000*/
	/*73606397800000  1v5 */
	//sprintf(zLice_QQ[3],XorStr<0xE6,15,0xA26FC4C5>("\xD1\xD4\xDE\xD9\xDC\xD8\xD5\xDA\xD6\xDF\xC0\xC1\xC2\xC3"+0xA26FC4C5).s);

	/*45524562000000*/ /*xor*/ /* QQ: 12066350100000*/
	/*57580812100000  88 */
	sprintf(zLice_QQ[3],XorStr<0xE6,15,0xA26FC4C5>("\xD1\xD4\xDE\xD9\xDC\xD8\xD5\xDA\xD6\xDF\xC0\xC1\xC2\xC3"+0xA26FC4C5).s);

	strcpy(cvar.zHAi_Check,zLice_QQ[3]);
	bLicensed=true;
//机器码校验(QQ号)
	GetQQ_Number();
	isFindQQ=9;
	bLicensed=false;

	if (QQ_Count==0 || zLice_QQ[3][0]=='\0' ){
		//sprintf(Message,"请登录你的QQ，再启动游戏！\n作者QQ:%s",zLice_QQ[0]);
		sprintf(Message,XorStr<0x4E,37,0xB185828B>("\x89\xA4\xE5\x96\x90\xEF\x90\xB6\xE3\x93\x09\x08\xF9\xF7\x88\x84\x98\xAB\xD6\xCE\xB1\xAD\xAB\xD2\xC5\xC6\x62\xBE\x9D\xBE\xB3\x3C\x3F\x55\x55\x02"+0xB185828B).s
			,XorStr<0xCF,10,0x2BD51B07>("\xF6\xE1\xE1\xEB\xE6\xE1\xEC\xE3\xE6"+0x2BD51B07).s);
		return false;
	}
	bLicensed=true;
	for (i = 0; i<QQ_Count;i++){
		for (Id=0;Id<15;Id++)
		{
			HigQQ=(int)(zLice_QQ[0][Id]-'0');
			LowQQ=(int)(QQ_Number[i][Id]-'0');
			intQQ=HigQQ+LowQQ;
			if (intQQ>9) intQQ-=10;
			zLice_QQ[5][Id]=intQQ+'0';
		}
		strcpy(cvar.zHAi_OrgQQ,QQ_Number[i]);

		if (strstr(zLice_QQ[3],zLice_QQ[5])){
			isFindQQ++;
			bLicensed=true;
			//sprintf(Message,"校验QQ:%s完成！",QQ_Number[i]);
			return true;
		}
	}
	bLicensed=false;
	//sprintf(Message,"无法找到QQ信息，请检查是否已登录正确的QQ号！\n请联系作者QQ:%s","910955951");
	sprintf(Message
		,XorStr<0x51,61,0x1FB775B3>("\x9F\x8C\xE4\xFC\x80\x84\xE2\xE5\x08\x0B\x8B\x99\x92\xFC\xFC\xCC\xA6\x89\xDF\x88\xD7\x8F\xAD\xAF\xDE\x9B\xB9\xBD\xD8\xA9\xAD\xCC\xA4\x8F\xBB\xC3\xC0\xB2\x26\x29\xC3\xBF\xD8\xDD\x77\xB9\x94\x41\x2B\x4D\x36\x53\x72\x53\x58\xD9\xD8\xB0\xAE\xFF"+0x1FB775B3).s
		,XorStr<0x05,10,0x48C25417>("\x3C\x37\x37\x31\x3C\x3F\x32\x39\x3C"+0x48C25417).s);
	return false;
}
//===========================================================
//typedef BOOL (WINAPI *QueryPerformanceCounterFunc)(LARGE_INTEGER*);
//QueryPerformanceCounterFunc QueryPerformanceCounterPtr=NULL;
FARPROC (WINAPI *pGetProcAddress)(HMODULE hModule, LPCSTR lpProcName);
//===========================================================
/*
	OGC ReRoute Function
*/

BOOL IATHook(HMODULE ModHandle, DWORD OriginalFunc, DWORD HookFunc, void **pOriginalFunc)
{
	DWORD pe_offset,CurAddr,CurPointer,IATanfang,IATende,base;
	BOOL Hooked=FALSE;
	IMAGE_NT_HEADERS *pehdr;
	if(!ModHandle || !OriginalFunc || !HookFunc)
		return FALSE;
	base=(DWORD)ModHandle;
	memcpy(&pe_offset,(void *)(base+0x3C),sizeof(DWORD));
	pehdr=(IMAGE_NT_HEADERS *)((DWORD)base + pe_offset);
	IATanfang=(DWORD)base+pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress;
	IATende=IATanfang+pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size;
	CurAddr=IATanfang;
	while(CurAddr<IATende)
	{
		memcpy(&CurPointer,(void *)CurAddr,sizeof(DWORD));
		if(CurPointer==OriginalFunc)
		{
			if(pOriginalFunc)
				*pOriginalFunc=(PVOID)CurPointer;
			DWORD old_attributes,old_attributes2;
			if(!VirtualProtect((void *)CurAddr,sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old_attributes))
				return FALSE;
			memcpy((void *)CurAddr,&HookFunc,sizeof(DWORD));
			if(!VirtualProtect((void *)CurAddr,sizeof(DWORD), old_attributes, &old_attributes2))
				return FALSE;
			Hooked=TRUE;
		}
		CurAddr+=sizeof(DWORD);
	}
	return Hooked;
}

//===========================================================
/*
BOOL WINAPI newQueryPerformanceCounter(LARGE_INTEGER *lp)
{
	BOOL  ret=(*QueryPerformanceCounterPtr)(lp);

	return ret;
}*/

//===========================================================
FARPROC WINAPI newGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	FARPROC nResult;
	nResult=GetProcAddress(hModule, lpProcName);

	if (HIWORD(lpProcName))
	{
		if(!strcmp(lpProcName,"GetProcAddress")){
            return (FARPROC)&newGetProcAddress;

		//} else if (!strcmp(lpProcName, "QueryPerformanceCounter")){
		//QueryPerformanceCounterPtr=(QueryPerformanceCounterFunc)GetProcAddress(hModule, lpProcName);
		//return (FARPROC) &newQueryPerformanceCounter;

		} else {
			ApplyOpenGLHook(&nResult, lpProcName);
			CheckClientHook(&nResult, lpProcName);
		}
	}
	return nResult;
}

//===========================================================
bool WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	char tmpName[255], LicMsg[255];
	GetModuleFileName(hinstDLL, tmpName, 255);
	strlwr(tmpName);
		sprintf(hackdir, tmpName);
		char* pos = hackdir+strlen(hackdir);
		while(pos>=hackdir && *pos!='\\') --pos;
		pos[1]=0;

		sprintf(ExeCfg, tmpName);
		char* pExeCfg = ExeCfg;
		*(pExeCfg++)='.';*(pExeCfg++)='c';*(pExeCfg++)='f';*(pExeCfg++)='g';
		*(pExeCfg++)='\0';

	/*//『宅』v5.dll
	if (!strstr(tmpName, XorStr<0xB4,13,0x7D9B4150>("\x15\x0F\x63\x1B\x19\x02\xCC\x8E\x92\xD9\xD2\xD3"+0x7D9B4150).s))
	{
		char tmpa[255];
		//"老宅已精尽人亡了，请烧香！\n(%s)
		sprintf(tmpa,XorStr<0xDF,32,0x74FA2235>("\x1F\x2F\x34\x4E\x31\x35\x5B\x4D\x59\x49\x21\x21\x26\x1A\x2C\x25\x4C\x5C\x36\x19\x3A\x21\x3A\x15\x54\x59\xF3\xD2\xDE\x8F\xD4"+0x74FA2235).s,tmpName);
		///『宅』辅助.v6.17
		MessageBox(NULL, tmpa, XorStr<0xFB,17,0xA85FF532>("\x5A\x46\x28\x52\x5E\xBB\xB9\xAA\xD5\xFE\x2B\x70\x31\x26\x38\x3D"+0xA85FF532).s, MB_OK);
		float pov;
			int* getlaid=(int*)malloc(sizeof(int));
			*getlaid=15;
			zHAi_Sale(getlaid,true);
			int* pt = getlaid;
			pov=zHAi_Sale2(pt,false);
			DWORD ciao = (DWORD)&ExitProcess;
			free(getlaid);
			__asm mov ecx, ciao
			__asm push 0
			__asm call ecx
		return(false);
	}

	/*bool bLic=xLice_Checker(LicMsg);
	if (!bLic){
		int RetCmd=MessageBox(NULL, LicMsg, XorStr<0xE6,17,0xA26FC4C5>("\x47\x5D\x3D\x45\x4B\x50\x54\x45\x38\x15\xDE\x87\xC3\xDD\xC4\xC5"+0xA26FC4C5).s, MB_OKCANCEL);
		float pov;
			int* getlaid=(int*)malloc(sizeof(int));
			*getlaid=15;
			zHAi_Sale(getlaid,true);
			int* pt = getlaid;
			pov=zHAi_Sale2(pt,false);
			DWORD ciao = (DWORD)&ExitProcess;
			free(getlaid);
			__asm mov ecx, ciao
			__asm push 0
			__asm call ecx
		return(false);
	}
	if ( !(isFindQQ > 9) ) {
		float pov;
			int* getlaid=(int*)malloc(sizeof(int));
			*getlaid=15;
			zHAi_Sale(getlaid,true);
			int* pt = getlaid;
			pov=zHAi_Sale2(pt,false);
			DWORD ciao = (DWORD)&ExitProcess;
			free(getlaid);
			__asm mov ecx, ciao
			__asm push 0
			__asm call ecx;
		return(false);
	}

	if (!bLicensed) {
		float pov;
			int* getlaid=(int*)malloc(sizeof(int));
			*getlaid=15;
			zHAi_Sale(getlaid,true);
			int* pt = getlaid;
			pov=zHAi_Sale2(pt,false);
			DWORD ciao = (DWORD)&ExitProcess;
			free(getlaid);
			__asm mov ecx, ciao
			__asm push 0
			__asm call ecx
		return(false);
	}

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
				float pov;
					int* getlaid=(int*)malloc(sizeof(int));
					*getlaid=15;
					zHAi_Sale(getlaid,true);
					int* pt = getlaid;
					pov=zHAi_Sale2(pt,false);
					DWORD ciao = (DWORD)&ExitProcess;
					free(getlaid);
					__asm mov ecx, ciao
					__asm push 0
					__asm call ecx
				return(false);
			}
		}
	}*/

	if (fdwReason==DLL_PROCESS_ATTACH) 
	{
		//获取文件名称
		char* pCfg = DllCfg;
		char* pTmp = tmpName;
		int NameLen=strlen(tmpName)-strlen(hackdir)-4;
		pTmp+=strlen(hackdir);
		while(NameLen--) *(pCfg++)=*(pTmp++);
		*(pCfg++)='.';*(pCfg++)='i';*(pCfg++)='n';*(pCfg++)='i';
		*(pCfg++)='\0';

		IATHook(GetModuleHandle(NULL),(DWORD)GetProcAddress,(DWORD)newGetProcAddress,(void **)&pGetProcAddress);
		if( !PatchHL() ){
			return(false);
		}
		glReadPixels_s = (glReadPixels_t)DetourFunction((PBYTE)GetProcAddress(GetModuleHandle("opengl32.dll"), "glReadPixels"), (PBYTE)m_glReadPixels );	
	}
	return true;
}
//===========================================================