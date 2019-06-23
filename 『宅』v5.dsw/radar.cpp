/**********************************************************
¡ô©©©Ç zHA! H00k vEr:5.0 ©Ï©©¡ô
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
¡ô©©©Ç      (c)2018      ©Ï©©¡ô
    File Name: Radar.cpp
************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <stdlib.h>
#include <math.h>
#undef NDEBUG
#include <assert.h>
#include <memory.h>
#include <map>
#include <vector>
#include <fstream>
#include "radar.h"
#include "cvars.h"
#include "drawing.h"
#include "calcscreen.h"
#include "players.h"
#include "xorstr.h"

extern SCREENINFO screeninfo;
int ov_radar_x, ov_radar_y;
float ov_zoom = 5.5f;
bool mapLoaded = false;

#define BOUND_VALUE(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}
#define PI 3.14159265358979f

model_s * m_MapSprite = 0;
int xTiles = 1, yTiles = 1;

float m_OverviewZoom = 1.0f;
float ov_dx=3, ov_dy=4;
bool initi = false;

void overview_calcRadarPoint(float* , int * , int *);

const float screenaspect = (float)(4.0/3.0);

struct overviewInfo_s
{
	float	zoom;		// zoom of map images
	int		layers;		// how may layers do we have
	float	origin[3];  // 
	float	layersHeights[1];
	char	layersImages[1][255];
	int		rotated;	// are map images rotated (90 degrees) ?
};
static overviewInfo_s m_OverviewData;

static bool LoadOvRadar = false;

using namespace std;

//===========================================================
bool parse_overview(char* overview_txt)
{
	// defaults
	m_OverviewData.origin[0] = 0.0f;
	m_OverviewData.origin[1] = 0.0f;
	m_OverviewData.origin[2] = 0.0f;
	m_OverviewData.zoom	= 1.0f;
	m_OverviewData.layers = 0;
	m_OverviewData.layersHeights[0]   = 0.0f;
	m_OverviewData.layersImages[0][0] = 0;
	
	// parse file:
	char token[1024];
	char* pfile = (char *)oEngfuncs.COM_LoadFile( overview_txt, 5, NULL);

	if (!pfile)
	{
		//gConsole.echo("can't open file %s.", overview_txt );
		mapLoaded = false;
		return false;
	}

	while (true)
	{
		pfile = oEngfuncs.COM_ParseFile(pfile, token);
		if (!pfile) 
		{ 
			break; 
		}

		if ( !stricmp( token, "global" ) )
		{
			// parse the global data
			pfile = oEngfuncs.COM_ParseFile(pfile, token);
			if ( stricmp( token, "{" ) ) 
			{
				//gConsole.echo("parse error in %s", overview_txt );
				mapLoaded = false;
				return false;
			}

			pfile = oEngfuncs.COM_ParseFile(pfile,token);
			if (!pfile) 
			{ 
				break; 
			}

			while (stricmp( token, "}") )
			{
				if ( !stricmp( token, "zoom" ) )
				{
					pfile = oEngfuncs.COM_ParseFile(pfile,token);
					m_OverviewData.zoom = (float)atof( token );
					m_OverviewZoom = m_OverviewData.zoom;
				} 
				else if ( !stricmp( token, "origin" ) )
				{
					pfile = oEngfuncs.COM_ParseFile(pfile, token); 
					m_OverviewData.origin[0] = (float)atof( token );
					pfile = oEngfuncs.COM_ParseFile(pfile,token); 
					m_OverviewData.origin[1] = (float)atof( token );
					pfile = oEngfuncs.COM_ParseFile(pfile, token); 
					m_OverviewData.origin[2] = (float)atof( token );
				}
				else if ( !stricmp( token, "rotated" ) )
				{
					pfile = oEngfuncs.COM_ParseFile(pfile,token); 
					m_OverviewData.rotated = atoi( token );
				}

				pfile = oEngfuncs.COM_ParseFile(pfile,token); // parse next token
			}
		}
		else if ( !stricmp( token, "layer" ) )
		{
			pfile = oEngfuncs.COM_ParseFile(pfile,token);
				
			if ( stricmp( token, "{" ) ) 
			{
				//gConsole.echo("parse error in %s", overview_txt );
				mapLoaded = false;
				return false;
			}

			pfile = oEngfuncs.COM_ParseFile(pfile,token);

			while (stricmp( token, "}") )
			{
				if ( !stricmp( token, "image" ) )
				{
					pfile = oEngfuncs.COM_ParseFile(pfile,token);
					strcpy(m_OverviewData.layersImages[ m_OverviewData.layers ], token);
				} 
				else if ( !stricmp( token, "height" ) )
				{
					pfile = oEngfuncs.COM_ParseFile(pfile,token); 
					float height = (float)atof(token);
					m_OverviewData.layersHeights[ m_OverviewData.layers ] = height;
				}
				pfile = oEngfuncs.COM_ParseFile(pfile,token); // parse next token
			}

			m_OverviewData.layers++;
		}
	}
	return true;
}
//===========================================================
void overview_load( char* levelname )
{
	// dont load same map again
	static char last_levelname[256]="";
	char overview_txt[256];

	if( !strcmp(last_levelname,levelname)) 	{ return;} 

	// parse file
	if(levelname[0] == NULL)::strcpy(levelname,"cs_miltia");
	sprintf(overview_txt, "overviews/%s.txt", levelname );
	bool parse_success = parse_overview(overview_txt);

	if(!parse_success) 
	{ 
		//gConsole.echo("couldnt parse %s",overview_txt); 
		strcpy(last_levelname,levelname);
		mapLoaded = false;
		return;
	}

	// map sprite
	m_MapSprite = oEngfuncs.LoadMapSprite( m_OverviewData.layersImages[0] );

	if( !m_MapSprite ) 
	{
		//gConsole.echo("couldnt load %s",m_OverviewData.layersImages[0]);
		strcpy(last_levelname,levelname);
		mapLoaded = false;
		return; 
	}

	mapLoaded = true;

	// set additional data	
	int i = m_MapSprite->numframes / (4*3);
	i = (int)sqrt(i);
	xTiles = i*4;
	yTiles = i*3;
}
//===========================================================
void overview_loadcurrent()
{
	if(!oEngfuncs.pfnGetLevelName)return;

	char levelname[256];

	strcpy(levelname, oEngfuncs.pfnGetLevelName()+5);

	if (strlen(levelname)<5)return;

	levelname[strlen(levelname)-4] = 0;

	overview_load( levelname );
}
//===========================================================
inline void get_player_tile_coords(float& x, float& y)
{
	if(m_OverviewData.rotated)
	{
		float origin_tilex = (float)(-ov_dy + m_OverviewData.zoom * (1.0/1024.0) * m_OverviewData.origin[0]);
		float origin_tiley = (float)( ov_dx + m_OverviewData.zoom * (1.0/1024.0) * m_OverviewData.origin[1]);

		y = (float)(origin_tilex - (1.0/1024) * m_OverviewData.zoom * me.pmEyePos[0]);
		x = (float)(origin_tiley - (1.0/1024) * m_OverviewData.zoom * me.pmEyePos[1]);
		y = -y;
	}
	else
	{
		float origin_tilex = (float)( ov_dx + m_OverviewData.zoom * (1.0/1024.0) * m_OverviewData.origin[0]);
		float origin_tiley = (float)( ov_dy + m_OverviewData.zoom * (1.0/1024.0) * m_OverviewData.origin[1]);

		x = (float)(origin_tilex - (1.0/1024) * m_OverviewData.zoom * me.pmEyePos[0]);
		y = (float)(origin_tiley - (1.0/1024) * m_OverviewData.zoom * me.pmEyePos[1]);
	}
}
//===========================================================
int clientopenglcalc(int number) // y axis correction :)
{
	return (screeninfo.iHeight - number);
}
//===========================================================
void drawcross(int x,int y,int w,int h,int r, int g, int b, int a)
{
	oEngfuncs.pfnFillRGBA( x - w, y , w * 2 , 2 ,r,g,b,a);
	oEngfuncs.pfnFillRGBA( x, y - h , 2 , h ,r,g,b,a);

	oEngfuncs.pfnFillRGBA( x - w, y-1 , w , 1 ,0,0,0,a);
	oEngfuncs.pfnFillRGBA( x + 2, y-1 , w - 2 , 1 ,0,0,0,a);
	oEngfuncs.pfnFillRGBA( x - w, y+2 , w * 2 , 1 ,0,0,0,a);

	oEngfuncs.pfnFillRGBA( x - w - 1, y-1 , 1 , 4 ,0,0,0,a);
	oEngfuncs.pfnFillRGBA( x + w, y-1 , 1 , 4 ,0,0,0,a);

	oEngfuncs.pfnFillRGBA( x-1, y-h , 1 , h-1 ,0,0,0,a);
	oEngfuncs.pfnFillRGBA( x+2, y-h , 1 , h-1 ,0,0,0,a);

	oEngfuncs.pfnFillRGBA( x-1, y-h-1 , 4 , 1 ,0,0,0,a);
}

//===========================================================
void overview_redraw()
{
	if (!LoadOvRadar)
	{
		LoadOvRadar = true;
		overview_loadcurrent();
	}
	int size = (int)cvar.radar_size;
	
	ov_radar_x = (screeninfo.iWidth /2);
	ov_radar_y = (screeninfo.iHeight/2);
	DrawWindow(cvar.radar_x-size-1, cvar.radar_y-size-1, 2*size+1, 2*size+1, /*-= À×´ï =-*/XorStr<0x05,11,0x48C25417>("\x28\x3B\x27\xC8\xDE\xBE\xE4\x2C\x30\x23"+0x48C25417).s);
	glViewport(cvar.radar_x-size,clientopenglcalc(cvar.radar_y+size),(size*2),(size*2));	

	if (m_MapSprite)
	{
		float z  = ( 90.0f - mainViewAngles[0] ) / 90.0f;		
		z *= m_OverviewData.layersHeights[0]; // gOverviewData.z_min - 32;	

		float xStep =  (2*4096.0f /  ov_zoom ) / xTiles;
		float yStep = -(2*4096.0f / (ov_zoom*screenaspect) ) / yTiles;

		float vStepRight[2];
		float vStepUp[2];
		float angle = (float)((mainViewAngles[1]+90.0)* (PI/180.0));
		
		if(m_OverviewData.rotated)	{ angle -= float(PI/2); }

		vStepRight[0] = (float)cos( angle )*xStep;
		vStepRight[1] = (float)sin( angle )*xStep;
		vStepUp   [0] = (float)cos( angle + (PI/2))*yStep;
		vStepUp   [1] = (float)sin( angle + (PI/2))*yStep;

		float tile_x, tile_y;
		get_player_tile_coords( tile_x, tile_y );

		float xs = ov_radar_x - tile_x*vStepRight[0] - tile_y*vStepUp[0];
		float ys = ov_radar_y - tile_x*vStepRight[1] - tile_y*vStepUp[1];

		float inner[2];
		float outer[2];

		outer[0] = xs;
		outer[1] = ys;	

		int frame = 0;	

		oEngfuncs.pTriAPI->RenderMode( kRenderTransTexture );
		oEngfuncs.pTriAPI->CullFace( TRI_NONE );

		glEnable (GL_BLEND);

		glColor4f(1.0f, 1.0f, 1.0f,0.8f);//set alpha value here.000

		for (int ix = 0; ix < yTiles; ix++)
		{
			inner[0] = outer[0];
			inner[1] = outer[1];

			for (int iy = 0; iy < xTiles; iy++)	
			{
				oEngfuncs.pTriAPI->SpriteTexture( m_MapSprite, frame );
				oEngfuncs.pTriAPI->Begin( TRI_QUADS );
				oEngfuncs.pTriAPI->TexCoord2f( 0, 0 );
				oEngfuncs.pTriAPI->Vertex3f (inner[0], inner[1], z);

				oEngfuncs.pTriAPI->TexCoord2f( 0, 1 );
				oEngfuncs.pTriAPI->Vertex3f (inner[0]+vStepRight[0], inner[1]+vStepRight[1], z);

				oEngfuncs.pTriAPI->TexCoord2f( 1, 1 );
				oEngfuncs.pTriAPI->Vertex3f (inner[0]+vStepRight[0]+vStepUp[0], inner[1]+vStepRight[1]+vStepUp[1], z);

				oEngfuncs.pTriAPI->TexCoord2f( 1, 0 );
				oEngfuncs.pTriAPI->Vertex3f (inner[0]+vStepUp[0], inner[1]+vStepUp[1], z);
				oEngfuncs.pTriAPI->End();

				frame++;

				inner[0] += vStepUp[0];
				inner[1] += vStepUp[1];
			}
		
			outer[0] += vStepRight[0];
			outer[1] += vStepRight[1];
		}
		glDisable (GL_BLEND);
	}
	glViewport(0,0,screeninfo.iWidth,screeninfo.iHeight);

	oglSubtractive = true;
	drawcross(cvar.radar_x,cvar.radar_y,size/10,size/10,255,255,255,254);			
	oglSubtractive = false;
}
//===========================================================
void VecRotateZ(float * in, float angle, float * out)
{
	float a,c,s;

	a = (float) (angle * PI/180);
	c = (float) cos(a);
	s = (float) sin(a);
	out[0] = c*in[0] - s*in[1];
	out[1] = s*in[0] + c*in[1];
	out[2] = in[2];
}
//===========================================================
void overview_calcRadarPoint(float* origin, int * screenx, int * screeny)
{
	if(	/*!me.alive ||*/	oEngfuncs.Con_IsVisible	())	return;

	float aim [3],newaim [3];

	aim[0] = origin[0] - me.pmEyePos[0];
	aim[1] = origin[1] - me.pmEyePos[1];
	aim[2] = origin[2] - me.pmEyePos[2];

	VecRotateZ(aim, -mainViewAngles[1], newaim);

	*screenx = (cvar.radar_x) - int( newaim[1]/ov_zoom * m_OverviewData.zoom * 0.3f * cvar.radar_size / 160);
	*screeny = (cvar.radar_y) - int( newaim[0]/ov_zoom * m_OverviewData.zoom * 0.4f * cvar.radar_size / 160);
}
//===========================================================
void DrawRadar()
{
	int size    = (int)cvar.radar_size;
	
	ColorEntry* clr = colorList.get(7);	
	
	DrawWindow(cvar.radar_x-size-1, cvar.radar_y-size-1, 2*size+1, 2*size+1, /*-= À×´ï =-*/XorStr<0x05,11,0x48C25417>("\x28\x3B\x27\xC8\xDE\xBE\xE4\x2C\x30\x23"+0x48C25417).s);

	oEngfuncs.pfnFillRGBA(cvar.radar_x,cvar.radar_y-size,1,2*size,clr->r,clr->g,clr->b,180);
	oEngfuncs.pfnFillRGBA(cvar.radar_x-size,cvar.radar_y,2*size,1,clr->r,clr->g,clr->b,180);
}