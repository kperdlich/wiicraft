#include "matrix4x4.h"

math::Matrix4x4::Matrix4x4(float matrix[4][4])
{
    for (uint32_t row = 0; row < 4; ++row)
    {
        for (uint32_t column = 0; column < 4; ++column)
        {
            mMatrix[row][column] = matrix[row][column];
        }
    }
}

math::Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03,
                           float m10, float m11, float m12, float m13,
                           float m20, float m21, float m22, float m23,
                           float m30, float m31, float m32, float m33)
{
    mMatrix[0][0] = m00; mMatrix[0][1] = m01;  mMatrix[0][2] = m02;  mMatrix[0][3] = m03;
    mMatrix[1][0] = m10; mMatrix[1][1] = m11;  mMatrix[1][2] = m12;  mMatrix[1][3] = m13;
    mMatrix[2][0] = m20; mMatrix[2][1] = m21;  mMatrix[2][2] = m22;  mMatrix[2][3] = m23;
    mMatrix[3][0] = m30; mMatrix[3][1] = m31;  mMatrix[3][2] = m32;  mMatrix[3][3] = m33;
}
