#pragma once
#include <cstdint>

struct Entity
{
    uint32_t id = 0;
    explicit operator bool() const { return id != 0; }
};
