#pragma once
#include <ComponentRegistry.h>

class RenderSystem
{
public:
    RenderSystem(ComponentRegistry registry);
    void update();

private:
    ComponentRegistry registry_;
};