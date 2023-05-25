#pragma once

#include <cstdint>

namespace Prototyping {

    struct Coordinate {
        uint32_t x = 0;
        uint32_t y = 0;

        bool operator==(const Coordinate& other) const { return x == other.x && y == other.y; }
    };
}