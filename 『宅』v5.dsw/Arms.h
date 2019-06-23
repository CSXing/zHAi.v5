
#ifndef WEAPONSLIST_H
#define WEAPONSLIST_H

	#include <vector>
	#include "client.h"

	#define Arms_P228			1
	#define	Arms_UNKNOWN1		2
	#define	Arms_SCOUT			3
	#define	Arms_HEGRENADE		4
	#define	Arms_XM1014			5
	#define	Arms_C4				6
	#define	Arms_MAC10			7
	#define	Arms_AUG			8
	#define	Arms_SMOKEGRENADE	9
	#define	Arms_ELITE			10
	#define	Arms_FIVESEVEN		11
	#define	Arms_UMP45			12
	#define	Arms_SG550			13
	#define	Arms_GALIL			14
	#define	Arms_FAMAS			15
	#define	Arms_USP			16
	#define	Arms_GLOCK18		17
	#define	Arms_AWP			18
	#define	Arms_MP5			19
	#define	Arms_M249			20
	#define	Arms_M3				21
	#define	Arms_M4A1			22
	#define	Arms_TMP			23
	#define	Arms_G3SG1			24
	#define	Arms_FLASHBANG		25
	#define	Arms_DEAGLE			26
	#define	Arms_SG552			27
	#define	Arms_AK47			28
	#define	Arms_KNIFE			29
	#define	Arms_P90			30


	#define CLIPCAP_AK47 30
	#define CLIPCAP_AUG 30
	#define CLIPCAP_AWP 10
	#define CLIPCAP_DEAGLE 7
	#define CLIPCAP_ELITE 15
	#define CLIPCAP_FIVESEVEN 20
	#define CLIPCAP_G3SG1 20
	#define CLIPCAP_GLOCK18 20
	#define CLIPCAP_M249 100
	#define CLIPCAP_M3 8
	#define CLIPCAP_M4A1 30
	#define CLIPCAP_MAC10 30
	#define CLIPCAP_MP5 30
	#define CLIPCAP_P228 13
	#define CLIPCAP_P90 50
	#define CLIPCAP_SCOUT 10
	#define CLIPCAP_SG550 30
	#define CLIPCAP_SG552 30
	#define CLIPCAP_TMP 30
	#define CLIPCAP_UMP45 25
	#define CLIPCAP_USP 12
	#define CLIPCAP_XM1014 7
	#define CLIPCAP_GALIL 35
	#define CLIPCAP_FAMAS 25

	enum
	{
		BULLETTYPE_0=0,
		BULLETTYPE_1=1,
		BULLETTYPE_2=2,
		BULLETTYPE_3=3,
		BULLETTYPE_4=4,
		BULLETTYPE_5=5,
		BULLETTYPE_6=6,
		BULLETTYPE_7=7,
		BULLETTYPE_8=8,
		BULLETTYPE_9=9,
		BULLETTYPE_10=10,
		BULLETTYPE_11=11,
		BULLETTYPE_12=12,
		BULLETTYPE_13=13,
		BULLETTYPE_14=14,
		BULLETTYPE_15=15
	};

	#define BULLET_NONE BULLETTYPE_0
	#define BULLET_AK47 BULLETTYPE_11
	#define BULLET_AUG BULLETTYPE_12
	#define BULLET_AWP BULLETTYPE_10
	#define BULLET_DEAGLE BULLETTYPE_13
	#define BULLET_ELITE BULLETTYPE_1
	#define BULLET_FIVESEVEN BULLETTYPE_15
	#define BULLET_G3SG1 BULLETTYPE_11
	#define BULLET_GLOCK18 BULLETTYPE_1
	#define BULLET_M249 BULLETTYPE_12
	#define BULLET_M4A1 BULLETTYPE_12
	#define BULLET_MAC10 BULLETTYPE_9
	#define BULLET_MP5 BULLETTYPE_1
	#define BULLET_P228 BULLETTYPE_15
	#define BULLET_P90 BULLETTYPE_14
	#define BULLET_SCOUT BULLETTYPE_11
	#define BULLET_SG550 BULLETTYPE_12
	#define BULLET_SG552 BULLETTYPE_12
	#define BULLET_TMP BULLETTYPE_1
	#define BULLET_UMP45 BULLETTYPE_9
	#define BULLET_USP BULLETTYPE_9
	#define BULLET_M3 BULLETTYPE_0
	#define BULLET_XM1014 BULLETTYPE_0
	#define BULLET_GALIL BULLETTYPE_12
	#define BULLET_FAMAS BULLETTYPE_12


	#define WALL_PEN0 0
	#define WALL_PEN1 1
	#define WALL_PEN2 2

	#define USP_SILENCER (1 << 0)
	#define GLOCK18_BURST (1 << 1)
	#define M4A1_SILENCER (1 << 2)
	#define ELITE_LEFT (1 << 4)

	#define MAX_WEAPON_NAME 128

	struct Arms_Types
	{
		char name[MAX_WEAPON_NAME];
		int len;
		char command[MAX_WEAPON_NAME];
		int AmmoType;
		int Ammo2Type;
		int Max1;
		int Max2;
		int Slot;
		int SlotPos;
		int Id;         // 枪支Id
		int Flags;
		int ClipCap;	// 能容纳的弹药数
		int CAmmo;		// 当前弹药数
		int XAmmo;		// 额外的弹药
		bool Active;	// 激活状态当前的武器
		bool CurWeapon; //
		float recoil;   // 反扩散数值
		int penetrate;
		float distance;
		int bullettype;
		float wallpierce1;
		float wallpierce2;
		int damage1;
		int damage2;
		weapon_data_t Datas; //枪支信息
	};

	class Class_Arms
	{
	protected:
		void init() 
		{
			ArmsItems.clear();
		}
	private:
		vector<Arms_Types> ArmsItems;
	public:
		bool bChanged,bGoted;
		int  nGotId;
		int  Index(void) {
			Arms_Types *TempIt = CurrentArms();
			if ( TempIt ) 
				return TempIt->Id;
			return -1;
		}
		char *Name(void) {
			Arms_Types *TempIt = CurrentArms();
			if ( TempIt ) {
				return TempIt->name;
			}
			return "";
		}
		float Recoil(void) {
			Arms_Types *TempIt = CurrentArms();
			if ( TempIt ) {
				return TempIt->recoil;
			}
			return 0.001f;
		}

		void SetDatas(struct Arms_Types *ItArms);
		void SetState(int Id,weapon_data_t Data);

		bool IsRecoil(int Id); //枪支是否反扩散
		bool IsRecoil(void);   //枪支是否反扩散
		bool IsSecond(void);   //正在使用手枪

		bool InAttack(void);      //正在使用可用武器(枪支,不含炸弹和C4)
		bool InCrosshair(int Id); //是否需要画准星
		bool InCrosshair(void);   //是否需要画准星
		bool InReload(void);      //正在换弹
		bool isPower(void);       //强自瞄枪支
		Arms_Types* CurrentArms(void); // 查找当前枪支
		// MsgHook Functions
		void MsgAdds(const char *weaponname, int ammo1type, int max1, int ammo2type, int max2, int slot, int slotpos, int id, int flags);
		void MsgSetAmmo(int isCurs, int Id, int Ammo);
		void MsgSetAmmoX(int Id, int Count);
		void UpdateBits(int wBits);
	public:
		Class_Arms() { init(); }
	};
	
	extern Class_Arms cArms;
#endif
