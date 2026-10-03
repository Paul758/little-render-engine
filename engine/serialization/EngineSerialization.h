#pragma once

class ComponentSerializationRegistry;

class EngineSerialization
{
public:
    static void registerComponents(ComponentSerializationRegistry& registry);
};