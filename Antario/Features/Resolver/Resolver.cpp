#include "Resolver.h"
#include "..\Aimbot\Aimbot.h"
#include "..\Aimbot\Autowall.h"
#include "..\Aimbot\LagComp.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\Utils\Math.h"
#include "..\..\Menu\Menu.h"

Resolver g_Resolver;

void Resolver::AnimationFix(C_BaseEntity* pEnt) // default resolver
{
	//not pasted
	static float oldSimtime[65];
	static float storedSimtime[65];
	static float ShotTime[65];
	static float SideTime[65][3];
	static int LastDesyncSide[65];
	static bool Delaying[65];
	static AnimationLayer StoredLayers[64][15];
	static C_AnimState * StoredAnimState[65];
	static float StoredPosParams[65][24];
	static Vector oldEyeAngles[65];
	static float oldGoalfeetYaw[65];
	float* PosParams = (float*)((uintptr_t)pEnt + 0x2774);
	bool update = false;
	bool shot = false;

	static bool jittering[65];

	auto* AnimState = pEnt->AnimState();
	if (!AnimState || !pEnt->AnimOverlays() || !PosParams)
		return;

	auto RemapVal = [](float val, float A, float B, float C, float D) -> float
	{
		if (A == B)
			return val >= B ? D : C;
		return C + (D - C) * (val - A) / (B - A);
	};

	if (storedSimtime[pEnt->EntIndex()] != pEnt->GetSimulationTime())
	{
		jittering[pEnt->EntIndex()] = false;
		pEnt->ClientAnimations(true);
		pEnt->UpdateClientAnimation();

		memcpy(StoredPosParams[pEnt->EntIndex()], PosParams, sizeof(float) * 24);
		memcpy(StoredLayers[pEnt->EntIndex()], pEnt->AnimOverlays(), (sizeof(AnimationLayer) * pEnt->NumOverlays()));

		oldGoalfeetYaw[pEnt->EntIndex()] = AnimState->m_flGoalFeetYaw;

		if (pEnt->GetActiveWeapon() && !pEnt->IsKnifeorNade())
		{
			if (ShotTime[pEnt->EntIndex()] != pEnt->GetActiveWeapon()->GetLastShotTime())
			{
				shot = true;
				ShotTime[pEnt->EntIndex()] = pEnt->GetActiveWeapon()->GetLastShotTime();
			}
			else
				shot = false;
		}
		else
		{
			shot = false;
			ShotTime[pEnt->EntIndex()] = 0.f;
		}

		float angToLocal = g_Math.NormalizeYaw(g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pEnt->GetOrigin()).y);

		float Back = g_Math.NormalizeYaw(angToLocal);
		float DesyncFix = 0;
		float Resim = g_Math.NormalizeYaw((0.24f / (pEnt->GetSimulationTime() - oldSimtime[pEnt->EntIndex()]))*(oldEyeAngles[pEnt->EntIndex()].y - pEnt->GetEyeAngles().y));

		if (Resim > 58.f)
			Resim = 58.f;
		if (Resim < -58.f)
			Resim = -58.f;

		if (pEnt->GetVelocity().Length2D() > 0.5f && !shot)
		{
			float Delta = g_Math.NormalizeYaw(g_Math.NormalizeYaw(g_Math.CalcAngle(Vector(0, 0, 0), pEnt->GetVelocity()).y) - g_Math.NormalizeYaw(g_Math.NormalizeYaw(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60)) + Resim));

			int CurrentSide = 0;

			if (Delta < 0)
			{
				CurrentSide = 1;
				SideTime[pEnt->EntIndex()][1] = g_pGlobalVars->curtime;
			}
			else if (Delta > 0)
			{
				CurrentSide = 2;
				SideTime[pEnt->EntIndex()][2] = g_pGlobalVars->curtime;
			}

			if (LastDesyncSide[pEnt->EntIndex()] == 1)
			{
				Resim += (58.f - Resim);
				DesyncFix += (58.f - Resim);
			}
			if (LastDesyncSide[pEnt->EntIndex()] == 2)
			{
				Resim += (-58.f - Resim);
				DesyncFix += (-58.f - Resim);
			}

			if (LastDesyncSide[pEnt->EntIndex()] != CurrentSide)
			{
				Delaying[pEnt->EntIndex()] = true;

				if (.5f < (g_pGlobalVars->curtime - SideTime[pEnt->EntIndex()][LastDesyncSide[pEnt->EntIndex()]]))
				{
					LastDesyncSide[pEnt->EntIndex()] = CurrentSide;
					Delaying[pEnt->EntIndex()] = false;
				}
			}

			if (!Delaying[pEnt->EntIndex()])
				LastDesyncSide[pEnt->EntIndex()] = CurrentSide;
		}
		else if (!shot)
		{
			float Brute = UseFreestandAngle[pEnt->EntIndex()] ? g_Math.NormalizeYaw(Back + FreestandAngle[pEnt->EntIndex()]) : pEnt->GetLowerBodyYaw();

			float Delta = g_Math.NormalizeYaw(g_Math.NormalizeYaw(Brute - g_Math.NormalizeYaw(g_Math.NormalizeYaw(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60))) + Resim));

			if (Delta > 58.f)
				Delta = 58.f;
			if (Delta < -58.f)
				Delta = -58.f;

			Resim += Delta;
			DesyncFix += Delta;

			if (Resim > 58.f)
				Resim = 58.f;
			if (Resim < -58.f)
				Resim = -58.f;
		}

		float Equalized = g_Math.NormalizeYaw(g_Math.NormalizeYaw(AnimState->m_flGoalFeetYaw + RemapVal(PosParams[11], 0, 1, -60, 60)) + Resim);

		float JitterDelta = fabs(g_Math.NormalizeYaw(oldEyeAngles[pEnt->EntIndex()].y - pEnt->GetEyeAngles().y));

		if (JitterDelta >= 70.f && !shot)
			jittering[pEnt->EntIndex()] = true;

		if (pEnt != g::pLocalEntity && pEnt->GetTeam() != g::pLocalEntity->GetTeam() && (pEnt->GetFlags() & FL_ONGROUND) && g_Menu.Config.Resolver)
		{
			if (jittering[pEnt->EntIndex()])
				AnimState->m_flGoalFeetYaw = g_Math.NormalizeYaw(pEnt->GetEyeAngles().y + DesyncFix);
			else
				AnimState->m_flGoalFeetYaw = Equalized;

			pEnt->SetLowerBodyYaw(AnimState->m_flGoalFeetYaw);
		}

		StoredAnimState[pEnt->EntIndex()] = AnimState;

		oldEyeAngles[pEnt->EntIndex()] = pEnt->GetEyeAngles();

		oldSimtime[pEnt->EntIndex()] = storedSimtime[pEnt->EntIndex()];

		storedSimtime[pEnt->EntIndex()] = pEnt->GetSimulationTime();

		update = true;
	}

	pEnt->ClientAnimations(false);

	if (pEnt != g::pLocalEntity && pEnt->GetTeam() != g::pLocalEntity->GetTeam() && (pEnt->GetFlags() & FL_ONGROUND) && g_Menu.Config.Resolver)
		pEnt->SetLowerBodyYaw(AnimState->m_flGoalFeetYaw);

	AnimState = StoredAnimState[pEnt->EntIndex()];

	memcpy((void*)PosParams, &StoredPosParams[pEnt->EntIndex()], (sizeof(float) * 24));
	memcpy(pEnt->AnimOverlays(), StoredLayers[pEnt->EntIndex()], (sizeof(AnimationLayer) * pEnt->NumOverlays()));

	if (pEnt != g::pLocalEntity && pEnt->GetTeam() != g::pLocalEntity->GetTeam() && (pEnt->GetFlags() & FL_ONGROUND) && g_Menu.Config.Resolver && jittering[pEnt->EntIndex()])
		pEnt->SetAbsAngles(Vector(0, pEnt->GetEyeAngles().y, 0));
	else
		pEnt->SetAbsAngles(Vector(0, oldGoalfeetYaw[pEnt->EntIndex()], 0));

	*reinterpret_cast<int*>(uintptr_t(pEnt) + 0xA30) = g_pGlobalVars->framecount;
	*reinterpret_cast<int*>(uintptr_t(pEnt) + 0xA28) = 0;
}

void Resolver::NewNewAnimationFix(C_BaseEntity* pEnt, C_BaseEntity* pLocalEnt)
{
	// i rly am just to lazy pls dont judge
	static float ShotTime[65];

	static float oldSimtime[65];
	static float storedSimtime[65];

	static Vector oldEyeAngles[65];
	static float oldGoalfeetYaw[65];
	static Vector oldOrigin[65];

	float* PosParams = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(pEnt) + 0x2774);
	bool shot = false;

	auto * AnimState = pEnt->AnimState();
	if (!AnimState || !pEnt->AnimOverlays() || !PosParams)
		return;

	if (*reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(AnimState) + 0x164) < 0) // sorry had to anti pasta this basically the key to nospread
		* reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(AnimState) + 0x110) = 0.f;

	bool update = false;

	if (storedSimtime[pEnt->EntIndex()] != pEnt->GetSimulationTime())
	{
		pEnt->ClientAnimations(true);
		pEnt->UpdateClientAnimation();
		pEnt->ClientAnimations(false);

		update = true;
	}

	oldGoalfeetYaw[pEnt->EntIndex()] = AnimState->m_flGoalFeetYaw;

	if (pEnt->GetActiveWeapon() && !pEnt->IsKnifeorNade())
	{
		if (ShotTime[pEnt->EntIndex()] != pEnt->GetActiveWeapon()->GetLastShotTime())
		{
			shot = true;
			ShotTime[pEnt->EntIndex()] = pEnt->GetActiveWeapon()->GetLastShotTime();
		}
		else
			shot = false;
	}
	else
	{
		shot = false;
		ShotTime[pEnt->EntIndex()] = 0.f;
	}

	if (pLocalEnt && pLocalEnt->IsAlive())
	{
		float angToLocal = g_Math.NormalizeYaw(g_Math.CalcAngle(pLocalEnt->GetOrigin(), pEnt->GetOrigin()).y);

		float Back = g_Math.NormalizeYaw(angToLocal);
		float DesyncFix = 0;

		float Resim = g_Math.NormalizeYaw((TICKS_TO_TIME(16) / (pEnt->GetSimulationTime() - oldSimtime[pEnt->EntIndex()])) * g_Math.NormalizeYaw(oldEyeAngles[pEnt->EntIndex()].y - pEnt->GetEyeAngles().y));

		if (Resim > 60.f)
			Resim = 60.f;
		if (Resim < -60.f)
			Resim = -60.f;

		if (g_Menu.Config.ResolverType == 0 && !shot && !isnan(angToLocal) && !isinf(angToLocal) && pEnt != pLocalEnt && pEnt->GetTeam() != pLocalEnt->GetTeam())
		{
			float AntiSide = 0.f;

			if (g::MissedShots[pEnt->EntIndex()] % 2)
			{
				if (g_Math.NormalizeYaw(pEnt->GetEyeAngles().y - Back) > 0.f)
					AntiSide = -90.f;
				else if (g_Math.NormalizeYaw(pEnt->GetEyeAngles().y - Back) < 0.f)
					AntiSide = 90.f;
			}
			else
			{
				if (g_Math.NormalizeYaw(pEnt->GetEyeAngles().y - g_Math.NormalizeYaw(Back + 90)) > 0.f)
					AntiSide = 180.f;
				else if (g_Math.NormalizeYaw(pEnt->GetEyeAngles().y - g_Math.NormalizeYaw(Back + 90)) < 0.f)
					AntiSide = 0.f;
			}

			float Brute = g_Math.NormalizeYaw(Back + AntiSide);
			float Delta = g_Math.NormalizeYaw(g_Math.NormalizeYaw(Brute - pEnt->GetEyeAngles().y) + Resim);

			if (Delta > 60.f) // who needs clamps
				Delta = 60.f;
			if (Delta < -60.f)
				Delta = -60.f;

			Resim += Delta;
			DesyncFix += Delta;

			if (Resim > 60.f)
				Resim = 60.f;
			if (Resim < -60.f)
				Resim = -60.f;
		}

		float Equalized;

		//	if (fabs(g_Math.NormalizeYaw(oldEyeAngles[pEnt->EntIndex()].y - pEnt->GetEyeAngles().y)) < 60.f) // yea basically im retarded
		Equalized = g_Math.NormalizeYaw(pEnt->GetEyeAngles().y + Resim);
		//	else
		//		Equalized = g_Math.NormalizeYaw(pEnt->GetEyeAngles().y - Resim);

		if (g_Menu.Config.ResolverType == 0 && !shot && pEnt != pLocalEnt && pEnt->GetTeam() != pLocalEnt->GetTeam() && (pEnt->GetFlags() & FL_ONGROUND))
			AnimState->m_flGoalFeetYaw = Equalized;
	}

	if (g_Menu.Config.ResolverType == 0 && !shot && pEnt != pLocalEnt && pEnt->GetTeam() != pLocalEnt->GetTeam()) // 1337 pitch resolver
	{
		switch (g::MissedShots[pEnt->EntIndex()] % 3) // i skip case 0 on purpose noob
		{
		case 1:PosParams[12] = 89.f; break;
		case 2:PosParams[12] = -89.f; break;
		}
	}

	if (shot && pEnt != pLocalEnt && pEnt->GetTeam() != pLocalEnt->GetTeam() && PosParams[12] <= -80) // ez shot pitch fix LMAO
		PosParams[12] = 89.f;

	if (update)
	{
		oldEyeAngles[pEnt->EntIndex()] = pEnt->GetEyeAngles();
		oldSimtime[pEnt->EntIndex()] = storedSimtime[pEnt->EntIndex()];
		storedSimtime[pEnt->EntIndex()] = pEnt->GetSimulationTime();
		oldOrigin[pEnt->EntIndex()] = pEnt->GetOrigin();
	}

	pEnt->SetAbsAngles(Vector(0, oldGoalfeetYaw[pEnt->EntIndex()], 0));
}


float Resolver::GetMaxDDelta(C_AnimState* animstate)
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

// Gets the delta that results in the angle closest to the player's backwards angle
float Resolver::get_weighted_desync_delta(C_BaseEntity* player, float abs_angle, bool breaking_lby)
{
	float delta = player->GetMaxDelta(player->AnimState()) + breaking_lby ? 30.f : 0.f;

	float relative = -abs_angle;
	float positive = abs_angle + delta;
	float negative = abs_angle - delta;

	float positive_delta = abs(g_Math.NormalizeYaw(relative - positive));
	float negative_delta = abs(g_Math.NormalizeYaw(relative - negative));

	return positive_delta > negative_delta ? -delta : delta;
}

void Resolver::NewAnimationFix(C_BaseEntity* pEnt) // terrible resolver
{
	if (pEnt == g::pLocalEntity) {
		pEnt->ClientAnimations(true);
		auto player_animation_state = pEnt->AnimState();
		player_animation_state->m_flLeanAmount = 20;
		player_animation_state->m_flCurrentTorsoYaw += 15;
		pEnt->UpdateClientAnimation();
		pEnt->SetAbsAngles(Vector(0, player_animation_state->m_flGoalFeetYaw, 0));
		pEnt->ClientAnimations(false);
	}
	else {
		auto player_index = pEnt->EntIndex() - 1;

		pEnt->ClientAnimations(true);

		auto old_curtime = g_pGlobalVars->curtime;
		auto old_frametime = g_pGlobalVars->frametime;

		g_pGlobalVars->curtime = pEnt->GetSimulationTime();
		g_pGlobalVars->frametime = g_pGlobalVars->intervalPerTick;

		auto player_animation_state = pEnt->AnimState();
		auto player_model_time = reinterpret_cast<int*>(player_animation_state + 112);
		if (player_animation_state != nullptr && player_model_time != nullptr)
			if (*player_model_time == g_pGlobalVars->framecount)
				* player_model_time = g_pGlobalVars->framecount - 1;


		pEnt->UpdateClientAnimation();

		g_pGlobalVars->curtime = old_curtime;
		g_pGlobalVars->frametime = old_frametime;

		float feetyaw = pEnt->AnimState()->m_flGoalFeetYaw;

		switch (g::shots % 3)
		{
		case 0:
			pEnt->SetAbsAngles(Vector(0, pEnt->AnimState()->m_flGoalFeetYaw, 0));
			break;
		case 1:
			pEnt->SetAbsAngles(Vector(0, (feetyaw - rand() % 58 - 29 - get_weighted_desync_delta(pEnt, pEnt->GetEyeAngles().y, true)), 0));
			break;
		case 2:
			pEnt->SetAbsAngles(Vector(0, (GetMaxDDelta(pEnt->AnimState())), 0));
			break;
		}
	
		pEnt->ClientAnimations(false); 
	}

}
float flAngleMod(float flAngle)
{
	return((360.0f / 65536.0f) * ((int32_t)(flAngle * (65536.0f / 360.0f)) & 65535));
}
float ApproachAngle(float target, float value, float speed)
{
	target = flAngleMod(target);
	value = flAngleMod(value);

	float delta = target - value;

	// Speed is assumed to be positive
	if (speed < 0)
		speed = -speed;

	if (delta < -180)
		delta += 360;
	else if (delta > 180)
		delta -= 360;

	if (delta > speed)
		value += speed;
	else if (delta < -speed)
		value -= speed;
	else
		value = target;

	return value;
}

void HandleBackUpResolve(C_BaseEntity * pEnt) {

	if (!g_Menu.Config.Resolver)
		return;

	if (pEnt->GetTeam() == g::pLocalEntity->GetTeam())
		return;

	const auto player_animation_state = pEnt->AnimState();

	if (!player_animation_state)
		return;

	float m_flLastClientSideAnimationUpdateTimeDelta = fabs(player_animation_state->m_iLastClientSideAnimationUpdateFramecount - player_animation_state->m_flLastClientSideAnimationUpdateTime);

	auto v48 = 0.f;

	if (player_animation_state->m_flFeetSpeedForwardsOrSideWays >= 0.0f)
	{
		v48 = fminf(player_animation_state->m_flFeetSpeedForwardsOrSideWays, 1.0f);
	}
	else
	{
		v48 = 0.0f;
	}

	float v49 = ((player_animation_state->m_flStopToFullRunningFraction * -0.30000001) - 0.19999999) * v48;

	float flYawModifier = v49 + 1.0;

	if (player_animation_state->m_fDuckAmount > 0.0)
	{
		float v53 = 0.0f;

		if (player_animation_state->m_flFeetSpeedUnknownForwardOrSideways >= 0.0)
		{
			v53 = fminf(player_animation_state->m_flFeetSpeedUnknownForwardOrSideways, 1.0);
		}
		else
		{
			v53 = 0.0f;
		}
	}

	float flMaxYawModifier = player_animation_state->pad10[516] * flYawModifier;
	float flMinYawModifier = player_animation_state->pad10[512] * flYawModifier;

	float newFeetYaw = 0.f;

	auto eyeYaw = player_animation_state->m_flEyeYaw;

	auto lbyYaw = player_animation_state->m_flGoalFeetYaw;

	float eye_feet_delta = fabs(eyeYaw - lbyYaw);

	if (eye_feet_delta <= flMaxYawModifier)
	{
		if (flMinYawModifier > eye_feet_delta)
		{
			newFeetYaw = fabs(flMinYawModifier) + eyeYaw;
		}
	}
	else
	{
		newFeetYaw = eyeYaw - fabs(flMaxYawModifier);
	}

	float v136 = fmod(newFeetYaw, 360.0);

	if (v136 > 180.0)
	{
		v136 = v136 - 360.0;
	}

	if (v136 < 180.0)
	{
		v136 = v136 + 360.0;
	}

	player_animation_state->m_flGoalFeetYaw = v136;

	
}

bool is_slow_walking(C_BaseEntity* entity) {
	float velocity_2D[64], old_velocity_2D[64];

	if (entity->GetVelocity().Length2D() != velocity_2D[entity->EntIndex()] && entity->GetVelocity().Length2D() != NULL) {
		old_velocity_2D[entity->EntIndex()] = velocity_2D[entity->EntIndex()];
		velocity_2D[entity->EntIndex()] = entity->GetVelocity().Length2D();
	}

	if (velocity_2D[entity->EntIndex()] > 0.1) {
		int tick_counter[64];

		if (velocity_2D[entity->EntIndex()] == old_velocity_2D[entity->EntIndex()])
			++tick_counter[entity->EntIndex()];
		else
			tick_counter[entity->EntIndex()] = 0;

		while (tick_counter[entity->EntIndex()] > (1 / g_pGlobalVars->intervalPerTick) * fabsf(0.1f))// should give use 100ms in ticks if their speed stays the same for that long they are definetely up to something..
			return true;

	}

}

void Resolver::fuckingshutthefuckupretard(C_BaseEntity* entity)
{

	auto local_player = g::pLocalEntity;
	if (!local_player)
		return;

	int i = entity->EntIndex();
	static float MoveReal[65], FakeWalkandslowwalkingfix[65];
	auto animation_state = entity->AnimState();
	auto animstate = uintptr_t(local_player->AnimState());
	float duckammount = *(float*)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));
	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));
	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;
	if (duckammount > 0) {
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}
	unk3 = *(float*)(animstate + 0x334) * unk2;


	if (duckammount > 0)
	{
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}
	unk3 = *(float*)(animation_state + 0x334) * unk2;

	auto feet_yaw = animation_state->m_flCurrentFeetYaw;
	float body_yaw = 58.f; animation_state->m_flCurrentTorsoYaw;
	auto move_yaw = 58.f;
	auto goal_feet_yaw = animation_state->m_flGoalFeetYaw;
	auto shit = body_yaw - feet_yaw;
	auto shitv2 = body_yaw + feet_yaw;

	float feet_yaw_rate = animation_state->m_flFeetYawRate;
	float fff = animation_state->m_flFeetSpeedForwardsOrSideWays;
	float forwardorsideways = animation_state->m_flFeetSpeedUnknownForwardOrSideways;
	float feet_cucle = animation_state->m_flFeetCycle;
	float headheighanimation = animation_state->m_flHeadHeightOrOffsetFromHittingGroundAnimation;
	float new_body_yaw = animation_state->m_flCurrentTorsoYaw;
	auto body_max_rotation = animation_state->pad10[416];
	auto normalized_eye_abs_yaw_diff = fmod((animation_state->m_flEyeYaw - feet_yaw), 360.0);
	auto body_min_rotation = animation_state->pad10[412];
	if (entity->GetVelocity().Length2D() < 35.f)
	{
		if (move_yaw)
		{
			animation_state->m_flEyeYaw = animation_state->m_flEyeYaw + move_yaw + feet_yaw * 32.f && feet_yaw + feet_yaw_rate / 32.f;
		}
		else
		{
			if (feet_yaw && move_yaw)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw + feet_yaw + feet_yaw_rate * -32.f && goal_feet_yaw + feet_yaw / 32.f;
			}
		}
	}
	else
	{
		if (entity->GetVelocity().Length2D() > 0 && entity->GetFlags() & FL_ONGROUND)
		{
			if (normalized_eye_abs_yaw_diff > 0 || normalized_eye_abs_yaw_diff == 0)
			{
				body_min_rotation / feet_yaw / 32.f;
			}
			else
			{
				body_max_rotation / feet_yaw / -32.f;
			}
			if (new_body_yaw == 58.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - body_yaw * -58.f + goal_feet_yaw + feet_yaw_rate + feet_yaw / 32.f;
			}
			else if (new_body_yaw >= -58.f && new_body_yaw == body_yaw)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - new_body_yaw / 45.f || 58.f && goal_feet_yaw - feet_yaw * 32.f;
			}
			else if (new_body_yaw <= 58.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - body_yaw * 58.f + feet_yaw / -32.f && goal_feet_yaw * 32.f;
			}
			else if (new_body_yaw == 58.f && new_body_yaw <= 58.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - goal_feet_yaw / 29.f + feet_yaw * -32.f && new_body_yaw * 58.f - body_yaw / -58.f;
			}
			else if (new_body_yaw >= -58.f && body_yaw == 58.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - new_body_yaw * 58.f - feet_yaw * -32.f && goal_feet_yaw - 32.f && feet_yaw / -32.f;
			}
		}
		if (is_slow_walking(entity))
		{
			if (normalized_eye_abs_yaw_diff > 0 || normalized_eye_abs_yaw_diff == 0)
			{
				body_min_rotation / move_yaw / -58.f;
			}
			else
			{
				body_max_rotation / move_yaw / 58.f;
			}
			if (goal_feet_yaw <= -29.f && feet_yaw >= -29.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - move_yaw / 58.f + feet_yaw - goal_feet_yaw * 29.f;
			}
			else if (feet_yaw >= 29.f && feet_yaw_rate <= 29.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw + move_yaw + 58.f - feet_yaw + feet_yaw_rate / 29.f;
			}
			else if (goal_feet_yaw >= -29.f)
			{
				animation_state->m_flEyeYaw = animation_state->m_flEyeYaw - move_yaw / 58.f + feet_yaw_rate - feet_cucle + 29.f && goal_feet_yaw * 29.f;
			}
		}
	}
	bool IsMoving = entity->GetVelocity().Length2D() > 36;
	bool SlowMo = entity->GetVelocity().Length2D() < 35;
	bool IsDucking = entity->GetFlags() & FL_DUCKING;

	auto lowerbody = entity->GetLowerBodyYaw();

	auto animationstate = entity->AnimState();

	for (int i = 1; i < g_pGlobalVars->maxClients; i++)
	{
		auto p_entity = g_pEntityList->GetClientEntity(i);
		if (p_entity && !p_entity->IsDormant())
		{

			if (feet_yaw <= 58)
			{
				if (-58 > feet_yaw)
					lowerbody = body_max_rotation + p_entity->GetLowerBodyYaw(); //desync
			}
			else
			{
				lowerbody = body_max_rotation - p_entity->GetLowerBodyYaw();
			}
			if (p_entity->AnimOverlays()->m_flPlaybackRate > 0.1)
			{
				for (int resolve_delta = 58.f; resolve_delta < -58.f; resolve_delta = resolve_delta - 29.f)
				{
					lowerbody = resolve_delta;
				}
			}
		}
	}
}

void HandleHits(C_BaseEntity* pEnt)
{
	auto NetChannel = g_pEngine->GetNetChannelInfo();

	if (!NetChannel)
		return;

	static float predTime[65];
	static bool init[65];

	if (g::Shot[pEnt->EntIndex()])
	{
		if (init[pEnt->EntIndex()])
		{
			g_Resolver.pitchHit[pEnt->EntIndex()] = pEnt->GetEyeAngles().x;
			predTime[pEnt->EntIndex()] = g_pGlobalVars->curtime + NetChannel->GetAvgLatency(FLOW_INCOMING) + NetChannel->GetAvgLatency(FLOW_OUTGOING) + TICKS_TO_TIME(1) + TICKS_TO_TIME(g_pEngine->GetNetChannel()->m_nChokedPackets);
			init[pEnt->EntIndex()] = false;
		}

		if (g_pGlobalVars->curtime > predTime[pEnt->EntIndex()] && !g::Hit[pEnt->EntIndex()])
		{
			g::MissedShots[pEnt->EntIndex()] += 1;
			g::Shot[pEnt->EntIndex()] = false;
		}
		else if (g_pGlobalVars->curtime <= predTime[pEnt->EntIndex()] && g::Hit[pEnt->EntIndex()])
			g::Shot[pEnt->EntIndex()] = false;

	}
	else
		init[pEnt->EntIndex()] = true;

	g::Hit[pEnt->EntIndex()] = false;
}


void Resolver::OnCreateMove() // cancer v2
{
	if (!g_Menu.Config.Resolver)
		return;

	if (!g::pLocalEntity->IsAlive())
		return;

	if (!g::pLocalEntity->GetActiveWeapon() || g::pLocalEntity->IsKnifeorNade())
		return;


	for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
		{
			UseFreestandAngle[i] = false;
			continue;
		}

		if (abs(pPlayerEntity->GetVelocity().Length2D()) > 29.f)
			UseFreestandAngle[pPlayerEntity->EntIndex()] = false;

		if (abs(pPlayerEntity->GetVelocity().Length2D()) <= 29.f && !UseFreestandAngle[pPlayerEntity->EntIndex()])
		{
			bool Autowalled = false, HitSide1 = false, HitSide2 = false;

			float angToLocal = g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pPlayerEntity->GetOrigin()).y;
			Vector ViewPoint = g::pLocalEntity->GetOrigin() + Vector(0, 0, 90);

			Vector2D Side1 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal))),(45 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
			Vector2D Side2 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

			Vector2D Side3 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal))),(50 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
			Vector2D Side4 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

			Vector Origin = pPlayerEntity->GetOrigin();

			Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

			Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

			for (int side = 0; side < 2; side++)
			{
				Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 90 };
				Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

				if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint))
				{
					if (side == 0)
					{
						HitSide1 = true;
						FreestandAngle[pPlayerEntity->EntIndex()] = 90;
					}
					else if (side == 1)
					{
						HitSide2 = true;
						FreestandAngle[pPlayerEntity->EntIndex()] = -90;
					}

					Autowalled = true;
				}
				else
				{
					for (int side222 = 0; side222 < 2; side222++)
					{
						Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 90 };

						if (g_Autowall.CanHitFloatingPoint(OriginAutowall222, OriginAutowall2))
						{
							if (side222 == 0)
							{
								HitSide1 = true;
								FreestandAngle[pPlayerEntity->EntIndex()] = 90;
							}
							else if (side222 == 1)
							{
								HitSide2 = true;
								FreestandAngle[pPlayerEntity->EntIndex()] = -90;
							}

							Autowalled = true;
						}
					}
				}
			}

			if (Autowalled)
			{
				if (HitSide1 && HitSide2)	
					UseFreestandAngle[pPlayerEntity->EntIndex()] = false;
				else
					UseFreestandAngle[pPlayerEntity->EntIndex()] = true;
			}
		}
	}
}


float AngleNormalize(float angle)
{
	angle = fmodf(angle, 360.0f);
	if (angle > 180)
	{
		angle -= 360;
	}
	if (angle < -180)
	{
		angle += 360;
	}
	return angle;
}

// some reversed bullshit off yougame https://yougame.biz/threads/93259/
// has shit added to it for memes
void bruhResolver(C_BaseEntity* ent)
{
	if (!g::pLocalEntity->IsAlive())
		return;

	auto animstate = ent->AnimState();
	auto v9 = (animstate->m_iLastClientSideAnimationUpdateFramecount - animstate->m_flLastClientSideAnimationUpdateTime);
	auto speedfraction = 0.0f;
	if (animstate->m_flFeetSpeedForwardsOrSideWays < 0.0f)
		speedfraction = 0.0f;
	else
		speedfraction = fminf(animstate->m_flFeetSpeedForwardsOrSideWays, 0x3F800000);
	auto v2 = (animstate->pad_0x0120() * -0.30000001 - 0.19999999) * speedfraction;
	auto v18 = v2;
	auto v3 = v2 + 1.0;
	auto v23 = v3;
	if (animstate->m_fDuckAmount > 0.0)
	{
		auto v29 = 0.0;
		if (animstate->m_flFeetSpeedUnknownForwardOrSideways < 0.0)
			v29 = 0.0;
		else
			v29 = fminf(animstate->m_flFeetSpeedUnknownForwardOrSideways, 0x3F800000);
	}
	auto localplayer_index = g::pLocalEntity->EntIndex();
	auto localplayer = g::pLocalEntity;
	if (localplayer)
	{
		auto fix_goal_feet_yaw = [](float rotation, float invertedrotation, float yawfeetdelta, float yaw, C_AnimState * state) // some shit i found on pastebin lol
		{
			if (yawfeetdelta < rotation)
			{
				if (invertedrotation > yawfeetdelta)
					* (float*)(uintptr_t(state) + 0x80) = invertedrotation + yaw;
			}
			else
				*(float*)(uintptr_t(state) + 0x80) = yaw - rotation;
		};

		auto get_rotation = [&](int rotation_type, C_AnimState * state) {
			float v43 = *(float*)((uintptr_t)state + 0xA4);
			float v54 = max(0, min(*reinterpret_cast<float*>((uintptr_t)state + 0xF8), 1));
			float v55 = max(0, min(1, *reinterpret_cast<float*>((uintptr_t)state + 0xFC)));

			float v56;
			v56 = ((*reinterpret_cast<float*>((uintptr_t)state + 0x11C) * -0.30000001) - 0.19999999) * v54;
			if (v43 > 0)
				v56 += ((v43 * v55) * (0.5 - v56));

			v56 = *(float*)((uintptr_t)state + rotation_type) * v56;
			return v56;
		};
		float inverted = get_rotation(0x2B4, ent->AnimState());
		float max = get_rotation(0x2B0, ent->AnimState());
		float yawfeetdelta = ent->AnimState()->m_flEyeYaw - ent->AnimState()->m_flGoalFeetYaw;
		float yaw = ent->GetEyeAngles().y;
		if (g_Menu.Config.FixFeet)
			fix_goal_feet_yaw(max, inverted, yawfeetdelta, yaw, ent->AnimState());
		float speed;
		if (*(float*)(animstate + 0xF8) < 0.f)
		{
			speed = 0.0;
		}
		else
		{
			speed = fminf(*(DWORD*)(animstate + 0xF8), 1.0f);
		}

		float flYawModifier = (*(float*)(animstate + 0x11C) * -0.30000001 - 0.19999999) * speed;
		flYawModifier += 1.0f;

		if (*(float*)(animstate + 0xA4) > 0.0 && *(float*)(animstate + 0xFC) >= 0.0)
			flYawModifier = fminf(*(float*)(uintptr_t(animstate) + 0xFC), 1.0f);

		float m_flMaxBodyYaw = *(float*)(uintptr_t(animstate) + 0x334) * flYawModifier;
		float m_flMinBodyYaw = *(float*)(uintptr_t(animstate) + 0x330) * flYawModifier;

		float ResolvedYaw = animstate->m_flEyeYaw;
		float delta = std::abs(animstate->m_flEyeYaw - animstate->m_flGoalFeetYaw);
		if (m_flMaxBodyYaw < delta)
		{
			ResolvedYaw = animstate->m_flEyeYaw - std::abs(m_flMaxBodyYaw);
		}
		else if (m_flMinBodyYaw > delta)
		{
			ResolvedYaw = animstate->m_flEyeYaw + std::abs(m_flMinBodyYaw);
		}
		auto player = ent;
		auto v8 = 0;
		auto v7 = 0;
		for (int a2a = 0; a2a < g::pLocalEntity->GetNumAnimOverlays(); ++a2a)
		{
			auto v32 = g::pLocalEntity->GetAnimOverlay(a2a);
			if (v32)
				auto v6 = g::pLocalEntity;
		}
		auto v20 = animstate->flUpVelocity * v23;
		auto a1 = animstate->m_vVelocityY * v23;
		auto v30 = 0.0;
		auto eye_angles_y = animstate->m_flEyeYaw;
		auto goal_feet_yaw = animstate->m_flGoalFeetYaw;
		auto v22 = (eye_angles_y - goal_feet_yaw);
		if (v20 < v22)
		{
			auto v11 = v20;
			auto v30 = eye_angles_y - v11;
		}
		else if (a1 > v22)
		{
			auto v12 = a1;
			auto v30 = v12 + eye_angles_y;
		}
		float v36 = std::fmodf(v30, 360.0f);
		if (v36 > 180.0f)
			v36 = v36 - 360.0f;
		if (v36 < 180.0f)
			v36 = v36 + 360.0f;
		float inverse = 0 - v36;
		switch (g::shots % 4)
		{
		case 0:
			animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw + 45.0;
			break;
		case 1:
			animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw - 45.0;
			break;
		case 2:
			animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw - 30.0;
			break;
		case 3:
			animstate->m_flGoalFeetYaw = animstate->m_flGoalFeetYaw + 30.0;
			break;
		}	
		switch (g::shots % 3)
		{
		case 0:
			ent->SetAbsAngles(Vector(0, v36, 0));
			break;
		case 1:
			ent->SetAbsAngles(Vector(0, inverse, 0));
			break;
		case 2:
			ent->SetAbsAngles(Vector(0, AngleNormalize(ResolvedYaw), 0));
			break;
		}
	}
}

void Resolver::FrameStage(ClientFrameStage_t stage)
{
	if (!g::pLocalEntity || !g_pEngine->IsInGame())
		return;

	static bool  wasDormant[65];

	for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive())
			continue;
		if (pPlayerEntity->IsDormant())
		{
			wasDormant[i] = true;
			continue;
		}

		if (stage == FRAME_RENDER_START)
		{
			if (pPlayerEntity == g::pLocalEntity) {
				AnimationFix(g::pLocalEntity);
			}
			else if (pPlayerEntity != g::pLocalEntity) {
				if (g_Menu.Config.Resolver) {
					if (g_Menu.Config.ResolverType == 0) {
						AnimationFix(pPlayerEntity);
						HandleHits(pPlayerEntity);
					}
					if (g_Menu.Config.ResolverType == 1) {
						//HandleHits(pPlayerEntity);
						//NewNewAnimationFix(pPlayerEntity, g::pLocalEntity);
						pPlayerEntity->ClientAnimations(true);
						bruhResolver(pPlayerEntity);
						pPlayerEntity->UpdateClientAnimation();
						pPlayerEntity->ClientAnimations(false);
					}
				}
			}
		}

		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
			if (g_Menu.Config.ResolverType == 1) {
				HandleBackUpResolve(pPlayerEntity);
			}
		}

		if (stage == FRAME_NET_UPDATE_END && pPlayerEntity != g::pLocalEntity)
		{
			auto VarMap = reinterpret_cast<uintptr_t>(pPlayerEntity) + 36;
			auto VarMapSize = *reinterpret_cast<int*>(VarMap + 20);

			for (auto index = 0; index < VarMapSize; index++)
				*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(VarMap) + index * 12) = 0;
		}

		wasDormant[i] = false;
	}
}
