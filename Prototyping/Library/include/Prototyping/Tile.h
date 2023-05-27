#pragma once

#include <cstdint>

namespace Prototyping {
    class Tile {
    public:
        struct Position {
            uint32_t x = 0;
            uint32_t y = 0;
        };

    private:
        Position _position;

    public:
        Tile(const Position& position) : _position(position) {}

        const Position& position() const { return _position; }
    };
}
