#pragma once

#include <string>
#include "inipp.h"

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
	T GetValue(const std::string& section, const std::string& name)
	{
		T value;
		inipp::extract(m_iniFile.sections[section][name], value);
		return value;
	}

private:
	inipp::Ini<char> m_iniFile;
};
}
