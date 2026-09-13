#pragma once

#include "math/Mat4.h"
#include "math/Vec3.h"

class Mat4;

class Quaternion
{
public:
    float w = 1.0f;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Quaternion() = default;

    Quaternion(float w, float x, float y, float z);

    static Quaternion identity();

    float length() const;

    Quaternion normalized() const;

    void normalize();

    static Quaternion fromAxisAngle(const Vec3& axis, float angleDegrees);
    static Quaternion fromEuler(const Vec3& rotationDegrees);
    static Quaternion fromRotationMatrix(const Mat4& matrix);
    static Quaternion lookRotation(const Vec3& forward, const Vec3& up);

    Quaternion operator*(const Quaternion& other) const;

    Quaternion conjugate() const;

    Vec3 rotate(const Vec3& vector) const;

};