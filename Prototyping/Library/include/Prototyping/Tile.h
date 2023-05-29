#pragma once

#include <cstdint>

namespace Prototyping {
    class Tile {
    public:
        // TODO make this a class and replce all .x with .x() etc
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
        bool     _isObstacle = false;
        Position _position;

    public:
        Tile(const Position& position, bool isObstacle = false)
            : _position(position), _isObstacle(isObstacle) {}

        // TODO make this upper case, everything should be except x() y() z() functions
        const Position& position() const { return _position; }

        bool IsObstacle() const { return _isObstacle; }
        void SetObstacle(bool isObstacle) { _isObstacle = isObstacle; }
    };
}
