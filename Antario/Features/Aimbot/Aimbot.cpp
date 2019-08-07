#include "Aimbot.h"
#include "Autowall.h"
#include "LagComp.h"
#include "..\AntiAim\AntiAim.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\Menu\Menu.h"
#include "../../SDK/IGameEvent.h"

Aimbot g_Aimbot;

int Aimbot::GetTickbase(CUserCmd* ucmd) {

	static int g_tick = 0;
	static CUserCmd* g_pLastCmd = nullptr;

	if (!ucmd)
		return g_tick;

	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = g::pLocalEntity->GetTickBase();
	}
	else {
		// Required because prediction only runs on frames, not ticks
		// So if your framerate goes below tickrate, m_nTickBase won't update every tick
		++g_tick;
	}

	g_pLastCmd = ucmd;
	return g_tick;
}

void Aimbot::Autostop()
{
	if (g::pLocalEntity->zeus())
		return;

	if (!g_Menu.Config.Autostop)
		return;

	if (!(g::pLocalEntity->GetFlags() & FL_ONGROUND))
		return;

	Vector Velocity = g::pLocalEntity->GetVelocity();
	static float Speed = 450.f;

	Vector Direction;
	Vector RealView;
	g_Math.VectorAngles(Velocity, Direction);
	g_pEngine->GetViewAngles(RealView += 180);
	Direction.y = RealView.y - Direction.y;

	Vector Forward;
	g_Math.AngleVectors(Direction, &Forward);
	Vector NegativeDirection = Forward * -Speed;

	g::pCmd->forwardmove = NegativeDirection.x;
	g::pCmd->sidemove = NegativeDirection.y;

}
void autoscope() {
	auto me = g::pLocalEntity;
	auto cmd = g::pCmd;
	if ((g::pLocalEntity->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SCAR20 || g::pLocalEntity->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_G3SG1 || g::pLocalEntity->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SSG08)) {
		if (g_Menu.Config.Aimbot) {
			if (!me->IsScoped()) {
				cmd->buttons |= IN_ATTACK2;
				return;
			}
			else if (me->IsScoped())
			{
				return;
			}
		}
	}
}

bool Aimbot::HitChance(C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance)
{
	if (chance == 0)
		return true;

	if (Backtrack[pEnt->EntIndex()] || ShotBacktrack[pEnt->EntIndex()]) // doing this bec im lazy
	{
		float Velocity = g::pLocalEntity->GetVelocity().Length();

		if (Velocity <= (g::pLocalEntity->GetActiveWeapon()->GetCSWpnData()->max_speed_alt * .34f))
			Velocity = 0.0f;

		float SpreadCone = pWeapon->GetAccuracyPenalty() * 256.0f / M_PI + pWeapon->GetCSWpnData()->max_speed * Velocity / 3000.0f; // kmeth https://github.com/DankPaster/kmethdude
		float a = (Point - g::pLocalEntity->GetEyePosition()).Length();
		float b = sqrt(tan(SpreadCone * M_PI / 180.0f) * a);
		if (2.2f > b) return true;
		return (chance <= ((2.2f / fmax(b, 2.2f)) * 100.0f));
	}

	static float Seeds = 256.f;

	Vector forward, right, up;

	g_Math.AngleVectors(Angle, &forward, &right, &up);

	int Hits = 0, neededHits = (Seeds * (chance / 100.f));

	float weapSpread = pWeapon->GetSpread(), weapInaccuracy = pWeapon->GetInaccuracy();

	bool Return = false;

	for (int i = 0; i < Seeds; i++)
	{
		float Inaccuracy = g_Math.RandomFloat(0.f, 1.f) * weapInaccuracy;
		float Spread = g_Math.RandomFloat(0.f, 1.f) * weapSpread;

		Vector spreadView((cos(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (cos(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Spread), (sin(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (sin(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Spread), 0), direction;
		direction = Vector(forward.x + (spreadView.x * right.x) + (spreadView.y * up.x), forward.y + (spreadView.x * right.y) + (spreadView.y * up.y), forward.z + (spreadView.x * right.z) + (spreadView.y * up.z)).Normalize(); // guess i cant put vector in a cast *nvm im retarded

		Vector viewanglesSpread, viewForward;

		g_Math.VectorAngles(direction, up, viewanglesSpread);
		g_Math.NormalizeAngles(viewanglesSpread);

		g_Math.AngleVectors(viewanglesSpread, &viewForward);
		viewForward.NormalizeInPlace();

		viewForward = g::pLocalEntity->GetEyePosition() + (viewForward * pWeapon->GetCSWpnData()->range);

		C_Trace Trace;

		g_pTrace->ClipRayToEntity(C_Ray(g::pLocalEntity->GetEyePosition(), viewForward), mask_shot | contents_grate, pEnt, &Trace);

		if (Trace.m_pEnt == pEnt)
			Hits++;

		if (((Hits / Seeds) * 100.f) >= chance)
		{
			Return = true;
			break;
		}

		if ((Seeds - i + Hits) < neededHits)
			break;
	}

	return Return;
}


void Aimbot::round_start_event(IGameEvent * pEvent) //rabbit or e0v or skythecoder idk some gay idiot for all the shotsMissed shit
{
	shotsMissed = 0;
}

void Aimbot::weapon_fire_event(IGameEvent * pEvent)
{
	if (g_pEngine->GetPlayerForUserID(pEvent->GetInt("userid")) == g_pEngine->GetLocalPlayer())
	{
		++shotsMissed;
	}
}

void Aimbot::player_hurt_event(IGameEvent * pEvent)
{
	int pDamage = pEvent->GetInt("dmg_health");
	int pVictimID = pEvent->GetInt("userid");
	int pAttackID = pEvent->GetInt("attacker");

	if (g_pEngine->GetPlayerForUserID(pAttackID) == g_pEngine->GetLocalPlayer() && g_pEngine->GetPlayerForUserID(pVictimID) != g_pEngine->GetLocalPlayer())
	{
		--shotsMissed;
	}


}

void Aimbot::player_death_event(IGameEvent * pEvent)
{
	if (g_pEngine->GetPlayerForUserID(pEvent->GetInt("attacker")) == g_pEngine->GetLocalPlayer() && g_pEngine->GetPlayerForUserID(pEvent->GetInt("userid")) != g_pEngine->GetLocalPlayer())
	{
		shotsMissed = 0;
	}
}


bool ShouldBaim(C_BaseEntity * pEnt) // probably dosnt make sense
{
	static float oldSimtime[65];
	static float storedSimtime[65];

	static float ShotTime[65];
	static float NextShotTime[65];
	static bool BaimShot[65];

	if (g_Menu.Config.BaimAlways)
	{
		BaimShot[pEnt->EntIndex()] = true;
	}
	else {

	}

	if (storedSimtime[pEnt->EntIndex()] != pEnt->GetSimulationTime())
	{
		oldSimtime[pEnt->EntIndex()] = storedSimtime[pEnt->EntIndex()];
		storedSimtime[pEnt->EntIndex()] = pEnt->GetSimulationTime();
	}

	float simDelta = storedSimtime[pEnt->EntIndex()] - oldSimtime[pEnt->EntIndex()];

	bool Shot = false;

	if (pEnt->GetActiveWeapon() && !pEnt->IsKnifeorNade())
	{
		if (ShotTime[pEnt->EntIndex()] != pEnt->GetActiveWeapon()->GetLastShotTime())
		{
			Shot = true;
			BaimShot[pEnt->EntIndex()] = false;
			ShotTime[pEnt->EntIndex()] = pEnt->GetActiveWeapon()->GetLastShotTime();
		}
		else
			Shot = false;
	}
	else
	{
		Shot = false;
		ShotTime[pEnt->EntIndex()] = 0.f;
	}

	if (Shot)
	{
		NextShotTime[pEnt->EntIndex()] = pEnt->GetSimulationTime() + pEnt->FireRate();

		if (simDelta >= pEnt->FireRate())
			BaimShot[pEnt->EntIndex()] = true;
	}

	if (BaimShot[pEnt->EntIndex()])
	{
		if (pEnt->GetSimulationTime() >= NextShotTime[pEnt->EntIndex()])
			BaimShot[pEnt->EntIndex()] = false;
	}

	if (g_Menu.Config.BaimPitch && BaimShot[pEnt->EntIndex()] && !(pEnt->GetFlags() & FL_ONGROUND))
		return true;

	if (g_Menu.Config.BaimInAir && !(pEnt->GetFlags() & FL_ONGROUND))
		return true;

	return false;
}

std::vector<Vector> Aimbot::GetMultiplePointsForHitbox(C_BaseEntity* local, C_BaseEntity* entity, int iHitbox, matrix3x4_t BoneMatrix[128]) //stackhack
{
	auto VectorTransform_Wrapper = [](const Vector & in1, const matrix3x4_t & in2, Vector & out)
	{
		auto VectorTransform = [](const float* in1, const matrix3x4_t & in2, float* out)
		{
			auto DotProducts = [](const float* v1, const float* v2)
			{
				return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
			};
			out[0] = DotProducts(in1, in2[0]) + in2[0][3];
			out[1] = DotProducts(in1, in2[1]) + in2[1][3];
			out[2] = DotProducts(in1, in2[2]) + in2[2][3];
		};
		VectorTransform(&in1.x, in2, &out.x);
	};

	studiohdr_t* pStudioModel = g_pModelInfo->GetStudiomodel(entity->GetModel());
	mstudiohitboxset_t* set = pStudioModel->GetHitboxSet(0);
	mstudiobbox_t* hitbox = set->GetHitbox(iHitbox);

	std::vector<Vector> vecArray;

	Vector max;
	Vector min;
	VectorTransform_Wrapper(hitbox->max, BoneMatrix[hitbox->bone], max);
	VectorTransform_Wrapper(hitbox->min, BoneMatrix[hitbox->bone], min);

	auto center = (min + max) * 0.5f;

	Vector CurrentAngles = g_Math.CalcAngle(center, local->GetEyePosition());

	Vector Forward;
	g_Math.AngleVectors(CurrentAngles, &Forward);

	Vector Right = Forward.Cross(Vector(0, 0, 1));
	Vector Left = Vector(-Right.x, -Right.y, Right.z);

	Vector Top = Vector(0, 0, 1);
	Vector Bot = Vector(0, 0, -1);

	switch (iHitbox) {
	case HITBOX_HEAD:
		for (auto i = 0; i < 4; ++i)
			vecArray.emplace_back(center);

		vecArray[1] += Top * (hitbox->radius * g_Menu.Config.HeadScale / 100.f);
		vecArray[2] += Right * (hitbox->radius * g_Menu.Config.HeadScale / 100.f);
		vecArray[3] += Left * (hitbox->radius * g_Menu.Config.HeadScale / 100.f);
		break;

	default:

		for (auto i = 0; i < 4; ++i)
			vecArray.emplace_back(center);

		vecArray[1] += Top * (hitbox->radius * g_Menu.Config.BodyScale / 100.f);
		vecArray[2] += Right * (hitbox->radius * g_Menu.Config.BodyScale / 100.f);
		vecArray[3] += Left * (hitbox->radius * g_Menu.Config.BodyScale / 100.f);
		break;
	}
	return vecArray;
}


bool Aimbot::BaimIfLethal(C_BaseEntity* player)
{
	if (!g_Menu.Config.BaimLethal)
		return false;

	if (player->GetHealth() <= g_Menu.Config.BaimLethalSlider)
		return true;
	else
		return false;
}

static int stfu;

static bool Zeus;

Vector Aimbot::Hitscan(C_BaseEntity * pEnt) // supremeemmemememememe
{
	static int bestHitbox, mostDamage;
	Vector vector_best_point = Vector(0, 0, 0);
	//static int maxDamage;

	Baim[pEnt->EntIndex()] = ShouldBaim(pEnt);

	int maxDamage = g_Menu.Config.Mindmg;

	if (GetAsyncKeyState(g_Menu.Config.BaimKey)) 
		Baim[pEnt->EntIndex()] = true;	

	if (g::pLocalEntity->zeus())
		Zeus = true;
	else
		Zeus = false;

	if (g_Menu.Config.BaimAfterEnable)
		if (shotsMissed >= g_Menu.Config.BaimSlider)
			Baim[pEnt->EntIndex()] = true;

	if (BaimIfLethal(pEnt) && g_Menu.Config.BaimLethal)
		Baim[pEnt->EntIndex()] = true;

	if (g_Menu.Config.forcebaim[pEnt->EntIndex()])
		Baim[pEnt->EntIndex()] = true;

	std::vector<int> Scan;

	if (!Baim[pEnt->EntIndex()] && g_Menu.Config.Head && !Zeus && !g_Menu.Config.forcebaim[pEnt->EntIndex()] && !GetAsyncKeyState(g_Menu.Config.BaimKey)) {
		Scan.push_back(HITBOX_HEAD);
		g_Menu.Config.espbaim[pEnt->EntIndex()] = false;
	}
	else {
		g_Menu.Config.espbaim[pEnt->EntIndex()] = true;
	}

	if (g_Menu.Config.Chest) {
		Scan.push_back(HITBOX_THORAX);
		Scan.push_back(HITBOX_LOWER_CHEST);
		Scan.push_back(HITBOX_UPPER_CHEST);
	}
	if (g_Menu.Config.Pelvis) {
		Scan.push_back(HITBOX_PELVIS);
		Scan.push_back(HITBOX_BELLY);
	}
	if (g_Menu.Config.Legs && !Zeus) {
		Scan.push_back(HITBOX_LEFT_FOOT);
		Scan.push_back(HITBOX_RIGHT_FOOT);
		Scan.push_back(HITBOX_LEFT_THIGH);
		Scan.push_back(HITBOX_RIGHT_THIGH);
		Scan.push_back(HITBOX_RIGHT_CALF);
		Scan.push_back(HITBOX_LEFT_CALF);
	}
	if (g_Menu.Config.Arms && !Zeus) {
		Scan.push_back(HITBOX_LEFT_UPPER_ARM);
		Scan.push_back(HITBOX_RIGHT_UPPER_ARM);
	}

	matrix3x4_t matrix[128];
	if (!pEnt->SetupBones(matrix, 128, 256, 0)) return Vector(0, 0, 0);

	for (int hitbox : Scan)
	{
		for (auto point : GetMultiplePointsForHitbox(g::pLocalEntity, pEnt, hitbox, matrix))
		{
			int damage = g_Autowall.Damage(point);
			if (damage > maxDamage)
			{
				bestHitbox = hitbox;
				maxDamage = damage;
				vector_best_point = point;

				if (maxDamage >= pEnt->GetHealth())
					return vector_best_point;
			}
		}
	}

	return vector_best_point;
}

float GetMaxDDDelta(C_AnimState* animstate)
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

static bool knife;

void Aimbot::OnCreateMove()
{
	if (!g_pEngine->IsInGame())
		return;

	Vector Aimpoint;
	C_BaseEntity* Target = nullptr;

	int targetID = 0;
	int tempDmg = 0;
	static bool shot = false;

	for (int i = 1; i <= g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant())
		{
			g_LagComp.ClearRecords(i);
			continue;
		}


		g_LagComp.StoreRecord(pPlayerEntity);

		if (pPlayerEntity == g::pLocalEntity || pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		g::EnemyEyeAngs[i] = pPlayerEntity->GetEyeAngles();

		if (g_LagComp.PlayerRecord[i].size() == 0 || !g::pLocalEntity->IsAlive() || !g_Menu.Config.Aimbot || g_Menu.Config.LegitBacktrack)
			continue;

		if (!g::pLocalEntity->GetActiveWeapon())
			continue;

		bestEntDmg = 0;

		Vector Hitbox = Hitscan(pPlayerEntity);
		if (Hitbox == Vector(0, 0, 0)) continue;

		aimspotx[pPlayerEntity->EntIndex()] = Hitbox.x;
		aimspoty[pPlayerEntity->EntIndex()] = Hitbox.y;
		aimspotz[pPlayerEntity->EntIndex()] = Hitbox.z;

		if (Hitbox != Vector(0, 0, 0) && tempDmg <= bestEntDmg)
		{
			locking = true;
		}
		else {
			locking = false;
		}

		if (Hitbox != Vector(0, 0, 0) && tempDmg <= bestEntDmg)
		{
			Aimpoint = Hitbox;
			Target = pPlayerEntity;
			targetID = Target->EntIndex();
			tempDmg = bestEntDmg;
		}
	}

	if (!g::pLocalEntity->IsAlive())
	{
		shot = false;
		return;
	}

	if (!g::pLocalEntity->GetActiveWeapon() || g::pLocalEntity->IsKnifeorNade())
	{
		shot = false;
		return;
	}

	auto local_player = g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	auto desync = GetMaxDDDelta(local_player->AnimState());

	if (shot)
	{
		if (g_Menu.Config.OnShotAA) // ik it dosnt realy fix much just makes ur pitch go down faster
		{
		//	g::bSendPacket = true;
			g_AntiAim.OnCreateMove();
		}
		if (g_Menu.Config.OnShotAA) {
			g::pCmd->viewangles.x = 180 - g::pCmd->viewangles.x;
			g::pCmd->viewangles.y = g::pCmd->viewangles.y + 180;
		}
		shot = false;
	}


	float flServerTime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
	bool canShoot = (g::pLocalEntity->GetActiveWeapon()->GetNextPrimaryAttack() <= flServerTime);

	if (Target)
	{
		g::TargetIndex = targetID;

		float SimulationTime = 0.f;

		if (Backtrack[targetID])
			SimulationTime = g_LagComp.PlayerRecord[targetID].at(0).SimTime;
		else
			SimulationTime = g_LagComp.PlayerRecord[targetID].at(g_LagComp.PlayerRecord[targetID].size() - 1).SimTime;

		if (ShotBacktrack[targetID])
			SimulationTime = g_LagComp.PlayerRecord[targetID].at(g_LagComp.ShotTick[targetID]).SimTime;

		Vector Angle = g_Math.CalcAngle(g::pLocalEntity->GetEyePosition(), Aimpoint);

		if (g::pLocalEntity->GetVelocity().Length() >= (g::pLocalEntity->GetActiveWeapon()->GetCSWpnData()->max_speed_alt * .34f) - 5 && !GetAsyncKeyState(VK_SPACE))
			Autostop();
		
		if (g_Menu.Config.Autoscope)
			autoscope();

		if (!g::pLocalEntity->zeus()) {
			if (!(g::pCmd->buttons & IN_ATTACK) && canShoot && HitChance(Target, g::pLocalEntity->GetActiveWeapon(), Angle, Aimpoint, g_Menu.Config.HitchanceValue))
			{
				if (!Backtrack[targetID] && !ShotBacktrack[targetID])
					g::Shot[targetID] = true;

				if (!g::bSendPacket) {
					if (g_Menu.Config.OnShotAA) {
						g::pCmd->viewangles.y -= (60 + desync);
					}
				}

				shot = true;
				g::pCmd->viewangles = Angle - (g::pLocalEntity->GetAimPunchAngle() * g_pCvar->FindVar("weapon_recoil_scale")->GetFloat());
				g_Aimbot.fired = true;
				g::pCmd->buttons |= IN_ATTACK;
				g::pCmd->tick_count = TIME_TO_TICKS(SimulationTime + g_LagComp.LerpTime());
				g::shots++;
			}
			else {
				g_Aimbot.fired = false;
			}
		}
		else {
			C_BaseCombatWeapon* local_weapon = g::pLocalEntity->GetActiveWeapon();
			float rad = local_weapon->GetCSWpnData()->range; 
			float disttoent = g::pLocalEntity->GetOrigin().DistTo(Target->GetOrigin());
			if (!(g::pCmd->buttons & IN_ATTACK) && canShoot && HitChance(Target, g::pLocalEntity->GetActiveWeapon(), Angle, Aimpoint, 100) && disttoent < rad)  
			{

				if (!Backtrack[targetID] && !ShotBacktrack[targetID])
					g::Shot[targetID] = true;

				g::bSendPacket = true;
				shot = true;

				g::pCmd->viewangles = Angle;
				g::pCmd->buttons |= IN_ATTACK;
				g::pCmd->tick_count = TIME_TO_TICKS(SimulationTime + g_LagComp.LerpTime());
			}
		}
	}
}