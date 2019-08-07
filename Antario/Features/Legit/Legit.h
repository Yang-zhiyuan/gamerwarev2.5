#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include "..\..\SDK\IGameEvent.h"
#include <deque>

class c_aimbot {
private:
	float aim_smooth;
	float aim_fov;
	int hitbox_id;
	Vector angle;
public:
	void run(CUserCmd* user_cmd);
	
};

extern c_aimbot g_legitbot;