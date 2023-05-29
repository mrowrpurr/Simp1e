#pragma once

#include <cstdint>

namespace Prototyping {
    class Tile {
    public:
        struct Position {
            uint32_t x = 0;
            uint32_t y = 0;
            uint32_t z = 0;

            bool operator==(const Position& other) const {
                return x == other.x && y == other.y && z == other.z;
            }
            bool operator!=(const Position& other) const { return !(*this == other); }

            static Position Invalid() { return Position{UINT32_MAX, UINT32_MAX}; }
            static bool     IsValid(const Position& position) { return position != Invalid(); }
        };

    private:
        Position _position;

    public:
        Tile(const Position& position) : _position(position) {}

        const Position& position() const { return _position; }
    };
}
