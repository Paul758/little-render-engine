#pragma once

#include "engine/serialization/SerializedValue.h"
#include "engine/math/Vec3.h"
#include "engine/math/Quaternion.h"

namespace SerializationUtils
{
    SerializedValue serializeVec3(const Vec3& value);
    Vec3 deserializeVec3(const SerializedValue& value);

    SerializedValue serializeQuaternion(const Quaternion& value);
    Quaternion deserializeQuaternion(const SerializedValue& value);
}