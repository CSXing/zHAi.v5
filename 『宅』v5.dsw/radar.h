#ifndef Radar_H
#define Radar_H
	#include <windows.h>
	#include "opengl.h"

	void overview_draw_entity(float * origin, int radius, int r, int g, int b, int a=255);
	void overview_calcRadarPoint(float* origin, int * screenx, int * screeny);
	void overview_redraw();
	void overview_cmd();
	void overview_loadcurrent();
	int clientopenglcalc(int number);
	void DrawRadar();

	extern bool mapLoaded;
#endif