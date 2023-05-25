#pragma once

#include <string_format.h>

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <vector>

#include "Coordinate.h"

namespace Prototyping {

    class GameCircleCube {
        Coordinate                         _circlePosition;
        uint32_t                           _rowCount    = 0;
        uint32_t                           _columnCount = 0;
        std::vector<std::function<void()>> _onCircleMovedCallbacks;

    public:
        GameCircleCube() = default;
        GameCircleCube(uint32_t coloumnCount, uint32_t rowCount)
            : _rowCount(rowCount), _columnCount(coloumnCount) {}

        uint32_t GetRowCount() const { return _rowCount; }
        uint32_t GetColumnCount() const { return _columnCount; }

        void OnCircleMoved(std::function<void()> callback) {
            _onCircleMovedCallbacks.push_back(callback);
        }
        void TriggerOnCircleMoved() {
            for (auto& callback : _onCircleMovedCallbacks) callback();
        }

        void MoveCircleTo(Coordinate coordinate) {
            if (coordinate.x >= _columnCount || coordinate.y >= _rowCount)
                throw std::runtime_error(string_format(
                    "({},{}) is out of bounds (map size: {}x{})", coordinate.x, coordinate.y,
                    _columnCount, _rowCount
                ));
            _circlePosition = coordinate;
            TriggerOnCircleMoved();
        }

        Coordinate& GetCirclePosition() { return _circlePosition; }
    };
}
