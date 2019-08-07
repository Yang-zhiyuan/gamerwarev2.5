#include "Menu.h"
#include "TGFCfg.h"
#include "..\SDK\Vector.h"
#include "..\SDK\ISurface.h"
#include "..\Utils\Color.h"
#include "..\Utils\GlobalVars.h"
#include "..\Utils\XorStr.h"
#include "..\Hooks.h"
#include "../SDK/CHudChat.h"
#include "../gui/zgui.h"
#include "../xor.h"
Menu g_Menu;

void Menu::Render()
{
	zgui::poll_input("Valve001");

	if (zgui::begin_window(xorstr("nigga").crypt_get(), { 500, 600 }, g::Verdana, zgui::zgui_window_flags_none))
	{
		if (zgui::tab_button(xorstr(" legitbot").crypt_get(), { 69, 30 }, Tabs.legitbot))
		{
			Tabs.legitbot = true;
			Tabs.ragebot = false;
			Tabs.visual = false;
			Tabs.misc = false;
			Tabs.config = false;
			Tabs.plist = false;
			Tabs.debug = false;
		}

		zgui::same_line();
		zgui::next_column(-5, 0);
		if (zgui::tab_button(xorstr("ragebot").crypt_get(), { 69, 30 }, Tabs.ragebot))
		{
			Tabs.legitbot = false;
			Tabs.ragebot = true;
			Tabs.visual = false;
			Tabs.misc = false;
			Tabs.config = false;
			Tabs.plist = false;
			Tabs.debug = false;
		}
		zgui::same_line();
		zgui::next_column(-5, 0);

		if (zgui::tab_button(xorstr("visuals").crypt_get(), { 69, 30 }, Tabs.visual))
		{
			Tabs.legitbot = false;
			Tabs.ragebot = false;
			Tabs.visual = true;
			Tabs.misc = false;
			Tabs.config = false;
			Tabs.plist = false;
			Tabs.debug = false;
		}

		zgui::same_line();
		zgui::next_column(-5, 0);


		if (zgui::tab_button(xorstr("misc").crypt_get(), { 69, 30 }, Tabs.misc))
		{
			Tabs.legitbot = false;
			Tabs.ragebot = false;
			Tabs.visual = false;
			Tabs.misc = true;
			Tabs.config = false;
			Tabs.plist = false;
			Tabs.debug = false;
		}

		zgui::same_line();
		zgui::next_column(-5, 0);

		if (zgui::tab_button(xorstr("config").crypt_get(), { 69, 30 }, Tabs.config))
		{
			Tabs.legitbot = false;
			Tabs.ragebot = false;
			Tabs.visual = false;
			Tabs.misc = false;
			Tabs.config = true;
			Tabs.plist = false;
			Tabs.debug = false;
		}

		zgui::same_line();
		zgui::next_column(-5, 0);

		if (zgui::tab_button(xorstr(" player list").crypt_get(), { 69, 30 }, Tabs.plist))
		{
			Tabs.legitbot = false;
			Tabs.ragebot = false;
			Tabs.visual = false;
			Tabs.misc = false;
			Tabs.config = false;
			Tabs.plist = true;
			Tabs.debug = false;
		}

		if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(0x44))
		{
			Tabs.legitbot = false;
			Tabs.ragebot = false;
			Tabs.visual = false;
			Tabs.misc = false;
			Tabs.config = false;
			Tabs.plist = false;
			Tabs.debug = true;
		}


		if (Tabs.legitbot)
			legittab();

		if (Tabs.ragebot)
			ragetab();

		if (Tabs.visual)
			vistab();

		if (Tabs.misc)
			misctab();

		if (Tabs.config)
			cfgtab();

		if (Tabs.plist)
			plisttab();

		if (Tabs.debug)
			debugtab();

		zgui::end_window();
	}
}

void  Menu::legittab()
{
	zgui::next_column(-401, 39);

	zgui::begin_groupbox(xorstr("aim").crypt_get(), { 230 , 520 }, zgui::zgui_window_flags_none);
	{
		zgui::checkbox(xorstr("enable").crypt_get(), Config.Legitbot);
		if (Config.Legitbot)
		{
			Config.Aimbot = false;
		}
		zgui::checkbox(xorstr("aimbot").crypt_get(), Config.LegitAimbot);
		if (Config.LegitAimbot) {
			zgui::slider_int(xorstr("aim fov").crypt_get(), 0, 30, Config.LegitFOV);
			zgui::slider_int(xorstr("aim smooth").crypt_get(), 0, 100, Config.LegitSmooth);
			zgui::checkbox(xorstr("rcs").crypt_get(), Config.AimRCS);
			if (Config.AimRCS)
				zgui::slider_int(xorstr("rcs amount").crypt_get(), 0, 100, Config.RCSamount);
		}
		zgui::checkbox(xorstr("backtrack").crypt_get(), Config.LegitBacktrack);
		zgui::end_groupbox();
	}
	zgui::same_line();
	zgui::next_column(230, 39);
	zgui::begin_groupbox(xorstr("individual").crypt_get(), { 230 , 520 }, zgui::zgui_window_flags_none);
	{
		if (!Config.LegitAimbot) {
			zgui::checkbox(xorstr("standalone rcs").crypt_get(), Config.StandaloneRCS);
			Config.RCSamount = 0;
			if (Config.StandaloneRCS)
				zgui::slider_int(xorstr("standalone amount").crypt_get(), 0, 100, Config.StandaloneRCSSlider);
		}
		zgui::checkbox(xorstr("antiaim").crypt_get(), Config.LegitAA);
		zgui::checkbox(xorstr("static").crypt_get(), Config.LegitAAStatic);
		zgui::slider_int(xorstr("additive").crypt_get(), 0, 180, Config.additive);

		if (Config.LegitAA)
			Config.Antiaim = false;
		zgui::end_groupbox();
	}
}

void  Menu::ragetab()
{
	zgui::next_column(-401, 39);

	zgui::begin_groupbox(xorstr("aimbot").crypt_get(), { 230 , 520 }, zgui::zgui_window_flags_none);
	{

		zgui::checkbox(xorstr("enable").crypt_get(), Config.Aimbot);
		if (Config.Aimbot)
		{
			Config.Legitbot = false;
		}
		zgui::checkbox(xorstr("multipoint").crypt_get(), Config.MultiPoint);
		if (Config.MultiPoint)
		{
			zgui::slider_int(xorstr("head scale").crypt_get(), 0, 100, Config.HeadScale);
			zgui::slider_int(xorstr("body scale").crypt_get(), 0, 100, Config.BodyScale);
		}
		zgui::multi_combobox(xorstr("hitboxes to target").crypt_get(), std::vector< zgui::multi_select_item >{ { "head", & Config.Head }, { "chest", &Config.Chest }, { "stomach", &Config.Pelvis }, { "arms", &Config.Arms }, { "legs", &Config.Legs }});
		zgui::slider_int(xorstr("mindamage").crypt_get(), 0, 100, Config.Mindmg);
		zgui::checkbox(xorstr("autostop").crypt_get(), Config.Autostop);
		zgui::checkbox(xorstr("autoscope").crypt_get(), Config.Autoscope);
		zgui::slider_int(xorstr("hitchance value").crypt_get(), 0, 100, Config.HitchanceValue);
		zgui::checkbox(xorstr("resolver").crypt_get(), Config.Resolver);
		if (Config.Resolver) {
			zgui::combobox(xorstr("resolver type").crypt_get(), { xorstr("default").crypt_get(), xorstr("experimental").crypt_get() }, Config.ResolverType);
			if (Config.ResolverType == 1)
			zgui::checkbox(xorstr("fix feet").crypt_get(), Config.FixFeet);
		}
		zgui::multi_combobox(xorstr("baim options").crypt_get(), std::vector< zgui::multi_select_item >{ { "lethal", & Config.BaimLethal }, { "in air", &Config.BaimInAir }, { "always", &Config.BaimAlways }});
		if (Config.BaimLethal)
		{
			zgui::slider_int(xorstr("lethal health").crypt_get(), 0, 100, Config.BaimLethalSlider);
		}
		zgui::checkbox(xorstr("baim after").crypt_get(), Config.BaimAfterEnable);
		if (Config.BaimAfterEnable)
		{
			zgui::slider_int(xorstr("x missed shots").crypt_get(), 0, 10, Config.BaimSlider);
		}
		zgui::key_bind(xorstr("force baim key").crypt_get(), Config.BaimKey);
		zgui::end_groupbox();
	}
	zgui::same_line();
	zgui::next_column(230, 39);
	zgui::begin_groupbox(xorstr("antiaim").crypt_get(), { 230 , 340 }, zgui::zgui_window_flags_none);
	{
		zgui::checkbox(xorstr("enable ").crypt_get(), Config.Antiaim);
		if (Config.Antiaim)
			Config.LegitAA = false;
		zgui::slider_int(xorstr("yaw add").crypt_get(), 0, 360, Config.Yaw);
		zgui::checkbox(xorstr("at targets").crypt_get(), Config.AtTarget);
		zgui::combobox(xorstr("jitter").crypt_get(), { "off", xorstr("random").crypt_get(), xorstr("switch").crypt_get(), xorstr("offset").crypt_get() }, Config.Jitter);
		zgui::slider_int(xorstr("jitter range").crypt_get(), 0, 180, Config.JitterRange);
		zgui::combobox(xorstr("freestanding").crypt_get(), { "off", xorstr("normal").crypt_get(), xorstr("desync").crypt_get() }, Config.FreestandType);
		if (Config.FreestandType == 0 || Config.FreestandType == 1) {
			zgui::combobox(xorstr("desync").crypt_get(), { xorstr("off").crypt_get(), xorstr("still").crypt_get(), xorstr("stretch").crypt_get(), xorstr("jitter").crypt_get(), xorstr("autism").crypt_get(), xorstr("test").crypt_get(), xorstr("fake angle").crypt_get() }, Config.DesyncType);
		}
		zgui::checkbox(xorstr("static desync").crypt_get(), Config.StaticDesync);
		zgui::checkbox(xorstr("on shot aa").crypt_get(), Config.OnShotAA);
		zgui::slider_int(xorstr("lby delta").crypt_get(), 0, 360, Config.LBYDelta);
		if (Config.FreestandType != 2 || Config.DesyncType != 5)
			zgui::key_bind(xorstr("desync invert key").crypt_get(), Config.DesyncInvertKey);
		zgui::end_groupbox();
	}
	zgui::begin_groupbox(xorstr("accuracy").crypt_get(), { 230 , 170 }, zgui::zgui_window_flags_none);
	{
		zgui::multi_combobox(xorstr("backtrack options").crypt_get(), std::vector< zgui::multi_select_item >{ { "shot", & Config.ShotBacktrack }, { "position", &Config.PosBacktrack }});
		zgui::checkbox(xorstr("delay shot").crypt_get(), Config.DelayShot);
		zgui::checkbox(xorstr("ignore limbs on-move").crypt_get(), Config.IgnoreLimbs);
		zgui::checkbox(xorstr("fake lag prediction (test)").crypt_get(), Config.PredictFakeLag);
		zgui::end_groupbox();
	}
}

void  Menu::vistab()
{
	zgui::next_column(-401, 39);

	zgui::begin_groupbox(xorstr("players").crypt_get(), { 230 , 270 }, zgui::zgui_window_flags_none);
	{
		zgui::checkbox(xorstr("enable#1").crypt_get(), Config.Esp);
		zgui::checkbox(xorstr("2d box").crypt_get(), Config.Box);
		zgui::colorpicker(xorstr("box color").crypt_get(), Config.BoxColor);
		zgui::checkbox(xorstr("draw name").crypt_get(), Config.Name);
		zgui::colorpicker(xorstr("font color").crypt_get(), Config.FontColor);
		zgui::checkbox(xorstr("draw weapon").crypt_get(), Config.Weapon);
		zgui::colorpicker(xorstr("wep color").crypt_get(), Config.WepColor);
		zgui::checkbox(xorstr("health bar").crypt_get(), Config.HealthBar);
		zgui::checkbox(xorstr("glow").crypt_get(), Config.Glow);
		zgui::colorpicker(xorstr("glow color").crypt_get(), Config.GlowColor);
		zgui::multi_combobox(xorstr("skeleton").crypt_get(), std::vector< zgui::multi_select_item >{ { "normal", & Config.Skeleton[0] }, { "backtrack", &Config.Skeleton[1] }});
		zgui::colorpicker(xorstr("skele color").crypt_get(), Config.SkeletonColor);
		zgui::checkbox(xorstr("chams").crypt_get(), Config.Chams);
		zgui::colorpicker(xorstr("chams color").crypt_get(), Config.XQZ);
		zgui::checkbox(xorstr("chams xqz").crypt_get(), Config.ChamsXQZ);
		zgui::colorpicker(xorstr("chams xqz color").crypt_get(), Config.ChamsColor);
		zgui::checkbox(xorstr("backtrack chams").crypt_get(), Config.Backtrackchams);
		zgui::colorpicker(xorstr("backtrack chams color").crypt_get(), Config.BacktrackchamsColor);
		zgui::checkbox(xorstr("local chams").crypt_get(), Config.LocalChams);
		zgui::colorpicker(xorstr("local chams color").crypt_get(), Config.LocalChamsColor);
		zgui::checkbox(xorstr("hand chams").crypt_get(), Config.HandChams);
		zgui::colorpicker(xorstr("hand chams color").crypt_get(), Config.HandChamsColor);
		zgui::checkbox(xorstr("fake lag model").crypt_get(), Config.FakeLagModel);
		zgui::colorpicker(xorstr("lag model chams color").crypt_get(), Config.FakeLagModelColor);
		zgui::checkbox(xorstr("flags").crypt_get(), Config.Flags);
		zgui::end_groupbox();
	}

	zgui::begin_groupbox(xorstr("world").crypt_get(), { 230 , 240 }, zgui::zgui_window_flags_none);
	{
		zgui::slider_int(xorstr("nightmode value").crypt_get(), 0, 100, Config.NightModeSlider);
		zgui::slider_int(xorstr("asus props").crypt_get(), 0, 100, Config.AsusProps);
		zgui::text(xorstr("skybox color").crypt_get());
		zgui::colorpicker(xorstr("skybox color").crypt_get(), Config.SkyboxColor);
		zgui::checkbox(xorstr("enable world modulation").crypt_get(), Config.NightMode);
		zgui::checkbox(xorstr("bomb esp").crypt_get(), Config.BombEsp);
		zgui::checkbox(xorstr("projectile esp").crypt_get(), Config.Projectiles);
		zgui::checkbox(xorstr("dropped weapon esp").crypt_get(), Config.DroppedWeapons);
		zgui::checkbox(xorstr("show enemies on radar").crypt_get(), Config.EngineRadar);
		zgui::checkbox(xorstr("show aimspot").crypt_get(), Config.ShowAimSpot);
		zgui::multi_combobox(xorstr("removals").crypt_get(), std::vector< zgui::multi_select_item >{ { "zoom", & Config.NoZoom }, { "scope", &Config.NoScope }, { "smoke", &Config.NoSmoke },{ "vis recoil", &Config.NoRecoil },{ "post processing", &Config.PostProcess } });

		zgui::end_groupbox();
	}

	zgui::same_line();
	zgui::next_column(230, 39);
	zgui::begin_groupbox(xorstr("others").crypt_get(), { 230 , 520 }, zgui::zgui_window_flags_none);
	{
		zgui::checkbox(xorstr("out of view indicator").crypt_get(), Config.OutOfView);
		if (Config.OutOfView)
		{
			zgui::colorpicker(xorstr("out of view color").crypt_get(), Config.OutOfViewColor);
			zgui::slider_int(xorstr("radius").crypt_get(), 0, 300, Config.OutOfViewRadius);
			zgui::slider_int(xorstr("size").crypt_get(), 0, 18, Config.OutOfViewSize);
		}
		zgui::checkbox(xorstr("antiaim arrows").crypt_get(), Config.Arrows);
		zgui::checkbox(xorstr("antiaim monitor").crypt_get(), Config.AntiaimMonitor);
		zgui::checkbox(xorstr("show impacts").crypt_get(), Config.Impacts);
		zgui::checkbox(xorstr("watermark").crypt_get(), Config.Watermark);
		zgui::checkbox(xorstr("draw hitboxes on hit").crypt_get(), Config.DrawHitboxes);
		zgui::colorpicker(xorstr("hitbox meme color").crypt_get(), Config.ImpactColor);
		if (Config.DrawHitboxes)
		{
			zgui::slider_int(xorstr("time").crypt_get(), 0, 10, Config.DrawHitboxTime);
		}
		zgui::combobox(xorstr("show weapon spread").crypt_get(), { "off", "square", "circle", "filled circle", "fading dots", "filled dots" }, Config.SpreadCrosshair);
		if (g_Menu.Config.SpreadCrosshair != 1)
			zgui::colorpicker(xorstr("spread color").crypt_get(), Config.SpreadColor);
		zgui::multi_combobox(xorstr("crosshair").crypt_get(), std::vector< zgui::multi_select_item >{ { "override", & Config.Crosshair[0] }, { "penetration", &Config.Crosshair[1] }, { "recoil", &Config.Crosshair[2] }, });
		zgui::combobox(xorstr("chams material").crypt_get(), { "metallic", "flat", "crystal blue", "pulse", "gold"}, Config.ChamsMaterial);
		zgui::combobox(xorstr("hand chams material").crypt_get(), { "metallic", "flat", "crystal blue", "pulse", "gold", "gloss", "crystal clear", "glass"}, Config.HandChamsMaterial);
		zgui::combobox(xorstr("shadow material").crypt_get(), {  "pulse", "gloss", "crystal clear", "glass", "chrome", "lightray"}, Config.ShadowMaterial);
		zgui::checkbox(xorstr("visual hitmarker").crypt_get(), Config.VisHitmarker);
		zgui::colorpicker(xorstr("hitmarker color").crypt_get(), Config.HitmarkerColor);
		if (Config.VisHitmarker)
		{
			zgui::slider_float(xorstr("expire time").crypt_get(), 0, 4, Config.ExpireTime);
			zgui::slider_float(xorstr("line size").crypt_get(), 0, 16, Config.MarkerSize);
			zgui::checkbox(xorstr("show damage").crypt_get(), Config.VisHitDmg);
		}
		zgui::checkbox(xorstr("desync invert indicator").crypt_get(), Config.AAInvertIndicators);
		zgui::end_groupbox();
	}
}

void  Menu::misctab()
{
	zgui::next_column(-401, 39);

	zgui::begin_groupbox(xorstr("1").crypt_get(), { 230 , 520 }, zgui::zgui_window_flags_none);
	{
		zgui::slider_int(xorstr("view fov").crypt_get(), 0, 150, Config.Fov);
		zgui::checkbox(xorstr("autojump").crypt_get(), Config.Bhop);
		zgui::checkbox(xorstr("autostrafe").crypt_get(), Config.AutoStrafe);
		zgui::combobox(xorstr("hitmarker").crypt_get(), { "off", "door", "metallic", "scaleform button", "warning", "bell", "custom" }, Config.Hitmarker);
		zgui::checkbox(xorstr("bullet tracer").crypt_get(), Config.Tracer);
		if (Config.Tracer) {
			zgui::slider_int(xorstr("tracer life").crypt_get(), 0, 5, Config.HitmarkerTime);
			zgui::combobox(xorstr("tracer material").crypt_get(), { "blueglow1", "bubble", "glow01", "physbeam", "purpleglow1", "purplelaser1", "radio", "white" }, Config.TracerMaterial);
		}
		zgui::checkbox(xorstr("fake duck").crypt_get(), Config.FakeDuck);
		zgui::key_bind(xorstr("fake duck key").crypt_get(), Config.FakeduckKey);
		zgui::checkbox(xorstr("p100 clantag").crypt_get(), Config.Clantag);
		zgui::end_groupbox();
	}
	zgui::same_line();
	zgui::next_column(230, 39);
	zgui::begin_groupbox(xorstr("2").crypt_get(), { 230 , 520 }, zgui::zgui_window_flags_none);
	{
		zgui::checkbox(xorstr("fakelag").crypt_get(), Config.FakeLagEnable);
		if (Config.FakeLagEnable) {
			zgui::checkbox(xorstr("while shooting").crypt_get(), Config.FakeLagShooting);
			zgui::combobox(xorstr("type").crypt_get(), { "static", "rand", "peek" }, Config.FakelagType);
			if (Config.FakelagType == 1)
				zgui::slider_int(xorstr("rand threshold").crypt_get(), 0, 6, Config.FakelagThreshold);
			zgui::slider_int(xorstr("limit").crypt_get(), 0, 14, Config.Fakelag);
		}
		zgui::combobox(xorstr("slowwalk").crypt_get(), { "off", "antiaim", "speed" }, Config.SlowWalk);
		if (Config.SlowWalk == 2) {
			zgui::slider_int(xorstr("speed").crypt_get(), 0, 100, Config.SlowWalkSlider);
		}
		zgui::checkbox(xorstr("thirdperson").crypt_get(), Config.Thirdperson);
		zgui::key_bind(xorstr("thirdperson key").crypt_get(), Config.ThirdpersonKey);
		zgui::checkbox(xorstr("show impacts").crypt_get(), Config.Impacts);
		zgui::checkbox(xorstr("aspect ratio").crypt_get(), Config.AspectRatio);
		if (Config.AspectRatio) {
			zgui::slider_int(xorstr("value").crypt_get(), 0, 100, Config.AspectRatioSlider);
		}
		zgui::combobox(xorstr("font").crypt_get(), { "small fonts", "visitor" }, Config.Font);
		zgui::end_groupbox();
	}
}
static bool unload;	
void  Menu::cfgtab()
{
	zgui::next_column(-401, 39);

	zgui::begin_groupbox(xorstr("configs").crypt_get(), { 230 , 520 }, zgui::zgui_window_flags_none);
	{
		zgui::combobox(xorstr("configs").crypt_get(), { "legit", "rage", "hvh", "gaming" }, Tabs.Config);
		zgui::end_groupbox();
	}
	zgui::same_line();
	zgui::next_column(230, 39);
	zgui::begin_groupbox(xorstr("functions").crypt_get(), { 230 , 520 }, zgui::zgui_window_flags_none);
	{
		auto cfilter = CHudChat::ChatFilters::CHAT_FILTER_NONE;
		if (Tabs.Config == 0 && zgui::button(xorstr("load").crypt_get(), { 50,20 })) {
			if (g_pEngine->IsInGame() && g_pEngine->IsConnected()) g_ChatElement->ChatPrintf(0, cfilter, xorstr(" \x02[gamerware] \x01""config loaded"" ").crypt_get());
			g_Config->Load();
		}
		else if (Tabs.Config == 1 && zgui::button(xorstr("load").crypt_get(), { 50,20 })) {
			if (g_pEngine->IsInGame() && g_pEngine->IsConnected()) g_ChatElement->ChatPrintf(0, cfilter, xorstr(" \x02[gamerware] \x01""config loaded"" ").crypt_get());
			g_Config->Load2();
		}
		else if (Tabs.Config == 2 && zgui::button(xorstr("load").crypt_get(), { 50,20 })) {
			if (g_pEngine->IsInGame() && g_pEngine->IsConnected()) g_ChatElement->ChatPrintf(0, cfilter, xorstr(" \x02[gamerware] \x01""config loaded"" ").crypt_get());
			g_Config->Load3();
		}
		else if (Tabs.Config == 3 && zgui::button(xorstr("load").crypt_get(), { 50,20 })) {
			if (g_pEngine->IsInGame() && g_pEngine->IsConnected()) g_ChatElement->ChatPrintf(0, cfilter, xorstr(" \x02[gamerware] \x01""config loaded"" ").crypt_get());
			g_Config->Load4();
		}

		if (Tabs.Config == 0 && zgui::button(xorstr(" save").crypt_get(), { 50,20 })) {
			if (g_pEngine->IsInGame() && g_pEngine->IsConnected()) g_ChatElement->ChatPrintf(0, cfilter, xorstr(" \x02[gamerware] \x01""config saved"" ").crypt_get());
			g_Config->Save();
		}
		else if (Tabs.Config == 1 && zgui::button(xorstr(" save").crypt_get(), { 50,20 })) {
			if (g_pEngine->IsInGame() && g_pEngine->IsConnected()) g_ChatElement->ChatPrintf(0, cfilter, xorstr(" \x02[gamerware] \x01""config saved"" ").crypt_get());
			g_Config->Save2();
		}
		else if (Tabs.Config == 2 && zgui::button(xorstr(" save").crypt_get(), { 50,20 })) {
			if (g_pEngine->IsInGame() && g_pEngine->IsConnected()) g_ChatElement->ChatPrintf(0, cfilter, xorstr(" \x02[gamerware] \x01""config saved"" ").crypt_get());
			g_Config->Save3();
		}
		else if (Tabs.Config == 3 && zgui::button(xorstr(" save").crypt_get(), { 50,20 })) {
			if (g_pEngine->IsInGame() && g_pEngine->IsConnected()) g_ChatElement->ChatPrintf(0, cfilter, xorstr(" \x02[gamerware] \x01""config saved"" ").crypt_get());
			g_Config->Save4();
		}
		zgui::text("");
		std::stringstream ss;
		ss << xorstr("build date: ").crypt_get() << __DATE__ << " " << __TIME__; 
		zgui::text(ss.str().c_str()); 
		zgui::end_groupbox();
	}
}

void  Menu::plisttab()
{
	zgui::next_column(-401, 39);

	zgui::begin_groupbox(xorstr("players").crypt_get(), { 470 , 520 }, zgui::zgui_window_flags_none);
	{

		for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
		{
			static auto sanitize = [](char* name) -> std::string {
				name[127] = '\0';

				std::string tmp(name);

				if (tmp.length() > 20) {
					tmp.erase(20, (tmp.length() - 20));
					tmp.append("...");
				}

				return tmp;
			};

			C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

			if (!pPlayerEntity
				|| !pPlayerEntity->IsAlive()
				|| pPlayerEntity == g::pLocalEntity
				|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			{
				continue;
			}

			Vector oldang = pPlayerEntity->GetEyeAngles();

			PlayerInfo_t player_info;
			if (g_pEngine->GetPlayerInfo(pPlayerEntity->EntIndex(), &player_info)) {
				std::string name = sanitize(player_info.szName);

				std::transform(name.begin(), name.end(), name.begin(), tolower);

				if (pPlayerEntity->IsDormant()) {
					name += " (dormant)";
				}

				std::string pitchmeme = "force baim (id:" + std::to_string(i) + ")";

				zgui::checkbox(name.c_str(), g::plist_player[i]);

				if (g::plist_player[i])
				{
					zgui::checkbox(pitchmeme.c_str(), Config.forcebaim[i]);
				}
			}
		}
		zgui::end_groupbox();
	}

}

void Menu::debugtab()
{
	zgui::next_column(-401, 39);

	zgui::begin_groupbox(xorstr("1").crypt_get(), { 470 , 520 }, zgui::zgui_window_flags_none);
	{
		zgui::text("welcome to the super secret debug menu");
		zgui::text("");
		zgui::checkbox(xorstr("debug show real (local server only)").crypt_get(), Config.DebugRealAngle);
	}
}