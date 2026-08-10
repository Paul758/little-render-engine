#pragma once

struct Vec2
{
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;
    Vec2(float x, float y);

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(float scalar) const;
    Vec2 operator/(float scalar) const;

    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);

    Vec2 operator-();

    float length() const;
    Vec2 normalized() const;

    static float dot(const Vec2& a, const Vec2& b);
    static Vec2 cross(const Vec2&a, const Vec2& b);
};