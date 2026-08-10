#include <iostream>

#include "Transform.h"
#include "math/Mat4.h"

void Transform::setPosition(const Vec3& position)
{
    position_ = position;
}

Vec3 Transform::getPosition()
{
    return position_;
}

void Transform::translate(const Vec3& offset)
{
    std::cout << "Add translation" << offset.x << "\n" << offset.y << "\n" << offset.z << "\n";
    position_ += offset;
}

void Transform::scale(const Vec3& scale)
{
    scale_ = scale;
}

void Transform::rotate(const Vec3& rotation)
{
    //std::cout << "Add rotation" << rotation.x << "\n" << rotation.y << "\n" << rotation.z << "\n";
    rotation_ = rotation;
}

Mat4 Transform::getModelMatrix() const
{
    //std::cout << "Now fetching model matrix \n";
    //std::cout << "Position is " << position_.x << " " << position_.y << " " << position_.z << "\n";
    //std::cout << "Rotation is " << rotation_.x << " " << rotation_.y << " " << rotation_.z << "\n";
    Mat4 result = Mat4::translate(position_) * Mat4::rotate(rotation_) * Mat4::scale(scale_);
    /*std::cout << "resulting matrix in renderer is \n" << result.at(0,0) << " " << result.at(0,1) << " " << result.at(0,2) << " " << result.at(0,3) << "\n"
                                  << result.at(1,0) << " " << result.at(1,1) << " " << result.at(1,2) << " " << result.at(1,3) << "\n"
                                  << result.at(2,0) << " " << result.at(2,1) << " " << result.at(2,2) << " " << result.at(2,3) << "\n"
                                  << result.at(3,0) << " " << result.at(3,1) << " " << result.at(3,2) << " " << result.at(3,3) << "\n";*/
    return Mat4::translate(position_) * Mat4::rotate(rotation_) * Mat4::scale(scale_);
}
