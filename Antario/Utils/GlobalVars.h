#pragma once
#include "..\SDK\CInput.h"
#include "..\SDK\CEntity.h"

namespace g
{
    extern CUserCmd*      pCmd;
    extern C_BaseEntity*  pLocalEntity;
	extern Vector		  pos;
    extern std::uintptr_t uRandomSeed;
	extern Vector         OriginalView;
	extern bool			  IsHit;
	extern bool           bSendPacket;
	extern bool			  LagPeek;
	extern int            TargetIndex;
	extern Vector         EnemyEyeAngs[65];
	extern Vector         AimbotHitbox[65][28];
	extern int			  bombsite;
	extern Vector         RealAngle;
	extern Vector         FakeAngle;
	extern Vector         m_angFakeAnglesAbs;
	extern bool           Shot[65];
	extern bool           BreakingLC[65];
	extern bool           Hit[65];
	extern int            flHurtTime;
	extern int            flHurtTick;
	extern int            MissedShots[65];
	extern bool			  plist_player[65];
	extern int			  plist_pitch[65];
	extern int			  shots;
	extern int			  resolvetype;
	extern int			  revolvertime;
	extern bool			  nightmodeupdated;
	extern bool			  isthirdperson;
	extern float          w2s_matrix[4][4];

	extern DWORD CourierNew;
	extern DWORD WeaponFont;
	extern DWORD memefont;
	extern DWORD supfont;
	extern DWORD smallestpixel;
	extern DWORD Verdana;
	extern DWORD NiggaPee;
	extern DWORD SauIsAFatRetard;
	extern DWORD IndicatorFont;
	extern DWORD FlagFont;
}