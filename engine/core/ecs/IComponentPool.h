#pragma once

class IComponentPool
{
public:
    virtual ~IComponentPool() = default;
    virtual void remove(Entity entity) = 0;
};