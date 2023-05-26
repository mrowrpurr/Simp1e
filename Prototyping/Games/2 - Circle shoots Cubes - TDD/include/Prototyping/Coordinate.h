#pragma once

#include <cstdint>
#include <functional>

namespace Prototyping {

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
    struct hash<Prototyping::Coordinate> {
        size_t operator()(const Prototyping::Coordinate& coordinate) const {
            return (hash<uint32_t>()(coordinate.x) ^ (hash<uint32_t>()(coordinate.y) << 1));
        }
    };
}