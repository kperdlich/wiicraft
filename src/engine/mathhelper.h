#pragma once

#include <math.h>

namespace math {

constexpr double PI = 3.14159265358979323846;

template<typename T>
inline T DegToRadians(T v);

template<typename T>
inline T Clamp(T value, T minValue, T maxValue);

template<typename T>
inline T Min(T value1, T value2);

template<typename T>
inline T Max(T value1, T value2);

template<typename T>
inline T Mod(T x, T y);

template <typename T>
inline int Sign(T val);

template<typename T>
inline T DegToRadians(T v)
{
    return v * (PI / 180);
}

template<typename T>
inline T Clamp(T value, T minValue, T maxValue)
{
    return Max(minValue, Min(value, maxValue));
}

template<typename T>
inline T Min(T value1, T value2)
{
    return (value1 < value2) ? (value1) : (value2);
}

template<typename T>
inline T Max(T value1, T value2)
{
    return (value1 > value2) ? (value1) : (value2);
}

template<typename T>
inline T Mod(T x, T y)
{
    return x - y * floor(x / y);
}

template <typename T>
inline int Sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

}
