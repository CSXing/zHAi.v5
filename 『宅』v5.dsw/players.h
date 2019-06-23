/*===========================================================
    zGuns_Head
===========================================================*/
#ifndef zGuns_Head
#define zGuns_Head
    #include <vector>
    #include "client.h"

    #define Guns_P228            1
    #define    Guns_UNKNOWN1     2
    #define    Guns_SCOUT        3
    #define    Guns_HEGRENADE    4
    #define    Guns_XM1014       5
    #define    Guns_C4           6
    #define    Guns_MAC10        7
    #define    Guns_AUG          8
    #define    Guns_SMOKEGRENADE 9
    #define    Guns_ELITE        10
    #define    Guns_FIVESEVEN    11
    #define    Guns_UMP45        12
    #define    Guns_SG550        13
    #define    Guns_GALIL        14
    #define    Guns_FAMAS        15
    #define    Guns_USP          16
    #define    Guns_GLOCK18      17
    #define    Guns_AWP          18
    #define    Guns_MP5          19
    #define    Guns_M249         20
    #define    Guns_M3           21
    #define    Guns_M4A1         22
    #define    Guns_TMP          23
    #define    Guns_G3SG1        24
    #define    Guns_FLASHBANG    25
    #define    Guns_DEAGLE       26
    #define    Guns_SG552        27
    #define    Guns_AK47         28
    #define    Guns_KNIFE        29
    #define    Guns_P90          30


    #define ClipCap_AK47      30
    #define ClipCap_AUG       30
    #define ClipCap_AWP       10
    #define ClipCap_DEAGLE    7
    #define ClipCap_ELITE     15
    #define ClipCap_FIVESEVEN 20
    #define ClipCap_G3SG1     20
    #define ClipCap_GLOCK18   20
    #define ClipCap_M249      100
    #define ClipCap_M3        8
    #define ClipCap_M4A1      30
    #define ClipCap_MAC10     30
    #define ClipCap_MP5       30
    #define ClipCap_P228      13
    #define ClipCap_P90       50
    #define ClipCap_SCOUT     10
    #define ClipCap_SG550     30
    #define ClipCap_SG552     30
    #define ClipCap_TMP       30
    #define ClipCap_UMP45     25
    #define ClipCap_USP       12
    #define ClipCap_XM1014    7
    #define ClipCap_GALIL     35
    #define ClipCap_FAMAS     25

    enum
    {
        BulletEnum_0=0,
        BulletEnum_1=1,
        BulletEnum_2=2,
        BulletEnum_3=3,
        BulletEnum_4=4,
        BulletEnum_5=5,
        BulletEnum_6=6,
        BulletEnum_7=7,
        BulletEnum_8=8,
        BulletEnum_9=9,
        BulletEnum_10=10,
        BulletEnum_11=11,
        BulletEnum_12=12,
        BulletEnum_13=13,
        BulletEnum_14=14,
        BulletEnum_15=15
    };

    #define Bullet_NONE      BulletEnum_0
    #define Bullet_AK47      BulletEnum_11
    #define Bullet_AUG       BulletEnum_12
    #define Bullet_AWP       BulletEnum_10
    #define Bullet_DEAGLE    BulletEnum_13
    #define Bullet_ELITE     BulletEnum_1
    #define Bullet_FIVESEVEN BulletEnum_15
    #define Bullet_G3SG1     BulletEnum_11
    #define Bullet_GLOCK18   BulletEnum_1
    #define Bullet_M249      BulletEnum_12
    #define Bullet_M4A1      BulletEnum_12
    #define Bullet_MAC10     BulletEnum_9
    #define Bullet_MP5       BulletEnum_1
    #define Bullet_P228      BulletEnum_15
    #define Bullet_P90       BulletEnum_14
    #define Bullet_SCOUT     BulletEnum_11
    #define Bullet_SG550     BulletEnum_12
    #define Bullet_SG552     BulletEnum_12
    #define Bullet_TMP       BulletEnum_1
    #define Bullet_UMP45     BulletEnum_9
    #define Bullet_USP       BulletEnum_9
    #define Bullet_M3        BulletEnum_0
    #define Bullet_XM1014    BulletEnum_0
    #define Bullet_GALIL     BulletEnum_12
    #define Bullet_FAMAS     BulletEnum_12


    #define WALL_PEN0 0
    #define WALL_PEN1 1
    #define WALL_PEN2 2

    #define USP_SILENCER (1 << 0)
    #define GLOCK18_BURST (1 << 1)
    #define M4A1_SILENCER (1 << 2)
    #define ELITE_LEFT (1 << 4)

    #define MAX_WEAPON_NAME 128

    typedef struct spread_info_s
    {
        unsigned int random_seed;
        int   recoil;
        float gtime;
        float prevtime;
        float brokentime; // This is only supposed to be set to zero when you buy the sg550 or the g3sg1
                            // not when you reload, switch weapons or pick up a weapon, this is do to the
                            // cs bugs for these guns (valve fix your code please)
        float spreadvar;
        float recoiltime;
        bool  firing;
        int   WeaponState;
        int   prcflags;
    } spread_info_t;

    struct Guns_Types
    {
        char  name[MAX_WEAPON_NAME];
        int   len;
        char  command[MAX_WEAPON_NAME];
        int   AmmoType;
        int   Ammo2Type;
        int   Max1;
        int   Max2;
        int   Slot;
        int   SlotPos;
        int   Id;			// 枪支Id
        int   Flags;
        int   ClipCap;		// 能容纳的弹药数
        int   CAmmo;        // 当前弹药数
        int   XAmmo;        // 额外的弹药
        bool  Active;		// 激活状态当前的武器
        bool  CurWeapon;	//
        int   penetrate;
        float distance;
        int   bullettype;
        float wallpierce1;
        float wallpierce2;
        int   damage1;
        int   damage2;
        weapon_data_t Datas; //枪支信息
        spread_info_t Spread;  // 反扩散默认值
        
        float recoil;        // 反扩散数值
        float recoiltimer;
        float aimbottimer;  //自瞄时间
    };

    class _zHAiGuns
    {
    protected:
        void Init(){ GunsItems.clear(); }
    private:
        vector<Guns_Types> GunsItems;
    public:
        bool bChanged,bGoted;
        int  nGotId;
        int  Index(void) {
            Guns_Types *TempIt = CurrentGuns();
            if ( TempIt ) return TempIt->Id;
            return -1;
        }
        char *Name(void) {
            Guns_Types *TempIt = CurrentGuns();
            if ( TempIt ) return TempIt->name;
            return "";
        }
        float Recoil(void) {
            Guns_Types *TempIt = CurrentGuns();
            if ( TempIt ) return TempIt->recoil;
            return 0.0f;
        }
		float RecoilTimer(){
            Guns_Types *TempIt = CurrentGuns();
            if ( TempIt ) return TempIt->recoiltimer;
            return 0.0f;
		}

		float AimbotTimer(){
            Guns_Types *TempIt = CurrentGuns();
            if ( TempIt ) return TempIt->aimbottimer;
            return 0.0f;
		}

        void SetDatas(struct Guns_Types *ItGuns);
        void SetState(int Id,weapon_data_t Data);

        bool IsRecoil(int Id); //枪支是否反扩散
        bool IsRecoil(void);   //枪支是否反扩散
        bool IsSecond(void);   //正在使用手枪

        bool InAttack(void);      //正在使用可用武器(枪支,不含炸弹和C4)
        bool InCrosshair(int Id); //是否需要画准星
        bool InCrosshair(void);   //是否需要画准星
        bool InReload(void);      //正在换弹
        bool IsPower(void);       //强自瞄枪支
        Guns_Types* CurrentGuns(void); // 查找当前枪支
        // MsgHook Functions
        void MsgAdds(const char *weaponname, int ammo1type, int max1, int ammo2type, int max2, int slot, int slotpos, int id, int flags);
        void MsgSetAmmo(int isCurs, int Id, int Ammo);
        void MsgSetAmmoX(int Id, int Count);
        void UpdateBits(int wBits);
    public:
        _zHAiGuns() { Init(); }
    };
#endif

/*===========================================================
    Players_Head
===========================================================*/
#ifndef Players_Head
#define Players_Head
    #include <string.h>
    #include "client.h"
    #include "timehandling.h"
    #include "color.h"

    //===========================================================

    enum {
        SEQUENCE_STANDING = 0, 
        SEQUENCE_CROUCHING = 1, 
        SEQUENCE_WALKING = 2, 
        SEQUENCE_RUNNING = 3, 
        SEQUENCE_CROUCH_RUNNING = 4, 
        SEQUENCE_JUMPING = 5, 
        SEQUENCE_SWIMMING = 6 
    };

    struct spread_info
    {
        unsigned int random_seed;
        int recoil;
        float gtime;
        float prevtime;
        float brokentime; // This is only supposed to be set to zero when you buy the sg550 or the g3sg1
                            // not when you reload, switch weapons or pick up a weapon, this is do to the
                            // cs bugs for these guns (valve fix your code please)
        float spreadvar;
        float recoiltime;
        bool firing;
        int WeaponState;
        int prcflags;
    };

    struct local_player_info
    {
        //Functions
        bool isAlive()           const { return m_Alive; }
        bool isOwner(int iIndex) const { return (m_Index==iIndex); }
        bool inZoomMode()        const { return m_ZoomMode; }        
        bool InAttack(){
            if ( m_Alive && m_BulletNums ) //&& zGuns.InAttack()
                return true;
            else
                return false;
        }
        //Sub
        void setAlive()    { if(m_Team>0) m_Alive = true; }
        void setDead()     { m_Alive=false; }
        void setZoomMode(const int iFov ) { m_ZoomMode = (bool)(iFov!=90); }
        void setAmmo(const int iState, const int iID, const int iClip){
            if (iState) 
            {
                m_BulletNums=iClip;
                static int currId = -1;
                if (currId!=iID) {
                    zGuns.bChanged = true;
                    zGuns.bGoted   = false;
                    currId         = iID;
                }
                zGuns.MsgSetAmmo(iState, iID, iClip);
            }
        }
        struct cl_entity_s* m_Entity;
        float  vPunchangle[3], vForward[3];

        int pmFlags, pmMoveType;
        vec3_t pmVelocity, pmEyePos, pmViewAngles;
        float pmMaxSpeed, pmAirAccelerate, pmGroundSpeed, pmInDuck;
        
        int m_Index, m_Team, iMaxBones,iMaxHitboxes, iFov; 

        float sin_yaw, minus_cos_yaw;

        bool  isHeadshot;
        int   Headshot_Nums;
        float Headshot_Rate;

        _zHAiGuns zGuns;
    private:
        int m_BulletNums;
        bool m_Alive, m_ZoomMode;
    };

    //PlayerState
    enum
    {
        enumPS_Missing = 0,
        enumPS_Entity  = 1,
        enumPS_Sound   = 2
    };
    enum{  enumMaxPlayers = 36 };

    //nospread
    #define USP_SILENCER (1 << 0)
    #define GLOCK18_BURST (1 << 1)
    #define M4A1_SILENCER (1 << 2)
    #define ELITE_LEFT (1 << 4)

/*===========================================================
    Player Item Class
===========================================================*/
    class _Player 
    {
    protected:
        friend class _Players;
        void Init( int _Index) 
        {
            m_Index    = _Index;
            m_Team     = 0;
            m_Alive    = false;
            m_Visible  = false;
            m_Distance = 12345.0f;
            m_Fovangle = 360.0f;
            bGotHead   = false;
            m_CalcOk   = false;
            m_AutoHitbox = 9;

			m_FirstKill = false;
            strcpy(FirstKill_Name,"zHAi.First.Kills");

            strcpy(m_Name,"\\missing-name\\");
            strcpy(m_WeaponName,"missing-model");

            frags      = 0;
            deaths     = 0;
            ratio      = 0;
            bestplayer = false;
        }
    public:
        int    m_Index, m_Team, m_AutoHitbox;
        bool   bGotHead, m_Visible, m_CalcOk;
        float  m_Distance, m_Fovangle;

        vec3_t vHitbox[22], vBones[55], m_AimbotAngles, vAimPosi[22];

        int    frags;
        int    deaths;
        float  ratio;
        bool   bestplayer; // based on ratio, not frags!

        void setAlive() { m_Alive = true;  }
        void setDead()  { m_Alive = false; m_State=enumPS_Missing; }
        bool isAlive()  { return  m_Alive; }

        struct cl_entity_s * getEnt() { return oEngfuncs.GetEntityByIndex(m_Index); }


        const int   SeqInfo()    const { return m_SeqInfo; }
        const char* Name()       const { return m_Name; }
        const char* WeaponName() const { return m_WeaponName; }

        void   AddEntity(vec3_t);
        void   Clear(){ m_State=enumPS_Missing; bGotHead=false; }
        void   Refresh(); // Player Name, Weapon Name, SeqInfo: Refresh

        void   Quick_Update(const float*);
        float* origin()                    { return m_origin; }

        // position update queries
        bool   isUpdated()           { return m_State!=enumPS_Missing; }
        bool   isUpdatedAddEnt()     { return m_State==enumPS_Entity;  }
        float  fUpdatedTime()        { return (float)(ClientTime::current-m_StateTime); }
        vec3_t SuspectNextOrigin(); //use it instead of a the function which normally returns the origins
        
        // Aimbot Functions
        bool __fastcall isEnemy();
        float  CalcFovAngle(int hBox);
        void   CalcHitbox();
        void   CalcViewAngles();
        bool   TargetRegion();
        void   calcFovangleAndVisibility();
        void   DrawHitBox(ColorEntry *);
        void   Glows();

        // First Kill Functions
        bool __fastcall bFirstKill()              { return m_FirstKill; }
        bool __fastcall isFirstKill()             { return m_FirstKillList; }
        void __fastcall setFirstKill_List(bool To){ m_FirstKillList=To; if (!To) strcpy(FirstKill_Name, "zHAi.First.Kills"); }
        void __fastcall setFirstKill()            {
			if (m_FirstKillList){
				if ( (!strcmp(m_Name,FirstKill_Name)) ) {
					strcpy(FirstKill_Name, "zHAi.First.Kills");
					m_FirstKill=false;
				} else {
					strcpy(FirstKill_Name,m_Name);
					m_FirstKill=true;
				}
			}
		}

    private:
        bool m_Alive, m_FirstKillList, m_FirstKill;

        int   m_SeqInfo;
        float m_origin[3];
        char  m_WeaponName[32], m_Name[32], FirstKill_Name[32];
        
        int    m_State;
        double m_StateTime;
        DWORD  dw_SoundTime;
        float  f_SoundPos[3], f_SoundOffset[3], flLastSoundpos[3];

    public:
        _Player() { Init(0); }
    };
/*===========================================================
    Player Container Calss
===========================================================*/
    class _Players
    {
    private:
        _Player* PlyItems;
		int PlyKillsId[enumMaxPlayers];
    public:
        int iTarget, FirstKill_SelId,FirstKill_Count;
		bool FirstKill_Active, bToAttack;

        ~_Players() { delete[] PlyItems; }
        _Players() 
        {
            CanAimbot        = false;
			FirstKill_Active = false;
			FirstKill_SelId  = 0;
			FirstKill_Count  = -1;

            PlyItems = new _Player[enumMaxPlayers];
            for(int i=0;i<enumMaxPlayers;i++) { PlyItems[i].Init(i); PlyKillsId[i]=0; }
        }

        inline _Player& operator [] (unsigned int i)
        {
            if(i>=enumMaxPlayers) {return PlyItems[0];}
            else                  {return PlyItems[i];}
        }
        
        inline unsigned int Count() { return enumMaxPlayers; }
        
        ColorEntry* Color(int nId){
            if (nId == iTarget)                 { return colorList.get(6); }
            if (PlyItems[nId].m_Team == 1)      { return colorList.get(2); }
            else if (PlyItems[nId].m_Team == 2) { return colorList.get(3); }
            else                                { return colorList.get(8); }
        }
        
        void UpdateLocalPlayer();
        void Clear(){
            for(int i=0;i<enumMaxPlayers;i++) 
            {
                PlyItems[i].Clear();
                PlyItems[i].setAlive();
            }
        }
        /*
            Aimbot Function
        */
        bool CanAimbot;
		void doAimbot(struct usercmd_s *);
        void __fastcall FindTarget();
        void __fastcall DrawHitBox();
        void SetViewAngles(float *);
        void SetViewAngles(float *, float *);
        int  Aimbot_KeyEvents(int);
		void __fastcall PistolCombo(struct usercmd_s *);

        // First Kill Functions
		void __fastcall FirstKill_Clear(){
            for(int i=0;i<enumMaxPlayers;i++) 
            {
                PlyItems[i].setFirstKill_List(false);
            }
		}
		void __fastcall FirstKill_Refresh();
		int FirstKill_KeyEvent(int);
		void FirstKill_Show();
    };

    extern _Players vPlayers;
    extern local_player_info me;

#endif