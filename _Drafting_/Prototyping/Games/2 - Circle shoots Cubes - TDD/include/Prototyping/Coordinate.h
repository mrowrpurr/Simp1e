#pragma once

#include <cstdint>
#include <functional>

namespace Simp1e {

    struct Coordinate {
        uint32_t x = 0;
        uint32_t y = 0;

        // TODO test
        bool operator==(const Coordinate& other) const { return x == other.x && y == other.y; }
    };
}

// TODO test
namespace std {
    template <>
    struct hash<Simp1e::Coordinate> {
        size_t operator()(const Simp1e::Coordinate& coordinate) const {
            return (hash<uint32_t>()(coordinate.x) ^ (hash<uint32_t>()(coordinate.y) << 1));
        }
    };
}