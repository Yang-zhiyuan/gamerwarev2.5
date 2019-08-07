#include "ESP.h"
#include "..\Aimbot\Aimbot.h"
#include "..\Aimbot\LagComp.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\SDK\ISurface.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\Utils\Math.h"
#include "..\..\Menu\Menu.h"
#include "..\..\SDK\IVModelInfo.h"
#include "..\..\SDK\IClientRenderable.h"
#include "..\Misc\Misc.h"
#include <locale> 
#include "../../SDK/ClientClass.h"
#include "../../Utils/GlowAutism.h"
#include "../../SDK/CHudChat.h"
#include "../../SDK/i_localize.h"
#include "../../xor.h"
#include "../Features.h"

ESP g_ESP;

void ESP::RenderBox()
{
	g_pSurface->OutlinedRect(Box.left, Box.top, Box.right, Box.bottom, color);
	g_pSurface->OutlinedRect(Box.left + 1, Box.top + 1, Box.right - 2, Box.bottom - 2, Color(0, 0, 0, color.alpha));
	g_pSurface->OutlinedRect(Box.left - 1, Box.top - 1, Box.right + 2, Box.bottom + 2, Color(0, 0, 0, color.alpha));
}

void ESP::RenderName(C_BaseEntity* pEnt, int iterator)
{
    PlayerInfo_t pInfo;
    g_pEngine->GetPlayerInfo(iterator, &pInfo);

	std::string data = pInfo.szName;
	std::transform(data.begin(), data.end(), data.begin(), ::toupper);

	if (g_Menu.Config.Name)
		g_pSurface->DrawT(Box.left + (Box.right / 2), Box.top - 10, textcolor, niggafont, true, data.c_str());
	/*else if (g_Menu.Config.Name == 2)
	{
		g_pSurface->DrawT(Box.left + Box.right + 5, Box.top, textcolor, font, false, pInfo.szName);
		offsetY += 1;
	}*/
}

void ESP::RenderWeaponName(C_BaseEntity* pEnt)
{
    auto weapon = pEnt->GetActiveWeapon();

    if (!weapon)
        return;

    auto strWeaponName = weapon->GetName();

    strWeaponName.erase(0, 7);

	std::string data = strWeaponName;
	std::transform(data.begin(), data.end(), data.begin(), ::toupper);

	if (g_Menu.Config.Weapon)
		g_pSurface->DrawT(Box.left + (Box.right / 2), Box.top + Box.bottom, wepcolor, niggafont, true, data.c_str());
	
}

void ESP::RenderHealth(C_BaseEntity* pEnt)
{
	if (g_Menu.Config.HealthBar)
	{
		g_pSurface->FilledRect(Box.left - 6, Box.top - 1, 4, Box.bottom + 2, Color(0, 0, 0, 150));
		int pixelValue = pEnt->GetHealth() * Box.bottom / 100;

		g_pSurface->FilledRect(Box.left - 5, Box.top + Box.bottom - (box_hpDifference * pixelValue / 100), 2, box_hpDifference * pixelValue / 100, Color(244, 100, 66, 255));

		g_pSurface->FilledRect(Box.left - 5, Box.top + Box.bottom - pixelValue, 2, pixelValue, Color(128, 244, 66, 250));

		int healthvalue = pEnt->GetHealth();
		if (healthvalue < 75)
		{
			std::string Health = std::to_string(pEnt->GetHealth());
			int pixelValue = pEnt->GetHealth() * Box.bottom / 100;
			g_pSurface->DrawT(Box.left - 14, Box.top + Box.bottom - pixelValue - 5, textcolor, niggafont, false, Health.c_str());
			offsetY += 1;
		}
	}

	
}

void ESP::Flags(C_BaseEntity* pEnt)
{
	auto helmet = pEnt->HasHelmet();
	auto armor = pEnt->ArmorValue();
	auto bomb = pEnt->IsKnifeorNade();
	auto real = pEnt->GetEyeAngles().y;
	auto lby = pEnt->GetLowerBodyYaw();
	auto scoped = pEnt->IsScoped();
	int angle = real - lby;
	bool fakeducking = pEnt->IsFakeDucking();

	//	std::string realangle = "realangle = " + std::to_string(std::round(real));

	//g_pSurface->DrawT(Box.left + Box.right + 28, Box.top, Color(59, 119, 181, 255), skeetfont, false, realangle.c_str());

	std::string fags;

	if (helmet) {
		fags = "H";
	}
	if (armor) {
		fags += "K";
	}
	
	if (helmet || armor) {
		g_pSurface->DrawT(Box.left + Box.right + 1, Box.top, Color(59, 119, 181, 255), niggafont, false, fags.c_str());
		if (scoped) {
			g_pSurface->DrawT(Box.left + Box.right + 1, Box.top + 9, Color(59, 119, 181, 255), niggafont, false, "ZOOM");
			if (fakeducking) {
				g_pSurface->DrawT(Box.left + Box.right + 1, Box.top + 18, Color(244, 100, 66, 255), niggafont, false, "FD");
			}
		}
		else {
			if (fakeducking) {
				g_pSurface->DrawT(Box.left + Box.right + 1, Box.top + 9, Color(244, 100, 66, 255), niggafont, false, "FD");
			}
		}
	}
	else {
		if (scoped) {
			g_pSurface->DrawT(Box.left + Box.right + 1, Box.top + 1, Color(59, 119, 181, 255), niggafont, false, "ZOOM");
			if (fakeducking) {
				g_pSurface->DrawT(Box.left + Box.right + 1, Box.top + 9, Color(244, 100, 66, 255), niggafont, false, "FD");
			}
		}
		else {
			if (fakeducking) {
				g_pSurface->DrawT(Box.left + Box.right + 1, Box.top + 1, Color(244, 100, 66, 255), niggafont, false, "FD");
			}
		}
	}
}

Color lockcolor;

void ESP::RenderHitboxPoints(C_BaseEntity* pEnt)
{	
	Vector2D w2sHitbox;
	Utils::WorldToScreen(Vector(g_Aimbot.aimspotx[pEnt->EntIndex()], g_Aimbot.aimspoty[pEnt->EntIndex()], g_Aimbot.aimspotz[pEnt->EntIndex()]), w2sHitbox);
	g_pSurface->OutlinedRect(w2sHitbox.x - 2, w2sHitbox.y - 2, 4, 4, Color(255, 255, 255, 255));
}

void ESP::RenderSkeleton(C_BaseEntity* pEnt) // the best
{
	if (g_LagComp.PlayerRecord[pEnt->EntIndex()].size() == 0)
		return;
	int size = 0;

	if (g_Menu.Config.Skeleton[0])
		size++;
	if (g_Menu.Config.Skeleton[1])
		size++;

	for (int mode = 0; mode < size; mode++)
	{
		Vector Hitbox[19];
		Vector2D Hitboxw2s[19];

		int Record = 0;

		if (mode == 0 && g_Menu.Config.Skeleton[0])
		{
			Record = g_LagComp.PlayerRecord[pEnt->EntIndex()].size() - 1;

			if (g_LagComp.ShotTick[pEnt->EntIndex()] != -1)
				Record = g_LagComp.ShotTick[pEnt->EntIndex()];
		}

		for (int hitbox = 0; hitbox < 19; hitbox++)
		{
			Hitbox[hitbox] = pEnt->GetHitboxPosition(hitbox, g_LagComp.PlayerRecord[pEnt->EntIndex()].at(Record).Matrix);
			Utils::WorldToScreen(Hitbox[hitbox], Hitboxw2s[hitbox]);
		}

		//spine
		g_pSurface->Line(Hitboxw2s[HITBOX_HEAD].x, Hitboxw2s[HITBOX_HEAD].y, Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_UPPER_CHEST].x, Hitboxw2s[HITBOX_UPPER_CHEST].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_UPPER_CHEST].x, Hitboxw2s[HITBOX_UPPER_CHEST].y, Hitboxw2s[HITBOX_LOWER_CHEST].x, Hitboxw2s[HITBOX_LOWER_CHEST].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_LOWER_CHEST].x, Hitboxw2s[HITBOX_LOWER_CHEST].y, Hitboxw2s[HITBOX_THORAX].x, Hitboxw2s[HITBOX_THORAX].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_THORAX].x, Hitboxw2s[HITBOX_THORAX].y, Hitboxw2s[HITBOX_BELLY].x, Hitboxw2s[HITBOX_BELLY].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_BELLY].x, Hitboxw2s[HITBOX_BELLY].y, Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, skelecolor);

		//right leg
		g_pSurface->Line(Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, Hitboxw2s[HITBOX_RIGHT_THIGH].x, Hitboxw2s[HITBOX_RIGHT_THIGH].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_THIGH].x, Hitboxw2s[HITBOX_RIGHT_THIGH].y, Hitboxw2s[HITBOX_RIGHT_CALF].x, Hitboxw2s[HITBOX_RIGHT_CALF].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_CALF].x, Hitboxw2s[HITBOX_RIGHT_CALF].y, Hitboxw2s[HITBOX_RIGHT_FOOT].x, Hitboxw2s[HITBOX_RIGHT_FOOT].y, skelecolor);

		//right arm
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].x, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].x, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].y, Hitboxw2s[HITBOX_RIGHT_FOREARM].x, Hitboxw2s[HITBOX_RIGHT_FOREARM].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_FOREARM].x, Hitboxw2s[HITBOX_RIGHT_FOREARM].y, Hitboxw2s[HITBOX_RIGHT_HAND].x, Hitboxw2s[HITBOX_RIGHT_HAND].y, skelecolor);

		//left leg
		g_pSurface->Line(Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, Hitboxw2s[HITBOX_LEFT_THIGH].x, Hitboxw2s[HITBOX_LEFT_THIGH].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_THIGH].x, Hitboxw2s[HITBOX_LEFT_THIGH].y, Hitboxw2s[HITBOX_LEFT_CALF].x, Hitboxw2s[HITBOX_LEFT_CALF].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_CALF].x, Hitboxw2s[HITBOX_LEFT_CALF].y, Hitboxw2s[HITBOX_LEFT_FOOT].x, Hitboxw2s[HITBOX_LEFT_FOOT].y, skelecolor);

		//left arm
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].x, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_UPPER_ARM].x, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].y, Hitboxw2s[HITBOX_LEFT_FOREARM].x, Hitboxw2s[HITBOX_LEFT_FOREARM].y, skelecolor);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_FOREARM].x, Hitboxw2s[HITBOX_LEFT_FOREARM].y, Hitboxw2s[HITBOX_LEFT_HAND].x, Hitboxw2s[HITBOX_LEFT_HAND].y, skelecolor);

		
	}
}

void ESP::BoundBox(C_BaseEntity* pEnt)
{
	Box.bottom = 0;
	Box.top = 0;
	Box.left = 0;
	Box.right = 0;

	Vector2D w2sBottom, w2sTop;

	Utils::WorldToScreen(pEnt->GetOrigin() - Vector(0, 0, 8), w2sBottom); 
	Utils::WorldToScreen(pEnt->GetHitboxPosition(0, g_Aimbot.Matrix[pEnt->EntIndex()]) + Vector(0, 0, 10), w2sTop);
	
	int Middle = w2sBottom.y - w2sTop.y;
	int Width = Middle / 3.f;

	Box.bottom = Middle;
	Box.top = w2sTop.y;
	Box.left = w2sBottom.x - Width;
	Box.right = Width * 2;
}

float GetMaxDDelta(C_AnimState* animstate)
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

void rotate(std::array< Vector2D, 3 >& points, float rotation)
{
	const auto points_center = (points.at(0) + points.at(1) + points.at(2)) / 3;
	for (auto& point : points) {
		point -= points_center;

		const auto temp_x = point.x;
		const auto temp_y = point.y;

		const auto theta = DEG2RAD(rotation);
		const auto c = cos(theta);
		const auto s = sin(theta);

		point.x = temp_x * c - temp_y * s;
		point.y = temp_x * s + temp_y * c;

		point += points_center;
	}
}

mstudiobbox_t* get_hitbox(C_BaseEntity* entity, int hitbox_index)
{
	if (entity->IsDormant() || entity->GetHealth() <= 0)
		return NULL;

	const auto pModel = entity->GetModel();
	if (!pModel)
		return NULL;

	auto pStudioHdr = g_pModelInfo->GetStudiomodel(pModel);
	if (!pStudioHdr)
		return NULL;

	auto pSet = pStudioHdr->GetHitboxSet(0);
	if (!pSet)
		return NULL;

	if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
		return NULL;

	return pSet->GetHitbox(hitbox_index);
}



Vector GetHitboxPosition(C_BaseEntity* entity, int hitbox_id)
{
	auto hitbox = get_hitbox(entity, hitbox_id);
	if (!hitbox)
		return Vector(0, 0, 0);

	auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

	Vector bbmin, bbmax;
	g_Math.VectorTransform(hitbox->min, bone_matrix, bbmin);
	g_Math.VectorTransform(hitbox->max, bone_matrix, bbmax);

	return (bbmin + bbmax) * 0.5f;
}

bool WorldToScreen(const Vector& world, Vector& screen) {
	auto screen_transform = [&world, &screen]() -> bool {
		static auto& matrix = g_pEngine->WorldToScreenMatrix();

		screen.x = matrix[0][0] * world.x + matrix[0][1] * world.y + matrix[0][2] * world.z + matrix[0][3];
		screen.y = matrix[1][0] * world.x + matrix[1][1] * world.y + matrix[1][2] * world.z + matrix[1][3];
		screen.z = 0.0f;

		float w = matrix[3][0] * world.x + matrix[3][1] * world.y + matrix[3][2] * world.z + matrix[3][3];

		if (w < 0.001f) {
			screen.x *= 100000;
			screen.y *= 100000;
			return false;
		}

		screen.x /= w;
		screen.y /= w;

		return true;
	};

	if (screen_transform()) {
		int w, h;
		g_pEngine->GetScreenSize(w, h);

		screen.x = (w / 2.0f) + (screen.x * w) / 2.0f;
		screen.y = (h / 2.0f) - (screen.y * h) / 2.0f;

		return true;
	}

	return false;
}

int arrow_alpha[65];
void arrows(C_BaseEntity* player) {
	auto local = g::pLocalEntity;

	if (!local)
		return;

	if (!local->IsAlive())
		return;

	if (!player)
		return;

	if (player->IsDormant())
		return;

	arrow_alpha[player->EntIndex()] = 255;
	Vector position;
	if (WorldToScreen(GetHitboxPosition(player, 2), position))
		return;

	Vector viewangles;
	int width, height;

	g_pEngine->GetViewAngles(viewangles);
	g_pEngine->GetScreenSize(width, height);

	const auto screen_center = Vector2D(width * .5f, height * .5f);

	const auto angle_yaw_rad = DEG2RAD(viewangles.y - g_Math.CalcAngle(local->GetEyePosition(), GetHitboxPosition(player, 2)).y - 90);

	float radius = (50 + g_Menu.Config.OutOfViewRadius) / 3.5f;
	int size = (2 + g_Menu.Config.OutOfViewSize);

	const auto new_point_x = screen_center.x + ((((width - (size * 3)) * .5f) * (radius / 100.0f)) * cos(angle_yaw_rad)) + (int)(6.0f * (((float)size - 4.f) / 16.0f));
	const auto new_point_y = screen_center.y + ((((height - (size * 3)) * .5f) * (radius / 100.0f)) * sin(angle_yaw_rad));

	static uint32_t cnt = 0;
	float freq = .015f;

	if (cnt++ >= (uint32_t)-1) cnt = 0;
	float nigga = g_Menu.Config.OutOfViewColor.a - 95;
	float r = std::sin(freq * cnt ) * 127 + nigga;

	if (r > 255)
		r = 255;

	if (r <= 0)
		r = 0;

	std::array< Vector2D, 3 >points{
		Vector2D(new_point_x - size, new_point_y - size),
		Vector2D(new_point_x + size, new_point_y),
		Vector2D(new_point_x - size, new_point_y + size)
	};

	rotate(points, viewangles.y - g_Math.CalcAngle(local->GetEyePosition(), GetHitboxPosition(player, 2)).y - 90);
	g_pSurface->draw_filled_triangle(points, Color(g_Menu.Config.OutOfViewColor.r, g_Menu.Config.OutOfViewColor.g, g_Menu.Config.OutOfViewColor.b, r));
}

int fps1, cFps1;
clock_t lastClock1 = 0;

void DrawLine(float flAngle, float range, int x, int y, int LineLength, Color col)
{
	float nigga = flAngle;
	nigga += 45.f;//cant remember wht this is here for tbh (this doesnt mess it up though)

	float flYaw = (nigga) * (PI / 180.0);

	float viewcosyawzzzzzzz = cos(flYaw);
	float viewsinyawzzzzzzz = sin(flYaw);

	float x2 = range * (-viewcosyawzzzzzzz) + range * viewsinyawzzzzzzz;
	float y2 = range * (-viewcosyawzzzzzzz) - range * viewsinyawzzzzzzz;

	int posonscreenX = x + int(x2 / range * (LineLength));
	int posonscreenY = y + int(y2 / range * (LineLength));

	g_pSurface->Line(x, y, posonscreenX, posonscreenY, col);
	g_pSurface->FilledCircle(Vector2D(posonscreenX, posonscreenY), 8, 4, col);
}

float aClampYaw(float yaw)
{
	while (yaw > 180.f)
		yaw -= 360.f;
	while (yaw < -180.f)
		yaw += 360.f;
	return yaw;
}

void ESP::Arrows()
{
	auto local = g::pLocalEntity;

	if (!local)
		return; 

	if (!local->IsAlive())
		return;	 

	if (g::pLocalEntity->IsAlive() && g_pEngine->IsInGame() && g_pEngine->IsConnected()) {
		const auto origin = g::pLocalEntity->GetOrigin();

		auto local_player = g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		int desync = GetMaxDDelta(local_player->AnimState());

		const auto yaw = g::pCmd->viewangles.y;

		const auto realangle = g::RealAngle.y;

		const auto fakeangle = g::FakeAngle.y;

		static HWND csgo = FindWindowA("Valve001", NULL);
		RECT screen;
		GetWindowRect(csgo, &screen);
		int screenx = screen.right - screen.left;
		int screeny = screen.bottom - screen.top;

		float feet = aClampYaw(g::pLocalEntity->AnimState()->m_flGoalFeetYaw);

		float delta = aClampYaw(abs(feet - fakeangle)); // real way of doing it btw (no lie)

	//	if (g_Aimbot.baiming)
	//		g_pSurface->DrawT(10, screeny - 70, Color(255, 0, 0, 255), indicatorfont, false, xorstr("BAIM").crypt_get());
		if (g_Menu.Config.AAInvertIndicators)
		g_AntiAim.invert ? g_pSurface->DrawT(10, screeny - 70, Color(0, 255, 0, 255), indicatorfont, false, xorstr("LEFT").crypt_get()) : g_pSurface->DrawT(10, screeny - 70, Color(0, 255, 0, 255), indicatorfont, false, xorstr("RIGHT").crypt_get());
		
		if (g_Menu.Config.Antiaim && g::pLocalEntity->IsAlive() && g_Menu.Config.DesyncType != 6)
		{
			float per = delta;
			int rcolor = per < 50 ? 255 : floorf(255 - (per * 2 - 100) * 255.f / 100.f);
			int gcolor = per > 50 ? 255 : floorf((per * 2) * 255.f / 100.f);
			g_pSurface->DrawT(10, screeny - 100, Color(rcolor, gcolor, 0, 255), indicatorfont, false, xorstr("FAKE").crypt_get());
			if (g_Menu.Config.LBYDelta > 0)
				g_pSurface->DrawT(10, screeny - 130, abs(g::RealAngle.y - g::pLocalEntity->GetLowerBodyYaw()) > (g_Menu.Config.Jitter + g_Menu.Config.LBYDelta - 10) ? Color(0, 255, 0, 255) : Color(255, 0, 0, 255), indicatorfont, false, xorstr("LBY").crypt_get());
		}
		if (g_Menu.Config.Antiaim && g::pLocalEntity->IsAlive() && g_Menu.Config.DesyncType == 6)
		{	
			if (g_Menu.Config.LBYDelta > 0)
				g_pSurface->DrawT(10, screeny - 100, abs(g::RealAngle.y - g::pLocalEntity->GetLowerBodyYaw()) > (g_Menu.Config.Jitter + g_Menu.Config.LBYDelta - 10) ? Color(0, 255, 0, 255) : Color(255, 0, 0, 255), indicatorfont, false, xorstr("LBY").crypt_get());
		}
		if (g_Menu.Config.LegitAA && g::pLocalEntity->IsAlive())
		{			
			float per = delta;
			int rcolor = per < 50 ? 255 : floorf(255 - (per * 2 - 100) * 255.f / 100.f);
			int gcolor = per > 50 ? 255 : floorf((per * 2) * 255.f / 100.f);

			g_pSurface->DrawT(10, screeny - 100, Color(rcolor, gcolor, 0, 255), indicatorfont, false, xorstr("FAKE").crypt_get());
			if (g_Menu.Config.LBYDeltaLegit > 0)
				g_pSurface->DrawT(10, screeny - 130, abs(g::RealAngle.y - g::pLocalEntity->GetLowerBodyYaw()) > (g_Menu.Config.Jitter + g_Menu.Config.LBYDeltaLegit - 10) ? Color(0, 255, 0, 255) : Color(255, 0, 0, 255), indicatorfont, false, xorstr("LBY").crypt_get());
		} 

		/*clock_t thisClock1 = clock();
		if ((((thisClock1 - lastClock1) / (double)CLOCKS_PER_SEC) * 1000.0) > 1000)
		{
			lastClock1 = thisClock1;
			fps1 = cFps1;
			cFps1 = 0;
		}
		else
			cFps1++;*/

		auto localplayer = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		auto eyeang = localplayer->GetEyeAngles().y;
		auto lby = localplayer->GetLowerBodyYaw();
		Vector viewangle; g_pEngine->GetViewAngles(viewangle);
		Vector2D screen1, screen2;
		Utils::WorldToScreen(origin, screen1);
		if (g::pLocalEntity->zeus() && g_Menu.Config.Aimbot) {
			C_BaseCombatWeapon* localwep = g::pLocalEntity->GetActiveWeapon();
			float weprange = localwep->GetCSWpnData()->range;
			Utils::WorldToScreen(g_Math.get_rotated_position(origin,viewangle.y, weprange), screen2);
			g_pSurface->Line(screen2.x, screen2.y, screen1.x, screen1.y, Color(255, 255, 255, 110));
			g_pSurface->DrawT(screen2.x, screen2.y, Color(255, 255, 255, 125), niggafont, false, xorstr("RANGE").crypt_get());
		}

		if (g_Menu.Config.Arrows) {
			if (g_Misc.isthirdperson) {
				constexpr auto distance = 50.f;
				constexpr auto lbydistance = 35.f;
				constexpr auto dist2 = 60.f;

				Utils::WorldToScreen(g_Math.get_rotated_position(origin, realangle, distance), screen2);
				g_pSurface->Line(screen2.x, screen2.y, screen1.x, screen1.y, Color(128, 244, 66, 255));
				g_pSurface->DrawT(screen2.x, screen2.y, Color(128, 244, 66, 255), niggafont, false, xorstr("REAL").crypt_get());

				Utils::WorldToScreen(g_Math.get_rotated_position(origin, lby, lbydistance), screen2);
				g_pSurface->Line(screen2.x, screen2.y, screen1.x, screen1.y, Color(66, 124, 244, 255));
				g_pSurface->DrawT(screen2.x, screen2.y, Color(66, 124, 244, 255), niggafont, false, xorstr("LBY").crypt_get());

				Utils::WorldToScreen(g_Math.get_rotated_position(origin, g::FakeAngle.y, distance), screen2);
				g_pSurface->Line(screen2.x, screen2.y, screen1.x, screen1.y, Color(244, 100, 66, 255));
				g_pSurface->DrawT(screen2.x, screen2.y, Color(244, 100, 66, 255), niggafont, false, xorstr("FAKE").crypt_get());

				Utils::WorldToScreen(g_Math.get_rotated_position(origin, g::pLocalEntity->AnimState()->m_flGoalFeetYaw, dist2 + 15), screen2);
				g_pSurface->Line(screen2.x, screen2.y, screen1.x, screen1.y, Color(75, 192, 235, 255));
				g_pSurface->DrawT(screen2.x, screen2.y, Color(75, 192, 235, 255), niggafont, false, xorstr("GOAL").crypt_get());

			}
		}
		if (g_Menu.Config.AntiaimMonitor) {
			int radius = 80;
			float range = 5;
			int LineLength = 50;
			int x = screenx / 2;
			int y = screeny / 5;

			g_pSurface->FilledCircle(Vector2D(x, y), 2 * radius, radius, Color(130, 130, 130, 185));
			g_pSurface->DrawCircleFast(x, y, radius, 2 * radius, Color(0, 0, 0, 225));

			Vector ViewAngles;
			g_pEngine->GetViewAngles(ViewAngles);
			ViewAngles.Normalize();

			Vector origview = Vector(0, 0, 0);
			origview.y = ViewAngles.y - g::FakeAngle.y;
			origview.Normalize();
			float origyaw = origview.y;
			DrawLine(origyaw, range, x, y, LineLength, Color(244, 100, 66, 255));//this the function from b4 btw

			Vector realtest = Vector(0, 0, 0);
			realtest.y = ViewAngles.y - g::RealAngle.y;
			realtest.Normalize();
			float realyaw = realtest.y;
			DrawLine(realyaw, range, x, y, LineLength, Color(128, 244, 66, 255));

			Vector lbytest = Vector(0, 0, 0);
			lbytest.y = ViewAngles.y - lby;
			lbytest.Normalize();
			float lbyyaw = lbytest.y;
			DrawLine(lbyyaw, range, x, y, LineLength - 10, Color(66, 124, 244, 255));

			g_pSurface->FilledCircle(Vector2D(x, y), 8, 4, Color(255, 255, 255, 225));
		}			
	}
}

void ESP::PenetrationCrosshair()
{
	float Damage;

	static HWND csgo = FindWindowA("Valve001", NULL);
	RECT screen;
	GetWindowRect(csgo, &screen);
	int screenx = screen.right - screen.left;
	int screeny = screen.bottom - screen.top;

	int screenCenterX = screenx / 2;
	int screenCenterY = screeny / 2;

	if (!g::pLocalEntity->GetActiveWeapon())
		return;

	Vector angles; g_pEngine->GetViewAngles(angles);

	int mode = 0;
	if (g_Menu.Config.Crosshair[1] && !g::pLocalEntity->IsKnifeorNade())
	{
		if (g_Autowall.CanWallbang(Damage))
		{
			g_pSurface->FilledRect(screenCenterX - 2, screenCenterY - 2, 5, 5, Color(0, 255, 0, 200));
			g_pSurface->OutlinedRect(screenCenterX - 2, screenCenterY - 2, 5, 5, Color(0, 0, 0, 200));
		} 
		else
		{
			g_pSurface->FilledRect(screenCenterX - 2, screenCenterY - 2, 5, 5, Color(255, 0, 0, 200));
			g_pSurface->OutlinedRect(screenCenterX - 2, screenCenterY - 2, 5, 5, Color(0, 0, 0, 200));
		}
	}
	if (g_Menu.Config.Crosshair[2]) {
		Vector punchAngle = g::pLocalEntity->m_aimPunchAngle();
		float rcsamount = g_Menu.Config.RCSamount / 100.0f;
		int x = screenx / 2;
		int y = screeny / 2;
		int dy = screeny / 90;
		int dx = screenx / 90;

		if (g_Menu.Config.RCStype != 1) {
			x -= (dx * (punchAngle.y) * (1 - rcsamount));
			y += (dy * (punchAngle.x) * (1 - rcsamount));
		}
		else {
			x -= (dx * (punchAngle.y) *  rcsamount);
			y += (dy * (punchAngle.x) *  rcsamount);
		}

		
		if (g_Autowall.CanWallbang(Damage)) {
			g_pSurface->FilledRect(x - 5, y, 10, 1, Color(0, 255, 0, 255));
			g_pSurface->FilledRect(x, y - 5, 1, 10, Color(0, 255, 0, 255));
		}
		else {
			g_pSurface->FilledRect(x - 5, y, 10, 1, Color(255, 0, 0, 255));
			g_pSurface->FilledRect(x, y - 5, 1, 10, Color(255, 0, 0, 255));
		}
	}
}

static bool filled;

void ESP::draw_spread() {

	if (g_Menu.Config.SpreadCrosshair == 4)
		filled = false;
	else if (g_Menu.Config.SpreadCrosshair == 5)
		filled = true;

	C_BaseCombatWeapon* weapon = g::pLocalEntity->GetActiveWeapon();
	if (g::pLocalEntity->IsAlive()) {
		int screen_w, screen_h;
		g_pEngine->GetScreenSize(screen_w, screen_h);
		int cross_x = screen_w / 2, cross_y = screen_h / 2;

		float recoil_step = screen_h / 180;
		/// this is to show da recoil pattern or smth
		//cross_x -= (int)(g::pLocalEntity->m_aimPunchAngle().y * recoil_step);
		//cross_y += (int)(g::pLocalEntity->m_aimPunchAngle().x * recoil_step);

		weapon->AccuracyPenalty();
		float inaccuracy = weapon->GetInaccuracy();
		float spread = weapon->GetSpread();

		float cone = inaccuracy * spread;
		cone *= screen_h * 0.7f;
		cone *= 90.f / 180;

		for (int seed{ }; seed < 256; ++seed) {
			g_Math.RandomSeed(rand() % 256);//255 + 1);
			float rand_a = g_Math.RandomFloat(0.f, 1.0f);
			float pi_rand_a = g_Math.RandomFloat(0.f, 2.0f * M_PI);
			float rand_b = g_Math.RandomFloat(0.0f, 1.0f);
			float pi_rand_b = g_Math.RandomFloat(0.f, 2.f * M_PI);

			float spread_x = cos(pi_rand_a) * (rand_a * inaccuracy) + cos(pi_rand_b) * (rand_b * spread);
			float spread_y = sin(pi_rand_a) * (rand_a * inaccuracy) + sin(pi_rand_b) * (rand_b * spread);

			float max_x = cos(pi_rand_a) * cone + cos(pi_rand_b) * cone;
			float max_y = sin(pi_rand_a) * cone + sin(pi_rand_b) * cone;

			float step = screen_h / 180 * 90.f;
			int screen_spread_x = (int)(spread_x * step * 0.7f);
			int screen_spread_y = (int)(spread_y * step * 0.7f);

			float percentage = (rand_a * inaccuracy + rand_b * spread) / (inaccuracy + spread);

			g_pSurface->FilledRect(cross_x + screen_spread_x, cross_y + screen_spread_y, 1, 1, 
				filled ?
				Color(g_Menu.Config.SpreadColor.r, g_Menu.Config.SpreadColor.g, g_Menu.Config.SpreadColor.b, g_Menu.Config.SpreadColor.a)
				: 
				Color(g_Menu.Config.SpreadColor.r, g_Menu.Config.SpreadColor.g, g_Menu.Config.SpreadColor.b, g_Menu.Config.SpreadColor.a * (0.4f + percentage * 0.6f)) * (0.1f + percentage * 0.9f));
		}
	}
}

void ESP::SpreadCrosshair()
{
	static HWND csgo = FindWindowA("Valve001", NULL);
	RECT screen;
	GetWindowRect(csgo, &screen);
	int screenx = screen.right - screen.left;
	int screeny = screen.bottom - screen.top;

	int screenCenterX = screenx / 2;
	int screenCenterY = screeny / 2;

	if (!g::pLocalEntity->GetActiveWeapon())
		return;

	auto pWeapon = g::pLocalEntity->GetActiveWeapon();

	if (!g::pLocalEntity->IsAlive())
		return;

	if (g::pLocalEntity->IsKnifeorNade())
		return;

	auto accuracy = pWeapon->GetInaccuracy() * 550.f; //3000

	static uint32_t cnt = 0;
	float freq = .009f;

	if (cnt++ >= (uint32_t)-1) cnt = 0;

	float r = std::sin(freq * cnt + 0) * 127 + 128;
	float g = std::sin(freq * cnt + 2) * 127 + 128;
	float b = std::sin(freq * cnt + 4) * 127 + 128;

	Color bruh = Color(g_Menu.Config.SpreadColor.r, g_Menu.Config.SpreadColor.g, g_Menu.Config.SpreadColor.b, g_Menu.Config.SpreadColor.a);

	if (g_Menu.Config.SpreadCrosshair == 1) {
		g_pSurface->add_gradient_rect(screenCenterX - .5 * (accuracy)+2, screenCenterY - .5 * (accuracy)+2, accuracy, accuracy, Color(r, g, b, 100), Color(b, r, g, 125), vertical);
	}
	else if (g_Menu.Config.SpreadCrosshair == 2) {
		g_pSurface->DrawCircleFast(screenCenterX, screenCenterY, accuracy, (25 * int(accuracy)), bruh);
	}
	else if (g_Menu.Config.SpreadCrosshair == 3) {
		g_pSurface->FilledCircle(Vector2D(screenCenterX, screenCenterY), (25 * int(accuracy)), accuracy, bruh);
	}
	else if (g_Menu.Config.SpreadCrosshair == 4 || g_Menu.Config.SpreadCrosshair == 5) {
		draw_spread();
	}
}

void ESP::projectiles(C_BaseEntity* entity) {
	if (!g_Menu.Config.Projectiles)
		return;

	if (!entity)
		return;

	auto client_class = entity->GetClientClass();
	auto model = entity->GetModel();

	if (!model)
		return;

	if (model) {
		Vector2D grenade_position;
		Vector grenade_origin;

		auto model = g_pModelInfo->GetStudiomodel(entity->GetModel());

		if (!model || !strstr(model->name, "thrown") && !strstr(model->name, "dropped"))
			return;

		std::string name = model->name;
		grenade_origin = entity->GetOrigin();

		if (!Utils::WorldToScreen(grenade_origin, grenade_position))
			return;

		if (name.find("flashbang") != std::string::npos) {
			g_pSurface->DrawT(grenade_position.x, grenade_position.y, Color(255, 255, 255, 255), niggafont, true, xorstr("FLASHBANG").crypt_get());
		}

		else if (name.find("smokegrenade") != std::string::npos) {
			g_pSurface->DrawT(grenade_position.x, grenade_position.y, Color(155, 155, 155, 255), niggafont, true, xorstr("SMOKE").crypt_get());
		}

		else if (name.find("incendiarygrenade") != std::string::npos) {
			g_pSurface->DrawT(grenade_position.x, grenade_position.y, Color(244, 100, 66, 255), niggafont, true, xorstr("INCENDIARY").crypt_get());
		}

		else if (name.find("molotov") != std::string::npos) {
			g_pSurface->DrawT(grenade_position.x, grenade_position.y, Color(244, 100, 66, 255), niggafont, true, xorstr("MOLOTOV").crypt_get());
		}

		else if (name.find("fraggrenade") != std::string::npos) {
			g_pSurface->DrawT(grenade_position.x, grenade_position.y, Color(244, 100, 66, 255), niggafont, true, xorstr("HEGRENADE").crypt_get());
		}

		else if (name.find("decoy") != std::string::npos) {
			g_pSurface->DrawT(grenade_position.x, grenade_position.y, Color(128, 244, 66, 255), niggafont, true, xorstr("DECOY").crypt_get());
		}
	}
}

float CSGO_Armor(float flDamage, int ArmorValue)
{
	float flArmorRatio = 0.5f;
	float flArmorBonus = 0.5f;
	if (ArmorValue > 0) {
		float flNew = flDamage * flArmorRatio;
		float flArmor = (flDamage - flNew) * flArmorBonus;

		if (flArmor > static_cast<float>(ArmorValue)) {
			flArmor = static_cast<float>(ArmorValue) * (1.f / flArmorBonus);
			flNew = flDamage - flArmor;
		}

		flDamage = flNew;
	}
	return flDamage;
}


RECT TextSize(const char* text, int font)
{
	int apple = 0;
	char Buffer[2048] = { '\0' };

	size_t Size = strlen(text) + 1;
	wchar_t* WideBuffer = new wchar_t[Size];
	//mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

	MultiByteToWideChar(CP_UTF8, MB_USEGLYPHCHARS, text, -1, WideBuffer, Size);
	int Width = 0, Height = 0;
	g_pSurface->GetTextSize(font, WideBuffer, Width, Height);
	RECT outcome = { 0, 0, Width, Height };
	delete[] WideBuffer;
	return outcome;
}
static std::string damagestring;
static bool lethal;
static int bombR = 0,bombG = 0;
static int lethaly;
void ESP::newbombtimer(C_BaseEntity* ent) // probably the most retarded thing you've ever seen
{
	Vector bomb_origin;
	Vector2D bomb_position;
	bomb_origin = ent->GetOrigin();
	if (g_Menu.Config.BombEsp)
	{
		float flDistance = g::pLocalEntity->GetOrigin().DistTo(bomb_origin);
		float a = 450.7f;
		float b = 75.68f;
		float c = 789.2f;
		float d = ((flDistance - b) / c);
		float flDamage = a * exp(-d * d);
		int dmg = CSGO_Armor(flDamage, g::pLocalEntity->ArmorValue());
		int damage = max((int)ceilf(dmg), 0);
		if (damage >= g::pLocalEntity->GetHealth())
			lethal = true;
		else
			lethal = false;

		int x, y; g_pEngine->GetScreenSize(x, y);
		float flblow = ent->c4_blow_time();
		float ExplodeTimeRemaining = flblow - (g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick);
		float fldefuse = ent->m_flDefuseCountDown();
		float DefuseTimeRemaining = fldefuse - (g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick);
		char TimeToExplode[64]; sprintf_s(TimeToExplode, "%.1f", ExplodeTimeRemaining);
		char TimeToDefuse[64]; sprintf_s(TimeToDefuse, "%.1f", DefuseTimeRemaining);
		auto explode_time = ent->c4_blow_time();
		explode_time -= g_pGlobalVars->intervalPerTick * g::pLocalEntity->GetTickBase();
		if (explode_time <= 0)
			explode_time = 0;
		if (explode_time > 10)
		{
			bombR = 0;
			bombG = 255;
		}
		else if (explode_time <= 10 && explode_time > 5)
		{
			bombR = 255;
			bombG = 255;
		}
		else if (explode_time <= 5)
		{
			bombR = 255;
			bombG = 0;
		}

		if (!lethal) {
			damagestring = "-";
			damagestring += std::to_string(dmg).c_str();
			damagestring += "HP";
		}
		else {
			damagestring = xorstr("LETHAL").crypt_get();
		}
		RECT rect = g_pSurface->GetTextSizeRect(niggafont, TimeToExplode);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		C_BaseEntity* Defuser = (C_BaseEntity*)g_pEntityList->GetClientEntityFromHandle(ent->m_hBombDefuser());
		if (ExplodeTimeRemaining > 0 && !ent->m_bBombDefused())
		{
			float fraction = ExplodeTimeRemaining / ent->m_flTimerLength();
			int onscreenwidth = fraction * x;
			float red = 255 - (fraction * 255);
			float green = fraction * 255;
			g_pSurface->DrawT(onscreenwidth - width, 10, Color(255, 255, 255, 255), font, false, TimeToExplode);
			g_pSurface->DrawSetColor(bombR, bombG, 0, 255);
			g_pSurface->DrawFilledRect(0, 0, onscreenwidth, 10);
		}
		RECT rect2 = g_pSurface->GetTextSizeRect(niggafont, TimeToExplode);
		width = rect2.right - rect2.left;
		if (Defuser)
		{
			lethaly = 20;
			float fraction = DefuseTimeRemaining / ent->m_flTimerLength();
			int onscreenwidth = fraction * x;
			g_pSurface->DrawSetColor(0, 0, 255, 255);
			g_pSurface->DrawFilledRect(0, 10, onscreenwidth, 20);
			g_pSurface->DrawT(onscreenwidth - width, 20, Color(255, 255, 255, 255), font, true, TimeToDefuse);
			if (!Utils::WorldToScreen(bomb_origin, bomb_position))	return;
			g_pSurface->DrawT(bomb_position.x, bomb_position.y + 12, Color(255, 0, 0), niggafont, true, xorstr("DEFUSING").crypt_get());
			if (DefuseTimeRemaining < 1)
				lethaly = 29;
			else
				lethaly = 20;
		}
		else {
			lethaly = 10;
		}
		if (g::pLocalEntity->IsAlive())
			g_pSurface->DrawT(1, lethaly, lethal ? Color(255, 0, 0, 255) : Color(255, 255, 255, 255), font, false, damagestring.c_str());
		if (!Utils::WorldToScreen(bomb_origin, bomb_position))	return;
		g_pSurface->DrawT(bomb_position.x, bomb_position.y, Color(255, 255, 255), niggafont, true, xorstr("BOMB").crypt_get());
	}
}

std::string clean_item_name(std::string name) {
	std::string Name = name;

	auto weapon_start = Name.find("weapon");
	if (weapon_start != std::string::npos)
		Name.erase(Name.begin() + weapon_start, Name.begin() + weapon_start + 6);

	if (Name[0] == '_')
		Name.erase(Name.begin());

	if (Name[0] == 'c') //optional for dropped weapons - designer
		Name.erase(Name.begin());

	return Name;
}

std::string unicode_to_ascii(const std::wstring& unicode) {
	std::string ascii_str(unicode.begin(), unicode.end());
	return ascii_str;
}

bool world_to_screen(const Vector& origin, Vector& screen) {
	auto matrix = g_pEngine->WorldToScreenMatrix();

	auto find_point = [](Vector & point, int screen_w, int screen_h, int degrees) -> void {
		float x2 = screen_w * 0.5f;
		float y2 = screen_h * 0.5f;

		float d = sqrt(pow((point.x - x2), 2) + (pow((point.y - y2), 2))); //Distance
		float r = degrees / d; //Segment ratio

		point.x = r * point.x + (1 - r) * x2; //find point that divides the segment
		point.y = r * point.y + (1 - r) * y2; //into the ratio (1-r):r
	};

	float w = matrix[3][0] * origin.x + matrix[3][1] * origin.y + matrix[3][2] * origin.z + matrix[3][3];

	int screen_width, screen_height;
	g_pEngine->GetScreenSize(screen_width, screen_height);

	float inverse_width = -1.0f / w;
	bool behind = true;

	if (w > 0.01) {
		inverse_width = 1.0f / w;
		behind = false;
	}

	screen.x = (float)((screen_width / 2) + (0.5 * ((matrix[0][0] * origin.x
		+ matrix[0][1] * origin.y
		+ matrix[0][2] * origin.z
		+ matrix[0][3]) * inverse_width) * screen_width + 0.5));

	screen.y = (float)((screen_height / 2) - (0.5 * ((matrix[1][0] * origin.x
		+ matrix[1][1] * origin.y
		+ matrix[1][2] * origin.z
		+ matrix[1][3]) * inverse_width) * screen_height + 0.5));

	if (screen.x > screen_width || screen.x < 0 || screen.y > screen_height || screen.y < 0 || behind) {
		find_point(screen, screen_width, screen_height, screen_height / 2);
		return false;
	}

	return !(behind);
}

void ESP::world(C_BaseEntity* entity) {
	auto local = g::pLocalEntity;
	if (!entity->is_valid_world(true) || !local)
		return;

	ClientClass * client_class = entity->GetClientClass();
	if (!client_class)
		return;

	const auto owner = g_pEntityList->GetClientEntityFromHandle(entity->m_hOwnerEntity());

	Vector zero = Vector(0, 0, 0);

	if (entity->GetOrigin() != zero && client_class->ClassID != ClassID::CBaseWeaponWorldModel && (std::strstr(client_class->pNetworkName, "Weapon")
		|| client_class->ClassID == ClassID::CDEagle || client_class->ClassID == ClassID::CAK47)) {

		auto class_id = entity->GetClientClass()->ClassID;
		auto model_name = g_pModelInfo->GetModelName(entity->GetModel());
		auto weapon = entity;

		Vector dropped_weapon_position, dropped_weapon_origin;

		dropped_weapon_origin = weapon->GetOrigin();

		if (!world_to_screen(dropped_weapon_origin, dropped_weapon_position))
			return;

		if (strstr(entity->GetClientClass()->pNetworkName, ("CWeapon"))) {
			std::string data = strstr(entity->GetClientClass()->pNetworkName, ("CWeapon"));
			std::transform(data.begin(), data.end(), data.begin(), ::toupper); //convert dropped weapons names to lowercase, looks cleaner - designer
			g_pSurface->DrawT(dropped_weapon_position.x, dropped_weapon_position.y, Color(255, 255, 255, 255), niggafont, true, clean_item_name(data).c_str());
		}

		if (class_id == ClassID::CAK47)
			g_pSurface->DrawT(dropped_weapon_position.x, dropped_weapon_position.y, Color(255, 255, 255), niggafont, true, "AK47");

		if (class_id == ClassID::CC4)
			g_pSurface->DrawT(dropped_weapon_position.x, dropped_weapon_position.y, Color(59, 119, 181, 255), niggafont, true, "BOMB");

		if (class_id == ClassID::CDEagle)
			g_pSurface->DrawT(dropped_weapon_position.x, dropped_weapon_position.y, Color(255, 255, 255), niggafont, true, "DEAGLE");

		if (strstr(model_name, "w_defuser"))
			g_pSurface->DrawT(dropped_weapon_position.x, dropped_weapon_position.y, Color(59, 119, 181, 255), niggafont, true, "DEFUSE KIT");
		
        
	}
}

void ESP::DrawHitboxes(C_BaseEntity* pEntity, int r, int g, int b, int a, float duration)
{
	matrix3x4_t matrix[128];

	if (!pEntity->SetupBones(matrix, 128, 0x00000100, g_pEngine->GetLastTimeStamp()))
		return;

	studiohdr_t* hdr = g_pModelInfo->GetStudiomodel(pEntity->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

	for (int i = 0; i < set->numhitboxes; i++)
	{
		mstudiobbox_t* hitbox = set->GetHitbox(i);

		if (!hitbox)
			continue;

		Vector vMin, vMax;
		g_Math.VectorTransform(hitbox->min, matrix[hitbox->bone], vMin);
		g_Math.VectorTransform(hitbox->max, matrix[hitbox->bone], vMax);

		g_pIVDebugOverlay->DrawPillReal(vMin, vMax, hitbox->radius, r, g, b, a, duration);
	}
}

void ESP::watermark() // who needs gettextsize LOL
{
	int width, height;
	g_pEngine->GetScreenSize(width, height);

	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%X", &tstruct);

	static int fps, old_tick_count;

	if ((g_pGlobalVars->tickcount - old_tick_count) > 50) {
		fps = static_cast<int>(1.f / g_pGlobalVars->frametime);
		old_tick_count = g_pGlobalVars->tickcount;
	}
	std::stringstream ss;

	ConVar* cl_cmdrate = g_pCvar->FindVar("cl_cmdrate");

	auto net_channel = g_pEngine->GetNetChannelInfo();
	auto local_player = g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	std::string incoming = local_player ? std::to_string((int)(net_channel->GetLatency(FLOW_INCOMING) * 1000)) : "0";
	std::string outgoing = local_player ? std::to_string((int)(net_channel->GetLatency(FLOW_OUTGOING) * 1000)) : "0";
	int ping = (int)(net_channel->GetLatency(FLOW_OUTGOING) * 1000);

	ss << xorstr("gamerware | rtt: ").crypt_get() << outgoing.c_str() << xorstr(" | rate: ").crypt_get() << cl_cmdrate->GetFloat() << xorstr(" | ").crypt_get() << buf;

	static int x, y, z;

	if (cl_cmdrate->GetFloat() == 64) {
		x = 265;
		y = 261;
	}
	else {
		x = 268;
		y = 261;
	}
	if (ping >= 10) { 
		y += 3;
		z = 3;
	}
	else if (ping >= 100) {
		y += 12;
		z = 12;
	} 

	g_pSurface->FilledRect(width - x - z, 4, y + z - 1, 20, Color(247, 116, 150, 235));
	g_pSurface->DrawT(width - y, 6.5f, Color(238, 216, 222, 245), supremacy, false, ss.str().c_str());
}

static bool defusing;

void ESP::Render()
{
    if (!g::pLocalEntity || !g_pEngine->IsInGame() || !g_Menu.Config.Esp)
        return;

	if (g_Menu.Config.Watermark)
		watermark();

	color = Color(g_Menu.Config.BoxColor.r, g_Menu.Config.BoxColor.g, g_Menu.Config.BoxColor.b, g_Menu.Config.BoxColor.a);
	textcolor = Color(g_Menu.Config.FontColor.r, g_Menu.Config.FontColor.g, g_Menu.Config.FontColor.b, g_Menu.Config.FontColor.a);
	wepcolor = Color(g_Menu.Config.WepColor.r, g_Menu.Config.WepColor.g, g_Menu.Config.WepColor.b, g_Menu.Config.WepColor.a);
	skelecolor = Color(g_Menu.Config.SkeletonColor.r, g_Menu.Config.SkeletonColor.g, g_Menu.Config.SkeletonColor.b, g_Menu.Config.SkeletonColor.a);

	font = g::Verdana;

	indicatorfont = g::IndicatorFont;
	if (g_Menu.Config.Font == 1)
		niggafont = g::NiggaPee;
	else
		niggafont = g::SauIsAFatRetard;

	weaponfont = g::WeaponFont;

	skeetfont = g::memefont;

	supremacy = g::supfont;

	if (g::pLocalEntity->IsAlive() && g_pEngine->IsInGame() && g_pEngine->IsConnected()) {
		Arrows();

		PenetrationCrosshair();

		SpreadCrosshair();

		//if (g_Menu.Config.Hitmarker != 0)
		//	hitmarker();
	}

	for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
    {
        C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

        if (!pPlayerEntity
            || !pPlayerEntity->IsAlive()
            || pPlayerEntity->IsDormant()
            || pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
            continue;

		offsetY = 0;

		BoundBox(pPlayerEntity);

		if (pPlayerEntity->m_bIsDefusing() == 1)
			defusing = true;
		else
			defusing = false;

		if (Box.bottom == 0)
			continue;

		if (g_Menu.Config.EngineRadar)
		{
			ConVar* mp_radar_showall = g_pCvar->FindVar("mp_radar_showall");
			*(int*)((DWORD)& mp_radar_showall->fnChangeCallback + 0xC) = 0; // ew
			mp_radar_showall->SetValue(1);
		}
		else {
			ConVar* mp_radar_showall = g_pCvar->FindVar("mp_radar_showall");
			*(int*)((DWORD)& mp_radar_showall->fnChangeCallback + 0xC) = 0; // ew
			mp_radar_showall->SetValue(0);
		}

		if (g_Menu.Config.Skeleton[0] || g_Menu.Config.Skeleton[1])
			RenderSkeleton(pPlayerEntity);

		if (g_Menu.Config.Box)
            RenderBox();

		if (g_Menu.Config.Flags)
			Flags(pPlayerEntity);

		if (g_Menu.Config.ShowAimSpot)
		RenderHitboxPoints(pPlayerEntity);

        if (g_Menu.Config.Name > 0)
            RenderName(pPlayerEntity, i);

        if (g_Menu.Config.Weapon)
            RenderWeaponName(pPlayerEntity);

		if (g_Menu.Config.HealthBar || g_Menu.Config.HealthVal > 0)
			RenderHealth(pPlayerEntity);

		if (g_Menu.Config.OutOfView)
		arrows(pPlayerEntity);
	}

	for (int i = g_pEngine->GetMaxClients() + 1; i < g_pEntityList->GetHighestEntityIndex(); i++) {
		C_BaseEntity* entity = g_pEntityList->GetClientEntity(i);
		if (!entity)
			continue;
		if (entity && entity != g::pLocalEntity) {
			auto model_name = g_pModelInfo->GetModelName(entity->GetModel());

			if (entity->GetClientClass()->ClassID == ClassID::CPlantedC4 && g_Menu.Config.BombEsp) {
				newbombtimer(entity);
			}

			if (g_Menu.Config.DroppedWeapons) 
				world(entity);

			projectiles(entity);
		}
	}
}