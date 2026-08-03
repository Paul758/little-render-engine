#pragma once

#include "math/Mat4.h"

class Transform
{
private:

Vec3 position_ = {0.0f, 0.0f, 0.0f};
Vec3 rotation_ = {0.0f, 0.0f, 0.0f};
Vec3 scale_ = {1.0f, 1.0f, 1.0f};

public:
    void setPosition(const Vec3& position);

    void translate(const Vec3& offset);

    void scale(const Vec3& scale);

    void rotate(const Vec3& rotation);

    Mat4 getModelMatrix() const;
};