#include "AntiAim.h"
#include "..\Aimbot\Autowall.h"
#include "..\Aimbot\Aimbot.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\Utils\Math.h"
#include "..\..\Menu\Menu.h"
#include "..\..\SDK\memalloc.h"
#include "..\..\SDK\ICvar.h"


AntiAim g_AntiAim;

bool Swtich = false;

void Jitter()
{

	if (g::bSendPacket)
	{
		Swtich = !Swtich;
	}

	if (g_Menu.Config.Jitter == 0) {

	} 
	if (g_Menu.Config.Jitter == 1) {
		g::pCmd->viewangles.y = g::pCmd->viewangles.y + g_Math.RandomFloat(-g_Menu.Config.JitterRange, g_Menu.Config.JitterRange);
	} 
	float Offset = g_Menu.Config.JitterRange / 2;
	if (g_Menu.Config.Jitter == 2) {
		Swtich ? g::pCmd->viewangles.y -= Offset : g::pCmd->viewangles.y += Offset;		
	} 
	if (g_Menu.Config.Jitter == 3) {
		switch (int(g_pGlobalVars->curtime * 4.8) % 2) {
		case 1: g::pCmd->viewangles.y += g_Menu.Config.JitterRange; break;
		case 2: g::pCmd->viewangles.y -= g_Menu.Config.JitterRange; break;
		}
	}
}

Vector calculate_angle(Vector src, Vector dst) {
	Vector angles;

	Vector delta = src - dst;
	float hyp = delta.Length2D();

	angles.y = std::atanf(delta.y / delta.x) * 57.2957795131f;
	angles.x = std::atanf(-delta.z / hyp) * -57.2957795131f;
	angles.z = 0.0f;

	if (delta.x >= 0.0f)
		angles.y += 180.0f;

	return angles;
}

float at_target() {
	int cur_tar = 0;
	float last_dist = 999999999999.0f;
	for (int i = 1; i <= g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* m_entity = g_pEntityList->GetClientEntity(i);

		if (!m_entity || m_entity == g::pLocalEntity)
			continue;

		if (!m_entity->is_valid())
			continue;

		auto m_player = (C_BaseEntity*)m_entity;
		if (!m_player->IsDormant() && m_player->is_valid() && m_player->GetTeam() != g::pLocalEntity->GetTeam()) {
			float cur_dist = (m_entity->GetOrigin() - g::pLocalEntity->GetOrigin()).Length();

			if (!cur_tar || cur_dist < last_dist) {
				cur_tar = i;
				last_dist = cur_dist;
			}
		}
	}

	if (cur_tar) {
		auto m_entity = static_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(cur_tar));
		if (!m_entity) {
			return g::pCmd->viewangles.y;
		}

		Vector target_angle = calculate_angle(g::pLocalEntity->GetOrigin(), m_entity->GetOrigin());
		return target_angle.y;
	}

	return g::pCmd->viewangles.y;
}

void AntiAim::NormalAA()
{
	if (g_Menu.Config.FreestandType == 0) {
		constexpr auto maxRange = 90.0f;
		int angleAdditive; 

		if (g_Menu.Config.AtTarget) {
			angleAdditive = g_Menu.Config.Yaw + at_target();
			if (g::pLocalEntity->GetFlags() & FL_ONGROUND)
				g::pCmd->viewangles.y = angleAdditive;
			else
				g::pCmd->viewangles.y = angleAdditive - maxRange / 2.f + std::fmodf(g_pGlobalVars->curtime * 60, maxRange);
		}
		else
		{
			angleAdditive = g_Menu.Config.Yaw;

			if (g::pLocalEntity->GetFlags() & FL_ONGROUND)
				g::pCmd->viewangles.y += angleAdditive;
			else
				g::pCmd->viewangles.y += angleAdditive - maxRange / 2.f + std::fmodf(g_pGlobalVars->curtime * 60, maxRange);
		}
	}


	if (g_Menu.Config.JitterRange != 0)
	{
		Jitter();
	}
}

void FreeStanding() // cancer v1
{
	static float FinalAngle;
	bool bside1 = false;
	bool bside2 = false;
	bool autowalld = false;
	for (int i = 1; i <= g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		float angToLocal = g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pPlayerEntity->GetOrigin()).y;
		Vector ViewPoint = pPlayerEntity->GetOrigin() + Vector(0, 0, 90);

		Vector2D Side1 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal))),(45 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side2 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector2D Side3 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal))),(50 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side4 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector Origin = g::pLocalEntity->GetOrigin();

		Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

		Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

		for (int side = 0; side < 2; side++)
		{
			Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 80 };
			Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

			if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint))
			{
				if (side == 0)
				{
					bside1 = true;
					FinalAngle = angToLocal + 90;
				}
				else if (side == 1)
				{
					bside2 = true;
					FinalAngle = angToLocal - 90;
				}
				autowalld = true;
			}
			else
			{
				for (int side222 = 0; side222 < 2; side222++)
				{
					Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 80 };

					if (g_Autowall.CanHitFloatingPoint(OriginAutowall222, OriginAutowall2))
					{
						if (side222 == 0)
						{
							bside1 = true;
							FinalAngle = angToLocal + 90;
						}
						else if (side222 == 1)
						{
							bside2 = true;
							FinalAngle = angToLocal - 90;
						}
						autowalld = true;
					}
				}
			}
		}
	}

	if (!autowalld || (bside1 && bside2)) {
		constexpr auto maxRange = 90.0f;
		int angleAdditive;

		if (g_Menu.Config.AtTarget) {
			angleAdditive = g_Menu.Config.Yaw + at_target();
			if (g::pLocalEntity->GetFlags() & FL_ONGROUND)
				g::pCmd->viewangles.y = angleAdditive;
			else
				g::pCmd->viewangles.y = angleAdditive - maxRange / 2.f + std::fmodf(g_pGlobalVars->curtime * 60, maxRange);
		}
		else
		{
			angleAdditive = g_Menu.Config.Yaw;

			if (g::pLocalEntity->GetFlags() & FL_ONGROUND)
				g::pCmd->viewangles.y += angleAdditive;
			else
				g::pCmd->viewangles.y += angleAdditive - maxRange / 2.f + std::fmodf(g_pGlobalVars->curtime * 60, maxRange);
		}
	}
	else
		g::pCmd->viewangles.y = FinalAngle;
}

float GetMaxDelta(C_AnimState* animstate) 
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

bool m_bBreakLowerBody = false;
float_t m_flSpawnTime = 0.f;
float_t m_flNextBodyUpdate = 0.f;
CBaseHandle* m_ulEntHandle = nullptr;

C_AnimState* m_serverAnimState = nullptr;

void ResetAnimationState(C_AnimState* state)
{
	if (!state)
		return;

	using ResetAnimState_t = void(__thiscall*)(C_AnimState*);
	static auto ResetAnimState = (ResetAnimState_t)Utils::FindSignature("client_panorama.dll", "56 6A 01 68 ? ? ? ? 8B F1");
	if (!ResetAnimState)
		return;

	ResetAnimState(state);
}

float ClampYaw(float yaw)
{
	while (yaw > 180.f)
		yaw -= 360.f;
	while (yaw < -180.f)
		yaw += 360.f;
	return yaw;
}


#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / g_pGlobalVars->intervalPerTick ) )
#define TICKS_TO_TIME( t )		( g_pGlobalVars->intervalPerTick *( t ) )

bool break_lby = false;
float next_update = 0;
void update_lowerbody_breaker() { // from HappyHack by "Incriminating" (unknowncheats)
	float server_time = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick, speed = g::pLocalEntity->GetVelocity().Length2DSqr(), oldCurtime = g_pGlobalVars->curtime;

	if (speed > 0.1) {
		next_update = server_time + 0.22;
	}

	break_lby = false;

	if (next_update <= server_time) {
		next_update = server_time + 1.1;
		break_lby = true;
	}


	if (!(g::pLocalEntity->GetFlags() & FL_ONGROUND)) {
		break_lby = false;
	}
}

void AntiAim::OldFreestandingdesync()   // freestanding desync
{
	static float FinalAngle;
	bool bside1 = false;
	bool bside2 = false;
	bool autowalld = false;
	auto local_player = g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	auto desync = GetMaxDelta(local_player->AnimState());

	if (g_Menu.Config.FreestandType == 2) {
		constexpr auto maxRange = 90.0f;
		int angleAdditive;

		if (g_Menu.Config.AtTarget) {
			angleAdditive = g_Menu.Config.Yaw + at_target();
			if (g::pLocalEntity->GetFlags() & FL_ONGROUND)
				g::pCmd->viewangles.y = angleAdditive;
			else
				g::pCmd->viewangles.y = angleAdditive - maxRange / 2.f + std::fmodf(g_pGlobalVars->curtime * 60, maxRange);
		}
		else
		{
			if (g::pLocalEntity->GetFlags() & FL_ONGROUND)
				g::pCmd->viewangles.y += angleAdditive;
			else
				g::pCmd->viewangles.y += angleAdditive - maxRange / 2.f + std::fmodf(g_pGlobalVars->curtime * 60, maxRange);
		}
		if (g_Menu.Config.StaticDesync) {
			if (!(g::pCmd->buttons & IN_FORWARD) && !(g::pCmd->buttons & IN_BACK) && !(GetAsyncKeyState(VK_SPACE))) {
				g::pCmd->forwardmove = g::pCmd->tick_count % 2 ? 1.01 : -1.01;
			}
		}
	}

	for (int i = 1; i <= g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		float angToLocal = g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pPlayerEntity->GetOrigin()).y;
		Vector ViewPoint = pPlayerEntity->GetOrigin() + Vector(0, 0, 90);

		Vector2D Side1 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal))),(45 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side2 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector2D Side3 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal))),(50 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side4 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector Origin = g::pLocalEntity->GetOrigin();

		Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

		Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

		g_Math.NormalizeYaw(g::pCmd->viewangles.y);


		for (int side = 0; side < 2; side++)
		{
			Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 80 };
			Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };
			Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 80 };


			if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint) || g_Autowall.CanHitFloatingPoint(OriginAutowall222, OriginAutowall2))
			{
				if (side == 0)
				{
					bside1 = true;
					if (!g::bSendPacket) {
						g::pCmd->viewangles.y -= (30 + desync);
					}

				}
				else if (side == 1)
				{
					bside2 = true;
					if (!g::bSendPacket) {
						g::pCmd->viewangles.y += (30 + desync);
					}

				}
				autowalld = true;
			}
		}
	}

	if (!autowalld || (bside1 && bside2)) {
		if (!g::bSendPacket) {
				Swtich ? g::pCmd->viewangles.y += (30 + desync) : g::pCmd->viewangles.y -= (30 + desync);;
		}
	}

	if (!g::bSendPacket && g_Menu.Config.LBYDelta > 0)
	{

		if (break_lby)
		{
			g::pCmd->viewangles.y += g_Menu.Config.LBYDelta;
		}
	}
}

static float next_lby_update_time = 0;
float get_curtime(CUserCmd* ucmd) {
	auto local_player = g::pLocalEntity;

	if (!local_player)
		return 0;

	int g_tick = 0;
	CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = (float)local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * g_pGlobalVars->intervalPerTick;
	return curtime;
}
bool next_lby_update(const float yaw_to_break, CUserCmd* cmd)
{
	auto local_player = g::pLocalEntity;

	if (!local_player)
		return false;

	static float next_lby_update_time = 0;
	float curtime = get_curtime(cmd);

	auto animstate = local_player->AnimState();
	if (!animstate)
		return false;

	if (!(local_player->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->speed_2d > 0.1)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}
int m_iJitter;
void AntiAim::awresolverbreaker()
{
	if (g_Menu.Config.DesyncType && g::pLocalEntity->AnimState()) {
		float desync = GetMaxDelta(g::pLocalEntity->AnimState());
		float balance = 1.0f;

		int type = rand() % 2;
		if (type == 2)
			balance = -1.0f;

		if (next_lby_update(g::pCmd->viewangles.y, g::pCmd)) {

			if (type == 1)
				g::pCmd->viewangles.y -= 180.0f;
			else
				g::pCmd->viewangles.y += (balance * 60.0f);
		}

		else if (type != 1) {
			int jitter_side = invert ? 1 : -1;
			g::pCmd->viewangles.y += 180.0f;

			float desync = GetMaxDelta(g::pLocalEntity->AnimState());
			float lby_delta = 60.0f - desync + 58.0f;
			float desync_length = 60.0f - lby_delta * 60.0f;
			float jitter = 60.0f * jitter_side;
			auto net_channel = g_pEngine->GetNetChannel();

			if (!net_channel)
				return;
			if (net_channel->m_nChokedPackets >= 10) {
				g::pCmd->viewangles.y = g_Math.NormalizeYaw(g::pCmd->viewangles.y);
				return;
			}
			if (jitter_side == 1)
				g::pCmd->viewangles.y += desync_length;

			else if (jitter_side == -1)
				g::pCmd->viewangles.y -= desync_length;


			int v19 = 0;
			if (g_pGlobalVars->curtime < g_pGlobalVars->curtime <= next_lby_update(g::pCmd->viewangles.y, g::pCmd)) {
				v19 = rand() % 30;
			}
			else {
				m_iJitter = 0;
			}

			int v20 = v19 - 1;
			if (v20) {
				if (v20 == 1) {
					if (jitter_side == 1)
						g::pCmd->viewangles.y += lby_delta;
					else
						g::pCmd->viewangles.y += desync - 68;
				}
			}
			else {
				if (jitter_side == 1)
					g::pCmd->viewangles.y += desync - 68;
				else
					g::pCmd->viewangles.y += lby_delta;
				g::bSendPacket = false;
			}

			if (++m_iJitter >= 3)
				m_iJitter = 0;

			g::pCmd->viewangles.y = g_Math.NormalizeYaw(g::pCmd->viewangles.y);
		}
	}
}


inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}
float normalize_yaw180(float yaw)
{
	if (yaw > 180)
		yaw -= (round(yaw / 360) * 360.f);
	else if (yaw < -180)
		yaw += (round(yaw / 360) * -360.f);

	return yaw;
}
void desync_laurie_experimental(CUserCmd* cmd, bool moving, bool packet)
{
	if (break_lby)
	{
		if (g_pEngine->GetNetChannel()->m_nChokedPackets >= 2) {
			cmd->viewangles.y = normalize_yaw180(cmd->viewangles.y);
			return;
		}
		cmd->viewangles.y -= 120.f;
	}

	if (!packet)
	{
		cmd->viewangles.y -= moving ? 40.f : 90.f;

	}
}
static bool dir, back, up = false;
void desync_stretch_override(CUserCmd* cmd, bool moving, bool packet)
{
	auto feetdelta = g::pLocalEntity->AnimState()->m_flGoalFeetYaw - cmd->viewangles.y;
	float desync = moving ? 29.f : 50.f;
	float lby_delta = 180.0f - desync + 10.0f;
	float desync_length = 180.0f - lby_delta - 10.f;
	float soviet_union = rand() % 21;
	bool it_sucks = soviet_union <= 10;
	int test = rand() % 9;
	if (test <= 3) {
		dir = true;
		back = false;
		up = false;
	}
	else if (test <= 6) {
		back = true;
		up = false;
		dir = false;
	}
	else {
		up = true;
		dir = false;
		back = false;

	}

	if (break_lby)
	{
		if (g_pEngine->GetNetChannel()->m_nChokedPackets >= 2) {
			cmd->viewangles.y = normalize_yaw180(cmd->viewangles.y);
			return;
		}
		if (dir && !back && !up)
		{
			cmd->viewangles.y + -it_sucks ? 80.0f : 100.f;
		}

		else if (!dir && !back && !up)
			cmd->viewangles.y -= it_sucks ? 80.0f : 100.f;

		else
			cmd->viewangles.y += it_sucks ? 160.0f : -160.f;
	}

	if (!packet)
	{
		if (feetdelta < desync)
		{
			cmd->viewangles.y += desync;
			g::pLocalEntity->AnimState()->m_flGoalFeetYaw += desync;
		}
		else {
			cmd->viewangles.y -= desync;
			g::pLocalEntity->AnimState()->m_flGoalFeetYaw -= desync;
		}
	}
}
static bool jitter = false;
void desync_cancer()
{
	auto local_player = g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	auto desync = GetMaxDelta(local_player->AnimState());
	if (!g::bSendPacket)
	{
		float server_time = (float)(local_player->GetTickBase() * g_pGlobalVars->intervalPerTick);
		float time = TIME_TO_TICKS(server_time);

		while (time >= server_time)
			time = 0.f;

		float idk = rand() % 100;

		jitter = !jitter;
		if (time >= server_time / 2)
		{
			if (idk < 70)
			{
				if (!jitter)
					g::pCmd->viewangles.y += desync - rand() % int(desync / 5);

			}
			else
			{
				if (!jitter)
					g::pCmd->viewangles.y -= desync - rand() %  2 / int(desync);

			}
		}
		else
		{
			if (idk < 70)
			{
				if (jitter)
					g::pCmd->viewangles.y -= rand() % int(desync) + rand() % 30;
			}
			else
			{
				if (jitter)
					g::pCmd->viewangles.y += desync;
			}
		}
	}
}
static bool moving;
void AntiAim::Desync()
{
	if (GetKeyState(g_Menu.Config.DesyncInvertKey))
		invert = true;
	else
		invert = false;

	g_Math.NormalizeYaw(g::pCmd->viewangles.y);

	int switchrange;
	
	if (g::bSendPacket)
	{
		Swtich = !Swtich;
	}

	int meme1 = rand() % 100;

	int meme2 = (meme1 / 90) - .5f;

	int flip1 = (int)floorf(g_pGlobalVars->curtime / 0.22f);

	int bro = rand() % 1000;
	int bro2 = rand() % 1000;

	auto local_player = g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	auto desync = GetMaxDelta(local_player->AnimState());

	if (local_player->GetVelocity().Length() > 0.1)
		moving = true;
	else
		moving = false;

	if (g_Menu.Config.FreestandType == 0 || g_Menu.Config.FreestandType == 1) {
		if (g_Menu.Config.DesyncType == 1) {
			if (!g::bSendPacket) { 
				g::pCmd->viewangles.y -= invert ? desync : -desync;
			}
		}
		if (g_Menu.Config.DesyncType == 2) {
			if (!g::bSendPacket) {
				g::pCmd->viewangles.y -= invert ? (30 + desync) : -(30 + desync);
			}
		}
		if (g_Menu.Config.DesyncType == 3) {
			if (!g::bSendPacket) {
				g::pCmd->viewangles.y -= invert ? (rand() % int(desync) + 30) : -(rand() % int(desync) + 30);
			}
		}
		if (g_Menu.Config.DesyncType == 4) {
			if (!g::bSendPacket)
					g::pCmd->viewangles.y -= invert ? (desync / 2.f + std::fmodf(g_pGlobalVars->curtime * 60, desync)) : -(desync / 2.f + std::fmodf(g_pGlobalVars->curtime * 60, desync));
		}
		if (g_Menu.Config.DesyncType == 5) {
			if (!g::bSendPacket)
				awresolverbreaker();
		}

		if (g_Menu.Config.DesyncType == 6) {
			g::pCmd->viewangles.y += g::bSendPacket ? 180 : 0;
			g::pCmd->viewangles.x = 89.0f;		
		}

		if (!g::bSendPacket && g_Menu.Config.LBYDelta > 0)
		{

			if (break_lby || m_bBreakLowerBody)
			{
				g::pCmd->viewangles.y += g_Menu.Config.LBYDelta;
			}

		}
		if (g_Menu.Config.StaticDesync) {
			if (!(g::pCmd->buttons & IN_FORWARD) && !(g::pCmd->buttons & IN_BACK) && !(GetAsyncKeyState(VK_SPACE))) {
				g::pCmd->forwardmove = g::pCmd->tick_count % 2 ? 1.01 : -1.01;
			}
		}
	}
	
}

void AntiAim::FakeDuck(CUserCmd* cmd)
{
	cmd->buttons |= IN_BULLRUSH;

	if (g_pEngine->GetNetChannel()->m_nChokedPackets > 7) {
		cmd->buttons |= IN_DUCK;
	}
	else 
	{
		cmd->buttons &= ~IN_DUCK;
	}	 
}

inline float FasttSqrt(float x)
{
	unsigned int i = *(unsigned int*)& x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)& i;
}
#define square( x ) ( x * x )
void slowmogang(CUserCmd* cmd, float fMaxSpeed) {
	if (!GetAsyncKeyState(VK_SHIFT))
		return;
	if (fMaxSpeed <= 0.f)
		return;
	float fSpeed = (float)(FasttSqrt(square(g::pCmd->forwardmove) + square(g::pCmd->sidemove) + square(g::pCmd->upmove)));
	if (fSpeed <= 0.f)
		return;
	if (cmd->buttons & IN_DUCK)
		fMaxSpeed *= 2.94117647f;
	if (fSpeed <= fMaxSpeed)
		return;
	float fRatio = fMaxSpeed / fSpeed;
	g::pCmd->forwardmove *= fRatio;
	g::pCmd->sidemove *= fRatio;
	g::pCmd->upmove *= fRatio;
}

void AntiAim::SlowWalk(CUserCmd* cmd)
{
	if (!GetAsyncKeyState(VK_SHIFT))
		return;

	auto weapon_handle = g::pLocalEntity->GetActiveWeapon();

	if (!weapon_handle)
		return;

	float amount = 0.0034f * g_Menu.Config.SlowWalkSlider;

	Vector velocity = g::pLocalEntity->GetVelocity();
	Vector direction;

	g_Math.VectorAngles(velocity, direction);

	float speed = velocity.Length2D();

	direction.y = cmd->viewangles.y - direction.y;

	Vector forward;

	g_Math.AngleVectors(direction, &forward);

	Vector source = forward * -speed;

	if (speed >= (weapon_handle->GetCSWpnData()->max_speed * amount))
	{
		cmd->forwardmove = source.x;
		cmd->sidemove = source.y;

	}
}
static int grenadetimer;
static int grenadetimer2;

void AntiAim::niggacheese() // hhhhhh
{
	if (g_Menu.Config.SlowWalk == 1) {
		if (GetAsyncKeyState(VK_SHIFT)) {
			static int choked = 0;
			choked = choked > 7 ? 0 : choked + 1;
			g::pCmd->forwardmove = choked < 2 || choked > 5 ? 0 : g::pCmd->forwardmove;
			g::pCmd->sidemove = choked < 2 || choked > 5 ? 0 : g::pCmd->sidemove;
			g::bSendPacket = choked < 1;
		}
	}

	if (g_Menu.Config.SlowWalk == 2)
		slowmogang(g::pCmd, g_Menu.Config.SlowWalkSlider);
}

void AntiAim::LegitOnCreateMove()
{

	update_lowerbody_breaker();

	if (g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_LADDER)
		return;

	if (!g_pEngine->IsInGame())
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

	if (g::LagPeek)
		return;
	
	auto local_player = g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	auto desync = GetMaxDelta(local_player->AnimState());

	int flip = (int)floorf(g_pGlobalVars->curtime / 1.1);
	
	if (g_Menu.Config.LegitAA) { 
		if (!g::bSendPacket)
		{
			g::pCmd->viewangles.y -= ( g_Menu.Config.additive + desync );
		}
		if (g_Menu.Config.LegitAAStatic) {
			if (!(g::pCmd->buttons & IN_FORWARD) && !(g::pCmd->buttons & IN_BACK) && !(GetAsyncKeyState(VK_SPACE))) {
				g::pCmd->forwardmove = g::pCmd->tick_count % 2 ? 1.01 : -1.01;
			}
		}
	}

}

void AntiAim::OnCreateMove()
{

	if (g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_LADDER) 
		return;
	
	if (!g_pEngine->IsInGame() || !g_Menu.Config.Antiaim)
		return;

	if (!g::pLocalEntity->IsAlive())
		return;

	if (GetAsyncKeyState(0x45))
		return;

	C_BaseCombatWeapon* weapon = g::pLocalEntity->GetActiveWeapon(); 
	if (weapon == nullptr) return;


	float flServerTime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
	bool canShoot = (weapon->GetNextPrimaryAttack() <= flServerTime);
	if (weapon->GetItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_REVOLVER) {
		if (canShoot && (g::pCmd->buttons & IN_ATTACK))
			return;
	}
	else {
		auto curtime = g_pGlobalVars->curtime;
		if (canShoot && g_Aimbot.fired && g::revolvertime + .5f >= curtime)
			return;
	}

	if (g::pLocalEntity->IsNade())
		return;

	if (g::bSendPacket)
		Swtich = !Swtich;

	update_lowerbody_breaker();

	g::pCmd->viewangles.x = 89.0f;

	if (g_Menu.Config.FreestandType == 0) {
		NormalAA();
	}
	else if (g_Menu.Config.FreestandType == 1) {
		if (g_Menu.Config.JitterRange != 0)
		{
			Jitter();
		}
		FreeStanding();
	}
	else if (g_Menu.Config.FreestandType == 2) {
		if (g_Menu.Config.JitterRange != 0)
		{
			Jitter();
		}
		OldFreestandingdesync();
	}
	
	if (g_Menu.Config.FakeDuck && GetAsyncKeyState(g_Menu.Config.FakeduckKey)) {
		FakeDuck(g::pCmd);
	}
	Desync();

}