#pragma once

#include "matrix4x4.h"

namespace renderer {

struct Plane
{
    float a, b, c, d;
};

struct Point
{
    float x, y, z;
};

enum Halfspace
{
     NEGATIVE = -1,
     ON_PLANE = 0,
     POSITIVE = 1,
};

void NormalizePlane(Plane & plane);
float DistanceToPoint(const Plane & plane, const Point & pt);
Halfspace ClassifyPoint(const Plane & plane, const Point & pt);
void ExtractPlanesD3D(Plane * p_planes, const math::Matrix4x4 & comboMatrix, bool normalize);

}

