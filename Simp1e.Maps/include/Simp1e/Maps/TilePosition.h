#pragma once

#include <cstdint>
#include <functional>

namespace Simp1e::Maps {

    // TODO - change into a class and have x() not x
    struct TilePosition {
        uint32_t x = 0;
        uint32_t y = 0;
        uint32_t z = 0;

        bool operator==(const TilePosition& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
        bool operator!=(const TilePosition& other) const { return !(*this == other); }

        static TilePosition Invalid() { return TilePosition{UINT32_MAX, UINT32_MAX, UINT32_MAX}; }
        static bool         IsValid(const TilePosition& position) { return position != Invalid(); }
    };
}