#include "engine/components/TransformComponent.h"

Mat4 TransformComponent::getLocalMatrix() const
{
    return Mat4::translate(position) 
         * Mat4::fromQuaternion(rotation) 
         * Mat4::scale(scale);
}

const Mat4 TransformComponent::getWorldMatrix() const
{
    return worldMatrix_;
}

void TransformComponent::setWorldMatrix(const Mat4& worldMatrix)
{
    worldMatrix_ = worldMatrix;
}