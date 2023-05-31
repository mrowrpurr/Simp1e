#pragma once

#include <cstdint>
#include <functional>

#include "TilePosition.h"

namespace Simp1e::Maps {
    class Tile {
        bool         _isObstacle = false;
        TilePosition _position;

    public:
        Tile(const TilePosition& position, bool isObstacle = false)
            : _position(position), _isObstacle(isObstacle) {}

        [[deprecated("Use GetPosition() instead")]] const TilePosition& position() const {
            return _position;
        }

        const TilePosition GetPosition() const { return _position; }

        bool IsObstacle() const { return _isObstacle; }
        void SetObstacle(bool isObstacle = true) { _isObstacle = isObstacle; }
    };
}
