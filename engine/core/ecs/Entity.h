#pragma once

#include <cstdint>

struct Entity
{
    std::uint32_t index = 0;
    std::uint32_t generation = 0;

    bool operator==(const Entity&) const = default;
};