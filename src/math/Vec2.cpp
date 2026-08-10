#include "Vec2.h"

#include <cmath>
#include <stdexcept>

Vec2::Vec2(float x, float y) :
    x(x), y(y)
{
}

Vec2 Vec2::operator+(const Vec2& other) const
{
    return {
        x + other.x,
        y + other.y
    };
}

Vec2 Vec2::operator-(const Vec2& other) const
{
    return {
        x - other.x,
        y - other.y
    };
}

Vec2 Vec2::operator*(float scalar) const
{
    return {
        x * scalar,
        y * scalar
    };
}

Vec2 Vec2::operator/(float scalar) const
{
    if(scalar == 0.0f)
    {
        throw std::runtime_error("Cannot divide a Vec3 by zero");
    }

    return {
        x / scalar,
        y / scalar
    };
}

Vec2& Vec2::operator+=(const Vec2& other)
{
    x += other.x;
    y += other.y;

    return *this;
}

Vec2& Vec2::operator-=(const Vec2& other)
{
    x -= other.x;
    y -= other.y;

    return *this;
}

Vec2 Vec2::operator-()
{
    return Vec2{-x, -y};
}

float Vec2::length() const
{
    return std::sqrt(x * x + y * y);
}

Vec2 Vec2::normalized() const 
{
    const float lengthSquared = x * x + y * y;

    constexpr float epsilon = 1e-12f;

    if(lengthSquared < epsilon)
    {
        throw std::runtime_error("Cannot normalize a zero-length vector");
    }

    const float inverseLength = 1.0f / std::sqrt(lengthSquared); 

    return *this * inverseLength;
}

float Vec2::dot(const Vec2& a, const Vec2& b)
{
    return a.x * b.x 
         + a.y * b.y;
}
