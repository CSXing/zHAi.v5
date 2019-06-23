/**********************************************************
¡ô©©©Ç zHA! H00k vEr:5.0 ©Ï©©¡ô
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
¡ô©©©Ç      (c)2018      ©Ï©©¡ô
    File Name: Bind.cpp
************************************************************/

#pragma warning (disable:4786) // vc++ stl "truncated browser info"
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>

#include "bind.h"
#include "console.h"
using namespace std;

//===================================================================
KeyBindManager keyBindManager;
void Con_Echo(const char *fmt, ... );

static int getConsoleExtraCode( int scancode )
{
	switch(scancode)
	{
	case 127:  return -1;   // backspace
	case 13:  return '\n'; // enter
	case 32:  return ' ';  // space 
	case 128: return -2;   // uparrow
	case 129: return -3;   // downarrow
	case 130: return -4;   // leftarrow
	case 131: return -5;   // rightarrow
	default:  return  0;   // 0= not found
	}
}

//===================================================================
bool KeyBindManager::keyBlocked(int scancode,bool down)
{
	
	if( (con_visible && con_visible()) || messageMode ) return false;

	const char* name = keyNames[scancode].c_str();
	if( !*name ){ return false; }

	if( gConsole.active && down )
	{
		if( !name[1] && name[0]!='~' )      return true; // alpha-numeric keys
		if( getConsoleExtraCode(scancode) ) return true; // extra keys
	}

	const char* keyname = keyNames[scancode].c_str();
	return myBind.find(keyname) ;
}

//===================================================================
bool KeyBindManager::keyBlocked(const char* name)
{
	bool block = myBind.find(name);
	return block;
}

//===================================================================
void KeyBindManager::remapKey(char* from, char* to)
{
	int scancode_from = -1;
	int scancode_to   = -1;

	for(int i=0;i<256;i++)
	{
		if( !strcmp(from,keyNames[i].c_str()) ){ scancode_from = i; }
		if( !strcmp(to  ,keyNames[i].c_str()) ){ scancode_to   = i; }
	}
	
	keyRemapTable[scancode_from] = scancode_to;
}

//===================================================================
void KeyBindManager::expandCommand(string& expand,bool down)
{
	if(expand[0]=='+')
	{
		if(down){
			CallBack_Execute(expand);
		}else {
			expand[0]='-';
			CallBack_Execute(expand);
			expand[0]='+';
		}
	}
	else if( (expand[0]=='.'||expand[0]=='#') && expand[1]=='+' )
	{
		if(down){
			CallBack_Execute(expand);
		}else {
			expand[1]='-';
			CallBack_Execute(expand);
			expand[1]='+';
		}
	}
	else
	{
		if(down) { CallBack_Execute(expand); }
	}
}

//===================================================================
void KeyBindManager::notifyKeyEvent(int scancode,bool down,bool repeat)
{
	const char* keyname = keyNames[scancode].c_str();

	if(!*keyname ) 
		return;
	
	if( !keyname[1] && down && (messageMode || (con_visible && con_visible()))  )
		return;

	if(( gConsole.active) && down)
	{ 
		Console *a;
			a = &gConsole;
		if( !keyname [1] && keyname [0]!='~' )
		{
			int code = keyname [0];
			if( GetAsyncKeyState(VK_SHIFT) )
			{
				code = shiftRemap[code];
			}

			a->key(code);
			return;
		} else {
			switch(scancode)
			{
			case 127: // backspace
				a->key(-1);
				return;
			case 13: // enter
				a->key('\n');
				return;
			case 32: // space 
				a->key(' ');
				return;
			case 128: // uparrow
				a->key(-2);
				return;
			case 129: // downarrow
				a->key(-3);
				return;
			case 130: // leftarrow
				a->key(-4);
				return;
			case 131: // rightarrow
				a->key(-5);
				return;
			}
		}
	}

	if(myBind.find(keyname) && !repeat)
	{
		string& expand = myBindExpand[myBind.num];
		expandCommand(expand,down);
	}
}

//===================================================================
void KeyBindManager::notifyMouseEvent(char* name,bool down)
{
	if(myBind.find(name))
	{
		string& expand = myBindExpand[myBind.num];
		expandCommand(expand,down);
	}
}

inline static void lowercase(char* str)
{
	while(*str){ *str = tolower(*str); ++str; }
}

//===================================================================
void KeyBindManager::addBind(char* key, char* value)
{
	if(!*key)
	{
		Con_Echo("valid keys: ");
		string line;
		for(int i=0;i<256;i++)
		{ 
			string& name = keyNames[i];
			if(!name.empty())
			{ 
				line += " [";
				line += name;
				line += "] ";
			}
			if(line.size()>50)
			{
				Con_Echo(line.c_str());
				line.erase();
			}
		}
		Con_Echo(" [mouse1] [mouse2] [mouse3] [mouse4]");
		Con_Echo(" [mouse5] [mwheelup] [mwheeldown]");
		return;
	}

	lowercase(key);
	if(myBind.find(key))
	{
		string& expand = myBindExpand[myBind.num];
		if(*value)
		{
			expand = value; 
		} else {
			Con_Echo( "%s is bound to \"%s\"",key,expand.c_str() );
		}
	} else {
		
		bool found = false;
		for(int i=0;i<256;i++)if(keyNames[i]==key) {found=true;break;}
		if( !strcmp(key,"mouse1") || !strcmp(key,"mouse2") || 
			!strcmp(key,"mouse3") || !strcmp(key,"mouse4") || 
			!strcmp(key,"mouse5") || !strcmp(key,"mwheelup") || 
			!strcmp(key,"mwheeldown") )
		{
			found = true;
		}

		if(found)
		{
			if(*value)
			{
				int index = myBindExpand.size();
				myBindExpand.push_back(value);
				myBind.add(key,index);
			} else {
				Con_Echo("%s is not bound",key);
			}
		} else {
			Con_Echo( "%s is not a valid key.\n",key);
		}
	}
}

//===================================================================
void KeyBindManager::removeBind(char* key)
{
	myBind.erase(key);
}

//===================================================================
void KeyBindManager::init()
{
	int i;
	for(i=0;i<256;i++)keyRemapTable[i]=i;

	keyNames[49] = "1";
	keyNames[50] = "2";
	keyNames[51] = "3";
	keyNames[52] = "4";
	keyNames[53] = "5";
	keyNames[54] = "6";
	keyNames[55] = "7";
	keyNames[56] = "8";
	keyNames[57] = "9";
	keyNames[48] = "0";
	keyNames[45] = "-";
	keyNames[61] = "=";
	keyNames[127] = "backspace";
	keyNames[9] = "tab";
	keyNames[113] = "q";
	keyNames[119] = "w";
	keyNames[101] = "e";
	keyNames[114] = "r";
	keyNames[116] = "t";
	keyNames[121] = "y";
	keyNames[117] = "u";
	keyNames[105] = "i";
	keyNames[111] = "o";
	keyNames[112] = "p";
	keyNames[91] = "[";
	keyNames[93] = "]";
	keyNames[13] = "enter";
	keyNames[133] = "ctrl";
	keyNames[97] = "a";
	keyNames[115] = "s";
	keyNames[100] = "d";
	keyNames[102] = "f";
	keyNames[103] = "g";
	keyNames[104] = "h";
	keyNames[106] = "j";
	keyNames[107] = "k";
	keyNames[108] = "l";
	keyNames[59] = ";";
	keyNames[39] = "'";
	keyNames[134] = "shift";
	keyNames[0] = "\\";
	keyNames[122] = "z";
	keyNames[120] = "x";
	keyNames[99] = "c";
	keyNames[118] = "v";
	keyNames[98] = "b";
	keyNames[110] = "n";
	keyNames[109] = "m";
	keyNames[44] = ",";
	keyNames[46] = ".";
	keyNames[47] = "/";
	keyNames[134] = "rshift";
	keyNames[42] = "*";
	keyNames[132] = "alt";
	keyNames[32] = "space";
	keyNames[175] = "capslock";
	keyNames[135] = "f1";
	keyNames[136] = "f2";
	keyNames[137] = "f3";
	keyNames[138] = "f4";
	keyNames[139] = "f5";
	keyNames[140] = "f6";
	keyNames[141] = "f7";
	keyNames[142] = "f8";
	keyNames[143] = "f9";
	keyNames[144] = "f10";
	keyNames[255] = "pause";
	keyNames[151] = "home";
	keyNames[128] = "uparrow";
	keyNames[150] = "pgup";
	keyNames[173] = "minus";
	keyNames[130] = "leftarrow";
	keyNames[170] = "kp_ins";
	keyNames[164] = "kp_5";
	keyNames[131] = "rightarrow";
	keyNames[174] = "plus";
	keyNames[152] = "end";
	keyNames[129] = "downarrow";
	keyNames[149] = "pgdn";
	keyNames[147] = "ins";
	keyNames[148] = "del";
	keyNames[145] = "f11";
	keyNames[146] = "f12";
	keyNames[240] = "mwheelup";
	keyNames[239] = "mwheeldown";
	keyNames[241] = "mouse1";
	keyNames[242] = "mouse2";
	keyNames[243] = "mouse3";
	keyNames[244] = "mouse4";
	keyNames[245] = "mouse5";

	for(i=0;i<=255;i++)  shiftRemap[i] = toupper((char)i);
	shiftRemap['1'] = '!';
	shiftRemap['2'] = '@';
	shiftRemap['3'] = '#';
	shiftRemap['4'] = '$';
	shiftRemap['5'] = '%';
	shiftRemap['6'] = '^';
	shiftRemap['7'] = '&';
	shiftRemap['8'] = '*';
	shiftRemap['9'] = '(';
	shiftRemap['0'] = ')';
	shiftRemap['-'] = '_';
	shiftRemap['='] = '+';
	shiftRemap['['] = '{';
	shiftRemap[']'] = '}';
	shiftRemap['\''] = '\"';
	shiftRemap[';'] = ':';
	shiftRemap[','] = '<';
	shiftRemap['.'] = '>';
	shiftRemap['/'] = '?';
}

//==========================================================
int getScanCode_FixMessage(LPMSG lpMsg)
{
	int scancode = (lpMsg->lParam>>16)&0xFF;
	scancode = keyBindManager.remapScanCode(scancode);
	lpMsg->lParam &=  0xFF00FFFF;
	lpMsg->lParam +=  (scancode<<16);
	return scancode;
}
