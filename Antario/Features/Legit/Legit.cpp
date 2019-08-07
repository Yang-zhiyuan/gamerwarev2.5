#define _USE_MATH_DEFINES
#include "Legit.h"
#include "..\AntiAim\AntiAim.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\Menu\Menu.h"
#include "../../SDK/IGameEvent.h"
#include "../../SDK/IEngineTrace.h"

c_aimbot g_legitbot;

Vector calculateAngleBetween(const Vector& source, const Vector& destination)
{
	Vector delta = source - destination;
//	delta /= g_Menu.Config.LegitSmooth / 100.f;
	float hyp = delta.Length2D();
	Vector angles;
	angles.x = atanf(delta.z / hyp) * 180 / M_PI;
	angles.y = atanf(delta.y / delta.x) * 180 / M_PI;
	angles.z = 0.0f;

	if (delta.x >= 0.0)
		angles.y += 180.0f;

	angles.Normalize();
	return angles;
}

static float getFov(const Vector& viewAngle, const Vector& aimAngle) noexcept
{
	Vector delta = aimAngle - viewAngle;
	delta.Normalize();
	return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}

static int findTarget(CUserCmd * cmd) 
{
	auto bestFov = g_Menu.Config.LegitFOV;
	auto bestTarget = 0;
	auto localPlayer = g::pLocalEntity;

	for (int i = 1; i <= g_pEngine->GetMaxClients(); i++) {
		auto entity = g_pEntityList->GetClientEntity(i);
		if (!entity || entity == localPlayer || entity->IsDormant() || !entity->IsAlive() || !entity->IsEnemy() || entity->IsImmune())
			continue;
		auto angle = calculateAngleBetween(localPlayer->GetEyePosition(), entity->getBonePosition(8));
		auto fov = getFov(cmd->viewangles, angle);
		if (fov < bestFov) {
			bestFov = fov;
			bestTarget = i;
		}
	}
	return bestTarget;
}

Vector TempVec1 = Vector(0, 0, 0);
Vector TempVec2 = Vector(0, 0, 0);
Vector TempVec3 = Vector(0, 0, 0);
float SmoothCurLen = 0;
float SmoothCurAngle = 0;
float SmoothProgress = 0;
int SmoothMF = 0;
bool SetSmoothStart = false;
Vector SmoothMaxDelta = Vector(0, 0, 0);
float StartLen = 0;
int SmoothMethod = 1;

float GetCurLen(Vector viewangles, Vector target)
{
	float SmeshX = target.y < 0 && viewangles.y > 0 ? (viewangles.y - target.y * -1.f) : (target.y - viewangles.y);
	float SmeshY = target.x < 0 && viewangles.x > 0 ? (viewangles.x - target.x * -1.f) : (target.x - viewangles.x);

	return sqrtf(powf(SmeshX, 2.f) + powf(SmeshY, 2.f));
}

float GetCurAngle(float Len, Vector viewangles, Vector target)
{

	float SmeshX = (target.y - viewangles.y) * -1.f;
	float SmeshY = (target.x - viewangles.x) * -1.f;
	return atanf(SmeshY / SmeshX) * PI;
}

Vector GetMaxDelta(float Len, float Angl, Vector viewangles, Vector target)
{
	return Vector(
		target.x + ((Len * sin(Angl * PI / 180.f)) * (viewangles.y < target.y ? -1.f : 1.f)),
		target.y + ((Len * cos(Angl * PI / 180.f)) * (viewangles.y < target.y ? -1.f : 1.f)),
		0);
}

Vector Smooth(Vector& viewangles, Vector target, float factor, bool silent) // obnoxious framework or smth
{
	factor /= 100.f;
	Vector delta = target - viewangles;
	int CCSmooth = g_Menu.Config.LegitSmooth;

	if (CCSmooth && factor > 0)
	{
		Vector OldViewAng = viewangles;
		Vector OldTargAng = target;

		TempVec3 = target;

		if (target.x >= 180)
			target.x = (180.f - (180.f - (360.f - target.x))) * -1.f;

		if (target.y >= 180)
			target.y = (180.f - (180.f - (360.f - target.y))) * -1.f;

		TempVec1 = viewangles;
		TempVec2 = target;

		SmoothCurLen = GetCurLen(viewangles, target);

		if (!SetSmoothStart)
		{
			StartLen = g_Menu.Config.LegitFOV / 3.f;
			SetSmoothStart = true;
		}

		SmoothCurAngle = GetCurAngle(SmoothCurLen, viewangles, target);
		SmoothMaxDelta = GetMaxDelta(StartLen, SmoothCurAngle, viewangles, target);

		SmoothProgress = (SmoothCurLen / (StartLen / 100.f));

		viewangles = OldViewAng;
		target = OldTargAng;
	}

	g_Math.ClampAngles(delta);

		Vector toChange = Vector(0, 0, 0);
		if (silent)
		{
			g_Math.ClampAngles(target);
			return target;
		}

		if (SmoothMF == 0)
		{
			float smooth = powf(factor, 0.27f);

			smooth = min(0.9999999f, smooth);

			toChange = delta - delta * smooth;
		}
		else if (SmoothMF == 1)
		{
			if (factor <= 0)
			{
				g_Math.ClampAngles(target);
				return target;
			}

			float smooth = powf(factor, 0.4f);
			smooth = min(0.9999999f, smooth);

			float coeff = fabsf(smooth - 1.f) / delta.Length() * 4.f;
			coeff = min(1.f, coeff);
			toChange = (delta * coeff);
		}

		Vector End = viewangles + toChange;

		g_Math.ClampAngles(End);

		return End;
	

	return viewangles;
}

void c_aimbot::run(CUserCmd * cmd)
{
	if (g_Menu.Config.LegitAimbot && cmd->buttons & IN_ATTACK) {
		if (auto target = findTarget(cmd)) {
			auto entity = g_pEntityList->GetClientEntity(target);

			auto localPlayer = g::pLocalEntity;

			auto angle = calculateAngleBetween(localPlayer->GetEyePosition(), entity->getBonePosition(8));

			if (g_Menu.Config.LegitSmooth > 0)
			{
				g_pEngine->SetViewAngles(Smooth(g::pCmd->viewangles, angle, g_Menu.Config.LegitSmooth, false));
			}
			else {
				g_pEngine->SetViewAngles(angle);
			}
		}
	}
}
