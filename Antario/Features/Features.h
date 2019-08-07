#pragma once

template<typename T>
class singleton
{
protected:
	singleton() {}
	~singleton() {}

	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;

	singleton(singleton&&) = delete;
	singleton& operator=(singleton&&) = delete;

public:
	static T& get()
	{
		static T inst{};
		return inst;
	}
};

#include "Aimbot\EnginePrediction.h"
#include "Visuals\ESP.h"
#include "Visuals\Chams.h"
#include "Misc\Misc.h"
#include "Resolver\Resolver.h"
#include "AntiAim\AntiAim.h"
#include "Aimbot\Aimbot.h"
#include "Legit\Legitbot.h"
#include "Legit\Legit.h"
