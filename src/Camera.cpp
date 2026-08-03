#include "Camera.h"
#include "math/MathUtils.h"

Camera::Camera(const Vec3& position, const Vec3& target, const Vec3& worldUp) :
    position(position), target(target), worldUp(worldUp)
{

}

Mat4 Camera::getViewMatrix() const
{
    return Mat4::lookAt(position, target, worldUp);
}

Mat4 Camera::getProjectionMatrix() const
{
    return Mat4::perspective(Math::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
}

const Vec3& Camera::getPosition() const
{
    return position;
}

void Camera::setPosition(const Vec3& newPosition)
{
    position = newPosition;
}

void Camera::setTarget(const Vec3& newTarget)
{
    target = newTarget;
}