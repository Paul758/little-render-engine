#pragma once

class ComponentSerializationRegistry;

class GameSerialization
{
public:
    static void registerComponents(ComponentSerializationRegistry& registry);
};