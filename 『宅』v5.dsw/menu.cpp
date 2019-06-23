/**********************************************************
◆┅┣ zHA! H00k vEr:5.0 ┫┅◆
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
◆┅┣      (c)2018      ┫┅◆
    File Name: Menu.cpp
************************************************************/

#include <windows.h>
#include "cVars.h"
#include "Client.h"
#include "Calcscreen.h"
#include "drawing.h"
#include "Font.h"
#include "Menu.h"
#include "XorStr.h"

extern float winampnext;
extern float winampprev;
extern float winampforward;
extern float winampback;
extern float winamppause;
extern float winampstop;
extern float winampplay;
extern float winamplist;

cMenu gMenu;

//===========================================================
int cMenu::AddEntry(int n, char title[1000], float* value, float min, float max, float step)
{
	strcpy( menuEntry[n].title,title);
	menuEntry[n].value = value;
	menuEntry[n].min = min;
	menuEntry[n].max = max;
	menuEntry[n].step = step;
	return (n+1);
}

//===========================================================
void cMenu::Init()
{
	int i = 0;

	if(cvar.Menu1)
	{
		i = AddEntry(i, "◆┅┣自瞄设置┫┅◆", &cvar.Menu1, 0, 1, 1);
		i = AddEntry(i, "　　　范围", &cvar.fov, 1, 5, 1);
		//i = AddEntry(i, "　　　位置", &cvar.hitbox, 0, 2, 1);
		i = AddEntry(i, "　　　距离", &cvar.disaim, 0, 768, 64);
	} else {
		i = AddEntry(i, "◇┅┣自瞄设置┫┅◇", &cvar.Menu1, 0, 1, 1);
	}
	if(cvar.Menu2)
	{
		i = AddEntry(i, "◆┅┣显示设置┫┅◆", &cvar.Menu2, 0, 1, 1);
		i = AddEntry(i, "　　　远透", &cvar.FarSound, 0, 1, 1);
		i = AddEntry(i, "　　　大头", &cvar.BigHead, 0, 1, 1);
		if(cvar.radar)
		{
			i = AddEntry(i, "　　◆雷达", &cvar.radar, 0, 2, 1);
			i = AddEntry(i, "　　　　大小", &cvar.radar_size, 60, 100, 10);
			i = AddEntry(i, "　　　　X坐标", &cvar.radar_x, 0, 800, 10);
			i = AddEntry(i, "　　　　Y坐标", &cvar.radar_y, 0, 600, 10);
		} else  
		i = AddEntry(i, "　　◇雷达", &cvar.radar, 0, 2, 1);
		/*i = AddEntry(i, "　　闪光烟雾", &cvar.floke, 0, 1, 1);
		i = AddEntry(i, "　　　模型", &cvar.sprites, 0, 1, 1);
		i = AddEntry(i, "　　　换弹", &cvar.reload_meter, 0, 1, 1);
		i = AddEntry(i, "　　　视角", &cvar.chase_cam, 0, 1, 1);*/
	} else {
		i = AddEntry(i, "◇┅┣显示设置┫┅◇", &cvar.Menu2, 0, 1, 1);
	}

	menuItems = i;
}

//===========================================================
void cMenu::Draw()
{
	if(!Active) return;

	char temp[255];
	enum{ TEXT_H = 20 };
	int w = 175, h = 0;
	int x = screeninfo.iWidth - w - 2;
	int y = 30;

	for(int b=0; b < menuItems; b++) h+=TEXT_H; // Font Size

	DrawWindow(x-28, y+3, w+15, h, /*-=『宅』辅助 vEr:5.0 =-*/XorStr<0xA1,24,0xB6EE8AB9>("\x8C\x9F\x02\x1E\x70\x0A\x06\x13\x11\x02\x7D\x56\x8D\xD8\xEA\xC2\x8B\x87\x9D\x84\x95\x8B\x9A"+0xB6EE8AB9).s);

	for(int i=0; i < menuItems; i++)
	{
		ColorEntry * clr;
		if(i == menuSelect)
			clr = colorList.get(8); //"yellow"
		else
			clr = colorList.get(0); //"white"
		DrawConsoleStringEx(x-22, y+i*TEXT_H+3,clr->r,clr->g,clr->b, menuEntry[i].title  );

		sprintf( temp,"%0.1f", menuEntry[i].value[0] );
		int iLen=(3-strlen(temp))*7;
		DrawConsoleStringEx(x+w-60+iLen,y+i*TEXT_H+3,255,255,255, temp );

		if( i == menuSelect )
		{
			whiteBorder(x-26,y+(TEXT_H*i)+5,w+12,TEXT_H-2);
		}
	}
}

//===========================================================
int cMenu::KeyEvent(int keynum)
{
	if( keynum == 128 || keynum == 240) //uparrow || mwheelup
	{
		if( menuSelect>0 ) menuSelect--;
		else menuSelect = menuItems - 1;
		return 0;
	}
	else if( keynum == 129 || keynum == 239) //downarrow || mwheeldown
	{
		if( menuSelect<menuItems-1 ) menuSelect++;
		else menuSelect = 0;
		return 0;
	}
	else if( keynum == 130 || keynum == 241 ) //leftarrow || leftbutton
	{
		if( menuEntry[menuSelect].value )
		{
			menuEntry[menuSelect].value[0] -= menuEntry[menuSelect].step;
			if( menuEntry[menuSelect].value[0] < menuEntry[menuSelect].min )
				menuEntry[menuSelect].value[0] = menuEntry[menuSelect].max;
		}
		return 0;
	}
	else if( keynum == 131 || keynum == 242 ) //rightarrow || rightbutton
	{
		if( menuEntry[menuSelect].value )
		{
			menuEntry[menuSelect].value[0] += menuEntry[menuSelect].step;
			if( menuEntry[menuSelect].value[0] > menuEntry[menuSelect].max )
				menuEntry[menuSelect].value[0] = menuEntry[menuSelect].min;
		}
		return 0;
	}
	return 1;
}
//===========================================================