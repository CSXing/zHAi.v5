/**********************************************************
¡ô©©©Ç zHA! H00k vEr:5.0 ©Ï©©¡ô
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
¡ô©©©Ç      (c)2018      ©Ï©©¡ô
    File Name: Interpreter.cpp
************************************************************/

#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <fstream>
#include "DefineCmds.h"
#include "cvars.h"
#include "client.h"

defineCommands defCMD;
#define cmd
//===========================================================

void defineCommands::logExec(const char* command)
{
	Con_Echo("FUNCTION %s CALL ArgCount %d\n",command,preExecArgs.size() );
	string collect;
	for(int i=0;i<preExecArgs.size();i++)
	{
		collect += " [";
		collect += preExecArgs[i];
		collect += "] ";
	}
	Con_Echo("Arguments %s", collect.c_str() );
} 

//===========================================================
void defineCommands::exec(const char* cmdlist)
{
	string my_copy = cmdlist;
	char* from = const_cast<char*>(my_copy.c_str());
	char* to   = from;	
	while(*from==' '||*from=='\t'){ ++from;++to; }
	while(*to>=' ' && *to!= ';'){
		if(*to=='\"')
		{
			do{ ++to; }while(*to && *to!='\"');
		}
		else
		{
			++to;
		}
	}
	do{
		if(from[0]=='/' && from[1]=='/') { return; }
		if(from<to)
		{
			char oldch = *to;
			*to = 0;
			exec_one(from);
			*to = oldch;
		}
		if(!*to) { break; }
		++to; 
		from = to;
		while(*from==' '||*from=='\t'){ ++from;++to; }  
		while(*to>=' ' && *to!= ';') ++to;            
	} while(1);
}

//===========================================================
void defineCommands::extractArguments(const char* const_args)
{
	preExecArgs.clear();
	char* args = const_cast<char*>(const_args);
	while(*args)
	{
		while(*args && *args<=' ') args++;
		if(!*args) break;
		char* start;
		if(*args=='\"') { start = ++args; while(*args!='\"' && *args ) args++;  }
		else            { start = args;   while(*args>' ') args++; }
		char last = *args;
		*args = 0;
		preExecArgs.push_back(start);
		*args = last;
		if(*args=='\"') args++;
	}
}

//===========================================================
void defineCommands::collectArguments(string& dest,int from,int to)
{
	dest.erase();
	--from;--to;
	int nArgs = preExecArgs.size();
	to = (to<nArgs) ? to : (nArgs-1);
	while(from<=to){
		dest += preExecArgs[from];
		if(from!=to) dest += " ";
		++from;
	};
}

//===========================================================
void defineCommands::createRandomPrefix()
{
	static char characterBox[] = "0123456789abcdefghijklmnopqrstuvwxyz"
		                         "ABCDEFGHIJKLMNOPQRSTUVWXYZ!$%&/()=?{}[]*#-.,<>~+_";
	static int len = sizeof(characterBox)-1;
	excludePrefixChar[0] = characterBox[rand()%len];
	excludePrefixChar[1] = characterBox[rand()%len];
	excludePrefixChar[2] = characterBox[rand()%len];
	excludePrefixChar[3] = characterBox[rand()%len];
	excludePrefixChar[4] = 0;
}

//===========================================================
void defineCommands::exec_one(const char* cur_cmd)
{
	if(*cur_cmd=='#' || *cur_cmd=='.')
	{
		if( false )
		{
			static string hlcommand;
			hlcommand.erase();
			hlcommand += excludePrefixChar;
			hlcommand += (cur_cmd+1);
			HlEngineCommand(hlcommand.c_str());
		} 
		else 
		{
			HlEngineCommand(cur_cmd+1);
		}
		return;
	}
	char  command[32];
	char* commandPos = command;
	int   commandCharsLeft = 31;
	while(*cur_cmd>' ' && commandCharsLeft)
	{
		*commandPos = tolower(*cur_cmd);
		commandPos++;
		cur_cmd++;
		commandCharsLeft--;
	}
	*commandPos = 0;
	while(*cur_cmd>' ') cur_cmd++;
	if(names.find(command))
	{
		Entry& entry = entries[names.num];
		switch(entry.type)
		{
		case Entry::ALIAS:{
			string& content = *(string*)(entry.data);
			exec(const_cast<char*>(content.c_str()));
						  }break;
		case Entry::COMMAND:{
			typedef void (*CmdFunc)();
			CmdFunc function = (CmdFunc)(entry.data);
			extractArguments(cur_cmd);
			function();
							}break;
		case Entry::CVAR_FLOAT:
			extractArguments(cur_cmd);
			HandleCvarFloat( command, (float*)entry.data );
			break;
		case Entry::HL_CVAR:
			extractArguments(cur_cmd);
			HandleHlCvar(command);
			break;
		}
	} else {
		if(!isHlCvar(command))
		{
            Con_Echo("Unknown command &w%s",command);
		}
		else
		{
			Add(command,Entry::HL_CVAR,NULL);
			extractArguments(cur_cmd);
			HandleHlCvar(command);
		}
	}
}

//===========================================================
void defineCommands::execFile(const char* filename)
{
	if(strstr(filename,"..")) 
	{
		Con_Echo( "forbidden.\n" );
		return;
	}
	ifstream ifs(filename);
	if(!ifs)
	{
		return;
	}
	char buf[1024];
	while(ifs)
	{
		ifs.getline(buf,550,10);
		if(!ifs) { break; }
		exec(buf);
	}
}

//===========================================================
void  defineCommands::Add( const char *name, int type, void* data  )
{
	if(names.find(name))
	{
//		Con_Echo("&w%s&a is already registered.\n",name);
		return;
	}
	int index = entries.size();
	Entry tmp = {type,data};
	entries.push_back(tmp);
	names.add(name,index);
}

//===========================================================
inline static void lowercase(char* str)
{
	while(*str){ *str = tolower(*str); ++str; }
}

//===========================================================
void  defineCommands::AddAlias( const char *NamE, string& newcontent )
{
	char name[36];
	strncpy(name,NamE,31);
	name[31]=0;
	lowercase(name);
	if(names.find(name))
	{
		Entry& entry = entries[names.num];
		if(entry.type!=Entry::ALIAS)
		{
	//		Con_Echo("&w%s&a is already registered.\n",name);
			return;
		}
		*(string*)(entry.data) = newcontent;
		return;
	}
	if(isHlCvar(name))
	{
		Con_Echo("&w%s&a is a HL cvar.\n",name);
		return;
	}
	int index = entries.size();
	string* initial_content = new string;
	*initial_content = newcontent;
	Entry tmp = {Entry::ALIAS, initial_content};
	entries.push_back(tmp);
	names.add(name,index);
}

//===========================================================
void defineCommands::save_cvars(std::ofstream& ofs)
{	
	ofs<<"[zHAi.v5]\n";
	
	for(names.it_start();names.it_running();names.it_next())
	{
		Entry& r = entries[names.num];
		if( r.type==Entry::CVAR_FLOAT )	ofs<<names.str<<"="<<*(float*)r.data<<"\n";
	}
}

//===========================================================
void defineCommands::load_cvars(void)
{	
	char cvar_f[100];
	DWORD size=500;

	for(names.it_start();names.it_running();names.it_next())
	{
		Entry& r = entries[names.num];

		if( r.type==Entry::CVAR_FLOAT )
		{
			GetPrivateProfileString("zHAi.v5",names.str,"NULL",cvar_f,size,getHackDirFile(DllCfg).c_str());
			*(float*)r.data = atof(cvar_f);
		}
	}
}
