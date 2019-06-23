#ifndef OPENGL_H
#define OPENGL_H
	#include "color.h"
	extern bool oglSubtractive;

	void glFillArea(float x, float y, int w, int h, int r, int g, int b, int a, int rr, int gg, int bb, int aa);
	void DrawLines(int x1,int y1,int x2,int y2,int width,ColorEntry *color);
	bool StoreOffsets( void );
	void oGL_ClearBackground();
	void ApplyOpenGLHook (FARPROC* pProc,LPCSTR lpProcName);
#endif
