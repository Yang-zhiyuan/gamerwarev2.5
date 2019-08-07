#pragma once
#include <Windows.h>
#include <string>
#include <filesystem>
#include "../gui/zgui.h"

namespace fs = std::experimental::filesystem;

struct ColorV2
{
	float red, green, blue, alpha;
};

enum 
{
	check_box,
	slider,
	combo_box,
	multi_box
};

class Menu
{
public:
	bool menuOpened;
	void Render();

	struct
	{
		bool Aimbot;
		bool Hitchance;
		int HitchanceValue;
		int Mindmg;
		bool Resolver;
		int ThirdpersonKey;
		bool AntiaimMonitor;
		int LegitAAType;
		int BodyScale;
		int HeadScale;
		bool MultiPoint;
		bool DelayShot;
		bool IgnoreLimbs;
		bool Autostop;
		bool FixShotPitch;
		bool PosBacktrack;
		bool ShotBacktrack;
		bool BaimLethal;
		bool BaimPitch;
		bool BaimInAir;
		bool BaimAlways;
		bool Clantag;
		bool BaimAfterEnable;
		int BaimSlider;
		bool Info;

		bool Hitboxes;
		bool Head;
		bool Neck; 
		bool Chest;
		bool Pelvis;
		bool Legs;
		bool Arms;

		bool Antiaim;
		bool DesyncAngle;
		int Jitter;
		int DesyncType;
		int	JitterRange;
		int AddRange;
		int	Fakelag;
		bool FakeLagOnPeek;
		bool ChokeShotOnPeek;
		bool Retard;
		bool Freestand;
		bool DesyncTest;
		bool StaticDesync;
		bool AtTarget;
		bool InfDuck;
		bool FakeDuck;
		bool EgoBoost;
		int FakeduckKey;
		int DesyncInvertKey;
		bool BackAA;
		int FreestandType;
		bool RagdollMeme;
		int SpreadDotSize;
		int RagdollMemeSlider;
		bool BombEsp;
		bool Projectiles;
		bool DroppedWeapons;
		bool GrenadePred;
		bool Impacts;
		bool Tracer;
		int FakelagThreshold;
		bool DisableOcclusion;
		bool NightMode;
		bool FakeLagEnable;
		int NightModeSlider;
		bool SkyboxMod;
		zgui::color SkyboxColor{ 237, 16, 68,255 };
		zgui::color BacktrackchamsColor{ 237, 16, 68,255 };
		ColorV2 PlayerIndicatorColor{ 237.f, 16.f, 68.f,255.f };
		bool PostProcess;
		zgui::color WepColor = { 255,255,255,255 };
		bool AspectRatio;
		int AspectRatioSlider;
		bool ViewFOV;
		int ViewFOVSlider;
		int net_flag;
		int BaimLethalSlider;
		bool ShowAimSpot;
		bool forcebaim[65];
		int GlowStyle;
		bool Unload;
		int ShadowMaterial;
		int	net_fjitter;
		int	net_floss;
		bool FakeChams;
		int Yaw;
		int FakelagType;
		bool OutOfView;
		bool PitchMeme;
		bool DrawHitboxes;
		int DrawHitboxTime;
		int ExploitKey;
		int ResolverType;
		int additive;
		bool DebugRealAngle;
		bool ChamsEnemiesOnly;
		bool OnShotAA;
		bool PredictFakeLag;
		bool GlowSelf;
		int HitmarkerTime;
		bool Crosshair[3] = { false,false,false };
		zgui::color MenuC{ 237, 16, 68, 255 };
		zgui::color GlowSelfColor{ 237, 16, 68,255 };
		zgui::color FakeLagModelColor{ 237, 16, 68,255 };

		bool Esp;
		int Font;
		zgui::color FontColor = { 255,255,255,255 };
		zgui::color ImpactColor = { 255,255,255,255 };
		zgui::color GlowColor = { 255,255,255,255};
		zgui::color TeamGlowColor = { 255,255,255,255 };
		zgui::color HandChamsColor = { 255,255,255,255 };
		zgui::color WeaponChamsColor = { 255,255,255,255 };
		int GlowAlpha;
		int ChamsMaterial;
		int KnifeModel;
		bool Name;
		bool OverrideKnife;
		bool FixFeet;
		bool AAInvertIndicators;
		bool FakeLagShooting;
		bool TeamGlow;
		int HealthVal;
		bool HandChams;
		bool Weapon;
		int AsusProps;
		bool LocalChams;
		bool WeaponChams;
		int HandChamsMaterial;
		bool Box;
		zgui::color BoxColor = { 255,255,255,255 };
		bool HealthBar;
		bool Skeleton[2] = {false,false};
		zgui::color SkeletonColor = { 255,255,255,255 };
		bool HitboxPoints;
		bool Chams;
		bool AutoBaim;
		bool Backtrackchams;
		zgui::color ChamsColor{ 0, 170, 255, 255 };
		zgui::color OutOfViewColor{ 255, 255, 255, 255 };
		zgui::color XQZ { 173, 244, 5, 255 };
		zgui::color LocalChamsColor = { 255,255,255,255 };
		zgui::color FakeHitbox = { 255,255,255,255 };
		bool NoZoom;
		bool Glow;
		bool ChamsXQZ;
		bool avasterisbraindead;
		bool breakinglby[65] = {};
		bool espbaim[65];
		bool LocalFake;
		float ExpireTime = 0.5f;
		float MarkerSize = 12.f;
		bool VisHitmarker;
		bool Animfix;
		bool Autoscope;
		int LBYDelta;
		bool ForcedConvars[2] = {};
		bool Watermark;
		int LBYDeltaLegit;
		bool NoScope;
		bool NoRecoil;
		bool NoSmoke;
		bool Spectators;
		bool VisHitDmg;
		bool EngineRadar;
		bool AlwaysIndicate;
		int Fov;
		int OutOfViewRadius;
		int OutOfViewSize;
		bool Legitbot;
		bool ModulateUpdate;
		int SlowWalkSlider;
		int SlowWalk;
		int BaimKey;
		bool FakeLagModel;
		bool SoundESP;
		bool Extrapolation;
		bool Flags;
		bool Arrows;
		int SpreadCrosshair;
		int TracerMaterial;
		bool AimStepTest;
		int AimStepValue;
		int LegitFOV;
		int LegitSmooth;
		bool LegitAimbot;
		zgui::color SpreadColor = { 255,255,255,255 };
		zgui::color HitmarkerColor = { 255,255,255,255 };
		int RCSamount;
		int RCStype;
		bool LegitAA;
		bool RCS;
		bool Thirdperson;
		bool Bhop;
		bool AutoStrafe;
		bool LegitBacktrack;
		int Hitmarker;
		int StandaloneRCStype;
		int StandaloneRCSSlider;
		bool AimRCS;
		bool StandaloneRCS;
		bool Ak47meme;
		bool LegitAAStatic;
		int colorpicker;
		int	Test;
	}Config;

	struct {
		bool legitbot, ragebot, visual, misc, config, plist, debug;
		int Config;
	}Tabs;

private:
	struct
	{
		float x = 0.f, y = 0.f;
	}Pos; // lol

	void legittab();

	void ragetab();

	void vistab();

	void misctab();

	void cfgtab();

	void plisttab();

	void debugtab();


	enum
	{
		check_box,
		slider,
		combo_box,
		multi_box
	};

	int ControlsX;
	int GroupTabBottom;
	int OffsetY;
	int OldOffsetY;
	int TabOffset;
	int SubTabOffset;
	float SubTabSize; // cpp fuckin sux had to make this a float or the whole thing crashes
	float TabSize;
	int GroupTabPos[4];

	int TabNum = 0;
	int SubTabNum = 0;
	int PreviousControl = -1;

	/*void Tab(std::string name);
	void SubTab(std::string name);
	void Text(std::string name);
	void CheckBox(std::string name, bool* item);
	void Slider(int max, std::string name, int* item);
	void ComboBox(std::string name, std::vector< std::string > itemname, int* item);
	void MultiComboBox(std::string name, std::vector< std::string > itemname, bool* item);*/
	void ColorPicker(std::string name, ColorV2& item);
};

extern Menu g_Menu;