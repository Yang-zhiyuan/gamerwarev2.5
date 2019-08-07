#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include "..\..\Hooks.h"

class AntiAim
{
public:
	
	void UpdateLBYBreaker(CUserCmd* usercmd);

	void re_work(ClientFrameStage_t stage);

	void fix_local_player_animations();
	bool invert;
	void NormalAA();
	void OldFreestandingdesync();
	void awresolverbreaker();
	void Desync();
	void FakeDuck(CUserCmd* cmd);
	void SlowWalk(CUserCmd* cmd);
	void niggacheese();
	void LegitOnCreateMove();
	void OnCreateMove();
	
private:
	
};
extern AntiAim g_AntiAim;