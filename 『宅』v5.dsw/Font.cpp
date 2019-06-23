/**********************************************************
¡ô©©©Ç zHA! H00k vEr:5.0 ©Ï©©¡ô
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
¡ô©©©Ç      (c)2018      ©Ï©©¡ô
    File Name: Font.cpp
************************************************************/

#pragma warning(disable:4786)
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"GLu32.lib")
#pragma comment(lib,"GLaux.lib")

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>

#include "font.h"

#define ADJUST_HEIGHT 15

HDC hMemDC = NULL;
HDC hDC = NULL;

bool Drawed = true;

float glR, glG, glB;

//===========================================================
void CreateBitmap(BITMAP *bmp, LONG width, LONG height) 
{ 
   FillMemory(bmp, sizeof(*bmp), 0); 
   bmp->bmWidth = width; 
   bmp->bmHeight = height; 
   bmp->bmWidthBytes = ((width + 7) / 8 + 1) & (~1); 
   bmp->bmPlanes = 1; 
   bmp->bmBitsPixel = 1; 
   bmp->bmBits = GlobalAlloc(GMEM_FIXED, bmp->bmWidthBytes*height); 
} 

//===========================================================
unsigned char *GetBitmapBits(HDC hDC, HBITMAP hBmp, int* size) 
{ 
   BITMAP bi; 
   unsigned char   *bits; 
   struct { 
       BITMAPINFOHEADER bih; 
       RGBQUAD col[2]; 
   }bic; 

   BITMAPINFO *binf = (BITMAPINFO *)&bic; 
   GetObject(hBmp, sizeof(bi), &bi); 
   *size = bi.bmHeight*(((bi.bmWidth + 31) & (~31)) / 8); 
   bits = (unsigned char *)GlobalAlloc(GPTR, *size); 
   binf->bmiHeader.biSize = sizeof(binf->bmiHeader); 
   binf->bmiHeader.biWidth = bi.bmWidth; 
   binf->bmiHeader.biHeight = bi.bmHeight; 
   binf->bmiHeader.biPlanes = 1; 
   binf->bmiHeader.biBitCount = 1; 
   binf->bmiHeader.biCompression = BI_RGB; 
   binf->bmiHeader.biSizeImage = *size; 
   binf->bmiHeader.biXPelsPerMeter = 1; 
   binf->bmiHeader.biYPelsPerMeter = 1; 
   binf->bmiHeader.biClrUsed = 0; 
   binf->bmiHeader.biClrImportant = 0; 
   GetDIBits(hDC, hBmp, 0, bi.bmHeight, bits, binf, DIB_RGB_COLORS); 
   return bits; 
} 

//===========================================================
unsigned char *CreateFontBitmap(HDC hDC, HFONT hFont, char *str, PSIZEL size) 
{ 
   BITMAP  bmp; 
   HBITMAP hbmp; 
   unsigned char    *pbm; 
   int     len = lstrlen(str); 
   
   HFONT   hFontOld = (HFONT)SelectObject(hDC, hFont); 

   GetTextExtentPoint32(hDC, str, len, size); 
   
   CreateBitmap(&bmp, size->cx, size->cy); 
   hbmp = CreateBitmapIndirect(&bmp); 
   
   GlobalFree(bmp.bmBits); 
   DeleteObject(&bmp); 

   if(hbmp && hMemDC)
   { 
     HBITMAP hPrevBmp = (HBITMAP)SelectObject(hMemDC, hbmp); 
     HFONT   hPrevFont; 
     int     size0; 
     BITMAP  bi; 

     SetBkColor(hMemDC, RGB(0,0,0)); 
     SetTextColor(hMemDC, RGB(255,255,255)); 
     SetBkMode(hMemDC, OPAQUE); 

     hPrevFont = (HFONT)SelectObject(hMemDC, hFont); 

     TextOut(hMemDC, 0, 0, str, len); 
     GetObject(hbmp, sizeof(bi), &bi); 

     pbm = GetBitmapBits(hMemDC, hbmp, &size0); 

     size->cx = ((bi.bmWidth+31)&(~31)); 
     size->cy = bi.bmHeight; 

	 GlobalFree(bi.bmBits); 
     DeleteObject(&bi); 

	 DeleteObject(hPrevFont); 
	 DeleteObject(hPrevBmp);
	 DeleteObject(hbmp); 
   } 
   SelectObject(hDC, hFontOld); 

   return pbm; 
}

//===========================================================
int DrawStringFont(int x,int y,char *str) 
{ 
   LOGFONT lf; 
   HFONT   hFont; 
   unsigned char *lpBitmap;
   SIZE size; 
   
   if(!hDC)
   {
	 hDC = wglGetCurrentDC();
	 hMemDC = CreateCompatibleDC(hDC);
   }

   FillMemory(&lf, sizeof(lf), 0); 
   lf.lfHeight = 10;
   lf.lfCharSet = DEFAULT_CHARSET; 
   lf.lfWeight = FW_BOLD; 
   lf.lfOutPrecision = OUT_STROKE_PRECIS; 
   lf.lfClipPrecision = CLIP_CHARACTER_PRECIS; 
   lstrcpy(lf.lfFaceName, "MS Sans Serif"); 

   hFont = CreateFontIndirect(&lf); 
   GlobalFree(&lf);

   lpBitmap = CreateFontBitmap(hDC, hFont, str, &size); 

   DeleteObject(hFont);
   glDisable(GL_TEXTURE_2D);                      
   glDisable(GL_LIGHTING);               
   glEnable(GL_COLOR_MATERIAL);
   glColor3f(glR, glG, glB);
   glRasterPos2f((float)x + 1, (float)y + 1 + ADJUST_HEIGHT); 
   glBitmap(size.cx, size.cy, 0.0, 0.0, 0.0, 0.0, lpBitmap);

   GlobalFree(lpBitmap); 

   Drawed = true;
   return x + size.cx;
} 

//===========================================================
void DrawSetTextColorEx(int r, int g, int b)
{
	glR = r/255.0f;
	glG = g/255.0f;
	glB = b/255.0f;
}

//===========================================================
int DrawConsoleStringEx(int x, int y,int r, int g, int b, char *str)
{
	if (!Drawed) {return 0;}
	
	Drawed = false;
	DrawSetTextColorEx(r,g,b);
	return DrawStringFont(x, y, str);
}

//===========================================================
