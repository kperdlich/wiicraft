#pragma once

#include <math.h>

namespace math {

constexpr double PI = 3.14159265358979323846;

inline float DegToRadians(float v);
inline double DegToRadians(double v);
inline float Clamp(float value, float minValue, float maxValue);
inline float Min(float value1, float value2);
inline float Max(float value1, float value2);
inline float Mod(float x, float y);

inline float DegToRadians(float v)
{
    return v * (static_cast<float>(PI) / 180.0f);
}

inline double DegToRadians(double v)
{
    return v * (PI / 180);
}

inline float Clamp(float value, float minValue, float maxValue)
{
    return Max(minValue, Min(value, maxValue));
}

inline float Min(float value1, float value2)
{
    return (value1 < value2) ? (value1) : (value2);
}

inline float Max(float value1, float value2)
{
    return (value1 > value2) ? (value1) : (value2);
}

inline float Mod(float x, float y)
{
    return x - y * floor(x / y);
}

}
