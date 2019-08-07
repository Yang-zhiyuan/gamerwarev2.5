#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "../../SDK/IGameEvent.h"

struct TestPos
{
	Vector Pos;
	float SimulationTime;
};



class ESP
{
public:
	int timer = 0;
	int size = 0;
	void speclist();
	void Arrows();
	void test(C_BaseEntity* pEnt);
	void PenetrationCrosshair();
	void hitmarker();
	void draw_spread();
	void SpreadCrosshair();
	void grenade_pred();
	void dropped_weapons(C_BaseEntity* entity);
	void projectiles(C_BaseEntity* entity);
	void bomb_esp(C_BaseEntity* entity, bool defusing);
	void newbombtimer(C_BaseEntity* ent);
	void PredictGrenade(C_BaseEntity* pLocal);
	void bomb_esp(C_BaseEntity* entity);
	void world(C_BaseEntity* entity);
	void DrawHitboxes(C_BaseEntity* pEntity, int r, int g, int b, int a, float duration);
	void glow();
	void watermark();
	void RunSpectators();
	void DrawArrow(C_BaseEntity* pEnt);
	void Render();
private:
	struct
	{
		int left, top, right, bottom;
	}Box;



	void weapons(C_BaseCombatWeapon* e);

	struct EntityBoxInfo_t
	{
		int x, y, w, h, alpha, alpha_anim;
		float dormancy = 0.f;

		int hp = -1;
		int hpDifference = 0;

		float hpDiffTime = 0.f;

		C_BaseEntity* pEnt;
	};
	Color color;
	Color textcolor;
	Color wepcolor;
	Color skelecolor;
	DWORD font;
	DWORD niggafont;
	DWORD idrinkpee;
	DWORD weaponfont;
	DWORD skeetfont;
	DWORD supremacy;
	DWORD flagfont;
	DWORD indicatorfont;
	int offsetY;
	
	int		 box_hp;
	int		 box_hpDifference;
	float	 box_hpDiffTime;

	void BoundBox(C_BaseEntity* pEnt);
    void RenderBox();
	void RenderWeaponName(C_BaseEntity* pEnt);
    void RenderName(C_BaseEntity* pEnt, int iterator);
	void RenderHealth(C_BaseEntity* pEnt);
	void DrawHealthBar(EntityBoxInfo_t inf);
	bool GetBox(C_BaseEntity* m_entity, EntityBoxInfo_t& box, bool dynamic);
	void Flags(C_BaseEntity* pEnt);
	void RenderHitboxPoints(C_BaseEntity* pEnt);
	void RenderSkeleton(C_BaseEntity* pEnt);
};
extern ESP g_ESP;