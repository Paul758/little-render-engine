#include <cmath>
#include <stdexcept>

#include "math/Quaternion.h"

#include "math/Vec3.h"
#include "MathUtils.h"

Quaternion::Quaternion(float w, float x, float y, float z)
    : w(w), x(x), y(y), z(z)
{

}

Quaternion Quaternion::identity()
{
    return Quaternion{
        1.0f,
        0.0f,
        0.0f,
        0.0f
    };
}

float Quaternion::length() const
{
    return std::sqrt(w * w + x * x + y * y + z * z);
}

Quaternion Quaternion::normalized() const
{
    float qLength = this->length();

    if (qLength == 0.0f)
    {
        throw std::runtime_error("Cannot normalize a zero-length quaternion");
    }

    return Quaternion{w / qLength, x / qLength, y / qLength, z / qLength};
}

void Quaternion::normalize()
{
    float qLength = this->length();

    if (qLength == 0.0f)
    {
        throw std::runtime_error("Cannot normalize a zero-length quaternion");
    }

    this->w = w / qLength;
    this->x = x / qLength;
    this->y = y / qLength;
    this->z = z / qLength;
}

Quaternion Quaternion::fromAxisAngle(const Vec3& axis, float angleDegrees)
{
    Vec3 normalizedAxis = axis.normalized();

    float radians = Math::radians(angleDegrees);
    float halfAngle = radians * 0.5f;

    float s = std::sin(halfAngle);
    float c = std::cos(halfAngle);

    return Quaternion{c, normalizedAxis.x * s, normalizedAxis.y * s, normalizedAxis.z * s};
}

Quaternion Quaternion::fromEuler(const Vec3& rotationDegrees)
{
    Quaternion xRotation = Quaternion::fromAxisAngle(Vec3{1.0f, 0.0f, 0.0f}, rotationDegrees.x);
    Quaternion yRotation = Quaternion::fromAxisAngle(Vec3{0.0f, 1.0f, 0.0f}, rotationDegrees.y);
    Quaternion zRotation = Quaternion::fromAxisAngle(Vec3{0.0f, 0.0f, 1.0f}, rotationDegrees.z);

    return (zRotation * yRotation * xRotation).normalized();
}

Quaternion Quaternion::operator*(const Quaternion& other) const
{
    return Quaternion{
        w * other.w - x * other.x - y * other.y - z * other.z,

        w * other.x + x * other.w + y * other.z - z * other.y,

        w * other.y - x * other.z + y * other.w + z * other.x,

        w * other.z + x * other.y - y * other.x + z * other.w
    };
}

Quaternion Quaternion::conjugate() const
{
    return Quaternion{w, -x, -y, -z};
}

Vec3 Quaternion::rotate(const Vec3& vector) const
{
    Quaternion q = normalized();

    Quaternion vectorQuaternion{0.0f, vector.x, vector.y, vector.z};

    Quaternion result = q * vectorQuaternion * q.conjugate();

    return Vec3{result.x, result.y, result.z};
}