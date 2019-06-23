
#ifndef MENU_H
#define MENU_H

	struct menu_entrys
	{
		char title[1000];
		float* value;
		float min;
		float max;
		float step;
	};

	class cMenu
	{
	private:

		int AddEntry(int n, char title[1000], float* value, float min, float max, float step);
		int menuSelect;
		int menuItems;
		menu_entrys menuEntry[1000];

	public:

		void Init();
		void Draw();
		int KeyEvent(int keynum);
		int Active;

	};

	extern cMenu gMenu;

#endif