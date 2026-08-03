#pragma once

#include "math/Mat4.h"
#include "math/Vec3.h"

class Camera
{
public:
    Camera(const Vec3& position, const Vec3& target, const Vec3& worldUp);

    Mat4 getViewMatrix() const;
    Mat4 getProjectionMatrix() const;
    const Vec3& getPosition() const;
    void setPosition(const Vec3& position);

    void setTarget(const Vec3& target);

private:
    Vec3 position;
    Vec3 target;
    Vec3 worldUp;
};