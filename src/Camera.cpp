#include <cmath>

#include "Camera.h"
#include "math/MathUtils.h"
#include <iostream>

Camera::Camera(Vec3 position) : position_(position)
{
}

Mat4 Camera::getViewMatrix() const
{
    return Mat4::lookAt(position_, target_, up_);
}

Mat4 Camera::getProjectionMatrix() const
{
    return Mat4::perspective(Math::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
}

const Vec3& Camera::getPosition() const
{
    return position_;
}

void Camera::setPosition(const Vec3& newPosition)
{
    position_ = newPosition;
}

void Camera::setTarget(const Vec3& newTarget)
{
    target_ = newTarget;
}

const Vec3& Camera::getTarget() const
{
    return target_;
}