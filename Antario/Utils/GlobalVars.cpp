#include "GlobalVars.h"

namespace g
{
    CUserCmd*      pCmd         = nullptr;
    C_BaseEntity*  pLocalEntity = nullptr;
    std::uintptr_t uRandomSeed  = NULL;
	Vector         OriginalView;
	bool		   IsHit;
	bool           bSendPacket  = true;
	bool		   LagPeek      = false;
	int            TargetIndex  = -1;
	Vector         EnemyEyeAngs[65];
	Vector         AimbotHitbox[65][28];
	int			   bombsite;
	Vector         RealAngle;
	Vector         FakeAngle;
	Vector         m_angFakeAnglesAbs;
	Vector		   pos;
	bool           Shot[65];
	bool           BreakingLC[65];
	bool           Hit[65];
	int			   flHurtTime;
	int			   flHurtTick;
	int            MissedShots[65];
	bool		   plist_player[65];
	int			   plist_pitch[65];
	int			   shots;
	int			   resolvetype;
	int			   revolvertime;
	bool		   nightmodeupdated;
	bool		   isthirdperson;
	float          w2s_matrix[4][4];

	DWORD CourierNew;
	DWORD WeaponFont;
	DWORD memefont;
	DWORD supfont;
	DWORD smallestpixel;
	DWORD Verdana;
	DWORD NiggaPee;
	DWORD SauIsAFatRetard;
	DWORD IndicatorFont;
	DWORD FlagFont;
}
