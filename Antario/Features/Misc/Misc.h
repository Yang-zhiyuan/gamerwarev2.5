#pragma once

#include "..\Aimbot\Autowall.h"
#include "..\Aimbot\Aimbot.h"
#include "..\Aimbot\LagComp.h"
#include "..\..\Utils\GlobalVars.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\SDK\CPrediction.h"
#include "..\..\Menu\Menu.h"
#include <iostream>
#include <algorithm>
#include "../../SDK/texture_group_names.h"
#include "../../SDK/IClientMode.h"
#include "../../SDK/IMaterial.h"
#include "../../SDK/IBaseClientDll.h"
#include <unordered_map>

// kinda just throw shit in here

#define _SOLVEY(a, b, c, d, e, f) ((c * b - d * a) / (c * f - d * e))
#define SOLVEY(...) _SOLVEY(?, ?, ?, ?, ?, ?)
#define SOLVEX(y, world, forward, right) ((world.x - right.x * y) / forward.x)


class Misc
{
public:
	void OnCreateMove()
	{
		this->pCmd = g::pCmd;
		this->pLocal = g::pLocalEntity;

		this->DoAutostrafe();
		this->DoBhop();
		if (!g_pEngine->IsVoiceRecording()) {
			this->DoFakeLag();
		}
		this->AutoRevolver();
	};

	void AutoRevolver() // xy0
	{
		auto me = g::pLocalEntity;
		auto cmd = g::pCmd;
		auto weapon = me->GetActiveWeapon();

		if (!g_Menu.Config.Aimbot)
			return;

		if (!me || !me->IsAlive() || !weapon)
			return;

		if (weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER)
		{
			static int delay = 0; 
			delay++;

			if (delay <= 15)
				g::pCmd->buttons |= IN_ATTACK;
			else {
				delay = 0;
				g::revolvertime = g_pGlobalVars->curtime;

			}
		}
	}

	void MovementFix(Vector& oldang)
	{
		Vector vMovements(g::pCmd->forwardmove, g::pCmd->sidemove, 0.f);

		if (vMovements.Length2D() == 0)
			return;

		Vector vRealF, vRealR;
		Vector aRealDir = g::pCmd->viewangles;
		aRealDir.Clamp();

		g_Math.AngleVectors(aRealDir, &vRealF, &vRealR, nullptr);
		vRealF[2] = 0;
		vRealR[2] = 0;

		VectorNormalize(vRealF);
		VectorNormalize(vRealR);

		Vector aWishDir = oldang;
		aWishDir.Clamp();

		Vector vWishF, vWishR;
		g_Math.AngleVectors(aWishDir, &vWishF, &vWishR, nullptr);

		vWishF[2] = 0;
		vWishR[2] = 0;

		VectorNormalize(vWishF);
		VectorNormalize(vWishR);

		Vector vWishVel;
		vWishVel[0] = vWishF[0] * g::pCmd->forwardmove + vWishR[0] * g::pCmd->sidemove;
		vWishVel[1] = vWishF[1] * g::pCmd->forwardmove + vWishR[1] * g::pCmd->sidemove;
		vWishVel[2] = 0;

		float a = vRealF[0], b = vRealR[0], c = vRealF[1], d = vRealR[1];
		float v = vWishVel[0], w = vWishVel[1];

		float flDivide = (a * d - b * c);
		float x = (d * v - b * w) / flDivide;
		float y = (a * w - c * v) / flDivide;

		g::pCmd->forwardmove = x;
		g::pCmd->sidemove = y;
	}

	void BootlegNightMode()
	{
		if (g_Menu.Config.PostProcess)
		{
			ConVar* mat_postprocess_enable = g_pCvar->FindVar("mat_postprocess_enable");
			*(int*)((DWORD)& mat_postprocess_enable->fnChangeCallback + 0xC) = 0; // ew
			mat_postprocess_enable->SetValue(0);
		}
		else
		{
			ConVar* mat_postprocess_enable = g_pCvar->FindVar("mat_postprocess_enable");
			*(int*)((DWORD)& mat_postprocess_enable->fnChangeCallback + 0xC) = 0; // ew
			mat_postprocess_enable->SetValue(1);

		}
		float ratio = (g_Menu.Config.AspectRatioSlider * 0.1) / 2;
		static auto bruh = g_pCvar->FindVar("r_aspectratio");
		if (g_Menu.Config.AspectRatio) {
			if (ratio > 0.001) {
				g_pCvar->FindVar("r_aspectratio")->SetValue(ratio);
			}
			else {
				g_pCvar->FindVar("r_aspectratio")->SetValue((35 * 0.1f) / 2);
			}
		}
		else {
			g_pCvar->FindVar("r_aspectratio")->SetValue((35 * 0.1f) / 2);
		}
	}

	void ShowImpacts()
	{
		if (g_Menu.Config.Impacts)
		{
			ConVar* sv_showimpacts = g_pCvar->FindVar("sv_showimpacts");
			*(int*)((DWORD)& sv_showimpacts->fnChangeCallback + 0xC) = 0; // ew
			sv_showimpacts->SetValue(1);

		}
		else {
			ConVar* sv_showimpacts = g_pCvar->FindVar("sv_showimpacts");
			*(int*)((DWORD)& sv_showimpacts->fnChangeCallback + 0xC) = 0; // ew
			sv_showimpacts->SetValue(0);
		}

	}


	IMaterial* sMaterial = nullptr;

	std::vector<const char*> SmokeMaterials =
	{
			"particle/vistasmokev1/vistasmokev1_fire",
			"particle/vistasmokev1/vistasmokev1_smokegrenade",
			"particle/vistasmokev1/vistasmokev1_emods",
			"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};

	void DoNoSmoke(ClientFrameStage_t curStage)
	{
		static auto linegoesthrusmoke = Utils::FindPattern("client_panorama.dll", (BYTE*)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");

		for (auto mat : SmokeMaterials)
		{
			sMaterial = g_pMaterialSys->FindMaterial(mat, TEXTURE_GROUP_OTHER);



			if (g_Menu.Config.NoSmoke)
			{
				sMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
				*(int*)(smokecout) = 0;
			}
			else
			{
				sMaterial->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);


			}
		}
	}


	void clan_tag() { // pasted from subliminal br0 ez
		auto apply = [](const char* name) -> void {
			using Fn = int(__fastcall*)(const char*, const char*);
			static auto fn = reinterpret_cast<Fn>(Utils::FindSignature("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
			fn(name, name);
		};

		static std::string tag = "      gamerware      ";

		static float last_time = 0;

		if (g_pGlobalVars->curtime > last_time) {
			tag += tag.at(0);
			tag.erase(0, 1);
			std::string dollarsign = u8"$   ";
			dollarsign += tag;
			apply(dollarsign.c_str());

			last_time = g_pGlobalVars->curtime + 0.5f;
		}

		if (fabs(last_time - g_pGlobalVars->curtime) > 1.f)
			last_time = g_pGlobalVars->curtime;

	}

	bool isthirdperson;

	void ThirdPerson(ClientFrameStage_t curStage)
	{
		if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected() || !g::pLocalEntity || !g_Menu.Config.Thirdperson)
			return;

		static bool init = false;

		if (GetKeyState(g_Menu.Config.ThirdpersonKey) && g::pLocalEntity->IsAlive())
		{
			if (init)
			{
				ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
				*(int*)((DWORD)& sv_cheats->fnChangeCallback + 0xC) = 0; // ew
				sv_cheats->SetValue(1);
				g_pEngine->ExecuteClientCmd("thirdperson");
				isthirdperson = true;
			}
			init = false;
		}
		else
		{
			if (!init)
			{
				ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
				*(int*)((DWORD)& sv_cheats->fnChangeCallback + 0xC) = 0; // ew
				sv_cheats->SetValue(1);
				g_pEngine->ExecuteClientCmd("firstperson");
				isthirdperson = false;
			}
			init = true;
		}

		if (curStage == FRAME_RENDER_START && GetKeyState(g_Menu.Config.ThirdpersonKey) && g::pLocalEntity->IsAlive())
		{
			g_pPrediction->SetLocalViewAngles(Vector(g::RealAngle.x, g::RealAngle.y, 0)); // lol
		}
	}
	void Crosshair()
	{
		if (!g::pLocalEntity)
			return;

		if (!g::pLocalEntity->IsAlive())
			return;

		if (!g_pEngine->IsInGame() && g_pEngine->IsConnected())
			return;

		if (g::pLocalEntity->IsScoped() && g_Menu.Config.NoScope && g::pLocalEntity->IsAlive())
		{
			int Height, Width;
			g_pEngine->GetScreenSize(Width, Height);

			Vector punchAngle = g::pLocalEntity->GetAimPunchAngle();

			float x = Width / 2;
			float y = Height / 2;
			int dy = Height / 90;
			int dx = Width / 90;
			x -= (dx * (punchAngle.y));
			y += (dy * (punchAngle.x));

			Vector2D screenPunch = { x, y };

			g_pSurface->Line(0, screenPunch.y, Width, screenPunch.y, Color(0, 0, 0, 255));
			g_pSurface->Line(screenPunch.x, 0, screenPunch.x, Height, Color(0, 0, 0, 255));
		}

		static bool init = false;
		static bool init2 = false;

		if (g_Menu.Config.Crosshair[0])
		{
			if (g::pLocalEntity->IsScoped())
			{
				if (init2)
				{
					ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
					*(int*)((DWORD)& sv_cheats->fnChangeCallback + 0xC) = 0; // ew
					sv_cheats->SetValue(1);

					g_pEngine->ExecuteClientCmd("weapon_debug_spread_show 0");
					g_pEngine->ExecuteClientCmd("crosshair 0");
				}
				init2 = false;
			}
			else
			{
				if (!init2)
				{
					ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
					*(int*)((DWORD)& sv_cheats->fnChangeCallback + 0xC) = 0; // ew
					sv_cheats->SetValue(1);

					g_pEngine->ExecuteClientCmd("weapon_debug_spread_show 3");
					g_pEngine->ExecuteClientCmd("crosshair 1");
				}
				init2 = true;
			}

			init = false;
		}
		else
		{
			if (!init)
			{
				ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
				*(int*)((DWORD)& sv_cheats->fnChangeCallback + 0xC) = 0; // ew
				sv_cheats->SetValue(1);

				g_pEngine->ExecuteClientCmd("weapon_debug_spread_show 0");
				g_pEngine->ExecuteClientCmd("crosshair 1");
			}
			init = true;
		}
	}

	bool fakelag;

	void NormalWalk() // heh
	{
		g::pCmd->buttons &= ~IN_MOVERIGHT;
		g::pCmd->buttons &= ~IN_MOVELEFT;
		g::pCmd->buttons &= ~IN_FORWARD;
		g::pCmd->buttons &= ~IN_BACK;

		if (g::pCmd->forwardmove > 0.f)
			g::pCmd->buttons |= IN_FORWARD;
		else if (g::pCmd->forwardmove < 0.f)
			g::pCmd->buttons |= IN_BACK;
		if (g::pCmd->sidemove > 0.f)
		{
			g::pCmd->buttons |= IN_MOVERIGHT;
		}
		else if (g::pCmd->sidemove < 0.f)
		{
			g::pCmd->buttons |= IN_MOVELEFT;
		}
	}

private:
	CUserCmd* pCmd;
	C_BaseEntity* pLocal;

	void DoBhop() const
	{
		if (!g_Menu.Config.Bhop)
			return;

		if (!g::pLocalEntity->IsAlive())
			return;

		static bool bLastJumped = false;
		static bool bShouldFake = false;

		if (!bLastJumped && bShouldFake)
		{
			bShouldFake = false;
			pCmd->buttons |= IN_JUMP;
		}
		else if (pCmd->buttons & IN_JUMP)
		{
			if (pLocal->GetFlags() & FL_ONGROUND)
				bShouldFake = bLastJumped = true;
			else
			{
				pCmd->buttons &= ~IN_JUMP;
				bLastJumped = false;
			}
		}
		else
			bShouldFake = bLastJumped = false;
	}

	template<class T, class U>
	static T clamp(T in, U low, U high) {
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}
	void DoAutostrafe() const
	{
		if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !g_Menu.Config.AutoStrafe)
			return;

		if (!g::pLocalEntity->IsAlive())
			return;

		if (g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_LADDER || g::pLocalEntity->GetMoveType() == MoveType_t::MOVETYPE_NOCLIP)
			return;
		/*						W						A							S						D*/
		if (GetAsyncKeyState(0x57) || GetAsyncKeyState(0x41) || GetAsyncKeyState(0x53) || GetAsyncKeyState(0x44) || GetAsyncKeyState(VK_SHIFT))
			return;

		if (!(g::pLocalEntity->GetFlags() & FL_ONGROUND)) {
			if (g::pCmd->mousedx > 1 || g::pCmd->mousedx < -1) {
				g::pCmd->sidemove = clamp(g::pCmd->mousedx < 0.f ? -450.0f : 450.0f, -450.0f, 450.0f);
			}
			else {
				g::pCmd->forwardmove = 10000.f / g::pLocalEntity->GetVelocity().Length();
				g::pCmd->sidemove = (g::pCmd->command_number % 2) == 0 ? -450.0f : 450.0f;
				if (g::pCmd->forwardmove > 450.0f)
					g::pCmd->forwardmove = 450.0f;
			}
		}
	}

	void DoFakeLag() const
	{
		if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !g_Menu.Config.FakeLagEnable)
			return;

		if (!g::pLocalEntity)
			return;

		if (!g::pLocalEntity->IsAlive())
			return;

		if (GetAsyncKeyState(0x58) || g::pLocalEntity->IsNade())
			return;

		auto NetChannel = g_pEngine->GetNetChannel();

		if (!NetChannel)
			return;

		if (g::pCmd->buttons & IN_ATTACK && !g_Menu.Config.FakeLagShooting)
			return;

		static float maxSpeed = 320.f; //320
		static float Acceleration = 5.5f;
		float maxAccelspeed = Acceleration * maxSpeed * g_pGlobalVars->intervalPerTick;

		float TicksToStop = g::pLocalEntity->GetVelocity().Length() / maxAccelspeed;

		bool canHit = false;
		static bool init = false;
		static bool stop = true;
		static bool stop2 = true;
		bool skip = false;

		if (g_Menu.Config.FakelagType == 2)
		{
			for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
			{
				C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

				if (!pPlayerEntity
					|| !pPlayerEntity->IsAlive()
					|| pPlayerEntity->IsDormant()
					|| pPlayerEntity == g::pLocalEntity
					|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
					continue;

				Vector EnemyHead = { pPlayerEntity->GetOrigin().x, pPlayerEntity->GetOrigin().y, (pPlayerEntity->GetHitboxPosition(0, g_Aimbot.Matrix[pPlayerEntity->EntIndex()]).z + 10.f) };

				Vector Head = { g::pLocalEntity->GetOrigin().x, g::pLocalEntity->GetOrigin().y, (g::pLocalEntity->GetHitboxPosition(0, g_Aimbot.Matrix[pPlayerEntity->EntIndex()]).z + 10.f) };
				Vector HeadExtr = (Head + (g::pLocalEntity->GetVelocity() * 0.203125f));
				Vector OriginExtr = ((g::pLocalEntity->GetOrigin() + (g::pLocalEntity->GetVelocity() * 0.21875f)) + Vector(0, 0, 8));

				float dmg;

				if (abs(g::pLocalEntity->GetVelocity().Length2D()) > 50.f && (g_Autowall.CanHitFloatingPoint(HeadExtr, EnemyHead) || g_Autowall.CanHitFloatingPoint(OriginExtr, EnemyHead)))
				{
					canHit = true;
				}
			}

			if (canHit)
			{
				if (stop2)
				{
					init = true;
					stop2 = false;
				}
			}
			else
				stop2 = true;

			if (init)
			{
				if (!stop)
				{
					g::bSendPacket = true;
					g::LagPeek = true;
					stop = true;
					skip = true;
				}
			}

			if (!skip)
			{
				if (g::LagPeek)
				{
					if (NetChannel->m_nChokedPackets < 13)
						g::bSendPacket = false;
					else
						g::LagPeek = false;
				}
				else
				{
					g::bSendPacket = (NetChannel->m_nChokedPackets >= g_Menu.Config.Fakelag);
					stop = false;
					init = false;
				}
			}
		}
		else
		{
			if (g_Menu.Config.FakeDuck && GetAsyncKeyState(g_Menu.Config.FakeduckKey)) {
				g::bSendPacket = (NetChannel->m_nChokedPackets >= 14);
			}
			else {
				if (g_Menu.Config.FakelagType == 0) {
					init = false;
					stop = true;
					stop2 = true;
					g::LagPeek = false;

					g::bSendPacket = (NetChannel->m_nChokedPackets >= g_Menu.Config.Fakelag);
				}
				else if (g_Menu.Config.FakelagType == 1) {
					init = false;
					stop = true;
					stop2 = true;
					g::LagPeek = false;


					int lagamount = rand() % g_Menu.Config.Fakelag + g_Menu.Config.FakelagThreshold;
					if (lagamount > 20)
						lagamount = 20;

					g::bSendPacket = (NetChannel->m_nChokedPackets >= lagamount);
				}
			}
		}

	}
};

extern Misc g_Misc;