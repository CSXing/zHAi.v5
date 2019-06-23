#ifndef IdHook_H
#define IdHook_H

	#include "players.h"

	class IdHook
	{
	public:
		bool Active;
		int Plys[MAX_VPLAYERS];
		int SameCheat[MAX_VPLAYERS];
		void Player_Clear();
		void Player_Relist();
		void Player_Add(int ax);
		int  KeyEvent(int keynum);
		void Show();
		struct Player;
		struct PlayerEntry{
			PlayerEntry() :player(0){}

			char name[256];
			char content[256];
			Player*  player;
		};

		struct Player {
			Player():parent(0),selection(0),seekselection(0){}
			void boundSelection()
			{ 
				// wrap around
				if(selection<0) { selection = items.size()-1; seekselection = 2;}
				else  if(selection >= (int)items.size()) { selection = 0; seekselection = 0;}
				if(selection==items.size()-1){seekselection = 2;}
				else if(selection==items.size()-2){seekselection = 1;}
				else {seekselection = 0;}
			}
			void boundSelection1()
			{ 
				// wrap around
				if(selection<0) { selection = 0; }
				else  if(selection >= (int)items.size()-3) 
				{ 
					if(seekselection == 0)
						selection = items.size()-3; 
					else if(seekselection == 1)
						selection = items.size()-2; 
					else if(seekselection == 2)
						selection = items.size()-1; 
				}
				else
				{
					if(seekselection == 1)
						selection = items.size()-2; 
					else if(seekselection == 2)
						selection = items.size()-1; 
				}
			}

			Player*  parent;
			string name; 
			int    selection;
			int seekselection;
			vector<PlayerEntry> items;
		};

		IdHook():basePlayer(0){}
		void init();

		struct Player* basePlayer;
	};

	void IdHook_RefPlayers();  //自动刷新菜单
	void func_kill_modes();    //专杀开关
	void func_kill_adds();     //添加专杀/专不杀对象
	void func_kill_clears();   //清除所有专杀/专不杀对象
	void func_kill_refs();     //刷新玩家列表

	void player_describe_current();
	void IdHook_Plys_Select();
	void IdHook_Plys_Back();
	void IdHook_Plys_Up();
	void IdHook_Plys_Down();

	extern IdHook idhook;
#endif