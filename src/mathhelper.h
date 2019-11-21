#pragma once

namespace math {

constexpr double PI = 3.14159265358979323846;

inline static float DegToRadians(float v)
{
    return v * (static_cast<float>(PI) / 180.0f);
}

inline static double DegToRadians(double v)
{
    return v * (PI / 180);
}

}
