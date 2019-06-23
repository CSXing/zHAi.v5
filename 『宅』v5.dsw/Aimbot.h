#ifndef Aimbot_H
#define Aimbot_H

	#include <windows.h>
	#include <vector>
	#include "client.h"

	using namespace std;

	struct aimvec
	{
		float h;
		float f;
		float r;
	};

	class CAimbot
	{
	protected:
		void init() 
		{ 
			iTarget = -1;
			active = false;
		}

	public:
		int iTarget;
		bool active,bToKill;
		vec3_t aim_viewangles;
		int CanPenetrate( float *start, float *end, int power );
		int CorrectGun();
		void Draw();
		void Draw_Player_Vecs(int ax);
		void CalculateHitbox( cl_entity_s *pEnt );
		void CalculateAimingView( void );
		inline void SetTarget( int iIndex ){ iTarget = iIndex; }
		bool HasTarget( void ){ return iTarget != -1; }
		void FindTarget( void );
		bool CheckTeam(int ax);
		//float calcFovAngle(int ax);
		float calcFovAngle(int ax);
		//float calcFovAngle(const float* origin_viewer, const float* angle_viewer, const float* origin_target);
		void calcFovangleAndVisibility(int ax);
		bool pathFree(float* xfrom,float* xto);
		bool TargetRegion(int ax);
		void CL_CreateMove(usercmd_s* usercmd);
	public:
		CAimbot() { init(); }
	};

	void VectorAngles( const float *forward, float *angles );

	extern CAimbot Aimbot;

#endif