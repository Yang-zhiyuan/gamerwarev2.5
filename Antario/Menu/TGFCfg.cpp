#include "Menu.h"
#include "TGFCfg.h"
#include "../xor.h"

// credits to team gamer food

void CConfig::Setup()
{
	SetupValue(g_Menu.Config.Aimbot, false, xorstr("aimbot").crypt_get());
	SetupValue(g_Menu.Config.Hitchance, 0, xorstr("hitchance").crypt_get());
	SetupValue(g_Menu.Config.HitchanceValue, 0, xorstr("hitchanceval").crypt_get());
	SetupValue(g_Menu.Config.Mindmg, 0, xorstr("mindmg").crypt_get());
	SetupValue(g_Menu.Config.Resolver, false, xorstr("resolver").crypt_get());
	SetupValue(g_Menu.Config.OnShotAA, false, xorstr("onshotantiaim").crypt_get());
	SetupValue(g_Menu.Config.ResolverType, 0, xorstr("resolvertype").crypt_get());
	SetupValue(g_Menu.Config.BodyScale, 0, xorstr("bodyscale").crypt_get());
	SetupValue(g_Menu.Config.HeadScale, 0, xorstr("headscale").crypt_get());
	SetupValue(g_Menu.Config.MultiPoint, false, xorstr("multipoint").crypt_get());
	SetupValue(g_Menu.Config.DelayShot, false, xorstr("delayshot").crypt_get());
	SetupValue(g_Menu.Config.IgnoreLimbs, false, xorstr("ignorelimbs").crypt_get());
	SetupValue(g_Menu.Config.PredictFakeLag, false, xorstr("whywontthisfuckingwork").crypt_get());
	SetupValue(g_Menu.Config.OutOfView, false, xorstr("playerindicator").crypt_get());
	SetupValue(g_Menu.Config.Autostop, false, xorstr("autostop").crypt_get());
	SetupValue(g_Menu.Config.FixShotPitch, false, xorstr("fixshotpitch").crypt_get());
	SetupValue(g_Menu.Config.ShotBacktrack, false, xorstr("shotbacktrk").crypt_get());
	SetupValue(g_Menu.Config.PosBacktrack, false, xorstr("posbacktrk").crypt_get());
	SetupValue(g_Menu.Config.BaimLethal, false, xorstr("baimlethal").crypt_get());
	SetupValue(g_Menu.Config.BaimLethal, false, xorstr("baimlethal").crypt_get());
	SetupValue(g_Menu.Config.BaimPitch, false, xorstr("bamepitch").crypt_get());
	SetupValue(g_Menu.Config.BaimInAir, false, xorstr("baiminair").crypt_get());
	SetupValue(g_Menu.Config.BaimAlways, false, xorstr("baimalways").crypt_get());
	SetupValue(g_Menu.Config.BaimAfterEnable, false, xorstr("baimafterenable").crypt_get());
	SetupValue(g_Menu.Config.BaimSlider, 0, xorstr("baimafterx").crypt_get());
	SetupValue(g_Menu.Config.BaimLethalSlider, 0, xorstr("baimathealth").crypt_get());
	SetupValue(g_Menu.Config.MarkerSize, 0, xorstr("hitmarkersize").crypt_get());
	SetupValue(g_Menu.Config.ExpireTime, 0, xorstr("hitmarkerexptime").crypt_get());
	SetupValue(g_Menu.Config.VisHitDmg, false, xorstr("hitmarkervisdmg").crypt_get());
	SetupValue(g_Menu.Config.VisHitmarker, false, xorstr("hitmarkervis").crypt_get());
	SetupValue(g_Menu.Config.ShowAimSpot, false, xorstr("showaimspot").crypt_get());

	SetupValue(g_Menu.Config.Antiaim, false, xorstr("antiaimd").crypt_get());
	SetupValue(g_Menu.Config.DesyncAngle, false, xorstr("desyncang").crypt_get());
	SetupValue(g_Menu.Config.Jitter, 0, xorstr("jitterenable").crypt_get());
	SetupValue(g_Menu.Config.JitterRange, 0, xorstr("jitterrange").crypt_get());
	SetupValue(g_Menu.Config.FakeLagModel, 0, xorstr("fakelagmodel").crypt_get());
	SetupValue(g_Menu.Config.ShadowMaterial, 0, xorstr("shadowmat").crypt_get());
	SetupValue(g_Menu.Config.FakeLagModelColor, 0, xorstr("fakelagmodelcolor").crypt_get());
	SetupValue(g_Menu.Config.Fakelag, 0, xorstr("fakelag").crypt_get());
	SetupValue(g_Menu.Config.FakeLagOnPeek, false, xorstr("onpeekflag").crypt_get());
	SetupValue(g_Menu.Config.FakeLagEnable, false, xorstr("fakelagenablememe").crypt_get());
	SetupValue(g_Menu.Config.FakelagThreshold, false, xorstr("randthreshold").crypt_get());
	SetupValue(g_Menu.Config.FakelagType, false, xorstr("fakelagtypebr0").crypt_get());
	SetupValue(g_Menu.Config.ChokeShotOnPeek, false, xorstr("chokeshotonpeek").crypt_get());
	SetupValue(g_Menu.Config.DesyncAngle, false, xorstr("desyncangle").crypt_get());
	SetupValue(g_Menu.Config.BackAA, false, xorstr("backaa").crypt_get());
	SetupValue(g_Menu.Config.Freestand, false, xorstr("freestand").crypt_get());

	SetupValue(g_Menu.Config.Hitboxes, false, xorstr("hitboxes").crypt_get());
	SetupValue(g_Menu.Config.Head, false, xorstr("head").crypt_get());
	SetupValue(g_Menu.Config.Chest, false, xorstr("chest").crypt_get());
	SetupValue(g_Menu.Config.Pelvis, false, xorstr("pelvis").crypt_get());
	SetupValue(g_Menu.Config.Arms, false, xorstr("arms").crypt_get());
	SetupValue(g_Menu.Config.Legs, false, xorstr("legs").crypt_get());

	SetupValue(g_Menu.Config.Info, false, xorstr("Info").crypt_get());
	SetupValue(g_Menu.Config.AAInvertIndicators, false, xorstr("aainvertindicators").crypt_get());
	SetupValue(g_Menu.Config.Esp, false, xorstr("esp").crypt_get());
	SetupValue(g_Menu.Config.LocalChams, false, xorstr("localchamsa").crypt_get());
	SetupValue(g_Menu.Config.Font, 0, xorstr("font").crypt_get());
	SetupValue(g_Menu.Config.DesyncInvertKey, 0, xorstr("desyncinvertkey").crypt_get());
	SetupValue(g_Menu.Config.ChamsMaterial, 0, xorstr("chamsmaterial").crypt_get());
	SetupValue(g_Menu.Config.AsusProps, 90, xorstr("asusprops").crypt_get());
	SetupValue(g_Menu.Config.LBYDelta, 90, xorstr("lbydelta").crypt_get());
	SetupValue(g_Menu.Config.FontColor, 255, xorstr("fontclr").crypt_get());
	SetupValue(g_Menu.Config.ChamsColor, 255, xorstr("chamscolor").crypt_get());
	SetupValue(g_Menu.Config.XQZ, 255, xorstr("xqzcolor").crypt_get());
	SetupValue(g_Menu.Config.OutOfViewColor, 255, xorstr("outofviewindicatorcolor").crypt_get());
	SetupValue(g_Menu.Config.ImpactColor, 255, xorstr("impact_color").crypt_get());
	SetupValue(g_Menu.Config.WepColor, 255, xorstr("weaponcolor").crypt_get());
	SetupValue(g_Menu.Config.LocalChamsColor, 255, xorstr("local_chamscolor").crypt_get());
	SetupValue(g_Menu.Config.SpreadColor, 255, xorstr("showspreadcolor").crypt_get());
	SetupValue(g_Menu.Config.BoxColor, 255, xorstr("boxcolor").crypt_get());
	SetupValue(g_Menu.Config.SkyboxColor, 255, xorstr("skyboxcolor").crypt_get());
	SetupValue(g_Menu.Config.HitmarkerColor, 255, xorstr("hitmarkercolor").crypt_get());
	SetupValue(g_Menu.Config.Name, false, xorstr("name").crypt_get()); 
	SetupValue(g_Menu.Config.FakeLagShooting, false, xorstr("shotfakelag").crypt_get()); 
	SetupValue(g_Menu.Config.Weapon, false, xorstr("weapon").crypt_get());
	SetupValue(g_Menu.Config.FreestandType, 0, xorstr("freestandtype").crypt_get());
	SetupValue(g_Menu.Config.Yaw, 180, xorstr("yawaddvalue").crypt_get());
	SetupValue(g_Menu.Config.Box, false, xorstr("box").crypt_get());
	SetupValue(g_Menu.Config.BoxColor, 255, xorstr("boxclr").crypt_get());
	SetupValue(g_Menu.Config.HandChams, false, xorstr("handchams").crypt_get());
	SetupValue(g_Menu.Config.HandChamsColor, 255, xorstr("handchamscolor").crypt_get());
	SetupValue(g_Menu.Config.HandChamsMaterial, 0, xorstr("handchamsmaterial").crypt_get());
	SetupValue(g_Menu.Config.Glow, false, xorstr("glowenable").crypt_get());
	SetupValue(g_Menu.Config.GlowColor, 255, xorstr("glowcolor").crypt_get());
	SetupValue(g_Menu.Config.HealthBar, false, xorstr("healthbar").crypt_get());
	SetupValue(g_Menu.Config.Thirdperson, false, xorstr("thirdpersonbruh").crypt_get());
	SetupValue(g_Menu.Config.DroppedWeapons, false, xorstr("droppedweapons").crypt_get());
	SetupValue(g_Menu.Config.AimRCS, false, xorstr("aimrcsenable").crypt_get());
	SetupValue(g_Menu.Config.EngineRadar, false, xorstr("engineradar").crypt_get());
	SetupValue(g_Menu.Config.Watermark, true, xorstr("watermark").crypt_get());
	SetupValue(g_Menu.Config.DrawHitboxTime, 0, xorstr("hitboxtime").crypt_get());
	SetupValue(g_Menu.Config.Clantag, false, xorstr("clantag").crypt_get());
	SetupValue(g_Menu.Config.Skeleton, false, 2, xorstr("skeleton").crypt_get());
	SetupValue(g_Menu.Config.ForcedConvars, false, 2, xorstr("forcedcvars").crypt_get());
	SetupValue(g_Menu.Config.DesyncType, false, xorstr("desynctype").crypt_get());
	SetupValue(g_Menu.Config.SkeletonColor, 255, xorstr("skeletonclr").crypt_get());
	SetupValue(g_Menu.Config.Backtrackchams, false, xorstr("btchams").crypt_get());
	SetupValue(g_Menu.Config.BacktrackchamsColor, 255, xorstr("btchamscolor").crypt_get());
	SetupValue(g_Menu.Config.HitboxPoints, false, xorstr("hitboxpoints").crypt_get());
	SetupValue(g_Menu.Config.Chams, false, xorstr("chams").crypt_get());
	SetupValue(g_Menu.Config.NoZoom, false, xorstr("nozoom").crypt_get());
	SetupValue(g_Menu.Config.NoScope, false, xorstr("noscope").crypt_get());
	SetupValue(g_Menu.Config.NoRecoil, false, xorstr("norecoil").crypt_get());
	SetupValue(g_Menu.Config.FakeChams, false, xorstr("localchams").crypt_get());
	SetupValue(g_Menu.Config.Arrows, false, xorstr("localarrows").crypt_get());

	SetupValue(g_Menu.Config.DrawHitboxes, false, xorstr("drawhitboxes").crypt_get());

	SetupValue(g_Menu.Config.RCSamount, 0, xorstr("rcsamount").crypt_get());
	SetupValue(g_Menu.Config.RCStype, 0, xorstr("rcst").crypt_get());
	SetupValue(g_Menu.Config.RCS, 0, xorstr("rcs").crypt_get());
	SetupValue(g_Menu.Config.StandaloneRCS, 0, xorstr("rcaaas").crypt_get());
	SetupValue(g_Menu.Config.StandaloneRCSSlider, 0, xorstr("raaaaaaaaacs").crypt_get());
	SetupValue(g_Menu.Config.StandaloneRCStype, 0, xorstr("rcaaaaasttt").crypt_get());
	SetupValue(g_Menu.Config.LegitAA, false, xorstr("legitaa").crypt_get());
	SetupValue(g_Menu.Config.Legitbot, 0, xorstr("legitenable").crypt_get());
	SetupValue(g_Menu.Config.LegitBacktrack, 0, xorstr("legitbtmeme").crypt_get());
	SetupValue(g_Menu.Config.TracerMaterial, 0, xorstr("tracermaterial").crypt_get());

	SetupValue(g_Menu.Config.Fov, 0, xorstr("fov").crypt_get());
	SetupValue(g_Menu.Config.OutOfViewRadius, 0, xorstr("outofviewradius").crypt_get());
	SetupValue(g_Menu.Config.OutOfViewSize, 0, xorstr("outofviewsize").crypt_get());
	SetupValue(g_Menu.Config.Crosshair, false, 3, xorstr("crosshair").crypt_get()); 
	SetupValue(g_Menu.Config.Flags, false, xorstr("flags").crypt_get());
	SetupValue(g_Menu.Config.Bhop, false, xorstr("bhop").crypt_get());
	SetupValue(g_Menu.Config.NoSmoke, false, xorstr("noshmoke").crypt_get());
	SetupValue(g_Menu.Config.AutoStrafe, false, xorstr("autostrafe").crypt_get());
	SetupValue(g_Menu.Config.Tracer, false, xorstr("tracermeme").crypt_get());
	SetupValue(g_Menu.Config.SpreadCrosshair, false, xorstr("spreadcrosshairmeme").crypt_get());
	SetupValue(g_Menu.Config.SkyboxMod, false, xorstr("skyboxmod").crypt_get());
	SetupValue(g_Menu.Config.RagdollMeme, false, xorstr("ragdollmeme").crypt_get());
	SetupValue(g_Menu.Config.AntiaimMonitor, false, xorstr("antiaimmonitor").crypt_get());
	SetupValue(g_Menu.Config.BombEsp, false, xorstr("bombesp").crypt_get());
	SetupValue(g_Menu.Config.Projectiles, false, xorstr("projectiles").crypt_get());
	SetupValue(g_Menu.Config.LocalFake, 0, xorstr("fakehitbox").crypt_get());
	SetupValue(g_Menu.Config.ChamsXQZ, false, xorstr("projectiles").crypt_get());
	SetupValue(g_Menu.Config.AtTarget, false, xorstr("attargets").crypt_get());
	SetupValue(g_Menu.Config.StaticDesync, false, xorstr("staticdesync").crypt_get());
	SetupValue(g_Menu.Config.Autoscope, false, xorstr("autoscope").crypt_get());
	SetupValue(g_Menu.Config.HitmarkerTime, 0, xorstr("tracertime").crypt_get());
	SetupValue(g_Menu.Config.Ak47meme, false, xorstr("ak47meme").crypt_get());
	SetupValue(g_Menu.Config.Hitmarker, 2, xorstr("hitmarker").crypt_get());
	SetupValue(g_Menu.Config.Impacts, 0, xorstr("showimpacts").crypt_get());
	SetupValue(g_Menu.Config.SlowWalkSlider , 0, xorstr("slowwalkslider").crypt_get());
	SetupValue(g_Menu.Config.SlowWalk, false, xorstr("fakeduck").crypt_get());
	SetupValue(g_Menu.Config.FakeDuck, false, xorstr("slowwalk").crypt_get());
	SetupValue(g_Menu.Config.FakeduckKey, 0, xorstr("fakeduckkey").crypt_get());
	SetupValue(g_Menu.Config.ThirdpersonKey, 0, xorstr("thirdpersonkey").crypt_get());
	SetupValue(g_Menu.Config.BaimKey, 0, xorstr("baimkey").crypt_get());

	SetupValue(g_Menu.Config.NightMode, false, xorstr("nightmode").crypt_get());
	SetupValue(g_Menu.Config.NightModeSlider, 0, xorstr("nightmodeslider").crypt_get());
	SetupValue(g_Menu.Config.PostProcess, false, xorstr("postprocess").crypt_get());
	SetupValue(g_Menu.Config.AspectRatio, false, xorstr("aspect").crypt_get());
	SetupValue(g_Menu.Config.AspectRatioSlider, 0, xorstr("aspectslider").crypt_get());
	SetupValue(g_Menu.Config.LegitAimbot, false, xorstr("legitaimbot").crypt_get());
	SetupValue(g_Menu.Config.AspectRatioSlider, 0, xorstr("aspectslider").crypt_get());
	SetupValue(g_Menu.Config.LegitFOV, 0, xorstr("legitaimfov").crypt_get());
	SetupValue(g_Menu.Config.LegitSmooth, 0, xorstr("legitaimsmooth").crypt_get());
	SetupValue(g_Menu.Config.Extrapolation, false, xorstr("predictfakelag").crypt_get());
}

void CConfig::SetupValue(int& value, int def, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue< int >(name, &value));
}

void CConfig::SetupValue(float& value, float def, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue< float >(name, &value));
}

void CConfig::SetupValue(bool& value, bool def, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue< bool >(name, &value));
}

void CConfig::SetupValue(zgui::color& value, float def, std::string name)
{
	value.r = def;
	value.g = def;
	value.b = def;
	value.a = def;

	ints.push_back(new ConfigValue< int >(name + "red", &value.r));
	ints.push_back(new ConfigValue< int >(name + "green", &value.g));
	ints.push_back(new ConfigValue< int >(name + "blue", &value.b));
	ints.push_back(new ConfigValue< int >(name + "alpha", &value.a));
}
void CConfig::SetupValue(bool* value, bool def, int size, std::string name) // for multiboxes
{
	for (int c = 0; c < size; c++)
	{
		value[c] = def;

		name += std::to_string(c);

		bools.push_back(new ConfigValue< bool >(name, &value[c]));
	}
}

static char name[] = "csgohvh";

void CConfig::Save()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + xorstr("\\csgohvh\\").crypt_get();
		file = std::string(path) + xorstr("\\csgohvh\\cfg.ini").crypt_get();
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(name, value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(name, value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(name, value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + xorstr("\\csgohvh\\").crypt_get();
		file = std::string(path) + xorstr("\\csgohvh\\cfg.ini").crypt_get();
	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

void CConfig::Save2()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + xorstr("\\csgohvh\\").crypt_get();
		file = std::string(path) + xorstr("\\csgohvh\\cfg2.ini").crypt_get();
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(name, value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(name, value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(name, value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load2()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + xorstr("\\csgohvh\\").crypt_get();
		file = std::string(path) + xorstr("\\csgohvh\\cfg2.ini").crypt_get();
	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

void CConfig::Save3()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + xorstr("\\csgohvh\\").crypt_get();
		file = std::string(path) + xorstr("\\csgohvh\\cfg3.ini").crypt_get();
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(name, value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(name, value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(name, value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load3()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + xorstr("\\csgohvh\\").crypt_get();
		file = std::string(path) + xorstr("\\csgohvh\\cfg3.ini").crypt_get();
	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}


void CConfig::Save4()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + xorstr("\\csgohvh\\").crypt_get();
		file = std::string(path) + xorstr("\\csgohvh\\cfg4.ini").crypt_get();
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(name, value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(name, value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(name, value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load4()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + xorstr("\\csgohvh\\").crypt_get();
		file = std::string(path) + xorstr("\\csgohvh\\cfg4.ini").crypt_get();
	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

CConfig* g_Config = new CConfig();