#pragma once

#include <sstream>
#include <string>

template<typename T>
std::string ToString(T value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}
