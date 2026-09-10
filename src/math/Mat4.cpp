#include <cmath>
#include <stdexcept>

#include "Mat4.h"
#include "MathUtils.h"

Mat4::Mat4() = default;

Mat4 Mat4::identity() 
{
    Mat4 result;

    result.at(0, 0) = 1.0f;
    result.at(1, 1) = 1.0f;
    result.at(2, 2) = 1.0f;
    result.at(3, 3) = 1.0f;

    return result;
}

float& Mat4::at(std::size_t row, std::size_t column)
{
    return values[column * 4 + row];
}

float Mat4::at(std::size_t row, std::size_t column) const
{
    return values[column * 4 + row];
}

const float* Mat4::data() const
{
    return values;
}

float* Mat4::data()
{
    return values;
}

Mat4 Mat4::operator*(const Mat4& other) const
{
    Mat4 result;

    for (std::size_t row = 0; row < 4; ++row)
    {
        for (std::size_t column = 0; column < 4; ++column)
        {
            float sum = 0.0f;

            for(std::size_t index = 0; index < 4; ++index)
            {
                sum += at(row, index) * other.at(index, column);
            }
            result.at(row, column) = sum;
        }
    }
    return result;
}

Vec3 Mat4::transformDirection(const Vec3& direction) const
{
    return Vec3{
        at(0, 0) * direction.x +
        at(0, 1) * direction.y +
        at(0, 2) * direction.z,

        at(1, 0) * direction.x +
        at(1, 1) * direction.y +
        at(1, 2) * direction.z,

        at(2, 0) * direction.x +
        at(2, 1) * direction.y +
        at(2, 2) * direction.z
    };
}

Vec3 Mat4::transformPoint(const Vec3& point) const
{
    return Vec3{
        at(0, 0) * point.x +
        at(0, 1) * point.y +
        at(0, 2) * point.z +
        at(0, 3),

        at(1, 0) * point.x +
        at(1, 1) * point.y +
        at(1, 2) * point.z +
        at(1, 3),

        at(2, 0) * point.x +
        at(2, 1) * point.y +
        at(2, 2) * point.z +
        at(2, 3)
    };
}

Mat4 Mat4::translate(const Vec3& translation)
{
    Mat4 result = identity();

    result.at(0, 3) = translation.x;
    result.at(1, 3) = translation.y;
    result.at(2, 3) = translation.z;

    return result;
}

Mat4 Mat4::scale(const Vec3& scale)
{
    Mat4 result = identity();

    result.at(0, 0) = scale.x;
    result.at(1, 1) = scale.y;
    result.at(2, 2) = scale.z;

    return result;
}

Mat4 Mat4::rotate(const Vec3& rotation)
{
    return rotateZ(rotation.z) * rotateY(rotation.y) * rotateX(rotation.x);
}

Mat4 Mat4::rotateX(float rotation)
{
    Mat4 result = Mat4::identity();
    float radians = Math::radians(rotation);
    float s = sin(radians);
    float c = cos(radians);

    result.at(1,1) = c;
    result.at(1, 2) = -s;

    result.at(2, 1) = s;
    result.at(2,2) = c;
    
    return result;
}

Mat4 Mat4::rotateY(float rotation)
{
    Mat4 result = Mat4::identity();
    float radians = Math::radians(rotation);
    float s = sin(radians);
    float c = cos(radians);

    result.at(0,0) = c;
    result.at(0, 2) = s;

    result.at(2, 0) = -s;
    result.at(2,2) = c;
    
    return result;
}

Mat4 Mat4::rotateZ(float rotation)
{
    Mat4 result = Mat4::identity();
    float radians = Math::radians(rotation);
    float s = sin(radians);
    float c = cos(radians);

    result.at(0,0) = c;
    result.at(0, 1) = -s;

    result.at(1, 0) = s;
    result.at(1,1) = c;
    
    return result;
}

Mat4 Mat4::fromQuaternion(const Quaternion& quaternion)
{
    Quaternion q = quaternion.normalized();

    const float xx = q.x * q.x;
    const float yy = q.y * q.y;
    const float zz = q.z * q.z;

    const float xy = q.x * q.y;
    const float xz = q.x * q.z;
    const float yz = q.y * q.z;

    const float wx = q.w * q.x;
    const float wy = q.w * q.y;
    const float wz = q.w * q.z;

    Mat4 result = Mat4::identity();

    result.at(0, 0) = 1.0f - 2.0f * (yy + zz);
    result.at(0, 1) = 2.0f * (xy - wz);
    result.at(0, 2) = 2.0f * (xz + wy);

    result.at(1, 0) = 2.0f * (xy + wz);
    result.at(1, 1) = 1.0f - 2.0f * (xx + zz);
    result.at(1, 2) = 2.0f * (yz - wx);

    result.at(2, 0) = 2.0f * (xz - wy);
    result.at(2, 1) = 2.0f * (yz + wx);
    result.at(2, 2) = 1.0f - 2.0f * (xx + yy);

    return result;
}


Mat4 Mat4::perspective(float fieldOfViewRadians, float aspectRatio, float nearPlane, float farPlane)
{
    if(aspectRatio == 0)
    {
        throw std::invalid_argument("Perspective aspect ratio cannot be zero");
    }

    if(nearPlane <= 0.0f || farPlane <= nearPlane)
    {
        throw std::invalid_argument("Invalid perspective near / far planes");
    }

    Mat4 result;

    const float tanHalfFov = std::tan(fieldOfViewRadians / 2.0);

    result.at(0, 0) = 1.0 / (aspectRatio * tanHalfFov);

    result.at(1, 1) = 1.0 / tanHalfFov;

    result.at(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);

    result.at(2, 3) = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);

    result.at(3,2) = -1.0f;

    return result;
}

Mat4 Mat4::orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
    if (right == left || top == bottom || farPlane == nearPlane)
    {
        throw std::invalid_argument("Invalid orthographic bounds");
    }

    Mat4 result = Mat4::identity();

    result.at(0, 0) = 2.0f / (right - left);

    result.at(1, 1) = 2.0f / (top - bottom);

    result.at(2, 2) = -2.0f / (farPlane - nearPlane);

    result.at(0,3) = -(right + left) / (right - left);

    result.at(1, 3) = -(top + bottom) / (top - bottom);

    result.at(2, 3) = -(farPlane + nearPlane) / (farPlane - nearPlane);

    return result;
}


Mat4& Mat4::setRow(size_t numRow, const Vec3& rowVec) 
{
    at(numRow, 0) = rowVec.x;
    at(numRow, 1) = rowVec.y;
    at(numRow, 2) = rowVec.z;
    return *this;
}

Mat4& Mat4::setColumn(size_t numColumn, const Vec3& columnVec) 
{
    at(0, numColumn) = columnVec.x;
    at(1, numColumn) = columnVec.y;
    at(2, numColumn) = columnVec.z;
    return *this;
}

Vec3 Mat4::getRow(size_t row) const
{
    return Vec3{
        this->at(row,0),
        this->at(row,1),
        this->at(row,2)};
}

Mat4 Mat4::lookAt(const Vec3& position, const Vec3& target, const Vec3& worldUp)
{
    Mat4 viewMatrix = Mat4::identity();

    //Construct camera basis vectors expressed in world coordinates
    Vec3 forward = (target - position).normalized();
    Vec3 right = Vec3::cross(forward, worldUp).normalized();
    Vec3 up = Vec3::cross(right, forward).normalized();

    viewMatrix.setRow(0, right);
    viewMatrix.setRow(1, up);
    viewMatrix.setRow(2, -forward);

    //Calculate last column as dot from base vectors with position
    Vec3 translation;
    translation.x = -Vec3::dot(right, position);
    translation.y = -Vec3::dot(up, position);
    translation.z = Vec3::dot(forward, position);

    viewMatrix.setColumn(3, translation);

    return viewMatrix;
}

Mat4 Mat4::view(const Vec3& position, const Vec3& forward, const Vec3& right, const Vec3& up)
{
    Mat4 result = Mat4::identity();

    result.setRow(0, right);
    result.setRow(1, up);
    result.setRow(2, -forward);

    result.at(0, 3) = -Vec3::dot(right, position);
    result.at(1, 3) = -Vec3::dot(up, position);
    result.at(2, 3) = Vec3::dot(forward, position);

    return result;
}