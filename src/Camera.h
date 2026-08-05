#pragma once

#include "math/Mat4.h"
#include "math/Vec3.h"

class Camera
{
private:
    Vec3 position_{0.0f, 0.0f, 3.0f};
    Vec3 target_{0.0f, 0.0f, 0.0f};
    Vec3 up_{0.0f, 1.0f, 0.0f};

public:
    explicit Camera(Vec3 position);

    Mat4 getViewMatrix() const;
    Mat4 getProjectionMatrix() const;

    const Vec3& getPosition() const;
    void setPosition(const Vec3& position);

    const Vec3& getTarget() const;
    void setTarget(const Vec3& target);
};