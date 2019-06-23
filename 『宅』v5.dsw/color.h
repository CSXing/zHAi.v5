#ifndef Color_H
#define Color_H
//#pragma once

	#include <vector>
	#include <assert.h>
	#include "stringfinder.h"
	typedef unsigned long DWORD;

	//====================================================================
	struct ColorEntry 
	{ 
		int r,g,b,a; 
		DWORD compiled;
		float fr,fg,fb;
		float onebased_r,onebased_g,onebased_b,onebased_a;
		ColorEntry():r(255),g(255),b(255),a(255)
			,fr(1.0),fg(1.0),fb(1.0)
			,onebased_r(1.0),onebased_g(1.0),onebased_b(1.0),onebased_a(1.0)
			,compiled(0xFFFFFFFF){}
		void ColorEntry::fill_missing()
		{
			compiled = (r<<24) + (g<<16) + (b<<8) + a;
			fr= float(r)/255.0f/30.0f;
			fg= float(g)/255.0f/30.0f;
			fb= float(b)/255.0f/30.0f;

			onebased_r=float(r)/255.0f;
			onebased_g=float(g)/255.0f;
			onebased_b=float(b)/255.0f;
			onebased_a=float(a)/255.0f;
		}

	};

	//====================================================================
	class ColorManager
	{
	public:
		ColorManager(){ init(); }
		void set(char* name, char* red, char* green, char* blue, char* alpha);
		
		// see init() for valid indexes
		ColorEntry* get(int index){ return &(list[index]); } 
		ColorEntry* getByName(char* name)
		{ 
			if (indexes.find(name))
			{
				int index = indexes.num;
				return &(list[index]); 
			}
			else
				return NULL;
		}

	private:
		StringFinder indexes;
		std::vector<ColorEntry> list;
		int insertpos;

		void add(const char* name,int r, int g, int b, int a=255);
		void init();
	};

	// Color Manager
	extern ColorManager colorList;
#endif