#pragma once

class ComponentRegistry;
struct LightingData;


class LightingSystem
{
public:
    LightingSystem(ComponentRegistry& registry);
    LightingData buildLightingData();

private:
    ComponentRegistry& registry_;

};
