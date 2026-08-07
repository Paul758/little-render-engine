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

Mat4& Mat4::operator+=(const Mat4& other)
{

    for (size_t i = 0; i < 4; ++i)
    {
        values[i] += other.values[i];
    }

    return *this;
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


Mat4& Mat4::setRow(Mat4& result, size_t numRow, const Vec3& rowVec) 
{
    result.at(numRow, 0) = rowVec.x;
    result.at(numRow, 1) = rowVec.y;
    result.at(numRow, 2) = rowVec.z;
    return result;
}

Mat4& Mat4::setColumn(Mat4& result, size_t numColumn, const Vec3& columnVec) 
{
    result.at(0, numColumn) = columnVec.x;
    result.at(1, numColumn) = columnVec.y;
    result.at(2, numColumn) = columnVec.z;
    return result;
}

Mat4 Mat4::lookAt(const Vec3& position, const Vec3& target, const Vec3& worldUp)
{
    Mat4 viewMatrix = Mat4::identity();

    //Construct camera basis vectors expressed in world coordinates
    Vec3 forward = (target - position).normalized();
    Vec3 right = Vec3::cross(forward, worldUp).normalized();
    Vec3 up = Vec3::cross(right, forward).normalized();

    viewMatrix.setRow(viewMatrix, 0, right);
    viewMatrix.setRow(viewMatrix, 1, up);
    viewMatrix.setRow(viewMatrix, 2, -forward);

    //Calculate last column as dot from base vectors with position
    Vec3 translation;
    translation.x = -Vec3::dot(right, position);
    translation.y = -Vec3::dot(up, position);
    translation.z = Vec3::dot(forward, position);

    viewMatrix.setColumn(viewMatrix, 3, translation);

    return viewMatrix;
}