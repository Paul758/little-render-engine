#include <cmath>
#include <stdexcept>

#include "engine/math/Quaternion.h"

#include "engine/math/Vec3.h"
#include "engine/math/Mat4.h"
#include "engine/math/MathUtils.h"

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

Quaternion Quaternion::lookRotation(const Vec3& forward, const Vec3& worldUp)
{
    if (forward.lengthSquared() < 0.000001f)
    {
        throw std::invalid_argument("lookRotation forward cannot be zero");
    }

    Vec3 f = forward.normalized();

    Vec3 r = Vec3::cross(f, worldUp).normalized();

    if (r.lengthSquared() < 0.000001f)
    {
        //Forward is almost parallelt to worldup
        //Choose another reference up axis
        Vec3 fallBackUp = std::abs(f.y) < 0.999f ? Vec3{0.0f, 1.0f, 0.0f} : Vec3{1.0f, 0.0f, 0.0f};
    }

    Vec3 u = Vec3::cross(r, f).normalized();

    Mat4 rotation = Mat4::identity();

    rotation.at(0, 0) = r.x;
    rotation.at(1, 0) = r.y;
    rotation.at(2, 0) = r.z;

    rotation.at(0, 1) = u.x;
    rotation.at(1, 1) = u.y;
    rotation.at(2, 1) = u.z;

    rotation.at(0, 2) = -f.x;
    rotation.at(1, 2) = -f.y;
    rotation.at(2, 2) = -f.z;

    return Quaternion::fromRotationMatrix(rotation);
}

Quaternion Quaternion::fromRotationMatrix(const Mat4& matrix)
{
    const float m00 = matrix.at(0, 0);
    const float m11 = matrix.at(1, 1);
    const float m22 = matrix.at(2, 2);

    const float trace = m00 + m11 + m22;

    Quaternion result;

    if (trace > 0.0f)
    {
        const float s = std::sqrt(trace + 1.0f) * 2.0f;

        result.w = 0.25f * s;
        result.x = (matrix.at(2, 1) - matrix.at(1, 2)) / s;
        result.y = (matrix.at(0, 2) - matrix.at(2, 0)) / s;
        result.z = (matrix.at(1, 0) - matrix.at(0, 1)) / s;

    }
    else if (m00 > m11 && m00 > m22)
    {
        const float s = std::sqrt(1.0f + m00 - m11 - m22) * 2.0f;

        result.w = (matrix.at(2, 1) - matrix.at(1, 2)) / s;
        result.x = 0.25f * s;
        result.y = (matrix.at(0, 1) + matrix.at(1, 0)) / s;
        result.z = (matrix.at(0, 2) + matrix.at(2, 0)) / s;

    }
    else if (m11 > m22)
    {
        const float s = std::sqrt(1.0f + m11 - m00 - m22) * 2.0f;

        result.w = (matrix.at(0, 2) - matrix.at(2, 0)) / s;
        result.x = (matrix.at(0, 1) + matrix.at(1, 0)) / s;
        result.y = 0.25f * s;
        result.z = (matrix.at(1, 2) + matrix.at(2, 1)) / s;
    }
    else
    {
        const float s = std::sqrt(1.0f + m22 - m00 - m11) * 2.0f;

        result.w = (matrix.at(1, 0) - matrix.at(0, 1)) / s;
        result.x = (matrix.at(0, 2) + matrix.at(2, 0)) / s;
        result.y = (matrix.at(1, 2) + matrix.at(2, 1)) / s;
        result.z = 0.25f * s;
    }

    return result.normalized();
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