#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include "..\..\SDK\IClientMode.h"
#include <deque>

class Resolver
{
public:
	bool UseFreestandAngle[65];
	float FreestandAngle[65];
	Vector absOriginBackup;
	float pitchHit[65];
	int resolvetype;
	void fuckingshutthefuckupretard(C_BaseEntity* entity);
	void OnCreateMove();
	void Anims(ClientFrameStage_t stage);
	void FrameStage(ClientFrameStage_t stage);
	float get_weighted_desync_delta(C_BaseEntity* player, float abs_angle, bool breaking_lby);
	void NewAnimationFix(C_BaseEntity* pEnt);
private:
	void AnimationFix(C_BaseEntity* pEnt);
	void NewNewAnimationFix(C_BaseEntity* pEnt, C_BaseEntity* pLocalEnt);
	float GetMaxDDelta(C_AnimState* animstate);
};
extern Resolver g_Resolver;