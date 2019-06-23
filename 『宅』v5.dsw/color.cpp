/**********************************************************
◆┅┣ zHA! H00k vEr:5.0 ┫┅◆
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
◆┅┣      (c)2018      ┫┅◆
    File Name: Color.cpp
************************************************************/
#include "color.h"

#define BOUND_INTEGER(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}

ColorManager colorList;

//===========================================================
void ColorManager::init()
{
	indexes.clear();
	list.clear();
	insertpos = 0;

	add("white"   ,255,255,255,255 ); // 白色: 0
	add("red"     ,255,  0,  0,255 ); // 红色: 1
	add("terror"  ,255, 50, 50,255 ); // 红色: 2
	add("counter" , 50, 50,255,255 ); // 蓝色: 3
	add("esp"     ,120,120,165,255 ); // index: 4
	add("special" ,155,155,155,255 ); // 灰色: 5
	add("greed"   , 69,139,  0,255 ); // 绿色: 6
	add("blue"    , 51, 51,204,123 ); // 蓝色: 7
	add("yellow"  ,255,255, 30,255 ); // 黄色: 8 
} 

//=========================================================== 
static void FillColorEntry(ColorEntry& tmp)
{
	tmp.compiled = (tmp.r<<24) + (tmp.g<<16) + (tmp.b<<8) + tmp.a;
	tmp.fr= float(tmp.r)/255.0/30.0;
	tmp.fg= float(tmp.g)/255.0/30.0;
	tmp.fb= float(tmp.b)/255.0/30.0;

	tmp.onebased_r=float(tmp.r)/255.0;
	tmp.onebased_g=float(tmp.g)/255.0;
	tmp.onebased_b=float(tmp.b)/255.0;
	tmp.onebased_a=float(tmp.a)/255.0;
}

//===========================================================
void ColorManager::add(const char* name,int r, int g, int b, int a)
{
	indexes.add(name,insertpos);

	ColorEntry tmp;
	tmp.r=r;
	tmp.g=g;
	tmp.b=b;
	tmp.a=a;
	FillColorEntry(tmp);

	list.push_back(tmp);

	insertpos++;
}

//===========================================================
void ColorManager::set(char* name, char* red, char* green, char* blue, char* alpha)
{
	if( !indexes.find(name) ){ return; }

	int index = indexes.num;
	char format[]="%d";

	ColorEntry tmp;
	sscanf(red   ,format,&tmp.r);
	sscanf(green ,format,&tmp.g);
	sscanf(blue  ,format,&tmp.b);
	sscanf(alpha ,format,&tmp.a);
	
	FillColorEntry(tmp);

	BOUND_INTEGER(index,0,(list.size()-1));
	list[index] = tmp;
}


