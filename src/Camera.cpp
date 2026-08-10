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
    if (projectionMode_ == ProjectionMode::Perspective) 
    {
        return Mat4::perspective(Math::radians(fieldOfView_), aspectRatio_, nearPlane_, farPlane_);
    }

    const float halfHeight = orthographicSize_;
    const float halfWidth = halfHeight *  aspectRatio_;

    return Mat4::orthographic(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane_, farPlane_);
    
}

void Camera::updateBasisVectors()
{
    forward_ = (target_ - position_).normalized();
    const Vec3 worldUp {0.0f, 1.0f, 0.0f};

    right_ = Vec3::cross(forward_, worldUp).normalized();
    up_ = Vec3::cross(right_, forward_).normalized();
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

void Camera::setProjectionMode(ProjectionMode mode)
{
    projectionMode_ = mode;
}

const Vec3& Camera::getForward() const
{
    return forward_;
}

const Vec3& Camera::getRight() const
{
    return right_;
}

const Vec3& Camera::getUp() const
{
    return up_;
}

float Camera::getOrthographicSize() const
{
    return orthographicSize_;
}