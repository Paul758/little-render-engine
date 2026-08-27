#include "TransformComponent.h"

Mat4 TransformComponent::getModelMatrix() const
{
    return Mat4::translate(position) * Mat4::rotate(rotation) * Mat4::scale(scale);
}