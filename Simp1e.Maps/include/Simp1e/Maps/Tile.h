#pragma once

#include <cstdint>
#include <functional>

namespace Simp1e::Maps {
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

        [[deprecated("Use GetPosition() instead")]] const Position& position() const {
            return _position;
        }

        const Position GetPosition() const { return _position; }

        bool IsObstacle() const { return _isObstacle; }
        void SetObstacle(bool isObstacle = true) { _isObstacle = isObstacle; }
    };
}

namespace std {

    template <>
    struct hash<Simp1e::Maps::Tile::Position> {
        std::size_t operator()(const Simp1e::Maps::Tile::Position& pos) const {
            std::size_t h1 = std::hash<uint32_t>{}(pos.x);
            std::size_t h2 = std::hash<uint32_t>{}(pos.y);
            std::size_t h3 = std::hash<uint32_t>{}(pos.z);
            return h1 ^ (h2 << 1) ^ (h3 << 2);  // Or any other kind of combination
        }
    };

}