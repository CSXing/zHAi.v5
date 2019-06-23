
#pragma optimize("gtp",on)

#pragma warning (disable:4786) // vc++ stl "truncated browser info"

#include <windows.h>
#include <vector>
#include "engine/wrect.h"
#include "engine/cl_dll.h"
#include "common/event_api.h"
#include "engine/pmtrace.h"
#include "engine/pm_defs.h"
#include "engine/cl_entity.h"
#include "players.h"
#include "eventhook.h"

void (*glock1Org)(struct event_args_s *args);
void (*glock2Org)(struct event_args_s *args);
void (*shotgun1Org)(struct event_args_s *args);
void (*shotgun2Org)(struct event_args_s *args);
void (*mp5Org)(struct event_args_s *args);
void (*pythonOrg)(struct event_args_s *args);
void (*gaussOrg)(struct event_args_s *args);
void (*gaussspinOrg)(struct event_args_s *args);
void (*trainOrg)(struct event_args_s *args);
void (*vehicleOrg)(struct event_args_s *args);
void (*uspOrg)(struct event_args_s *args);
void (*mp5nOrg)(struct event_args_s *args);
void (*ak47Org)(struct event_args_s *args);
void (*augOrg)(struct event_args_s *args);
void (*deagleOrg)(struct event_args_s *args);
void (*g3sg1Org)(struct event_args_s *args);
void (*sg550Org)(struct event_args_s *args);
void (*glock18Org)(struct event_args_s *args);
void (*m249Org)(struct event_args_s *args);
void (*m3Org)(struct event_args_s *args);
void (*m4a1Org)(struct event_args_s *args);
void (*mac10Org)(struct event_args_s *args);
void (*p90Org)(struct event_args_s *args);
void (*p228Org)(struct event_args_s *args);
void (*awpOrg)(struct event_args_s *args);
void (*scoutOrg)(struct event_args_s *args);
void (*sg552Org)(struct event_args_s *args);
void (*tmpOrg)(struct event_args_s *args);
void (*fivesevenOrg)(struct event_args_s *args);
void (*ump45Org)(struct event_args_s *args);
void (*xm1014Org)(struct event_args_s *args);
void (*elite_leftOrg)(struct event_args_s *args);
void (*elite_rightOrg)(struct event_args_s *args);
void (*knifeOrg)(struct event_args_s *args);
void (*decal_resetOrg)(struct event_args_s *args);
void (*createsmokeOrg)(struct event_args_s *args);

void Event_usp(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		if ( now_Gun ){
			if (now_Gun->Spread.prevtime)
			{
				now_Gun->Spread.spreadvar = now_Gun->Spread.spreadvar - (0.275 * (0.3 - (now_Gun->Spread.gtime - now_Gun->Spread.prevtime)));

				if (now_Gun->Spread.spreadvar > 0.92)
					now_Gun->Spread.spreadvar = 0.92f;
				else if (now_Gun->Spread.spreadvar < 0.6)
					now_Gun->Spread.spreadvar = 0.6f;
			}

			now_Gun->Spread.recoil++;
			now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
		}
	}

	uspOrg(args);
}


void Event_mp5n(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		if ( now_Gun ){
			now_Gun->Spread.recoil++;
			now_Gun->Spread.spreadvar = now_Gun->Spread.recoil * now_Gun->Spread.recoil * 0.004543389368468878 + 0.45;

			if (now_Gun->Spread.spreadvar > 0.75)
				now_Gun->Spread.spreadvar = 0.75f;

			now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
			now_Gun->Spread.firing = true;
		}
	}

	mp5nOrg(args);
}


void Event_ak47(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		if ( now_Gun ){
			now_Gun->Spread.recoil++;
			now_Gun->Spread.firing = true;

			now_Gun->Spread.spreadvar = now_Gun->Spread.recoil * now_Gun->Spread.recoil * now_Gun->Spread.recoil / 200 + 0.35;

			if (now_Gun->Spread.spreadvar > 1.25)
				now_Gun->Spread.spreadvar = 1.25f;

			now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
		}
	}

	ak47Org(args);
}


void Event_aug(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		if ( now_Gun ){
			now_Gun->Spread.recoil++;
			now_Gun->Spread.firing = true;

			now_Gun->Spread.spreadvar = now_Gun->Spread.recoil * now_Gun->Spread.recoil * now_Gun->Spread.recoil / 215 + 0.3;

			if (now_Gun->Spread.spreadvar > 1.0)
				now_Gun->Spread.spreadvar = 1.0f;

			now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
		}
	}

	augOrg(args);
}


void Event_deagle(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		if ( now_Gun ){
			if (now_Gun->Spread.prevtime)
			{
				now_Gun->Spread.spreadvar = now_Gun->Spread.spreadvar - (0.35 * (0.4 - (now_Gun->Spread.gtime - now_Gun->Spread.prevtime)));

				if (now_Gun->Spread.spreadvar > 0.9)
					now_Gun->Spread.spreadvar = 0.9f;
				else if (now_Gun->Spread.spreadvar < 0.55)
					now_Gun->Spread.spreadvar = 0.55f;
			}

			now_Gun->Spread.recoil++;
			now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
		}
	}

	deagleOrg(args);
}


void Event_g3sg1(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		if ( now_Gun ){
			if (now_Gun->Spread.brokentime)
			{
				now_Gun->Spread.spreadvar = 0.55 + (0.3 * (now_Gun->Spread.gtime - now_Gun->Spread.brokentime));

				if (now_Gun->Spread.spreadvar > 0.98)
					now_Gun->Spread.spreadvar = 0.98f;
			}

			now_Gun->Spread.recoil++;
			now_Gun->Spread.brokentime = now_Gun->Spread.gtime;
			now_Gun->Spread.firing = true;
		}
	}

	g3sg1Org(args);
}


void Event_sg550(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		if (now_Gun->Spread.brokentime)
		{
			now_Gun->Spread.spreadvar = 0.65 + (0.35 * (now_Gun->Spread.gtime - now_Gun->Spread.brokentime));

			if (now_Gun->Spread.spreadvar > 0.98)
				now_Gun->Spread.spreadvar = 0.98f;
		}

		now_Gun->Spread.recoil++;
		now_Gun->Spread.brokentime = now_Gun->Spread.gtime;
		now_Gun->Spread.firing = true;
	}

	sg550Org(args);
}


void Event_glock18(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		if (now_Gun->Spread.prevtime)
		{
			now_Gun->Spread.spreadvar = now_Gun->Spread.spreadvar - (0.275 * (0.325 - (now_Gun->Spread.gtime - now_Gun->Spread.prevtime)));

			if (now_Gun->Spread.spreadvar > 0.9)
				now_Gun->Spread.spreadvar = 0.9f;
			else if (now_Gun->Spread.spreadvar < 0.6)
				now_Gun->Spread.spreadvar = 0.6f;
		}

		now_Gun->Spread.recoil++;
		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
	}

	glock18Org(args);
}


void Event_m249(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		now_Gun->Spread.recoil++;
		now_Gun->Spread.firing = true;
		
		now_Gun->Spread.spreadvar = now_Gun->Spread.recoil * now_Gun->Spread.recoil * now_Gun->Spread.recoil / 175 + 0.4;

		if (now_Gun->Spread.spreadvar > 0.9)
			now_Gun->Spread.spreadvar = 0.9f;

		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
	}

	m249Org(args);
}


void Event_m3(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		now_Gun->Spread.recoil++;
		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
		now_Gun->Spread.firing = true;
	}

	m3Org(args);
}


void Event_m4a1(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		now_Gun->Spread.recoil++;
		now_Gun->Spread.firing = true;
		
		now_Gun->Spread.spreadvar = now_Gun->Spread.recoil * now_Gun->Spread.recoil * now_Gun->Spread.recoil / 220 + 0.3;

		if (now_Gun->Spread.spreadvar > 1)
			now_Gun->Spread.spreadvar = 1.0f;

		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
	}

	m4a1Org(args);
}


void Event_mac10(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		now_Gun->Spread.recoil++;
		now_Gun->Spread.firing = true;
		
		now_Gun->Spread.spreadvar = now_Gun->Spread.recoil * now_Gun->Spread.recoil * now_Gun->Spread.recoil / 200 + 0.6;

		if (now_Gun->Spread.spreadvar > 1.65)
			now_Gun->Spread.spreadvar = 1.65f;

		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
	}

	mac10Org(args);
}


void Event_p90(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		now_Gun->Spread.recoil++;
		now_Gun->Spread.firing = true;

		now_Gun->Spread.spreadvar = now_Gun->Spread.recoil * now_Gun->Spread.recoil / 175 + 0.45;

		if (now_Gun->Spread.spreadvar > 1)
			now_Gun->Spread.spreadvar = 1.0f;

		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
	}

	p90Org(args);
}


void Event_p228(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		if (now_Gun->Spread.prevtime)
		{
			now_Gun->Spread.spreadvar = now_Gun->Spread.spreadvar - (0.3 * (0.325 - (now_Gun->Spread.gtime - now_Gun->Spread.prevtime)));

			if (now_Gun->Spread.spreadvar > 0.9)
				now_Gun->Spread.spreadvar = 0.9f;
			else if (now_Gun->Spread.spreadvar < 0.6)
				now_Gun->Spread.spreadvar = 0.6f;
		}

		now_Gun->Spread.recoil++;
		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
	}

	p228Org(args);
}


void Event_awp(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		now_Gun->Spread.recoil++;
		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
		now_Gun->Spread.firing = true;
	}

	awpOrg(args);
}


void Event_scout(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		now_Gun->Spread.recoil++;
		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
		now_Gun->Spread.firing = true;
	}

	scoutOrg(args);
}


void Event_sg552(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		now_Gun->Spread.recoil++;
		now_Gun->Spread.firing = true;
		
		now_Gun->Spread.spreadvar = now_Gun->Spread.recoil * now_Gun->Spread.recoil * now_Gun->Spread.recoil / 220 + 0.3;

		if (now_Gun->Spread.spreadvar > 1)
			now_Gun->Spread.spreadvar = 1.0f;

		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
	}

	sg552Org(args);
}


void Event_tmp(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		now_Gun->Spread.recoil++;
		now_Gun->Spread.firing = true;

		now_Gun->Spread.spreadvar = now_Gun->Spread.recoil * now_Gun->Spread.recoil * now_Gun->Spread.recoil / 200 + 0.55;

		if (now_Gun->Spread.spreadvar > 1.4)
			now_Gun->Spread.spreadvar = 1.4f;

		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
	}

	tmpOrg(args);
}


void Event_fiveseven(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		if (now_Gun->Spread.prevtime)
		{
			now_Gun->Spread.spreadvar = now_Gun->Spread.spreadvar - (0.25 * (0.275 - (now_Gun->Spread.gtime - now_Gun->Spread.prevtime)));

			if (now_Gun->Spread.spreadvar > 0.92)
				now_Gun->Spread.spreadvar = 0.92f;
			else if (now_Gun->Spread.spreadvar < 0.725)
				now_Gun->Spread.spreadvar = 0.725f;
		}

		now_Gun->Spread.recoil++;
		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
	}

	fivesevenOrg(args);
}


void Event_ump45(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		now_Gun->Spread.recoil++;
		now_Gun->Spread.firing = true;

		now_Gun->Spread.spreadvar = now_Gun->Spread.recoil * now_Gun->Spread.recoil / 210 + 0.5;

		if (now_Gun->Spread.spreadvar > 1.0)
			now_Gun->Spread.spreadvar = 1.0f;

		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
	}

	ump45Org(args);
}


void Event_xm1014(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		now_Gun->Spread.recoil++;
		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
		now_Gun->Spread.firing = true;
	}

	xm1014Org(args);
}


void Event_elite_left(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		if (now_Gun->Spread.prevtime)
		{
			now_Gun->Spread.spreadvar = now_Gun->Spread.spreadvar - (0.275 * (0.325 - (now_Gun->Spread.gtime - now_Gun->Spread.prevtime)));

			if (now_Gun->Spread.spreadvar > 0.88)
				now_Gun->Spread.spreadvar = 0.88f;
			else if (now_Gun->Spread.spreadvar < 0.55)
				now_Gun->Spread.spreadvar = 0.55f;
		}

		now_Gun->Spread.recoil++;
		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
	}

	elite_leftOrg(args);
}


void Event_elite_right(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		if (now_Gun->Spread.prevtime)
		{
			now_Gun->Spread.spreadvar = now_Gun->Spread.spreadvar - (0.275 * (0.325 - (now_Gun->Spread.gtime - now_Gun->Spread.prevtime)));

			if (now_Gun->Spread.spreadvar > 0.88)
				now_Gun->Spread.spreadvar = 0.88f;
			else if (now_Gun->Spread.spreadvar < 0.55)
				now_Gun->Spread.spreadvar = 0.55f;
		}

		now_Gun->Spread.recoil++;
		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
	}

	elite_rightOrg(args);
}


void Event_knife(struct event_args_s *args)
{
	if (args->entindex == me.m_Index)
	{
		Guns_Types* now_Gun=me.zGuns.CurrentGuns();
		now_Gun->Spread.recoil++;
		now_Gun->Spread.prevtime = now_Gun->Spread.gtime;
	}

	knifeOrg(args);
}


void Event_decal_reset(struct event_args_s *args)
{
	decal_resetOrg(args);
}


void Event_createsmoke(struct event_args_s *args)
{
	createsmokeOrg(args);
}

void PlaybackEvent(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2)
{
	oEngfuncs.pfnPlaybackEvent(flags, pInvoker, eventindex, delay, origin, angles, fparam1, fparam2, iparam1, iparam2, bparam1, bparam2);
}


void HookEvent(char *name, void (*pfnEvent)(struct event_args_s *args))
{
	oEngfuncs.pfnHookEvent(name, pfnEvent);
	if (!strcmp(name, "events/usp.sc")) 
	{
		uspOrg = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_usp);
	}
	else if (!strcmp(name, "events/mp5n.sc")) 
	{
		mp5nOrg = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_mp5n);
	}
	else if (!strcmp(name, "events/ak47.sc")) 
	{
		ak47Org = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_ak47);
	}
	else if (!strcmp(name, "events/aug.sc")) 
	{
		augOrg = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_aug);
	}
	else if (!strcmp(name, "events/deagle.sc")) 
	{
		deagleOrg = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_deagle);
	}
	else if (!strcmp(name, "events/g3sg1.sc")) 
	{
		g3sg1Org = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_g3sg1);
	}
	else if (!strcmp(name, "events/sg550.sc")) 
	{
		sg550Org = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_sg550);
	}
	else if (!strcmp(name, "events/glock18.sc")) 
	{
		glock18Org = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_glock18);
	}
	else if (!strcmp(name, "events/m249.sc")) 
	{
		m249Org = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_m249);
	}
	else if (!strcmp(name, "events/m3.sc")) 
	{
		m3Org = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_m3);
	}
	else if (!strcmp(name, "events/m4a1.sc")) 
	{
		m4a1Org = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_m4a1);
	}
	else if (!strcmp(name, "events/mac10.sc")) 
	{
		mac10Org = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_mac10);
	}
	else if (!strcmp(name, "events/p90.sc")) 
	{
		p90Org = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_p90);
	}
	else if (!strcmp(name, "events/p228.sc")) 
	{
		p228Org = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_p228);
	}
	else if (!strcmp(name, "events/awp.sc")) 
	{
		awpOrg = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_awp);
	}
	else if (!strcmp(name, "events/scout.sc")) 
	{
		scoutOrg = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_scout);
	}
	else if (!strcmp(name, "events/sg552.sc")) 
	{
		sg552Org = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_sg552);
	}
	else if (!strcmp(name, "events/tmp.sc")) 
	{
		tmpOrg = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_tmp);
	}
	else if (!strcmp(name, "events/fiveseven.sc")) 
	{
		fivesevenOrg = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_fiveseven);
	}
	else if (!strcmp(name, "events/ump45.sc")) 
	{
		ump45Org = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_ump45);
	}
	else if (!strcmp(name, "events/xm1014.sc")) 
	{
		xm1014Org = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_xm1014);
	}
	else if (!strcmp(name, "events/elite_left.sc")) 
	{
		elite_leftOrg = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_elite_left);
	}
	else if (!strcmp(name, "events/elite_right.sc")) 
	{
		elite_rightOrg = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_elite_right);
	}
	else if (!strcmp(name, "events/knife.sc")) 
	{
		knifeOrg = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_knife);
	}
	else if (!strcmp(name, "events/decal_reset.sc")) 
	{
		decal_resetOrg = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_decal_reset);
	}
	else if (!strcmp(name, "events/createsmoke.sc")) 
	{
		createsmokeOrg = pfnEvent;
		oEngfuncs.pfnHookEvent(name, Event_createsmoke);
	}
}
