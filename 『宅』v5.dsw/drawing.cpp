/**********************************************************
¡ô©©©Ç zHA! H00k vEr:5.0 ©Ï©©¡ô
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
¡ô©©©Ç      (c)2018      ©Ï©©¡ô
    File Name: Drawing.cpp
************************************************************/
#include <windows.h>
#include <vector>
#include "client.h"
#include "calcscreen.h"
#include "opengl.h"
#include "Font.h"
#include "drawing.h"

//===========================================================
int DrawLen(char *fmt)
{
	int len=0;
	for ( char * p = fmt; *p; p++ ) len+=screeninfo.charWidths[*p]; 
	return len;
}

//==============================================================================
void DrawHudStringCenter (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	
	// y-check
	int borderY = displayCenterY*2-18;
	if( y<0 || y>borderY ) { return; }

	int drawLen = DrawLen(buf);
	x = x-drawLen/2;

	int borderX = displayCenterX*2-11;
	int minX = x;
	int maxX = x+drawLen;
	bool needSingleCheck = (minX<1 || maxX>borderX);

	if( needSingleCheck )
	{
		for ( char * p = buf; *p; p++ )
		{
			int next = x + screeninfo.charWidths[*p];
			// IMPORTANT NOTE: when drawing admin-mod style charactters
			//    you MAY NOT provide x/y coordinates that cause drawing
			//    off screen. This causes HL to crash or just quit
			if( x>0 && x<borderX )
				oEngfuncs.pfnDrawCharacter (x, y, *p, r, g, b);
			x = next;
		}
	} else {
		for ( char * p = buf; *p; p++ )
		{
			int next = x + screeninfo.charWidths[*p];
			
			// IMPORTANT NOTE: when drawing admin-mod style charactters
			//    you MAY NOT provide x/y coordinates that cause drawing
			//    off screen. This causes HL to crash or just quit
			oEngfuncs.pfnDrawCharacter (x, y, *p, r, g, b);
			x = next;
		}
	}
}

//===========================================================
void DrawHudString (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	
	// y-check
	int borderY = displayCenterY*2-18;
	if( y<0 || y>borderY ) { return; }
	
	bool needSingleCheck=false;
	int borderX = displayCenterX*2-11;
		
	int drawLen = DrawLen(buf);
	if( x<1 ){ needSingleCheck=true; }
	else
	{
		int maxX = x+drawLen;
		needSingleCheck = (maxX>borderX);
	}

	if( needSingleCheck )
	{
		for ( char * p = buf; *p; p++ )
		{
			int next = x + screeninfo.charWidths[*p];
			// IMPORTANT NOTE: when drawing admin-mod style charactters
			//    you MAY NOT provide x/y coordinates that cause drawing
			//    off screen. This causes HL to crash or just quit
			if( x>0 && x<borderX )
				oEngfuncs.pfnDrawCharacter (x, y, *p, r, g, b);
			x = next;
		}
	}else {
		for ( char * p = buf; *p; p++ )
		{
			int next = x + screeninfo.charWidths[*p];
			// IMPORTANT NOTE: when drawing admin-mod style charactters
			//    you MAY NOT provide x/y coordinates that cause drawing
			//    off screen. This causes HL to crash or just quit
			oEngfuncs.pfnDrawCharacter (x, y, *p, r, g, b);
			x = next;
		}
	}	
}

//===========================================================
void DrawConString (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	int length, height;

	y += 4;
	oEngfuncs.pfnDrawConsoleStringLen( buf, &length, &height );
	oEngfuncs.pfnDrawSetTextColor(r/255.0f, g/255.0f, b/255.0f);
	oEngfuncs.pfnDrawConsoleString(x,y,buf);
}

//===========================================================
void DrawConStringCenter (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	int length, height;

	oEngfuncs.pfnDrawConsoleStringLen( buf, &length, &height );
	x -= length/2;
	y += 4;
	oEngfuncs.pfnDrawSetTextColor(r/255.0f, g/255.0f, b/255.0f);
	oEngfuncs.pfnDrawConsoleString(x,y,buf);
}

//===========================================================
void gDrawBoxAtScreenXY( int x, int y, int r, int g, int b, int alpha, int radius=1)
{
	int radius2 = radius<<1;
	oEngfuncs.pfnFillRGBA(x-radius+2, y-radius, radius2-2,2,r,g,b,alpha);
	oEngfuncs.pfnFillRGBA(x-radius, y-radius, 2,radius2,r,g,b,alpha);
	oEngfuncs.pfnFillRGBA(x-radius, y+radius, radius2,2,r,g,b,alpha);
	oEngfuncs.pfnFillRGBA(x+radius, y-radius, 2,radius2+2,r,g,b,alpha);
}
void gDrawFilledBoxAtLocation( float* origin, int red,int green,int blue,int alpha, int radius)
{
	float vecScreen[2]; 
	if( !CalcScreen(origin, vecScreen) ) { return; }
	int radius2 = radius<<1;

	oEngfuncs.pfnFillRGBA(vecScreen[0]-radius,vecScreen[1]-radius,radius2,radius2,red,green,blue,alpha);
}

//===========================================================
void whiteBorder(int x,int y,int w, int h)
{
	oglSubtractive = true;
	oEngfuncs.pfnFillRGBA( x-1, y-1, w+2, 1 ,255,255,255,254 ); //top
	oEngfuncs.pfnFillRGBA( x-1, y, 1, h-1 ,255,255,255,254 );	//left
	oEngfuncs.pfnFillRGBA( x+w, y, 1, h-1 ,255,255,255,254 );	//right
	oEngfuncs.pfnFillRGBA( x-1, y+h-1, w+2, 1 ,255,255,255,254 ); //bottom
	oglSubtractive = false;
}

//===========================================================
void blackBorder(int x,int y,int w, int h)
{
	oglSubtractive = true;
	oEngfuncs.pfnFillRGBA( x-1, y-1, w+2, 1 ,0,0,0,254 ); //top
	oEngfuncs.pfnFillRGBA( x-1, y, 1, h-1 ,0,0,0,254 );	//left
	oEngfuncs.pfnFillRGBA( x+w, y, 1, h-1 ,0,0,0,254 );	//right
	oEngfuncs.pfnFillRGBA( x-1, y+h-1, w+2, 1 ,0,0,0,254 ); //bottom
	oglSubtractive = false;
}

//===========================================================
void tintArea(int x,int y,int w,int h, ColorEntry* clr)
{
	oglSubtractive = true;
	oEngfuncs.pfnFillRGBA(x,y,w,h,clr->r,clr->g,clr->b,clr->a);
	oglSubtractive = false;
}

//===========================================================
void tintArea(int x,int y,int w,int h,int r,int g,int b,int a)
{
	oglSubtractive = true;
	oEngfuncs.pfnFillRGBA(x,y,w,h,r,g,b,a);
	oglSubtractive = false;
}

//===========================================================
void colorBorder(int x,int y,int w, int h,int r,int g,int b,int a)
{
	oglSubtractive = true;
	oEngfuncs.pfnFillRGBA( x-1, y-1, w+2, 1 ,r,g,b,a );   //top
	oEngfuncs.pfnFillRGBA( x-1, y, 1, h-1 ,r,g,b,a );	  //left
	oEngfuncs.pfnFillRGBA( x+w, y, 1, h-1 ,r,g,b,a );	  //right
	oEngfuncs.pfnFillRGBA( x-1, y+h-1, w+2, 1 ,r,g,b,a ); //bottom
	oglSubtractive = false;
}

//===========================================================
void D3DBorder(int x,int y,int w, int h, int a)
{
	oglSubtractive = true;
	oEngfuncs.pfnFillRGBA( x-1, y-1, w+2, 1 ,255,255,255,a ); //top
	oEngfuncs.pfnFillRGBA( x-1, y, 1, h-1 ,255,255,255,a );	//left
	oEngfuncs.pfnFillRGBA( x+w, y, 1, h-1 ,0,0,0,a );	//right
	oEngfuncs.pfnFillRGBA( x-1, y+h-1, w+2, 1 ,0,0,0,a ); //bottom
	oglSubtractive = false;
}

//===========================================================
void D3DBorder2(int x,int y,int w, int h, int a)
{
	oglSubtractive = true;
	oEngfuncs.pfnFillRGBA( x-1, y-1, w+2, 1 ,0,0,0,a ); //top
	oEngfuncs.pfnFillRGBA( x-1, y, 1, h-1 ,0,0,0,a );	//left
	oEngfuncs.pfnFillRGBA( x+w, y, 1, h-1 ,255,255,255,a );	//right
	oEngfuncs.pfnFillRGBA( x-1, y+h-1, w+2, 1 ,255,255,255,a ); //bottom
	oglSubtractive = false;
}

//===========================================================
void DrawBox(int x, int y)
{
	tintArea(x,y,1,15,255,255,255,255);
	tintArea(x+15,y,1,15,255,255,255,255);
	tintArea(x,y,15,1,255,255,255,255);
	tintArea(x,y+15,16,1,255,255,255,255);
}
//===========================================================
void DrawBox(int x, int y,int r,int g,int b,int a, int radius)
{
	int w = radius, h = radius;

	tintArea( x-1, y-1, w+2, 1, r, g, b, a);  //top
	tintArea( x-1, y+0, 1  , h, r, g, b, a);  //left
	tintArea( x+w, y+0, 1  , h, r, g, b, a);  //right
	tintArea( x-1, y+h, w+2, 1, r, g, b, a);  //bottom
}

//===========================================================
void DrawMove(int x,int y)
{
	tintArea(x,y,1,1,255,255,255,255);
	tintArea(x-1,y+1,1,1,255,255,255,255);
	tintArea(x+1,y+1,1,1,255,255,255,255);
	tintArea(x-1,y+6,1,1,255,255,255,255);
	tintArea(x+1,y+6,1,1,255,255,255,255);
	tintArea(x,y+7,1,1,255,255,255,255);
}

//===========================================================
void DrawVectorLine(float *flSrc,float *flDestination,int width,ColorEntry *color)
{
	vec3_t vScreenSrc,vScreenDest;
	if(!CalcScreen(flSrc,vScreenSrc) | !CalcScreen(flDestination,vScreenDest))
		return;
	DrawLines((int)vScreenSrc[0],(int)vScreenSrc[1],(int)vScreenDest[0],(int)vScreenDest[1],width,color);
}

//===========================================================
int Count = 0;	
void DrawWindow(int x, int y, int w, int h, const char* title, ...)
{
	va_list va_alist;
	char buf[256];

	va_start(va_alist, title);
	_vsnprintf(buf, sizeof(buf), title, va_alist);
	va_end(va_alist);

	oglSubtractive = true;
	oEngfuncs.pfnFillRGBA( x, y-20, w, 1 ,0,0,0,255 ); //top main box
	oEngfuncs.pfnFillRGBA( x, y-20, 1, 20 ,0,0,0,255 );	//left main box
	oEngfuncs.pfnFillRGBA( x+w, y-20, 1, 20+1 ,0,0,0,255 );	//right main box

	glFillArea(x+1,y-19,w-1,20,255,94,94,190,163,0,0,190); // title box body

	Count++; 
	if(Count < 100)
	{
		oEngfuncs.pfnFillRGBA(x+7,y-12,6,6,0,0,0,255); // little black box
		oEngfuncs.pfnFillRGBA(x+6,y-13,1,7,255,255,255,255); // left side little black box
		oEngfuncs.pfnFillRGBA(x+6,y-13,7,1,255,255,255,255); // top little black box
		oEngfuncs.pfnFillRGBA(x+6,y+7-13,7+1,1,255,255,255,255); // bottem side little black box
		oEngfuncs.pfnFillRGBA(x+7+6,y-13,1,7+1,255,255,255,255); // right little black box
	}
	else if(Count > 100 && Count < 200)
	{
		oEngfuncs.pfnFillRGBA(x+7,y-12,6,6,255,255,255,255); // little white box
		oEngfuncs.pfnFillRGBA(x+6,y-13,1,7,0,0,0,255); // left side little black box
		oEngfuncs.pfnFillRGBA(x+6,y-13,7,1,0,0,0,255); // top little black box
		oEngfuncs.pfnFillRGBA(x+6,y+7-13,7+1,1,0,0,0,255); // bottem side little black box
		oEngfuncs.pfnFillRGBA(x+7+6,y-13,1,7+1,0,0,0,255); // right little black box
	}
	else if(Count > 200)
	{
		oEngfuncs.pfnFillRGBA(x+7,y-12,6,6,0,0,0,255); // little black box
		oEngfuncs.pfnFillRGBA(x+6,y-13,1,7,255,255,255,255); // left side little black box
		oEngfuncs.pfnFillRGBA(x+6,y-13,7,1,255,255,255,255); // top little black box
		oEngfuncs.pfnFillRGBA(x+6,y+7-13,7+1,1,255,255,255,255); // bottem side little black box
		oEngfuncs.pfnFillRGBA(x+7+6,y-13,1,7+1,255,255,255,255); // right little black box
		Count = 0;
	}

	//DrawConString(x+18, y-19, 80,222,140, buf);
	DrawConsoleStringEx( x+18, y-19, 255,255,0, buf );

	oEngfuncs.pfnFillRGBA( x, y, w, 1 ,0,0,0,255 ); //top main box
	oEngfuncs.pfnFillRGBA( x, y, 1, h ,0,0,0,255 );	//left main box
	oEngfuncs.pfnFillRGBA( x+w, y, 1, h+1 ,0,0,0,255 );	//right main box
	oEngfuncs.pfnFillRGBA( x, y+h, w+1, 1 ,0,0,0,255 ); //bottom main box

	oEngfuncs.pfnFillRGBA(x+1,y+1,w-1,h-1,200,200,200,160); // main box body
	oglSubtractive = false;
}

void DrawWindowCon(int x, int y, int w, int h, const char* title, ...)
{
	va_list va_alist;
	char buf[256];

	va_start(va_alist, title);
	_vsnprintf(buf, sizeof(buf), title, va_alist);
	va_end(va_alist);

	oglSubtractive = true;
	oEngfuncs.pfnFillRGBA( x, y-20, w, 1 ,0,0,0,255 ); //top main box
	oEngfuncs.pfnFillRGBA( x, y-20, 1, 20 ,0,0,0,255 );	//left main box
	oEngfuncs.pfnFillRGBA( x+w, y-20, 1, 20+1 ,0,0,0,255 );	//right main box

	glFillArea(x+1,y-19,w-1,20,255,94,94,190,163,0,0,190); // title box body

	if(Count < 100)
	{
		oEngfuncs.pfnFillRGBA(x+7,y-12,6,6,0,0,0,255); // little black box
		oEngfuncs.pfnFillRGBA(x+6,y-13,1,7,255,255,255,255); // left side little black box
		oEngfuncs.pfnFillRGBA(x+6,y-13,7,1,255,255,255,255); // top little black box
		oEngfuncs.pfnFillRGBA(x+6,y+7-13,7+1,1,255,255,255,255); // bottem side little black box
		oEngfuncs.pfnFillRGBA(x+7+6,y-13,1,7+1,255,255,255,255); // right little black box
	}
	else if(Count > 100 && Count < 200)
	{
		oEngfuncs.pfnFillRGBA(x+7,y-12,6,6,255,255,255,255); // little white box
		oEngfuncs.pfnFillRGBA(x+6,y-13,1,7,0,0,0,255); // left side little black box
		oEngfuncs.pfnFillRGBA(x+6,y-13,7,1,0,0,0,255); // top little black box
		oEngfuncs.pfnFillRGBA(x+6,y+7-13,7+1,1,0,0,0,255); // bottem side little black box
		oEngfuncs.pfnFillRGBA(x+7+6,y-13,1,7+1,0,0,0,255); // right little black box
	}
	else if(Count > 200)
	{
		oEngfuncs.pfnFillRGBA(x+7,y-12,6,6,0,0,0,255); // little black box
		oEngfuncs.pfnFillRGBA(x+6,y-13,1,7,255,255,255,255); // left side little black box
		oEngfuncs.pfnFillRGBA(x+6,y-13,7,1,255,255,255,255); // top little black box
		oEngfuncs.pfnFillRGBA(x+6,y+7-13,7+1,1,255,255,255,255); // bottem side little black box
		oEngfuncs.pfnFillRGBA(x+7+6,y-13,1,7+1,255,255,255,255); // right little black box
	}

	//DrawConString(x+18, y-19, 80,222,140, buf);
	DrawConsoleStringEx( x+18, y-19, 255,255,0, buf );

	oEngfuncs.pfnFillRGBA( x, y, w, 1 ,0,0,0,255 ); //top main box
	oEngfuncs.pfnFillRGBA( x, y, 1, h ,0,0,0,255 );	//left main box
	oEngfuncs.pfnFillRGBA( x+w, y, 1, h+1 ,0,0,0,255 );	//right main box
	oEngfuncs.pfnFillRGBA( x, y+h, w+1, 1 ,0,0,0,255 ); //bottom main box

	oEngfuncs.pfnFillRGBA(x+1,y+1,w-1,h-1,200,200,200,160); // main box body

	oEngfuncs.pfnFillRGBA(x,y+h-14,w,1,0,0,0,255); // main box line
	oglSubtractive = false;
}

//===========================================================
void DrawCrosshair(void)
{
	oglSubtractive = true;
		oEngfuncs.pfnFillRGBA( displayCenterX-4, displayCenterY-1, 3, 1, 255, 0, 0, 255 );
		oEngfuncs.pfnFillRGBA( displayCenterX+2, displayCenterY-1, 3, 1, 255, 0, 0, 255 );
		oEngfuncs.pfnFillRGBA( displayCenterX-4, displayCenterY+1, 3, 1, 255, 0, 0, 255 );
		oEngfuncs.pfnFillRGBA( displayCenterX+2, displayCenterY+1, 3, 1, 255, 0, 0, 255 );
		
		oEngfuncs.pfnFillRGBA( displayCenterX-1, displayCenterY-4, 1, 3, 255, 0, 0, 255 );
		oEngfuncs.pfnFillRGBA( displayCenterX-1, displayCenterY+2, 1, 3, 255, 0, 0, 255 );
		oEngfuncs.pfnFillRGBA( displayCenterX+1, displayCenterY-4, 1, 3, 255, 0, 0, 255 );
		oEngfuncs.pfnFillRGBA( displayCenterX+1, displayCenterY+2, 1, 3, 255, 0, 0, 255 );
	oglSubtractive = false;
}