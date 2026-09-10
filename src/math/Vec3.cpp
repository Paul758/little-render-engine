#include "Vec3.h"

#include <cmath>
#include <stdexcept>

Vec3::Vec3(float x, float y, float z) :
    x(x), y(y), z(z)
{
}

Vec3 Vec3::operator+(const Vec3& other) const
{
    return {
        x + other.x,
        y + other.y,
        z + other.z
    };
}

Vec3 Vec3::operator-(const Vec3& other) const
{
    return {
        x - other.x,
        y - other.y,
        z - other.z
    };
}

Vec3 Vec3::operator*(float scalar) const
{
    return {
        x * scalar,
        y * scalar,
        z * scalar
    };
}

Vec3 Vec3::operator/(float scalar) const
{
    if(scalar == 0.0f)
    {
        throw std::runtime_error("Cannot divide a Vec3 by zero");
    }

    return {
        x / scalar,
        y / scalar,
        z / scalar
    };
}

Vec3& Vec3::operator+=(const Vec3& other)
{
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
}

Vec3& Vec3::operator-=(const Vec3& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
}

Vec3 Vec3::operator-()
{
    return Vec3{-x, -y, -z};
}

Vec3 Vec3::operator-() const
{
    return Vec3{-x, -y, -z};
}

float Vec3::length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

float Vec3::lengthSquared() const
{
    return x * x + y * y + z * z;
}


Vec3 Vec3::normalized() const 
{
    const float lengthSquared = x * x + y * y + z * z;

    constexpr float epsilon = 1e-12f;

    if(lengthSquared < epsilon)
    {
        throw std::runtime_error("Cannot normalize a zero-length vector");
    }

    const float inverseLength = 1.0f / std::sqrt(lengthSquared); 

    return *this * inverseLength;
}

float Vec3::dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x 
         + a.y * b.y 
         + a.z * b.z;
}

Vec3 Vec3::cross(const Vec3& a, const Vec3& b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}
