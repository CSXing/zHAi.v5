/**********************************************************
¡ô©©©Ç zHA! H00k vEr:5.0 ©Ï©©¡ô
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
¡ô©©©Ç      (c)2018      ©Ï©©¡ô
    File Name: Console.cpp
************************************************************/
#pragma warning (disable:4786)
#include <windows.h>
#include <vector>
#include "Client.h"
#include "console.h"
#include "DefineCmds.h"
#include "xorstr.h"

// global
Console gConsole;

void DrawBox(int x, int y);
void DrawMove(int x, int y);
//===========================================================
void Console::draw(int con_x, int con_y, int con_w, int con_h)
{
	if( !active ) return;

	DrawWindowCon(con_x, con_y, con_w, con_h, /*-= ¿ØÖÆÌ¨ =-*/XorStr<0x87,13,0xC1CEEC92>("\xAA\xB5\xA9\x35\x53\x5A\x4B\x42\x27\xB0\xAC\xBF"+0xC1CEEC92).s);

	////////// SCROLL BAR DRAWING!!!! /////////
	int con_h1 = con_h + 19;
	int con_y1 = con_y + 33;
	if( scrollbar > con_h - 42 ) scrollbar = con_h - 42;
	if( scrollbar < 0 ) scrollbar = 0;

	tintArea( con_x + ( con_w - 20 ) , con_y1 - 25 , 11 , con_h1 - 51 , 255 , 255 , 255 , 123 );       // scrollbar backgound
	tintArea( con_x + ( con_w - 21 ) , con_y1 + con_h1 - scrollbar - 91 , 14 , 14 , 0 , 0 , 0 , 255 ); // scrollbar block background

	DrawBox( con_x + ( con_w - 22 ) , con_y1 + con_h1 - scrollbar - 92 );
	DrawMove( con_x + ( con_w - 15 ) , con_y1 + con_h1 - scrollbar - 88 );
	////////// SCROLL BAR DRAWING!!!! /////////

	int x = con_x+3;
	int y = con_y+con_h-14;

	// advance blink:
	if(blinkTimer.expired())
	{
		blink = !blink;
		if(blink) { blinkTimer.countdown(0.2); }
		else      { blinkTimer.countdown(0.2); }
	}
	
	// draw cursor
	if(blink)
	{
		// get len of text to cursor.
		int length, height, ch_length;
		char  save;
		save = cursorpos[0]; cursorpos[0]=0;
		oEngfuncs.pfnDrawConsoleStringLen( editbuf, &length, &height );
		cursorpos[0]=save;

		// get cursor size:
		save = cursorpos[1]; cursorpos[1]=0;
		oEngfuncs.pfnDrawConsoleStringLen( cursorpos, &ch_length, &height );
		cursorpos[1]=save;
		if(!*cursorpos) ch_length=5;
        
		oEngfuncs.pfnDrawSetTextColor(1,1,1);
		oEngfuncs.pfnDrawConsoleString(x+length,y,"|");
	}
	
	ColorEntry* color = colorList.get(0); // "white"
	oEngfuncs.pfnDrawSetTextColor(color->onebased_r,color->onebased_g,color->onebased_b);
	oEngfuncs.pfnDrawConsoleString(x,y,editbuf);

	lines.reset();

	for(int loop=0;loop<scrollbar;loop++)
		lines.prev();

	for(;;)
	{
		y-=14;
		if(y<con_y) break;
		
		string& curLine = lines.read(); 
		lines.prev();

		drawConsoleLine(curLine,x,y);
	}
}

//===========================================================
void Console::echo(const char *fmt, ... )
{
	va_list va_alist;
	char buf[384];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	lines.add( buf );
}

//===========================================================
void Console::say(const char* text, const char* name, int team )
{
	if(team==1) { echo("&r%s :", name); echo(" %s",text); }
	else        { echo("&b%s :", name); echo(" %s",text); }
}


//===========================================================
void Console::setcolortag(unsigned char ch, int r, int g, int b)
{
	ch -= 'a';
	if(ch>=26) { echo("color tags must be labeled \'a\' to \'z\'"); return; }
	
	colorTags[ch].r = r;
	colorTags[ch].g = g;
	colorTags[ch].b = b;
	colorTags[ch].fill_missing();
}

//===========================================================
void Console::drawConsoleLine( const string& str, int x, int y )
{
	const char* line = str.c_str();
	char  buf[256];
	char* bufpos;

	for(;;)
	{
		// extract string part
		bufpos=buf;
		for(;;) { *bufpos=*line; if(!*line||*line=='&')break;  ++line; ++bufpos; };
		bufpos[0]=0;bufpos[1]=0;

		// draw
		int length, height;
		oEngfuncs.pfnDrawConsoleStringLen( buf, &length, &height );
		oEngfuncs.pfnDrawSetTextColor(colorTags[curColorTag].onebased_r, 
										 colorTags[curColorTag].onebased_g, 
										 colorTags[curColorTag].onebased_b);
		oEngfuncs.pfnDrawConsoleString(x,y,buf);

		// advance
		x+=length;
		if(*line=='&')
		{
			unsigned char ch = *++line - 'a';
			if(ch<26) curColorTag=ch;
			else         break;
			if(!*++line) break;
		}
		else 
		{
			break;
		}
	}
	curColorTag=0;
}

//===========================================================
void Console::key(int ch)
{
	char* pos;
	switch(ch)
	{
	case -1: // backspace
		if(cursorpos==editline) return;
		pos = --cursorpos;
		while(pos[0]) { pos[0]=pos[1]; ++pos; }
		return;
		
	case -2: // uparrow
		if(hist_direction!=DIR_BACK) { history.prev(); history.prev(); hist_direction=DIR_BACK;}
		strcpy(editline, history.read().c_str());
		cursorpos = editline + strlen(editline);
		history.prev();
		return;

	case -3: // downarrow
		if(hist_direction==DIR_BACK) { history.next(); history.next(); hist_direction=DIR_FORWARD;}
		strcpy(editline, history.read().c_str());
		cursorpos = editline + strlen(editline);
		history.next();
		return;
	
	case -4: // leftarrow
		if(cursorpos!=editline) --cursorpos;
		return;

	case -5: // leftarrow
		if(cursorpos!=(editline+strlen(editline))) ++cursorpos;
		return;

	case '\n':
		if( !strcmp(editline,"===") )
		{
			if(mode==MODE_EXECUTE) { mode=MODE_CHAT;    echo("&b*** &aCONSOLE: &wCHAT MODE &b***"); }
			else                   { mode=MODE_EXECUTE; echo("&b*** &aCONSOLE: &wEXEC MODE &b***"); }
		}
		else if(mode==MODE_EXECUTE)
		{
			echo    ( "&x%s",editbuf );
			if(editline[0])
			{
				defCMD.exec( editline );
				history.add(editline);
				history.reset();
			}
		} 
		else if(mode==MODE_CHAT)
		{
			char* text = editline; while(*text==' ')++text;
			char buf[256];sprintf(buf,"say \"%s\"",text);
			oEngfuncs.pfnClientCmd(buf);
		}	
		editline[0]=0;
		cursorpos = editline;
		return;
	default:
		// insert character
		if(strlen(editbuf)>(EDIT_MAX-4)) return;
		if(!cursorpos[0]) { cursorpos[0]=ch; ++cursorpos; cursorpos[0]=0; return; }

		pos = editbuf+strlen(editbuf)+1; 
		while(pos>cursorpos) { pos[0]=pos[-1]; --pos; }

		*cursorpos = ch;
		++cursorpos;
		return;
	}
}