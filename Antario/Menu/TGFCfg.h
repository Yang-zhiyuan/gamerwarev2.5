#pragma once

#include "ShlObj_core.h"

template< typename T >
class ConfigItem
{
	std::string name;
	T* value;
public:
	ConfigItem(std::string name, T* value)
	{
		this->name = name;
		this->value = value;
	}
};

template< typename T >
class ConfigValue
{
public:
	ConfigValue(std::string name_, T* value_)
	{
		name = name_;
		value = value_;
	}

	std::string name;
	T* value;
};

class CConfig
{
protected:
	std::vector< ConfigValue< int >* > ints;
	std::vector< ConfigValue< bool >* > bools;
	std::vector< ConfigValue< float >* > floats;
private:
	void SetupValue(int&, int, std::string);
	void SetupValue(bool&, bool, std::string);
	void SetupValue(float&, float, std::string);
	//void SetupValue(ColorV2&, float, std::string);
	void SetupValue(zgui::color& value, float def, std::string name);
	void SetupValue(bool*, bool, int, std::string);
public:
	CConfig()
	{
		Setup();
	}

	void Setup();

	void Save();
	void Load();
	void Save2();
	void Load2();
	void Save3();
	void Load3();
	void Save4();
	void Load4();
};

extern CConfig* g_Config;