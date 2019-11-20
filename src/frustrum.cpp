#include "frustrum.h"
#include "math.h"

void renderer::NormalizePlane(renderer::Plane & plane)
{
    float mag;
    mag = sqrt(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);
    plane.a = plane.a / mag;
    plane.b = plane.b / mag;
    plane.c = plane.c / mag;
    plane.d = plane.d / mag;
    }

float renderer::DistanceToPoint(const renderer::Plane & plane, const renderer::Point & pt)
{
    return plane.a*pt.x + plane.b*pt.y + plane.c*pt.z + plane.d;
}

renderer::Halfspace renderer::ClassifyPoint(const renderer::Plane & plane, const renderer::Point & pt)
{
    float d;
    d = plane.a*pt.x + plane.b*pt.y + plane.c*pt.z + plane.d;
    if (d < 0) return NEGATIVE;
    if (d > 0) return POSITIVE;
    return ON_PLANE;
}

void renderer::ExtractPlanesD3D(renderer::Plane* p_planes, const math::Matrix4x4 & comboMatrix, bool normalize)
{
     // Left clipping plane
     p_planes[0].a = comboMatrix._41() + comboMatrix._11();
     p_planes[0].b = comboMatrix._42() + comboMatrix._12();
     p_planes[0].c = comboMatrix._43() + comboMatrix._13();
     p_planes[0].d = comboMatrix._44() + comboMatrix._14();
     // Right clipping plane        ()                  ()
     p_planes[1].a = comboMatrix._41() - comboMatrix._11();
     p_planes[1].b = comboMatrix._42() - comboMatrix._12();
     p_planes[1].c = comboMatrix._43() - comboMatrix._13();
     p_planes[1].d = comboMatrix._44() - comboMatrix._14();
     // Top clipping plane          ()                  ()
     p_planes[2].a = comboMatrix._41() - comboMatrix._21();
     p_planes[2].b = comboMatrix._42() - comboMatrix._22();
     p_planes[2].c = comboMatrix._43() - comboMatrix._23();
     p_planes[2].d = comboMatrix._44() - comboMatrix._24();
     // Bottom clipping plane       ()                  ()
     p_planes[3].a = comboMatrix._41() + comboMatrix._21();
     p_planes[3].b = comboMatrix._42() + comboMatrix._22();
     p_planes[3].c = comboMatrix._43() + comboMatrix._23();
     p_planes[3].d = comboMatrix._44() + comboMatrix._24();
     // Near clipping plane         ()                  ()
     p_planes[4].a = comboMatrix._41() + comboMatrix._31();
     p_planes[4].b = comboMatrix._42() + comboMatrix._32();
     p_planes[4].c = comboMatrix._43() + comboMatrix._33();
     p_planes[4].d = comboMatrix._44() + comboMatrix._34();
     // Far clipping plane          ()                  ()
     p_planes[5].a = comboMatrix._41() - comboMatrix._31();
     p_planes[5].b = comboMatrix._42() - comboMatrix._32();
     p_planes[5].c = comboMatrix._43() - comboMatrix._33();
     p_planes[5].d = comboMatrix._44() - comboMatrix._34();
     // Normalize the plane equations, if requested
     if (normalize == true)
     {
         NormalizePlane(p_planes[0]);
         NormalizePlane(p_planes[1]);
         NormalizePlane(p_planes[2]);
         NormalizePlane(p_planes[3]);
         NormalizePlane(p_planes[4]);
         NormalizePlane(p_planes[5]);
     }
}
