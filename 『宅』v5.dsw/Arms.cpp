/**********************************************************
¡ô©©©Ç zHA! H00k vEr:5.0 ©Ï©©¡ô
    Edit by: X!ing,cHEn
	Last Edit Date: 2018-04-01
    QQ: 910955951
¡ô©©©Ç      (c)2018      ©Ï©©¡ô
    File Name: WeaponsList.cpp
************************************************************/
#pragma warning (disable:4786) // vc++ stl "truncated browser info"

#include <string.h>
#include "engine/wrect.h"
#include "engine/cl_dll.h"
#include "common/com_model.h"
#include "Arms.h"

using namespace std;

Class_Arms cArms;
//===========================================================
void Class_Arms::SetDatas(struct Arms_Types *ItArms)
{
	switch (ItArms->Id)
	{
		case Arms_P228:
			ItArms->ClipCap = CLIPCAP_P228;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 4096.0f;
			ItArms->wallpierce1 = 0.8f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 32;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_P228;
			ItArms->recoil = 0.011f;
			break;
		case Arms_SCOUT:
			ItArms->ClipCap = CLIPCAP_SCOUT;
			ItArms->penetrate = WALL_PEN2;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.98f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 75;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_SCOUT;
			ItArms->recoil = 1.42f;
			break;
		case Arms_HEGRENADE:
			ItArms->ClipCap = 0;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 0.0f;
			ItArms->wallpierce1 = 0.0f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 0;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_NONE;
			ItArms->recoil = 0.00f;
			break;
		case Arms_XM1014:
			ItArms->ClipCap = CLIPCAP_XM1014;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 0.0f;
			ItArms->wallpierce1 = 0.0f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 0;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_XM1014;
			ItArms->recoil = 1.00f;
			break;
		case Arms_C4:
			ItArms->ClipCap = 0;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 0.0f;
			ItArms->wallpierce1 = 0.0f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 0;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_NONE;
			ItArms->recoil = 0.00f;
			break;
		case Arms_MAC10:
			ItArms->ClipCap = CLIPCAP_MAC10;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.82f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 29;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_MAC10;
			ItArms->recoil = 1.50f;
			break;
		case Arms_AUG:
			ItArms->ClipCap = CLIPCAP_AUG;
			ItArms->penetrate = WALL_PEN1;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.96f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 32;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_AUG;
			ItArms->recoil = 1.62f;
			break;
		case Arms_SMOKEGRENADE:
			ItArms->ClipCap = 0;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 0.0f;
			ItArms->wallpierce1 = 0.0f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 0;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_NONE;
			ItArms->recoil = 0.00f;
			break;
		case Arms_ELITE:
			ItArms->ClipCap = CLIPCAP_ELITE;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.75f; // left
			ItArms->wallpierce2 = 0.75f; // right
			ItArms->damage1 = 20; // left
			ItArms->damage2 = 27; // right
			ItArms->bullettype = BULLET_ELITE;
			ItArms->recoil = 0.029f;
			break;
		case Arms_FIVESEVEN:
			ItArms->ClipCap = CLIPCAP_FIVESEVEN;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 4096.0f;
			ItArms->wallpierce1 = 0.885f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 14;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_FIVESEVEN;
			ItArms->recoil = 0.0105f;
			break;
		case Arms_UMP45:
			ItArms->ClipCap = CLIPCAP_UMP45;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.82f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 30;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_UMP45;
			ItArms->recoil = 1.60f;
			break;
		case Arms_SG550:
			ItArms->ClipCap = CLIPCAP_SG550;
			ItArms->penetrate = WALL_PEN1;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.98f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 40;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_SG550;
			ItArms->recoil = 0.10f;
			break;
		case Arms_GALIL:
			ItArms->ClipCap = CLIPCAP_GALIL;
			ItArms->penetrate = WALL_PEN1;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.96f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 33;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_GALIL;
			ItArms->recoil = 0.85f;
			break;
		case Arms_FAMAS:
			ItArms->ClipCap = CLIPCAP_FAMAS;
			ItArms->penetrate = WALL_PEN1;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.96f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 33;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_FAMAS;
			ItArms->recoil = 1.65f;
			break;
		case Arms_USP:
			ItArms->ClipCap = CLIPCAP_USP;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 4096.0f;
			ItArms->wallpierce1 = 0.79f; // unsilenced
			ItArms->wallpierce2 = 0.79f; // silenced
			ItArms->damage1 = 34; // unsilenced
			ItArms->damage2 = 30; // silenced
			ItArms->bullettype = BULLET_USP;
			ItArms->recoil = 0.015f;
			break;
		case Arms_GLOCK18:
			ItArms->ClipCap = CLIPCAP_GLOCK18;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.75f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 20;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_GLOCK18;
			ItArms->recoil = 0.015f;
			break;
		case Arms_AWP:
			ItArms->ClipCap = CLIPCAP_AWP;
			ItArms->penetrate = WALL_PEN2;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.99f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 115;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_AWP;
			ItArms->recoil = 0.80f;
			break;
		case Arms_MP5:
			ItArms->ClipCap = CLIPCAP_MP5;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.84f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 26;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_MP5;
			ItArms->recoil = 1.60f;
			break;
		case Arms_M249:
			ItArms->ClipCap = CLIPCAP_M249;
			ItArms->penetrate = WALL_PEN1;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.97f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 32;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_M249;
			ItArms->recoil = 1.72f;
			break;
		case Arms_M3:
			ItArms->ClipCap = CLIPCAP_M3;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 0.0f;
			ItArms->wallpierce1 = 0.0f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 0;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_M3;
			ItArms->recoil = 1.67f;
			break;
		case Arms_M4A1:
			ItArms->ClipCap = CLIPCAP_M4A1;
			ItArms->penetrate = WALL_PEN1;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.97f; // unsilenced
			ItArms->wallpierce2 = 0.95f; // silenced
			ItArms->damage1 = 32; // unsilenced
			ItArms->damage2 = 33; // silenced
			ItArms->bullettype = BULLET_M4A1;
			ItArms->recoil = 1.65f;
			break;
		case Arms_TMP:
			ItArms->ClipCap = CLIPCAP_TMP;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.85f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 20;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_TMP;
			ItArms->recoil = 1.50f;
			break;
		case Arms_G3SG1:
			ItArms->ClipCap = CLIPCAP_G3SG1;
			ItArms->penetrate = WALL_PEN1;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.98f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 60;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_G3SG1;
			ItArms->recoil = 0.20f;
			break;
		case Arms_FLASHBANG:
			ItArms->ClipCap = 0;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 0.0f;
			ItArms->wallpierce1 = 0.0f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 0;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_NONE;
			ItArms->recoil = 0.00f;
			break;
		case Arms_DEAGLE:
			ItArms->ClipCap = CLIPCAP_DEAGLE;
			ItArms->penetrate = WALL_PEN1;
			ItArms->distance = 4096.0f;
			ItArms->wallpierce1 = 0.81f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 54;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_DEAGLE;
			ItArms->recoil = 0.019f;
			break;
		case Arms_SG552:
			ItArms->ClipCap = CLIPCAP_SG552;
			ItArms->penetrate = WALL_PEN1;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.955f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 33;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_SG552;
			ItArms->recoil = 1.67f;
			break;
		case Arms_AK47:
			ItArms->ClipCap = CLIPCAP_AK47;
			ItArms->penetrate = WALL_PEN1;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.98f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 36;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_AK47;
			ItArms->recoil = 1.923f;
			break;
		case Arms_KNIFE:
			ItArms->ClipCap = 0;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 0.0f;
			ItArms->wallpierce1 = 0.0f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 0;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_NONE;
			ItArms->recoil = 1.50f;
			break;
		case Arms_P90:
			ItArms->ClipCap = CLIPCAP_P90;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 8192.0f;
			ItArms->wallpierce1 = 0.885f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 21;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_P90;
			ItArms->recoil = 1.50f;
			break;
		default:
			ItArms->ClipCap = 0;
			ItArms->penetrate = WALL_PEN0;
			ItArms->distance = 0.0f;
			ItArms->wallpierce1 = 0.0f;
			ItArms->wallpierce2 = 0.0f;
			ItArms->damage1 = 0;
			ItArms->damage2 = 0;
			ItArms->bullettype = BULLET_NONE;
			ItArms->recoil = 0.302f;
			break;
	}
}
//===========================================================
void Class_Arms::MsgAdds(const char *weaponname, int ammo1type, int max1, int ammo2type, int max2, int slot, int slotpos, int id, int flags)
{
	struct Arms_Types dummy;
	int index, len;

	len = strlen(weaponname);
	if (len > MAX_WEAPON_NAME)
		return;
	
	index=-1;
	for (int i=0; i<ArmsItems.size(); i++)
	{
		if (ArmsItems[i].Id == id) {
			index=i;
			break;
		}
	}

	if (index==-1) {
		ArmsItems.push_back(dummy);
		index = ArmsItems.size() - 1;
		if (index>MAX_WEAPON_NAME) return;

		strcpy( ArmsItems[index].command, weaponname );
		if(strstr(weaponname,"weapon_")==weaponname) { weaponname += 7;}
		strcpy( ArmsItems[index].name, weaponname );

		ArmsItems[index].AmmoType = ammo1type;
		if (max1 == 255)
			ArmsItems[index].Max1 = -1;
		else
			ArmsItems[index].Max1 = max1;

		ArmsItems[index].Ammo2Type = ammo2type;
		if (max2 == 255)
			ArmsItems[index].Max2 = -1;
		else
			ArmsItems[index].Max2 = max2;

		ArmsItems[index].Slot = slot;
		ArmsItems[index].SlotPos = slotpos;

		ArmsItems[index].Id = id;
		ArmsItems[index].Flags = flags;

		ArmsItems[index].CAmmo = 0;
		ArmsItems[index].XAmmo = 0;

		ArmsItems[index].Active = false;

		SetDatas(&ArmsItems[index]);

		memset(&ArmsItems[index].Datas, 0, sizeof(weapon_data_t));
	}
}

//===========================================================
void Class_Arms::MsgSetAmmo(int isCurs, int Id, int Ammo)
{
	for (int i=0; i<ArmsItems.size(); i++)
	{
		if (ArmsItems[i].Id == Id) {
			ArmsItems[i].Active  = true;
			ArmsItems[i].CAmmo  = Ammo;
		} else {
			if ( ArmsItems[i].Active ){
				ArmsItems[i].Active = false;
			}
		}
	}
}
//===========================================================
void Class_Arms::MsgSetAmmoX(int Id, int Count)
{
	for (int i=0; i<ArmsItems.size(); i++)
	{
		if (ArmsItems[i].AmmoType == Id)
			ArmsItems[i].XAmmo = Count;
	}
}

//===========================================================
Arms_Types* Class_Arms::CurrentArms(void)
{
	//if ( !bGoted ){
		for (int i=0; i<ArmsItems.size(); i++)
		{
			if ( ArmsItems[i].Active ){
				nGotId = i;
	//			bGoted = true;
				return &ArmsItems[nGotId];
			}
		}
	//} else {
	//	return &ArmsItems[nGotId];
	//}
	return NULL;
}

bool Class_Arms::isPower(void)
{
	Arms_Types *TempIt = CurrentArms();
	if ( TempIt ){
		int Id = TempIt->Id;
		if (Id == Arms_DEAGLE ||
			Id == Arms_AK47)
		return true;
	}
	return false;
}
//===========================================================
bool Class_Arms::IsRecoil(int Id)
{
	if (   /*WeaponId == Arms_AUG
		|| WeaponId == Arms_M249
		|| WeaponId == Arms_M4A1
		|| WeaponId == Arms_MP5
		|| WeaponId == Arms_DEAGLE
		|| WeaponId == Arms_SG552
		|| WeaponId == Arms_AK47
		|| WeaponId == Arms_FAMAS
		|| WeaponId == Arms_GALIL
		|| WeaponId == Arms_P90*/
		Id == Arms_M4A1   ||
		Id == Arms_MP5    ||
		Id == Arms_DEAGLE ||
		Id == Arms_AK47   ||
		Id == Arms_P90
		)
		return true;

	return false;
}
bool Class_Arms::IsRecoil(void)
{
	Arms_Types *TempIt = CurrentArms();
	if ( TempIt ) 
		return IsRecoil( TempIt->Id );

	return false;
}
//===========================================================
bool Class_Arms::IsSecond(void)
{
	Arms_Types *TempIt = CurrentArms();
	if ( TempIt )
		return (TempIt->Slot == 1);

	return false;
}
//===========================================================
bool Class_Arms::InAttack(void)
{
	Arms_Types *TempIt = CurrentArms();
	if ( TempIt ) {
		if ( TempIt->Id == Arms_KNIFE || 
			 TempIt->Id == Arms_HEGRENADE || 
			 TempIt->Id == Arms_FLASHBANG || 
			 TempIt->Id == Arms_C4 || 
			 TempIt->Id == Arms_SMOKEGRENADE || 
			 TempIt->Datas.m_fInReload
			 // ||TempIt->Datas.m_flNextPrimaryAttack>0
		)
			return false;
		else
			return true;
	}
		
	return false;
}

//===========================================================
bool Class_Arms::InCrosshair(int WeaponId)
{
	if ( WeaponId == Arms_SG550 ||
		 WeaponId == Arms_G3SG1 ||
		 WeaponId == Arms_SCOUT ||
		 WeaponId == Arms_AWP)
		return true;

	return false;
}
bool Class_Arms::InCrosshair(void)
{
	Arms_Types *TempIt = CurrentArms();
	if ( TempIt ) 
		return InCrosshair( TempIt->Id );

	return false;
}

//===========================================================
void Class_Arms::SetState(int Id,weapon_data_t Data){
	Arms_Types *TempIt = CurrentArms();
	if ( TempIt ) 
	if ( TempIt->Id==Id ){
		TempIt->Datas.m_iClip=Data.m_iClip;
		TempIt->Datas.m_fInZoom=Data.m_fInZoom;
		TempIt->Datas.m_fInReload=Data.m_fInReload;
		TempIt->Datas.m_iWeaponState=Data.m_iWeaponState;
		TempIt->Datas.m_flNextPrimaryAttack=Data.m_flNextPrimaryAttack;
	}
}

bool Class_Arms::InReload(void)
{
	Arms_Types *TempIt = CurrentArms();
	if ( TempIt ) 
	if (TempIt->Datas.m_fInReload)
		return true;

	return false;
}
//===========================================================
void Class_Arms::UpdateBits(int wBits)
{
	static int Old_wBits = 0;

	if (wBits != Old_wBits)
	{
		Old_wBits = wBits;
		for (int i=0; i<ArmsItems.size(); i++)
		{
			if (wBits & (1 << ArmsItems[i].Id))
			{
				ArmsItems[i].Active = true;
			}
			else
			{
				ArmsItems[i].Active = false;
				ArmsItems[i].CAmmo = 0;
				ArmsItems[i].Datas.m_fInReload=-1;
			}
		}
	}
}