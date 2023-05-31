#pragma once

#include "Coordinate.h"

namespace Simp1e {

    struct BoundingBox {
        Coordinate topLeft;
        Coordinate bottomRight;

        // TODO test
        bool Contains(const Coordinate& coordinate) const {
            return coordinate.x >= topLeft.x && coordinate.x <= bottomRight.x &&
                   coordinate.y >= topLeft.y && coordinate.y <= bottomRight.y;
        }

        // TODO test
        bool operator==(const BoundingBox& other) const {
            return topLeft == other.topLeft && bottomRight == other.bottomRight;
        }
    };
}