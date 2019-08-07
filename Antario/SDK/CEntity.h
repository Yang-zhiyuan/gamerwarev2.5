#pragma once
#include "Definitions.h"
#include "IClientUnknown.h"
#include "IClientEntityList.h"
#include "CInput.h"
#include "..\Utils\Utils.h"
#include "..\Utils\NetvarManager.h"

#define MAX_QPATH  260

#define BONE_CALCULATE_MASK             0x1F
#define BONE_PHYSICALLY_SIMULATED       0x01    // bone is physically simulated when physics are active
#define BONE_PHYSICS_PROCEDURAL         0x02    // procedural when physics is active
#define BONE_ALWAYS_PROCEDURAL          0x04    // bone is always procedurally animated
#define BONE_SCREEN_ALIGN_SPHERE        0x08    // bone aligns to the screen, not constrained in motion.
#define BONE_SCREEN_ALIGN_CYLINDER      0x10    // bone aligns to the screen, constrained by it's own axis.

#define BONE_USED_MASK                  0x0007FF00
#define BONE_USED_BY_ANYTHING           0x0007FF00
#define BONE_USED_BY_HITBOX             0x00000100    // bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT         0x00000200    // bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK        0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0        0x00000400    // bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1        0x00000800
#define BONE_USED_BY_VERTEX_LOD2        0x00001000
#define BONE_USED_BY_VERTEX_LOD3        0x00002000
#define BONE_USED_BY_VERTEX_LOD4        0x00004000
#define BONE_USED_BY_VERTEX_LOD5        0x00008000
#define BONE_USED_BY_VERTEX_LOD6        0x00010000
#define BONE_USED_BY_VERTEX_LOD7        0x00020000
#define BONE_USED_BY_BONE_MERGE         0x00040000    // bone is available for bone merge to occur against it

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )

#define MAX_NUM_LODS 8
#define MAXSTUDIOBONES		128		// total bones actually used

#define BONE_TYPE_MASK                  0x00F00000
#define BONE_FIXED_ALIGNMENT            0x00100000    // bone can't spin 360 degrees, all interpolation is normalized around a fixed orientation

#define BONE_HAS_SAVEFRAME_POS          0x00200000    // Vector48
#define BONE_HAS_SAVEFRAME_ROT64        0x00400000    // Quaternion64
#define BONE_HAS_SAVEFRAME_ROT32        0x00800000    // Quaternion32

// class predefinition
class C_BaseCombatWeapon;

class C_AnimState
{
public:
	char pad[3];
	char bUnknown; //0x4
	char pad2[91];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float_t& m_flVelocity()
	{
		return *(float_t*)((uintptr_t)this + 0xEC);
	}
	float_t& pad_0x0120()
	{
		return *(float_t*)((uintptr_t)this + 0x0120);
	}
	float* feetyaw() //rofl
	{
		return reinterpret_cast<float*>((DWORD)this + 0x0080);
	}
	float* poseparam() //rofl
	{
		return reinterpret_cast<float*>((DWORD)this + 0x2774);
	}

	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];
	void UpdateServerAnimations();
};

class AnimationLayer {
public:
	char  pad_0000[20];
	// These should also be present in the padding, don't see the use for it though
	//float	m_flLayerAnimtime;
	//float	m_flLayerFadeOuttime;
	unsigned int m_nOrder; //0x0014
	unsigned int m_nSequence; //0x0018
	float m_flPrevCycle; //0x001C
	float m_flWeight; //0x0020
	float m_flWeightDeltaRate; //0x0024
	float m_flPlaybackRate; //0x0028
	float m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038


class C_BaseEntity : public IClientEntity
{
private:
    template<class T>
    T GetPointer(const int offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
    // To get value from the pointer itself
    template<class T>
    T GetValue(const int offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }

	template <typename T>
	T& SetValue(uintptr_t offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

public:

	C_AnimState *AnimState()
	{
		return *reinterpret_cast<C_AnimState**>(uintptr_t(this) + 0x3900);
	}

	float_t m_flSpawnTime()
	{
		return *(float_t*)((uintptr_t)this + 0xA350);
	}

	AnimationLayer *AnimOverlays()
	{
		return *reinterpret_cast<AnimationLayer**>(uintptr_t(this) + 0x2980);
	}

	int GetNumAnimOverlays()
	{
		return *(int*)((DWORD)this + 0x298C);
	}

	int NumOverlays()
	{
		return 15;
	}

	void UpdateClientAnimation()
	{
		Utils::GetVFunc<void(__thiscall*)(void*)>(this, 221)(this);
	}

	void ClientAnimations(bool value)
	{
		static int m_bClientSideAnimation = g_pNetvars->GetOffset("DT_BaseAnimating", "m_bClientSideAnimation");
		*reinterpret_cast<bool*>(uintptr_t(this) + m_bClientSideAnimation) = value;
	}

	template<class T, class U>
	static T clamp(T in, U low, U high) {
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}

	float GetMaxDelta(C_AnimState* animstate)
	{

		float flRunningSpeed = clamp(animstate->m_flFeetSpeedForwardsOrSideWays, 0.f, 1.f);
		float flYawModifier = ((animstate->m_flStopToFullRunningFraction * -0.3f) - 0.2f) * flRunningSpeed;
		float flYawModifier2 = flYawModifier + 1.f;


		if (animstate->m_fDuckAmount > 0.f)
		{
			float maxVelocity = clamp(animstate->m_flFeetSpeedForwardsOrSideWays, 0.f, 1.f);
			float duckSpeed = animstate->m_fDuckAmount * maxVelocity;
			flYawModifier2 += (duckSpeed * (0.5f - flYawModifier2));
		}

		return *(float*)((uintptr_t)animstate + 0x334) * flYawModifier2;
	}

	int GetSequence()
	{
		static int m_nSequence = g_pNetvars->GetOffset("DT_BaseAnimating", "m_nSequence");
		return GetValue<int>(m_nSequence);
	}


	void SetSequence(int Sequence)
	{
		static int m_nSequence = g_pNetvars->GetOffset("DT_BaseAnimating", "m_nSequence");
		*reinterpret_cast<int*>(uintptr_t(this) + m_nSequence) = Sequence;
	}

	void SimulatedEveryTick(bool value)
	{
		static int m_bSimulatedEveryTick = g_pNetvars->GetOffset("DT_BaseEntity", "m_bSimulatedEveryTick");
		*reinterpret_cast<bool*>(uintptr_t(this) + m_bSimulatedEveryTick) = value;
	}

	void SetAbsAngles(Vector angles);
	void SetAbsOrigin(Vector origin);

	Vector GetAbsOrigin()
	{
		return Utils::GetVFunc<Vector& (__thiscall*)(void*)>(this, 10)(this);
	}

	
	void SetAbsVelocity(Vector velocity);

	void CreateAnimationState(C_AnimState* state);

	bool is_valid(C_BaseEntity* local_player = nullptr, bool dormant = true, bool team_check = true) {
		if (!this)
			return false;

		if (dormant)
			if (this->IsDormant())
				return false;

		if (team_check && local_player)
			if (this->GetTeam() == local_player->GetTeam())
				return false;

		if (this->GetHealth() <= 0)
			return false;

		return true;
	}

	int m_hOwnerEntity()
	{
		static int m_hOwnerEntity = g_pNetvars->GetOffset("DT_BaseEntity", "m_hOwnerEntity");
		return GetValue<int>(m_hOwnerEntity);
	}

	bool is_valid_world(bool check_dormant) {
		bool ret = true;

		if (check_dormant && this->IsDormant())
			ret = false;

		return ret;
	}

	matrix3x4_t GetBoneMatrix(int BoneID)
	{
		matrix3x4_t matrix;

		auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x26A8);
		if (offset)
			matrix = *reinterpret_cast<matrix3x4_t*>(offset + 0x30 * BoneID);

		return matrix;
	}


    C_BaseCombatWeapon* GetActiveWeapon()
    {
        static int m_hActiveWeapon = g_pNetvars->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
        const auto weaponData      = GetValue<CBaseHandle>(m_hActiveWeapon);
		if (!weaponData.IsValid())
		return nullptr;
        return reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntityFromHandle(weaponData));
    }

	int GetActiveWeaponIndex()
	{
		static int m_hActiveWeapon = g_pNetvars->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
		return *reinterpret_cast<int*>(uintptr_t(this) + m_hActiveWeapon) & 0xFFF; //m_hActiveWeapon
	}

	short* fixskins()
	{
		return (short*)((uintptr_t)this + 0x2FAA);
	}

    int GetTeam()
    {
        static int m_iTeamNum = g_pNetvars->GetOffset("DT_BaseEntity", "m_iTeamNum");
        return GetValue<int>(m_iTeamNum);
    }

    EntityFlags GetFlags()
    {
        static int m_fFlags = g_pNetvars->GetOffset("DT_BasePlayer", "m_fFlags");
        return GetValue<EntityFlags>(m_fFlags);
    }

	void SetFlags(int offset)
	{
		static int m_fFlags = g_pNetvars->GetOffset("DT_BasePlayer", "m_fFlags");
		*reinterpret_cast<int*>(uintptr_t(this) + m_fFlags) = offset;
	}

    MoveType_t GetMoveType()
    {
        static int m_Movetype = g_pNetvars->GetOffset("DT_BaseEntity", "m_nRenderMode") + 1;
        return GetValue<MoveType_t>(m_Movetype);
    }

	float GetSimulationTime()
	{
		static int m_flSimulationTime = g_pNetvars->GetOffset("DT_BaseEntity", "m_flSimulationTime");
		return GetValue<float>(m_flSimulationTime);
	}

	float GetOldSimulationTime()
	{
		static int m_flOldSimulationTime = g_pNetvars->GetOffset("DT_BaseEntity", "m_flSimulationTime") + 4;
		return GetValue<float>(m_flOldSimulationTime);
	}

	float GetLowerBodyYaw()
	{
		static int m_flLowerBodyYawTarget = g_pNetvars->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		return GetValue<float>(m_flLowerBodyYawTarget);
	}

	C_BaseEntity* GetObserverTarget()
	{
		ULONG ent = *(PULONG)((DWORD)this + g_pNetvars->GetOffset("CBasePlayer", "m_hObserverTarget"));
		return (C_BaseEntity*)(g_pEntityList->GetClientEntityFromHandle(ent));
	}

	float c4_blow_time()
	{
		static int m_flC4Blow = g_pNetvars->GetOffset("DT_PlantedC4", "m_flC4Blow");
		return GetValue<float>(m_flC4Blow);
	}

	int c4_site()
	{
		static int m_flC4Blow = g_pNetvars->GetOffset("DT_PlantedC4", "m_nBombSite");
		return GetValue<float>(m_flC4Blow);
	}
	int m_hBombDefuser()
	{
		static int m_hBombDefuser = g_pNetvars->GetOffset("DT_PlantedC4", "m_hBombDefuser");
		return GetValue<int>(m_hBombDefuser);
	}
	bool m_bBombDefused()
	{
		static int m_bBombDefused = g_pNetvars->GetOffset("DT_PlantedC4", "m_bBombDefused");
		return GetValue<bool>(m_bBombDefused);
	}
	int m_flTimerLength()
	{
		static int m_flTimerLength = g_pNetvars->GetOffset("DT_PlantedC4", "m_flTimerLength");
		return GetValue<float>(m_flTimerLength);
	}
	float m_flDefuseCountDown()
	{
		static int m_flDefuseCountDown = g_pNetvars->GetOffset("DT_PlantedC4", "m_flDefuseCountDown");
		return GetValue<float>(m_flDefuseCountDown);
	}
	float m_flDefuseLength()
	{
		static int m_flDefuseLength = g_pNetvars->GetOffset("DT_PlantedC4", "m_flDefuseLength");
		return GetValue<float>(m_flDefuseLength);
	}

	float m_nSmokeEffectTickBegin()
	{
		static int m_nSmokeEffectTickBegin = g_pNetvars->GetOffset("DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin");
		return GetValue<float>(m_nSmokeEffectTickBegin);
	}

	void SetLowerBodyYaw(float value)
	{
		static int m_flLowerBodyYawTarget = g_pNetvars->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		*reinterpret_cast<float*>(uintptr_t(this) + m_flLowerBodyYawTarget) = value;
	}

	bool GetHeavyArmor()
	{
		static int m_bHasHeavyArmor = g_pNetvars->GetOffset("DT_CSPlayer", "m_bHasHeavyArmor");
		return GetValue<bool>(m_bHasHeavyArmor);
	}

	int ArmorValue()
	{
		static int m_ArmorValue = g_pNetvars->GetOffset("DT_CSPlayer", "m_ArmorValue");
		return GetValue<int>(m_ArmorValue);
	}

	bool HasHelmet()
	{
		static int m_bHasHelmet = g_pNetvars->GetOffset("DT_CSPlayer", "m_bHasHelmet");
		return GetValue<bool>(m_bHasHelmet);
	}

	void invalidate_bone_cache(void)
	{
		static auto invalidate_bone_bache_fn = Utils::FindSignature("client_panorama.dll", "80 3D ?? ?? ?? ?? ?? 74 16 A1 ?? ?? ?? ?? 48 C7 81");

		*(uintptr_t*)((uintptr_t)this + 0x2924) = 0xFF7FFFFF;
		*(uintptr_t*)((uintptr_t)this + 0x2690) = **(uintptr_t * *)((uintptr_t)invalidate_bone_bache_fn + 10) - 1;
	}

    bool GetLifeState()
    {
        static int m_lifeState = g_pNetvars->GetOffset("DT_BasePlayer", "m_lifeState");
        return GetValue<bool>(m_lifeState);
    }

	bool IsScoped()
	{
		static int m_bIsScoped = g_pNetvars->GetOffset("DT_CSPlayer", "m_bIsScoped");
		return GetValue<bool>(m_bIsScoped);
	}

	bool m_bSpotted()
	{
		static int m_bSpotted = g_pNetvars->GetOffset("DT_CSPlayer", "m_bSpotted");
		return GetValue<bool>(m_bSpotted);
	}

    int GetHealth()
    {
        static int m_iHealth = g_pNetvars->GetOffset("DT_BasePlayer", "m_iHealth");
        return GetValue<int>(m_iHealth);
    }

	float m_flDuckSpeed()
    {
        static int m_flDuckSpeed = g_pNetvars->GetOffset("DT_BasePlayer", "m_flDuckSpeed");
        return GetValue<float>(m_flDuckSpeed);
    }

	bool IsKnifeorNade();

	bool isknifeorzeus();

	bool knife();

	bool IsFakeDucking();

	bool zeus();

	bool IsNade();

    bool IsAlive() { return this->GetHealth() > 0 && this->GetLifeState() == 0; }

	bool IsEnemy();

    bool IsImmune()
    {
        static int m_bGunGameImmunity = g_pNetvars->GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
        return GetValue<bool>(m_bGunGameImmunity);
    }

    int GetTickBase()
    {
        static int m_nTickBase = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_nTickBase");
        return GetValue<int>(m_nTickBase);
    }

	int GetShotsFired()
	{
		static int m_iShotsFired = g_pNetvars->GetOffset("DT_CSPlayer", "cslocaldata", "m_iShotsFired");
		return GetValue<int>(m_iShotsFired);
	}

	void SetTickBase(int TickBase)
	{
		static int m_nTickBase = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_nTickBase");
		*reinterpret_cast<int*>(uintptr_t(this) + m_nTickBase) = TickBase;
	}

	Vector GetEyeAngles()
	{
		static int m_angEyeAngles = g_pNetvars->GetOffset("DT_CSPlayer", "m_angEyeAngles");
		return GetValue<Vector>(m_angEyeAngles);
	}
	
	int m_bIsDefusing()
	{
		static int m_bIsDefusing = g_pNetvars->GetOffset("DT_CSPlayer", "m_bIsDefusing");
		return GetValue<int>(m_bIsDefusing);
	}

	Vector GetEyeAngles1()
	{
		static int m_angEyeAngles = g_pNetvars->GetOffset("DT_CSPlayer", "m_angEyeAngles[1]");
		return GetValue<Vector>(m_angEyeAngles);
	}

	void SetEyeAngles(Vector Angle)
	{
		static int m_angEyeAngles = g_pNetvars->GetOffset("DT_CSPlayer", "m_angEyeAngles");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_angEyeAngles) = Angle;
	}

	void update_state(C_AnimState* state, Vector ang) {
		using fn = void(__vectorcall*)(void*, void*, float, float, float, void*);
		static auto ret = reinterpret_cast<fn>(Utils::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24"));

		if (!ret)
			return;

		ret(state, NULL, NULL, ang.y, ang.x, NULL);
	}

    Vector GetOrigin()
    {
        static int m_vecOrigin = g_pNetvars->GetOffset("DT_BaseEntity", "m_vecOrigin");
        return GetValue<Vector>(m_vecOrigin);
    }

	Vector get_ragdoll_pos()
	{
		static int m_ragPos = g_pNetvars->GetOffset("DT_Ragdoll", "m_ragPos");
		return GetValue<Vector>(m_ragPos);
	}

	Vector GetOldOrigin()
	{
		static int m_vecOldOrigin = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_flFriction") + 12;
		return GetValue<Vector>(m_vecOldOrigin);
	}
	
	constexpr static auto MAX_WEAPONS = 48;

	std::array<CBaseHandle, MAX_WEAPONS>* GetWeaponsHandle()
	{
		static int m_iItemIDHigh = g_pNetvars->GetOffset("DT_BaseCombatCharacter", "m_hMyWeapons");
		return GetValue<std::array<CBaseHandle, MAX_WEAPONS>*>(m_iItemIDHigh);
	}

	Vector GetNetworkOrigin()
	{
		static int m_vecNetworkOrigin = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_flFriction") + 40;
		return GetValue<Vector>(m_vecNetworkOrigin);
	}

	void SetOrigin(Vector Origin)
	{
		static int m_vecOrigin = g_pNetvars->GetOffset("DT_BaseEntity", "m_vecOrigin");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_vecOrigin) = Origin;
	}

	Vector GetVelocity()
	{
		static int m_vecVelocity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
		return GetValue<Vector>(m_vecVelocity);
	}

	std::array<float, 24>& m_flPoseParameter()
	{
		static int _m_flPoseParameter = g_pNetvars->GetOffset("DT_BaseAnimating", "m_flPoseParameter");
		return *(std::array<float, 24>*)((uintptr_t)this + _m_flPoseParameter);
	}


	AnimationLayer* GetAnimOverlays()
	{
		// to find offset: use 9/12/17 dll
		// sig: 55 8B EC 51 53 8B 5D 08 33 C0
		return *(AnimationLayer * *)((DWORD)this + 0x2980);
	}

	AnimationLayer* GetAnimOverlay(int i)
	{
		if (i < 15)
			return &GetAnimOverlays()[i];
		return nullptr;
	}

	int m_nFallbackPaintKit()
	{
		static int m_nFallbackPaintKit = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit");
		return GetValue<int>(m_nFallbackPaintKit);
	}

	void set_m_nFallbackPaintKit(int yo)
	{
		static int offset = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit");
		*reinterpret_cast<int*>(uintptr_t(this) + offset) = yo;
	}

	int m_nFallbackSeed()
	{
		static int m_nFallbackSeed = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackSeed");
		return GetValue<int>(m_nFallbackSeed);
	}

	void set_m_nFallbackSeed(int yo)
	{
		static int offset = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackSeed");
		*reinterpret_cast<int*>(uintptr_t(this) + offset) = yo;
	}

	int m_nFallbackStatTrak()
	{
		static int m_nFallbackStatTrak = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak");
		return GetValue<int>(m_nFallbackStatTrak);
	}

	void set_m_nFallbackStatTrak(int yo)
	{
		static int offset = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak");
		*reinterpret_cast<int*>(uintptr_t(this) + offset) = yo;
	}

	float m_flFallbackWear()
	{
		static int m_flFallbackWear = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_flFallbackWear");
		return GetValue<float>(m_flFallbackWear);
	}

	void set_m_flFallbackWear(float yo)
	{
		static int offset = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_flFallbackWear");
		*reinterpret_cast<float*>(uintptr_t(this) + offset) = yo;
	}
	
	void m_OriginalOwnerXuidLow(int yo)
	{
		static int offset = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
		*reinterpret_cast<int*>(uintptr_t(this) + offset) = yo;
	}
	
	void m_OriginalOwnerXuidHigh(int yo)
	{
		static int offset = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
		*reinterpret_cast<int*>(uintptr_t(this) + offset) = yo;
	}

	int uidhigh()
	{
		static int m_OriginalOwnerXuidHigh = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
		return GetValue<int>(m_OriginalOwnerXuidHigh);
	}

	int uidlow()
	{
		static int m_OriginalOwnerXuidLow = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
		return GetValue<int>(m_OriginalOwnerXuidLow);
	}

	ItemDefinitionIndex GetItemDefinitionIndex()
	{
		static int m_iItemDefinitionIndex = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
		return GetValue<ItemDefinitionIndex>(m_iItemDefinitionIndex);
	}


	void set_m_szCustomName(char* yo)
	{
		static int offset = g_pNetvars->GetOffset("CBaseAttributableItem", "m_szCustomName");
		*reinterpret_cast<char**>(uintptr_t(this) + offset) = yo;
	}

	void SetModelIndex(const int index)
	{
		Utils::GetVFunc<void(__thiscall*)(C_BaseEntity*, int)>(this, 75)(this, index);
	}

	inline int GetModelIndex() { // HARD CODED OFFSET GANG
		return *(int*)((DWORD)this + 0x2FAA);
	}

	std::array<float, 24>* setparam(C_BaseEntity* ent) //rofl
	{
		return (std::array<float, 24>*)((uintptr_t)ent + 0x2774);
	}

	int* m_iEntityQuality(int ent) //rofl
	{
		return (int*)((uintptr_t)ent + 0x2FAC);
	}

	Vector m_aimPunchAngle()
	{
		static int m_aimPunchAngle = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_Local", "m_aimPunchAngle");
		return GetValue<Vector>(m_aimPunchAngle);
	}

	void SetVelocity(Vector velocity)
	{
		static int m_vecVelocity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
		*reinterpret_cast<Vector*>(uintptr_t(this) + m_vecVelocity) = velocity;
	}

	Vector GetAimPunchAngle()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + uintptr_t(0x302C));
	}

	Vector GetViewPunchAngle()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + uintptr_t(0x3020));
	}

    Vector GetEyePosition() 
	{
		Vector ret;
		typedef void(__thiscall *OrigFn)(void *, Vector&);
		Utils::GetVFunc<OrigFn>(this, 281)(this, ret);
		return ret;
	}

	ICollideable* GetCollideable()
	{
		return (ICollideable*)((DWORD)this + 0x318);
	}

	ICollideable* GetCollision()
	{
		if (this)
			return reinterpret_cast<ICollideable*>((DWORD)this + 0x31C);
	}

	VMatrix& GetCollisionBoundTrans()
	{
		if (this)
			return *reinterpret_cast<VMatrix*>((DWORD)this + 0x0444);
	}

	void SetCurrentCommand(CUserCmd *cmd)
	{
		static int m_hConstraintEntity = g_pNetvars->GetOffset("DT_BasePlayer", "localdata", "m_hConstraintEntity");
		*reinterpret_cast<CUserCmd**>(uintptr_t(this) + m_hConstraintEntity - 0xC) = cmd;
	}

	float FireRate();

	void FixSetupBones(matrix3x4_t *Matrix);
	Vector getBonePosition(int bone);
	bool is_dropped_weapon(C_BaseEntity* e);
	Vector GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix, float *Radius);
	Vector GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix);
};

class CBaseAttributableItem : public C_BaseEntity
{
private:
	template<class T>
	T GetPointer(const int offset)
	{
		return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	// To get value from the pointer itself
	template<class T>
	T GetValue(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}

public:

	int m_nFallbackPaintKit()
	{
		static int m_nFallbackPaintKit = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit");
		return GetValue<int>(m_nFallbackPaintKit);
	}

	void set_m_nFallbackPaintKit(int yo)
	{
		static int offset = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit");
		*reinterpret_cast<int*>(uintptr_t(this) + offset) = yo;
	}
	
	int m_iEntityQuality()
	{
		static int m_iEntityQuality = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_iEntityQuality");
		return GetValue<int>(m_iEntityQuality);
	}	

	void set_m_iEntityQuality(int yo)
	{
		static int offset = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_iEntityQuality");
		*reinterpret_cast<int*>(uintptr_t(this) + offset) = yo;
	}

	int m_nFallbackSeed()
	{
		static int m_nFallbackSeed = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackSeed");
		return GetValue<int>(m_nFallbackSeed);
	}

	void set_m_nFallbackSeed(int yo)
	{
		static int offset = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackSeed");
		*reinterpret_cast<int*>(uintptr_t(this) + offset) = yo;
	}

	int m_nFallbackStatTrak()
	{
		static int m_nFallbackStatTrak = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak");
		return GetValue<int>(m_nFallbackStatTrak);
	}

	void set_m_nFallbackStatTrak(int yo)
	{
		static int offset = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak");
		*reinterpret_cast<int*>(uintptr_t(this) + offset) = yo;
	}

	float m_flFallbackWear()
	{
		static int m_flFallbackWear = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_flFallbackWear");
		return GetValue<float>(m_flFallbackWear);
	}

	void set_m_flFallbackWear(float yo)
	{
		static int offset = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_flFallbackWear");
		*reinterpret_cast<float*>(uintptr_t(this) + offset) = yo;
	}
	int m_iItemIDHigh()
	{
		static int m_iItemIDHigh = g_pNetvars->GetOffset("DT_BaseCombatWeapon", "m_iItemIDHigh");
		return GetValue<float>(m_iItemIDHigh);
	}

	void set_m_szCustomName(char* yo)
	{
		static int offset = g_pNetvars->GetOffset("CBaseAttributableItem", "m_szCustomName");
		*reinterpret_cast<char**>(uintptr_t(this) + offset) = yo;
	}

};


class C_BaseCombatWeapon : public C_BaseEntity
{
private:
	template<class T>
    T GetPointer(const int offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
    // To get value from the pointer itself
    template<class T>
    T GetValue(const int offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }

public:

    ItemDefinitionIndex GetItemDefinitionIndex()
    {
        static int m_iItemDefinitionIndex = g_pNetvars->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
        return GetValue<ItemDefinitionIndex>(m_iItemDefinitionIndex);
    }

	float GetInaccuracy()
	{
		typedef float(__thiscall* oInaccuracy)(PVOID);
		return Utils::GetVFunc< oInaccuracy >(this, 476)(this); //471
	}

	float GetSpread()
	{
		typedef float(__thiscall* oWeaponSpread)(PVOID);
		return Utils::GetVFunc< oWeaponSpread >(this, 446)(this); //439
	}

	float GetAccuracyPenalty()
	{
		static int m_fAccuracyPenalty = g_pNetvars->GetOffset("DT_WeaponCSBase", "m_fAccuracyPenalty");
		return GetValue<float>(m_fAccuracyPenalty);
	}

	float GetLastShotTime()
	{
		static int m_fLastShotTime = g_pNetvars->GetOffset("DT_WeaponCSBase", "m_fLastShotTime");
		return GetValue<float>(m_fLastShotTime);
	}

	void AccuracyPenalty()
	{
		typedef void(__thiscall *OrigFn)(void *);
		return Utils::GetVFunc<OrigFn>(this, 477)(this);
	}

    float GetNextPrimaryAttack()
    {
        static int m_flNextPrimaryAttack = g_pNetvars->GetOffset("DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
        return GetValue<float>(m_flNextPrimaryAttack);
    }

    int GetAmmo()
    {
        static int m_iClip1 = g_pNetvars->GetOffset("DT_BaseCombatWeapon", "m_iClip1");
        return GetValue<int>(m_iClip1);
    }

    WeaponInfo_t* GetCSWpnData()
    {
        return Utils::CallVFunc<454, WeaponInfo_t*>(this);
    }

    std::string GetName()
    {
        ///TODO: Test if szWeaponName returns proper value for m4a4 / m4a1-s or it doesnt recognize them.
        return std::string(this->GetCSWpnData()->weapon_name);
    }
};

extern std::string cleanstd;

class OverlayText_t;

class IVDebugOverlay
{
public:
	virtual void            __unkn() = 0;
	virtual void            AddEntityTextOverlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
	virtual void            AddBoxOverlay(const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, int r, int g, int b, int a, float duration) = 0;
	virtual void            AddSphereOverlay(const Vector& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration) = 0;
	virtual void            AddTriangleOverlay(const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void            AddLineOverlay(const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration) = 0;
	virtual void            AddTextOverlay(const Vector& origin, float duration, const char* format, ...) = 0;
	virtual void            AddTextOverlay(const Vector& origin, int line_offset, float duration, const char* format, ...) = 0;
	virtual void            AddScreenTextOverlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char* text) = 0;
	virtual void            AddSweptBoxOverlay(const Vector& start, const Vector& end, const Vector& mins, const Vector& max, const QAngle& angles, int r, int g, int b, int a, float flDuration) = 0;
	virtual void            AddGridOverlay(const Vector& origin) = 0;
	virtual void            AddCoordFrameOverlay(const matrix3x4_t& frame, float flScale, int vColorTable[3][3] = NULL) = 0;
	virtual int             ScreenPosition(const Vector& point, Vector& screen) = 0;
	virtual int             ScreenPosition(float flXPos, float flYPos, Vector& screen) = 0;
	virtual OverlayText_t* GetFirst(void) = 0;
	virtual OverlayText_t* GetNext(OverlayText_t* current) = 0;
	virtual void            ClearDeadOverlays(void) = 0;
	virtual void            ClearAllOverlays() = 0;
	virtual void            AddTextOverlayRGB(const Vector& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char* format, ...) = 0;
	virtual void            AddTextOverlayRGB(const Vector& origin, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
	virtual void            AddLineOverlayAlpha(const Vector& origin, const Vector& dest, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void            AddBoxOverlay2(const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, const uint8_t* faceColor, const uint8_t* edgeColor, float duration) = 0;
	virtual void            PurgeTextOverlays() = 0;
	virtual void            DrawPill(const Vector& mins, const Vector& max, float& diameter, int r, int g, int b, int a, float duration) = 0;

public:

	void DrawPillReal(const Vector& mins, const Vector& max, float& diameter, int r, int g, int b, int a, float duration)
	{
		typedef void(__thiscall * OriginalFn)(PVOID, const Vector&, const Vector&, float&, int, int, int, int, float);
		Utils::GetVFunc<OriginalFn>(this, 23)(this, mins, max, diameter, r, g, b, a, duration);
	}
	void AddCapsuleOverlay(Vector& mins, Vector& maxs, float pillradius, int r, int g, int b, int a, float duration)
	{
		Utils::GetVFunc<void(__thiscall*)(void*, Vector&, Vector&, float&, int, int, int, int, float)>(this, 24)(this, mins, maxs, pillradius, r, g, b, a, duration);
	};
};


extern IVDebugOverlay* g_pIVDebugOverlay;
