
#pragma once

#include "math/Mat4.h"
#include "math/Vec3.h"
#include "ProjectionMode.h"
#include "graphics/PixelSettings.h"

class Camera
{
private:
    Vec3 position_{0.0f, 0.0f, 3.0f};
    Vec3 target_{0.0f, 0.0f, 0.0f};
    Vec3 up_{0.0f, 1.0f, 0.0f};
    float aspectRatio_ = 16.0f / 9.0f;

    ProjectionMode projectionMode_ = ProjectionMode::Perspective;

    float fieldOfView_ = 45.0f;
    float nearPlane_ = 0.1f;
    float farPlane_ = 100.0f;

    float orthographicSize_ = PixelSettings::orthographicSize();

    Vec3 forward_ {0.0f, 0.0f, -1.0f};
    Vec3 right_ {1.0f, 0.0f, 0.0f};

public:
    explicit Camera(Vec3 position);

    Mat4 getViewMatrix() const;
    Mat4 getProjectionMatrix() const;

    void setProjectionMode(ProjectionMode mode);
    void setOrthographicSize(float size);

    const Vec3& getPosition() const;
    void setPosition(const Vec3& position);

    const Vec3& getTarget() const;
    void setTarget(const Vec3& target);

    void updateBasisVectors();

    const Vec3& getForward() const;
    const Vec3& getRight() const;
    const Vec3& getUp() const;

    float getOrthographicSize() const;
};