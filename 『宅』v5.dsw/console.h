#ifndef Console_H
#define Console_H
	#include "timehandling.h"
	#include "color.h"
	#include "drawing.h"

	//=====================================================================================
	template < typename T, int N >
	class WrapHistory
	{
	protected:
		void increase(int& idx) { ++idx; if(idx==N) idx=0;     }
		void decrease(int& idx) { --idx; if(idx<0 ) idx=(N-1); }

	public:
		WrapHistory() : readpos(0), writepos(0) {}
		void add(const T& entry) 
		{ 
			increase(writepos);
			entries[writepos]=entry; 
			reset();
		}

		void reset(){ readpos = writepos; }
		T&   read (){ return entries[readpos]; }
		void prev (){ decrease(readpos); if(readpos==writepos) increase(readpos); }
		void next (){ if(readpos!=writepos) increase(readpos);  }

	protected:
		int readpos;
		int writepos;
		T entries[N];
	};


	//=====================================================================================
	using namespace std;
	class Console
	{
	public:
		int  active;
		int  mode;   enum{ MODE_EXECUTE=0, MODE_CHAT=1 };

		void draw(int x, int y, int w, int h);
		void echo(const char *fmt, ... );
		void say (const char* text, const char* name, int team );
		void key (int ch);
		void setcolortag(unsigned char ch, int r, int g, int b);
		int scrollbar;

	protected:
		// con lines
		WrapHistory<string,1000> lines;
		
		// edit history
		WrapHistory<string,1000> history;
		enum { DIR_BACK, DIR_FORWARD } hist_direction;

		// edit line
		enum { EDIT_MAX=160 };
		char  editbuf[EDIT_MAX];
		char* const editline;
		
		// cursor
		char* cursorpos;
		StopTimer blinkTimer;
		bool  blink;

		// color tags
		ColorEntry colorTags[26];
		int        curColorTag;  // ==0 <=> 'a'

		// draws a console line, and takes care of color tags
		void drawConsoleLine( const string& str, int x, int y );

	public:
	Console() : editline(editbuf+1) , hist_direction(DIR_BACK)
		{ 
			editbuf[0] = '>';
			editbuf[1] =  0 ;
			editbuf[EDIT_MAX-1] = -1;
			cursorpos = editline;
			curColorTag = 0;
			colorTags['a'-'a'] = *colorList.get(8); // "con_text"  -> 'a'
			colorTags['x'-'a'] = *colorList.get(5); // "con_text2" -> 'x'
			mode = MODE_EXECUTE;
			scrollbar = 0;
		}
		int ircmode;
	};


	extern Console gConsole;
#endif