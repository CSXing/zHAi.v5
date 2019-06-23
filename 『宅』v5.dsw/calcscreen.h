#ifndef CalcScreen_H
#define CalcScreen_H
//===========================================================
	extern float mainViewOrigin[3], mainViewAngles[3],TempNewAim[3],_inOrigin[3];
	extern float mainViewFov;
	extern float displayCenterX, displayCenterY;
	extern float fCurrentFOV;

	int NewCalcScreen(float in[3], float out[2]);
	int NewCalcScreen1(float in[3],float out[2]);
	bool __fastcall CalcScreen(float *origin, float *vecScreen);
//===========================================================
#endif
