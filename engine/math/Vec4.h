#pragma once

struct Vec4
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;

    Vec4() = default;
    Vec4(float x, float y, float z, float w);

    Vec4 operator+(const Vec4& other) const;
    Vec4 operator-(const Vec4& other) const;
    Vec4 operator*(float scalar) const;
    Vec4 operator/(float scalar) const;

    Vec4& operator+=(const Vec4& other);
    Vec4& operator-=(const Vec4& other);

    bool operator==(const Vec4&) const = default;

    Vec4 operator-();
    Vec4 operator-() const;

    float length() const;
    float lengthSquared() const;
    Vec4 normalized() const;
};