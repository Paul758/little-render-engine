#pragma once

#include <vector>
#include <optional>

struct Entity;
class ComponentRegistry;

class HierarchySystem
{
public:
    explicit HierarchySystem(ComponentRegistry& registry);

    void setParent(Entity child, Entity parent);
    void removeParent(Entity child);

    bool hasParent(Entity child) const;
    std::optional<Entity> getParent(Entity child) const;

    std::vector<Entity> getChildren(Entity parent) const;

private:
    bool wouldCreateCycle(Entity child, Entity parent) const;

private:
    ComponentRegistry& registry_;
};