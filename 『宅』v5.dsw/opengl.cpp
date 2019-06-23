/**********************************************************
¡ô©©©Ç zHA! H00k vEr:5.0 ©Ï©©¡ô
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
¡ô©©©Ç      (c)2018      ©Ï©©¡ô
    File Name: Opengl32.cpp
************************************************************/

#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"GLu32.lib")
#pragma comment(lib,"GLaux.lib")
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include "cvars.h"
#include "players.h"
#include "zEngines.h"

//===========================================================
typedef void(APIENTRY *func_wglSwapBuffers)( HDC );
func_wglSwapBuffers org_wglSwapBuffers;

typedef void(__stdcall* t_glBegin)(GLenum);
typedef void(__stdcall* t_glEnd)(void);
typedef void(__stdcall* t_glBlendFunc)(GLenum, GLenum);
typedef void(__stdcall* t_glClear)(GLbitfield);
typedef void(__stdcall* t_glVertex3f)(GLfloat,GLfloat,GLfloat);
typedef void(__stdcall* t_glVertex3fv)(const GLfloat *);
typedef void(__stdcall* t_glViewport)(GLint,GLint,GLsizei,GLsizei);
typedef BOOL(__stdcall* t_wglSwapBuffers)(HDC);
typedef void(__stdcall* t_glEnable)(GLenum);
typedef void(__stdcall* t_glDisable)(GLenum);
typedef void(__stdcall* t_glPopMatrix)();
typedef void(__stdcall* t_glPushMatrix)();
typedef void(__stdcall* t_glRotatef)(GLfloat,GLfloat,GLfloat,GLfloat);
typedef void(__stdcall* t_glColor4f)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void(__stdcall* t_glReadPixels)(GLint, GLint,GLsizei, GLsizei, GLenum, GLenum, GLvoid *);

t_glColor4f		pOrig_glColor4f = NULL;
t_glBegin		pOrig_glBegin = NULL;
t_glBlendFunc	pOrig_glBlendFunc = NULL;
t_glClear		pOrig_glClear = NULL;
t_glDisable		pOrig_glDisable = NULL;
t_glEnable		pOrig_glEnable = NULL;
t_glEnd			pOrig_glEnd = NULL;
t_glVertex3f	pOrig_glVertex3f = NULL;
t_glVertex3fv	pOrig_glVertex3fv = NULL;
t_glViewport	pOrig_glViewport = NULL;
t_glPopMatrix	pOrig_glPopMatrix = NULL;
t_glPushMatrix	pOrig_glPushMatrix = NULL;
t_glRotatef		pOrig_glRotatef = NULL;
t_wglSwapBuffers pOrig_wglSwapBuffers = NULL;
t_glReadPixels  pOrig_glReadPixels = NULL;
//===========================================================
bool oglSubtractive = false;
bool bDrawingFlash  = false;
bool bDrawingSmoke  = false;
bool bTex = false;
int viewport_c=0;
extern float displayCenterX, displayCenterY;
extern SCREENINFO screeninfo;
extern cl_enginefuncs_s *pEngfuncs;

//===========================================================
void DrawLines(int x1,int y1,int x2,int y2,int width,ColorEntry *color)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(color->r,color->g,color->b,color->a);
	glLineWidth((float)width);
	glBegin(GL_LINES);
	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

//===========================================================
void glFillArea(float x, float y, int w, int h, int r, int g, int b, int a, int rr, int gg, int bb, int aa)
{
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLE_STRIP);
	glColor4ub(rr,gg,bb,aa);
	glVertex2f(x,y + h);
	glVertex2f(x + w,y + h);
	glColor4ub(r,g,b,a);
	glVertex2f(x,y);
	glVertex2f(x + w,y);
	glEnd();
	
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}
//===========================================================
void oGL_ClearBackground()
{
	if(&glClearColor && &glClear)
	{
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
//===========================================================
void __stdcall oGL_glBegin(GLenum mode)
{
	float curcol[4];
	glGetFloatv(GL_CURRENT_COLOR, curcol);
	/*
	if(!zHAi.bShotMaked && zHAi.bGameActive && cvar.FarSound){
		if(mode==GL_TRIANGLE_STRIP || mode==GL_TRIANGLE_FAN || mode==GL_TRIANGLES){ 
			glDepthRange(0, 0.4);
		} else {
			glDepthRange(0.5, 1);
		}
	}
	*/

	if(!zHAi.bShotMaked && zHAi.bGameActive){
		if ( (curcol[0]==curcol[1] && curcol[0]==curcol[2]) && (curcol[0]==0.0 || curcol[0]==1.0) 
			&& me.inZoomMode() && curcol[3]==1.0)
		{
			glColor4f(curcol[0],curcol[1],curcol[2],0.4f);
		}
	}
	(*pOrig_glBegin)(mode);
}

//===========================================================
void __stdcall oGL_glBlendFunc(GLenum sfactor, GLenum dfactor)
{
  	if (oglSubtractive)
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	else
		glBlendFunc(sfactor,dfactor);
}

//===========================================================
void __stdcall oGL_glClear(GLbitfield mask)
{
	if(!zHAi.bShotMaked && zHAi.bGameActive )
	{
		glClearColor(0.47058823529411764705882352941176f,0.47058823529411764705882352941176f,1.0f,1.0f);
	}
	(*pOrig_glClear)(mask);
}
//===========================================================
void __stdcall oGL_glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
/*
	if(!zHAi.bShotMaked && zHAi.bGameActive && cvar.sprites){
		cl_entity_t *pEnt;
		pEnt = oEngStudio.GetCurrentEntity();
		if(pEnt){
			if (pEnt->player){
				red   = 1.0f;
				green = 1.0f;
				blue  = 0.0f;
				alpha = 1.0f;
			}
		}
	}
*/
	(*pOrig_glColor4f)(red, green, blue, alpha);
}
//===========================================================
void __stdcall oGL_glDisable(GLenum mode)
{
	if(!(mode==GL_TEXTURE_2D)) bTex=true;

	(*pOrig_glDisable)(mode);
}
//===========================================================
void __stdcall oGL_glEnable(GLenum cap)
{
	if(cap==GL_TEXTURE_2D) bTex=true;

	(*pOrig_glEnable)(cap);
}

//===========================================================
void __stdcall oGL_glEnd(void)
{

	(*pOrig_glEnd)();
}

//===========================================================
void __stdcall oGL_glPopMatrix(void)
{
	/*cl_entity_t *pEnt;
	// check to see if we are in a studio rendering phase, and if a player is being rendered, get the head position from the bones
	pEnt = oEngStudio.GetCurrentEntity();
	if(pEnt && pEnt->player){
		vPlayers[pEnt->index].CalcHitbox();
	}*/

	(*pOrig_glPopMatrix)();
}

//===========================================================
void __stdcall oGL_glPushMatrix(void)
{
	(*pOrig_glPushMatrix)();
}

//===========================================================
void __stdcall oGL_glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	/*if(!zHAi.bShotMaked && zHAi.bGameActive ){
		glColor3f( 1.0f, 1.0f, 1.0f);
	}*/
	(*pOrig_glVertex3f)(x, y, z);
}

//===========================================================
void __stdcall oGL_glVertex3fv(const GLfloat *var)
{
	//if (bDrawingSmoke) return;

	(*pOrig_glVertex3fv)(var);
}

//===========================================================
void __stdcall oGL_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	
	viewport_c++; 

	static bool bStart=false;
	if (!bStart)
	{
		bStart=true;
		
		oEngfuncs.pfnConsolePrint("\n     zHAi Script\nvER:5.0.0    Author: X!nG\n\n");
		//oEngfuncs.pfnClientCmd("fs_lazy_precache 1");
		oEngfuncs.pfnClientCmd("toggleconsole");
	}
	(*pOrig_glViewport)(x,y,width,height);
}

//===========================================================
void __stdcall oGL_glRotatef ( GLfloat angle, GLfloat x,  GLfloat y,  GLfloat z ) 
{ 
	(*pOrig_glRotatef)(angle,x,y,z);
}

//===========================================================
void __stdcall oGL_wglSwapBuffers(HDC hDC)
{
	viewport_c = 0;
	(*pOrig_wglSwapBuffers)(hDC);
}

void __stdcall oGL_glReadPixels (GLint x,  GLint y,  GLsizei width,  GLsizei height,  GLenum format,  GLenum type,  GLvoid *pixels)
{
    pOrig_glReadPixels(x, y, width, height, format, type, pixels);
}
//===========================================================
void ApplyOpenGLHook(FARPROC* pProc, LPCSTR lpProcName)
{
	if (!strcmp(lpProcName,"glBegin")){
		pOrig_glBegin = (t_glBegin)*pProc;
		*pProc = (FARPROC)&oGL_glBegin;

	} else if(!strcmp(lpProcName,"glBlendFunc")){
		pOrig_glBlendFunc = (t_glBlendFunc)*pProc;
		*pProc = (FARPROC)&oGL_glBlendFunc;

	} else if(!strcmp(lpProcName,"glClear")){
		pOrig_glClear = (t_glClear)*pProc;
		*pProc = (FARPROC)&oGL_glClear;

	} else if(!strcmp(lpProcName,"glColor4f")){
		pOrig_glColor4f = (t_glColor4f)*pProc;
		*pProc = (FARPROC)&oGL_glColor4f;

	} else if(!strcmp(lpProcName,"glDisable")){
		pOrig_glDisable = (t_glDisable)*pProc;
		*pProc = (FARPROC)&oGL_glDisable;

	} else if(!strcmp(lpProcName,"glEnable")){
		pOrig_glEnable = (t_glEnable)*pProc;
		*pProc = (FARPROC)&oGL_glEnable;

	} else if(!strcmp(lpProcName,"glEnd")){
		pOrig_glEnd = (t_glEnd)*pProc;
		*pProc = (FARPROC)&oGL_glEnd;

	} else if(!strcmp(lpProcName,"glVertex3f")){
		pOrig_glVertex3f = (t_glVertex3f)*pProc;
		*pProc = (FARPROC)&oGL_glVertex3f;

	} else if(!strcmp(lpProcName,"glVertex3fv")){
		pOrig_glVertex3fv = (t_glVertex3fv)*pProc;
		*pProc = (FARPROC)&oGL_glVertex3fv;

	} else if(!strcmp(lpProcName,"glPopMatrix")){
		pOrig_glPopMatrix = (t_glPopMatrix)*pProc;
		*pProc = (FARPROC)&oGL_glPopMatrix;

	} else if(!strcmp(lpProcName,"glPushMatrix")){
		pOrig_glPushMatrix = (t_glPushMatrix)*pProc;
		*pProc = (FARPROC)&oGL_glPushMatrix;

	} else if(!strcmp(lpProcName,"glViewport")){
		pOrig_glViewport = (t_glViewport)*pProc;
		*pProc = (FARPROC)&oGL_glViewport;

	} else if(!strcmp(lpProcName,"glRotatef")){
		pOrig_glRotatef = (t_glRotatef)*pProc;
		*pProc = (FARPROC)&oGL_glRotatef;

	} else if(!strcmp(lpProcName,"wglSwapBuffers")){
		pOrig_wglSwapBuffers = (t_wglSwapBuffers)*pProc;
		*pProc = (FARPROC)&oGL_wglSwapBuffers;

	}
}
//===========================================================