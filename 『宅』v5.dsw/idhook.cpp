/**********************************************************
������ zHA! H00k vEr:5.0 �ϩ���
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
������      (c)2018      �ϩ���
    File Name: IdHook.cpp
************************************************************/

#pragma warning(disable:4786)

#include <windows.h>
#include "client.h"
#include "color.h"
#include "idhook.h"
#include "cvars.h"
#include "DefineCmds.h"
#include "calcscreen.h"
#include "players.h"
#include "drawing.h"
#include "font.h"
#include "misc/defs.h"

IdHook idhook;
typedef IdHook::Player Player;
typedef IdHook::PlayerEntry PlayerEntry;

static IdHook::Player* curPlayer = idhook.basePlayer;
extern bool oglSubtractive;
int selection     = 0;
int seekselection = 0;
//===========================================================
void func_kill_adds() //���רɱ����
{
	if(!defCMD.argS(1).empty())
		idhook.Player_Add(defCMD.argI(1));
	if(cvar.id_mode!=1)
		cvar.id_mode=1;
}

//===========================================================
void func_kill_clears() //�������רɱ����
{
	idhook.Player_Clear();

	if(cvar.id_mode!=0)
		cvar.id_mode=0;
}

//===========================================================
void func_kill_refs() //ˢ������б�
{
	idhook.Player_Relist();
}

//===========================================================
void func_kill_modes()
{
	if(cvar.id_mode!=1)
		cvar.id_mode=1; //1=�б�����
	else
		cvar.id_mode=0; //0=��
}

//===========================================================
void player_describe_current()
{
	if(!idhook.Active||!curPlayer) return;
	IdHook::PlayerEntry& entry = curPlayer->items[curPlayer->selection];
}

//===========================================================
void IdHook_Plys_Select() //ѡ��
{
	if(!idhook.Active)
	{
		idhook.init();
	}
//	if(!curPlayer) //����BUG
//	{
		curPlayer = idhook.basePlayer;
//		idhook.Active = true;
		curPlayer->boundSelection();
		player_describe_current();
//		return;
//	}
	if(idhook.Active)
	{
		IdHook::PlayerEntry& entry = curPlayer->items[curPlayer->selection];
		if(entry.player)
		{
			curPlayer = entry.player;
			curPlayer->boundSelection();
			player_describe_current();
		}
		else
		{
			int i=curPlayer->selection;
			int j=curPlayer->seekselection;
			defCMD.exec(const_cast<char*>(entry.content));
			idhook.Active=false;
			IdHook_Plys_Select();
			curPlayer->selection=i;
			curPlayer->seekselection=j;
			curPlayer->boundSelection1();
			player_describe_current();
		}
	}
	else
	{
		curPlayer->selection=selection;
		curPlayer->seekselection=seekselection;
		curPlayer->boundSelection1();
		player_describe_current();
	}
	idhook.Active=true;
}

//===========================================================
void IdHook_Plys_Back() //���ϼ�
{
	if(!idhook.Active || !curPlayer) return;
	curPlayer = curPlayer->parent;
	if(!curPlayer) 
	{ 
		curPlayer = idhook.basePlayer;
		idhook.Active=false; 
	}
	selection=curPlayer->selection;
	seekselection=curPlayer->seekselection;
	curPlayer->boundSelection();
	player_describe_current();
}

//===========================================================
void IdHook_Plys_Up() //��������һ��
{
	if(!idhook.Active || !curPlayer) return;
	--curPlayer->selection;
	curPlayer->boundSelection();
	player_describe_current();
}

//===========================================================
void IdHook_Plys_Down() //��������һ��
{
	if(!idhook.Active || !curPlayer) return;
	++curPlayer->selection;
	curPlayer->boundSelection();
	player_describe_current();
}

//===========================================================
void IdHook_RefPlayers() //�Զ�ˢ��
{
	if(idhook.Active)
	{
		int i=curPlayer->selection;
		int j=curPlayer->seekselection;
		idhook.Player_Relist();
		idhook.Active=false;
		IdHook_Plys_Select();
		curPlayer->selection=i;
		curPlayer->seekselection=j;
		curPlayer->boundSelection1();
		player_describe_current();
	}
}

//===========================================================
void IdHook_ShowList()
{
	if ( !idhook.Active ) return;
	if (!curPlayer)	curPlayer = idhook.basePlayer;

	vector<IdHook::PlayerEntry>& items = curPlayer->items;

	enum{ TEXT_H = 20 };

	int cW = screeninfo.iWidth/2.3;
	int x, y, w, top=30;
	x = displayCenterX - cW/2;
	y = top;
	w = 350;

	int frame_h = items.size()*TEXT_H;
	if (frame_h>TEXT_H) frame_h -= TEXT_H;

	oglSubtractive = true;

	DrawWindow(x,y,w,frame_h, curPlayer->name.c_str());  //����

	int nSelId=curPlayer->selection;

	if ( nSelId==0 ){
		whiteBorder(x+2,y+2,(w-150),TEXT_H-2);
	} else if ( nSelId==1 ){
		whiteBorder(x+2+(w-150),y+2,(w-200)-3,TEXT_H-2);
	} else if ( nSelId>=2 ){
		y += (nSelId-1)*TEXT_H;
		whiteBorder(x+2,y+2,w-3,TEXT_H-2);
	}

	y = top;
	x+=4;
	ColorEntry* clr;
	if ( cvar.id_mode )
		clr = colorList.get(1); // "red"
	else 
		clr = colorList.get(0); // "white"

	DrawConsoleStringEx(x,y, clr->r,clr->g,clr->b ,items[0].name);

	clr = colorList.get(0); // "white"
	DrawConsoleStringEx(x+(w-100),y, clr->r,clr->g,clr->b ,items[1].name);

	y+=TEXT_H;
	for(int i=2;i<items.size();i++)
	{
		IdHook::PlayerEntry& item = items[i];

		if(strstr(item.name,"��")){
			clr = colorList.get(2); // "����"
		}else if(strstr(item.name,"��")){
			clr = colorList.get(3); // "����"
		} else {
			clr = colorList.get(0); // "white"
		}

		DrawConsoleStringEx(x,y, clr->r,clr->g,clr->b ,item.name);
		//clr = colorList.get(6); // "greed"
		if(strstr(item.name,"��")){
			clr = colorList.get(0); // רɱ
			DrawConsoleStringEx(x+w-45,y, clr->r,clr->g,clr->b ,"����");
		}
		y+=TEXT_H;
	}
	oglSubtractive = false;
}

//===========================================================
void Kill_Plys_List(Player* pPlayer)
{
	PlayerEntry newEntry;

	if (cvar.id_mode) {
		sprintf(newEntry.name,"רɱģʽ��ѡ������[1]");
	} else {
		sprintf(newEntry.name,"רɱģʽ����[0]");
	}
	sprintf(newEntry.content,"kill_modes");
	newEntry.player = 0;
	pPlayer->items.push_back(newEntry);

	sprintf(newEntry.name,"����б�");
	sprintf(newEntry.content,"kill_clears");
	newEntry.player = 0;
	pPlayer->items.push_back(newEntry);

	int ax;
	if ( me.team ==2){
		for (ax=0;ax<vPlayers.size();ax++)
		{
			PlayerInfo& r = vPlayers[ax];
			r.updateEntInfo();
			if(strcmp(vPlayers[ax].getName(),"\\missing-name\\") && vPlayers[ax].getName()[0]!='\0' && vPlayers[ax].team>0 && vPlayers[ax].team==1)
			{
				//����
				if(idhook.Plys[ax]==0)
					sprintf(newEntry.name,"�����ˡ�%s",vPlayers[ax].getName());
				else if(idhook.Plys[ax]==1)
					sprintf(newEntry.name,"�����ˡ�%s",vPlayers[ax].getName());

				sprintf(newEntry.content,"kill_adds %d",ax);
				newEntry.player = 0;
				pPlayer->items.push_back(newEntry);
			}
		}
	} else {
		for (ax=0;ax<vPlayers.size();ax++)
		{
			PlayerInfo& r = vPlayers[ax];
			r.updateEntInfo();
			if(strcmp(vPlayers[ax].getName(),"\\missing-name\\") && vPlayers[ax].getName()[0]!='\0' && vPlayers[ax].team>0 && vPlayers[ax].team==2)
			{
				//����
				if(idhook.Plys[ax]==0)
					sprintf(newEntry.name,"�����졡%s",vPlayers[ax].getName());
				else if(idhook.Plys[ax]==1)
					sprintf(newEntry.name,"�ﾯ�졡%s",vPlayers[ax].getName());

				sprintf(newEntry.content,"kill_adds %d",ax);
				newEntry.player = 0;
				pPlayer->items.push_back(newEntry);
			}
		}
	}
}

//===========================================================
void IdHook::init()
{
	if(basePlayer) delete basePlayer;
	basePlayer = new Player;
	basePlayer->name = "��լ��������Ȼ�ɱ���б�ϡ󡡡�vEr:1.0";
	Kill_Plys_List(basePlayer);
}

//===========================================================
void IdHook::Player_Add(int ax)
{
	if(Plys[ax]==0){
		Plys[ax]=1;
	}
	else if(Plys[ax]==1) {
		Plys[ax]=0;
	}
}

//===========================================================
void IdHook::Player_Clear()
{
	for(int i=0;i<MAX_VPLAYERS;i++)
	{
		Plys[i]=0;
	}
}

//===========================================================
void IdHook::Player_Relist()
{
	for(int i=0;i<MAX_VPLAYERS;i++)
	{
		if(i>=vPlayers.size())
			return;
		if(vPlayers[i].getName()[0]=='\0')
			Plys[i]=0;
	}
}

//===========================================================
int IdHook::KeyEvent(int keynum)
{
	if ( keynum == cvar.KillKey )             //��רɱ/ר��ɱ����б�˵�
	{
		if(!idhook.Active)
		{
			IdHook_Plys_Select();
		}
		else
		{
			selection=curPlayer->selection;
			seekselection=curPlayer->seekselection;
			idhook.Active=false;
		}
		return 0;
	}
	else if( keynum == 128 || keynum == 240 ) //uparrow || mwheelup
	{
		IdHook_Plys_Up();
		return 0;
	}
	else if( keynum == 129 || keynum == 239 ) //downarrow || mwheeldown
	{
		IdHook_Plys_Down();
		return 0;
	}
	else if( keynum == 130 || keynum == 241 ) //leftarrow || leftbutton(mouse1)
	{
		IdHook_Plys_Select();
		return 0;
	}
	else if( keynum == 131 || keynum == 242 ) //rightarrow || rightbutton(mouse2)
	{
		IdHook_Plys_Back();
		return 0;
	}
	return 1;
}

//===========================================================
void IdHook::Show(){
	IdHook_ShowList();
};

//===========================================================
