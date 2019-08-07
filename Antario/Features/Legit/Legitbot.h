#pragma once

#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include <deque>


struct legit_backtrackdata
{
	float simtime;
	Vector hitboxPos;
};


class Legitbot
{
public:
	void backtrack(CUserCmd* cmd);
	void RecoilControl();
	void AntiAim();
	void CreateMove(CUserCmd* cmd);
private:
};
extern Legitbot g_Legitbot;