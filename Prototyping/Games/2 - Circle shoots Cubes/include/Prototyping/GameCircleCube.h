#pragma once

#include <string_format.h>

#include <cstdint>
#include <stdexcept>

#include "Coordinate.h"

namespace Prototyping {

    class GameCircleCube {
        Coordinate _circlePosition;
        uint32_t   _rowCount    = 0;
        uint32_t   _columnCount = 0;

    public:
        GameCircleCube() = default;
        GameCircleCube(uint32_t coloumnCount, uint32_t rowCount)
            : _rowCount(rowCount), _columnCount(coloumnCount) {}

        uint32_t GetRowCount() const { return _rowCount; }
        uint32_t GetColumnCount() const { return _columnCount; }

        void MoveCircleTo(Coordinate coordinate) {
            if (coordinate.x >= _columnCount || coordinate.y >= _rowCount)
                throw std::runtime_error(string_format(
                    "({},{}) is out of bounds (map size: {}x{})", coordinate.x, coordinate.y,
                    _columnCount, _rowCount
                ));
            _circlePosition = coordinate;
        }

        Coordinate& GetCirclePosition() { return _circlePosition; }
    };
}
