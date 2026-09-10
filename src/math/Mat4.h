#pragma once

#include <cstddef>

#include "Vec3.h"
#include "Quaternion.h"


class Mat4
{
private:
    float values[16] = {};

public:
    Mat4();

    static Mat4 identity();
    static Mat4 translate(const Vec3& translation);
    static Mat4 scale(const Vec3& scale);
    static Mat4 rotate(const Vec3& rotation);
    static Mat4 rotateX(float rotation);
    static Mat4 rotateY(float rotation);
    static Mat4 rotateZ(float rotation);

    static Mat4 perspective(float fovRadians, float aspectRatio, float nearPlane, float farPlane);
    static Mat4 orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);

    static Mat4 lookAt(const Vec3& position, const Vec3& target, const Vec3& worldUp);
    static Mat4 view(const Vec3& position, const Vec3& forward, const Vec3& right, const Vec3& up);

    static Mat4 fromQuaternion(const Quaternion& q);

    Mat4 operator*(const Mat4& other) const;
    Vec3 operator*(const Vec3& other) const;

    float& at(std::size_t row, std::size_t column);
    float at(std::size_t row, std::size_t column) const;

    const float* data() const;
    float* data();

    Mat4& setRow(size_t numRow, const Vec3& rowVec);
    Mat4& setColumn(size_t numColumn, const Vec3& columnVec);

    Vec3 getRow(size_t row) const;
};