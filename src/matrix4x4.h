#pragma once

#include "wiidefines.h"

namespace math {
// The elements of the 4x4 matrix are stored in
// row-major order.
class Matrix4x4
{
public:

    Matrix4x4();	
    Matrix4x4(float matrix[4][4]);
    Matrix4x4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33);
    ~Matrix4x4() = default;
    Matrix4x4(const Matrix4x4&) = default;
    Matrix4x4(Matrix4x4&&) = default;
    Matrix4x4& operator=(const Matrix4x4&) = default;
    Matrix4x4& operator=(Matrix4x4&&) = default;

    inline void SetIdentity();

    inline Matrix4x4 operator+(const Matrix4x4& other) const;
    inline Matrix4x4 operator*(const Matrix4x4& other) const;
    inline Matrix4x4 operator*(float value) const;
    inline void operator+=(const Matrix4x4& other);
    inline void operator*=(const Matrix4x4& other);
    inline bool operator==(const Matrix4x4& other) const;
    inline bool operator!=(const Matrix4x4& other) const; 

    inline float _11() const;
    inline float _12() const;
    inline float _13() const;
    inline float _14() const;

    inline float _21() const;
    inline float _22() const;
    inline float _23() const;
    inline float _24() const;

    inline float _31() const;
    inline float _32() const;
    inline float _33() const;
    inline float _34() const;

    inline float _41() const;
    inline float _42() const;
    inline float _43() const;
    inline float _44() const;

private:
    float mMatrix[4][4];
};

inline void Matrix4x4::SetIdentity()
{    
    mMatrix[0][0] = 1; mMatrix[0][1] = 0;  mMatrix[0][2] = 0;  mMatrix[0][3] = 0;
    mMatrix[1][0] = 0; mMatrix[1][1] = 1;  mMatrix[1][2] = 0;  mMatrix[1][3] = 0;
    mMatrix[2][0] = 0; mMatrix[2][1] = 0;  mMatrix[2][2] = 1;  mMatrix[2][3] = 0;
    mMatrix[3][0] = 0; mMatrix[3][1] = 0;  mMatrix[3][2] = 0;  mMatrix[3][3] = 1;
}

inline Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const
{
    Matrix4x4 result;
    for (uint32_t x = 0; x < 4; ++x)
    {
        for (uint32_t y = 0; y < 4; ++y)
        {
            result.mMatrix[x][y] = mMatrix[x][y] + other.mMatrix[x][y];
        }
    }
    return result;
}

inline Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
    Matrix4x4 result;
    for(uint32_t i = 0; i < 4; ++i)
    {
        for(uint32_t j = 0; j < 4; ++j)
        {
            for(uint32_t k = 0; k < 4; ++k)
            {
                result.mMatrix[i][j] += mMatrix[i][k] * other.mMatrix[k][j];
            }
        }
    }
    return result;
}

inline Matrix4x4 Matrix4x4::operator*(float value) const
{
    Matrix4x4 result;
    for (uint32_t x = 0; x < 4; ++x)
    {
        for (uint32_t y = 0; y < 4; ++y)
        {
            result.mMatrix[x][y] = mMatrix[x][y] * value;
        }
    }
    return result;
}

inline void Matrix4x4::operator+=(const Matrix4x4& other)
{
    *this = *this + other;
}

inline void Matrix4x4::operator*=(const Matrix4x4& other)
{
    *this = *this * other;
}

inline bool Matrix4x4::operator==(const Matrix4x4& other) const
{
    for (uint32_t x = 0; x < 4; ++x)
    {
        for (uint32_t y = 0; y < 4; ++y)
        {
            if (mMatrix[x][y] != other.mMatrix[x][y]) // maybe add a equals method with a custom tolerance?
            {
                return false;
            }
        }
    }
    return true;
}

inline bool Matrix4x4::operator!=(const Matrix4x4& other) const
{
    return !(*this == other);
}

inline float Matrix4x4::_11() const {return mMatrix[0][0];}

inline float Matrix4x4::_12() const {return mMatrix[0][1];}

inline float Matrix4x4::_13() const {return mMatrix[0][2];}

inline float Matrix4x4::_14() const {return mMatrix[0][3];}

inline float Matrix4x4::_21() const {return mMatrix[1][0];}

inline float Matrix4x4::_22() const {return mMatrix[1][1];}

inline float Matrix4x4::_23() const {return mMatrix[1][2];}

inline float Matrix4x4::_24() const {return mMatrix[1][3];}

inline float Matrix4x4::_31() const {return mMatrix[2][0];}

inline float Matrix4x4::_32() const {return mMatrix[2][1];}

inline float Matrix4x4::_33() const {return mMatrix[2][2];}

inline float Matrix4x4::_34() const {return mMatrix[2][3];}

inline float Matrix4x4::_41() const {return mMatrix[3][0];}

inline float Matrix4x4::_42() const {return mMatrix[3][1];}

inline float Matrix4x4::_43() const {return mMatrix[3][2];}

inline float Matrix4x4::_44() const {return mMatrix[3][3];}
}
