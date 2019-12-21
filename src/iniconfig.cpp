#include <fstream>
#include <stdexcept>
#include "iniconfig.h"
#include "core.h"

void core::IniConfig::Parse(const std::string& file)
{
	std::ifstream iniFile(file);
	m_iniFile.parse(iniFile);
    ASSERT_TEXT(m_iniFile.errors.size() == 0, "Error while parsing Game.ini!");
	m_iniFile.default_section(m_iniFile.sections["Default"]);
	m_iniFile.interpolate();
}
