#include <cmath>
#include <stdexcept>

#include "engine/math/Vec4.h"

Vec4::Vec4(float x, float y, float z, float w) :
    x(x), y(y), z(z), w(w)
{
}

Vec4 Vec4::operator+(const Vec4& other) const
{
    return {
        x + other.x,
        y + other.y,
        z + other.z,
        w + other.w
    };
}

Vec4 Vec4::operator-(const Vec4& other) const
{
    return {
        x - other.x,
        y - other.y,
        z - other.z,
        w - other.w
    };
}

Vec4 Vec4::operator*(float scalar) const
{
    return {
        x * scalar,
        y * scalar,
        z * scalar,
        w * scalar
    };
}

Vec4 Vec4::operator/(float scalar) const
{
    if(scalar == 0.0f)
    {
        throw std::runtime_error("Cannot divide a Vec3 by zero");
    }

    return {
        x / scalar,
        y / scalar,
        z / scalar,
        w / scalar
    };
}

Vec4& Vec4::operator+=(const Vec4& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;

    return *this;
}

Vec4& Vec4::operator-=(const Vec4& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;

    return *this;
}

Vec4 Vec4::operator-()
{
    return Vec4{-x, -y, -z, -w};
}

Vec4 Vec4::operator-() const
{
    return Vec4{-x, -y, -z, -w};
}

float Vec4::length() const
{
    return std::sqrt(x * x + y * y + z * z + w * w);
}

float Vec4::lengthSquared() const
{
    return x * x + y * y + z * z + w * w;
}


Vec4 Vec4::normalized() const 
{
    const float lengthSquared = x * x + y * y + z * z + w * w;

    constexpr float epsilon = 1e-12f;

    if(lengthSquared < epsilon)
    {
        throw std::runtime_error("Cannot normalize a zero-length vector");
    }

    const float inverseLength = 1.0f / std::sqrt(lengthSquared); 

    return *this * inverseLength;
}