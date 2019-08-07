#include "Legitbot.h"
#include "..\Aimbot\Aimbot.h"
#include "..\Aimbot\LagComp.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\Menu\Menu.h"
#include "../../SDK/IVModelInfo.h"
#include "../../SDK/IEngineTrace.h"
#include "../../SDK/ICvar.h"

Legitbot g_Legitbot;

legit_backtrackdata headPositions[64][12];

Vector get_hitbox_pos(C_BaseEntity* entity, int hitbox_id)
{
	auto getHitbox = [](C_BaseEntity * entity, int hitboxIndex) ->mstudiobbox_t *
	{
		if (entity->IsDormant() || entity->GetHealth() <= 0) return NULL;

		const auto pModel = entity->GetModel();
		if (!pModel) return NULL;

		auto pStudioHdr = g_pModelInfo->GetStudiomodel(pModel);
		if (!pStudioHdr) return NULL;

		auto pSet = pStudioHdr->GetHitboxSet(0);
		if (!pSet) return NULL;

		if (hitboxIndex >= pSet->numhitboxes || hitboxIndex < 0) return NULL;

		return pSet->GetHitbox(hitboxIndex);
	};

	auto hitbox = getHitbox(entity, hitbox_id);
	if (!hitbox) return Vector(0, 0, 0);

	auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

	Vector bbmin, bbmax;
	g_Math.VectorTransform(hitbox->min, bone_matrix, bbmin);
	g_Math.VectorTransform(hitbox->max, bone_matrix, bbmax);

	return (bbmin + bbmax) * 0.5f;
}

inline Vector angle_vector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(M_PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(M_PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(M_PI));
	auto cp = cos(meme.x / 180.f * static_cast<float>(M_PI));

	return Vector(cp * cy, cp * sy, -sp);
}

inline float point_to_line(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x * Dir.x + Dir.y * Dir.y + Dir.z * Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}

void Legitbot::backtrack(CUserCmd* cmd) //phook backtrack muahhahahahaaha
{
	int bestTargetIndex = -1;
	float bestFov = FLT_MAX;
	PlayerInfo_t info;

	auto local_player = g::pLocalEntity;
	if (!local_player) return;

	for (int i = 1; i < 65; i++)
	{
		auto entity = g_pEntityList->GetClientEntity(i);
		if (!entity) continue;
		if (!g_pEngine->GetPlayerInfo(i, &info)) continue;
		if (entity->IsDormant()) continue;
		if (entity->GetTeam() == local_player->GetTeam()) continue;
		if (entity->GetHealth() <= 0) continue;
		if (local_player->GetHealth() <= 0) return;

		float simtime = entity->GetSimulationTime(); Vector hitboxPos = get_hitbox_pos(entity, 0);
		headPositions[i][cmd->command_number % 12] = legit_backtrackdata{ simtime, hitboxPos };
		Vector ViewDir = angle_vector(cmd->viewangles + (local_player->GetViewPunchAngle() * 2.f));
		float FOVDistance = point_to_line(hitboxPos, local_player->GetEyePosition(), ViewDir);

		if (bestFov > FOVDistance)
		{
			bestFov = FOVDistance;
			bestTargetIndex = i;
		}
	}

	float bestTargetSimTime;
	if (bestTargetIndex != -1)
	{
		float tempFloat = FLT_MAX;
		Vector ViewDir = angle_vector(cmd->viewangles + (local_player->GetViewPunchAngle() * 2.f));

		for (int t = 0; t < 12; ++t)
		{
			float tempFOVDistance = point_to_line(headPositions[bestTargetIndex][t].hitboxPos, local_player->GetEyePosition(), ViewDir);
			if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > local_player->GetSimulationTime() - 1)
			{
				tempFloat = tempFOVDistance;
				bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
			}
		}

		if (cmd->buttons & IN_ATTACK)
			cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
	}
}

void Legitbot::RecoilControl()
{
	float rcsamount = g_Menu.Config.RCSamount / 100.0f;
	float srcsamount = g_Menu.Config.StandaloneRCSSlider / 100.0f;
	static ConVar* weapon_recoil_scale = g_pCvar->FindVar("weapon_recoil_scale");
	static Vector oldPunch;
	if (g_Menu.Config.StandaloneRCS && !g_Menu.Config.LegitAimbot) {
		g_pEngine->SetViewAngles(g::pCmd->viewangles - (g::pLocalEntity->m_aimPunchAngle() - oldPunch) * weapon_recoil_scale->GetFloat() * srcsamount);
		oldPunch = g::pLocalEntity->m_aimPunchAngle();
	}
	if (g_Menu.Config.AimRCS) {
		g::pCmd->viewangles -= g::pLocalEntity->m_aimPunchAngle() * weapon_recoil_scale->GetFloat() * rcsamount;
	}
}

float DesyncDelta(C_AnimState* animstate)
{

	float flRunningSpeed = std::clamp(animstate->m_flFeetSpeedForwardsOrSideWays, 0.f, 1.f);
	float flYawModifier = ((animstate->m_flStopToFullRunningFraction * -0.3f) - 0.2f) * flRunningSpeed;
	float flYawModifier2 = flYawModifier + 1.f;


	if (animstate->m_fDuckAmount > 0.f)
	{
		float maxVelocity = std::clamp(animstate->m_flFeetSpeedForwardsOrSideWays, 0.f, 1.f);
		float duckSpeed = animstate->m_fDuckAmount * maxVelocity;
		flYawModifier2 += (duckSpeed * (0.5f - flYawModifier2));
	}

	return *(float*)((uintptr_t)animstate + 0x334) * flYawModifier2;
}

bool flip_yaw = false;

/*void Legitbot::AntiAim()
{

	if (g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_LADDER)
		return;

	if (!g_pEngine->IsInGame() || !g_Menu.Config.LegitAA)
		return;

	if (!g::pLocalEntity->IsAlive())
		return;

	if (GetAsyncKeyState(0x45))
		return;

	if (g::pCmd->buttons & IN_ATTACK)
		return;

	if (GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON))
		return;

	if (g::pLocalEntity->IsNade())
		return;

	auto local_player = g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	auto desync = DesyncDelta(local_player->AnimState());

	int flip = (int)floorf(g_pGlobalVars->curtime / 1.1) % 2;

	flip_yaw = !flip_yaw;

	if (g_Menu.Config.LegitAAType == 0)
	{
		if (g::bSendPacket) {
			flip ? g::pCmd->viewangles.y += desync : g::pCmd->viewangles.y -= desync;
			flip ? g::pLocalEntity->AnimState()->m_flGoalFeetYaw += desync : g::pLocalEntity->AnimState()->m_flGoalFeetYaw -= desync;
		}
	}
	else if (g_Menu.Config.LegitAAType == 0) {
		g::pCmd->viewangles.y += desync;
	}

	if (g::pLocalEntity->GetFlags() & FL_ONGROUND && g::pCmd->sidemove < 3 && g::pCmd->sidemove > -3) {
		static bool switch_ = false;
		if (switch_)
			g::pCmd->sidemove = 2;
		else
			g::pCmd->sidemove = -2;
		switch_ = !switch_;
	}
}*/


void Legitbot::CreateMove(CUserCmd* cmd)
{
	if (g_pEngine->IsInGame() && g_pEngine->IsConnected() && g_Menu.Config.Legitbot) {

		RecoilControl();

		if (g_Menu.Config.LegitBacktrack)
			backtrack(cmd);

		//if (g_Menu.Config.LegitAA)
		//	AntiAim();
	}
}