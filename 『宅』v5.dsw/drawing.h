//==============================================================================
// drawing.h
//==============================================================================
#ifndef DRAWING_H
#define DRAWING_H

	int  DrawLen(char *fmt);
	void DrawHudStringCenter (int x, int y, int r, int g, int b, const char *fmt, ... );
	void DrawHudString (int x, int y, int r, int g, int b, const char *fmt, ... );
	void DrawConStringCenter (int x, int y, int r, int g, int b, const char *fmt, ... );
	void DrawConString (int x, int y, int r, int g, int b, const char *fmt, ... );
	void gDrawBoxAtScreenXY( int x, int y, int r, int g, int b, int alpha, int radius);
	void whiteBorder(int x,int y,int w, int h);
	void blackBorder(int x,int y,int w, int h);
	void tintArea(int x,int y,int w,int h, struct ColorEntry* clr);
	void tintArea(int x,int y,int w,int h,int r,int g,int b,int a);
	void colorBorder(int x,int y,int w, int h,int r,int g,int b,int a);
	void D3DBorder(int x,int y,int w, int h, int a);
	void D3DBorder2(int x,int y,int w, int h, int a);
	void gDrawFilledBoxAtLocation( float* origin, int red,int green,int blue,int alpha, int radius);
	void DrawBox(int x, int y,int red,int green,int blue,int alpha, int radius);
	void DrawBox(int x, int y);
	void DrawVectorLine(float *flSrc,float *flDestination,int width,ColorEntry *color);

	void DrawWindow(int x, int y, int w, int h, const char* title, ...);
	void DrawWindowCon(int x, int y, int w, int h, const char* title, ...);
	void DrawCrosshair(void);
#endif