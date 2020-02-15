#pragma once

#include <string>
#include "inipp.h"
#include "core.h"

namespace core {
class IniConfig
{
public:
    IniConfig() = default;
	IniConfig(const IniConfig&) = delete;
	IniConfig(IniConfig&&) = delete;
	void operator=(const IniConfig&) = delete;
	void operator=(IniConfig&&) = delete;

	void Parse(const std::string& file);

	template<typename T>
    T GetValue(const std::string& section, const std::string& name, const T& defaultValue)
	{
		T value;
        if (inipp::extract(m_iniFile.sections[section][name], value))
        {
            return value;
        }
        return defaultValue;
	}

    template<typename T>
    T GetValue(const std::string& section, const std::string& name)
    {
        T value;
        ASSERT_TEXT(inipp::extract(m_iniFile.sections[section][name], value), "Game.ini Error: Could not get value for [%s] %s", section.c_str(), name.c_str());
        return value;
    }

private:
	inipp::Ini<char> m_iniFile;
};
}
