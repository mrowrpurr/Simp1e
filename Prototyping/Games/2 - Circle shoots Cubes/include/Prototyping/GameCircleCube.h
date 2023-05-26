#pragma once

#include <string_format.h>

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#include "Coordinate.h"

namespace Prototyping {

    class GameCircleCube {
        uint32_t                       _rowCount    = 0;
        uint32_t                       _columnCount = 0;
        Coordinate                     _circleTile;
        std::unordered_set<Coordinate> _cubeTiles;
        std::vector<std::function<void()>>
            _onCircleMovedCallbacks;  // TODO update to include the circle coordinates
        std::vector<std::function<void(const Coordinate&)>> _onCubeAddedCallbacks;

        void ThrowIfOutOfBounds(Coordinate coordinate) {
            if (coordinate.x >= _columnCount || coordinate.y >= _rowCount)
                throw std::runtime_error(string_format(
                    "({},{}) is out of bounds (map size: {}x{})", coordinate.x, coordinate.y,
                    _columnCount, _rowCount
                ));
        }

    public:
        GameCircleCube() = default;
        GameCircleCube(uint32_t columnCount, uint32_t rowCount)
            : _rowCount(rowCount), _columnCount(columnCount) {}

        uint32_t GetRowCount() const { return _rowCount; }
        uint32_t GetColumnCount() const { return _columnCount; }

        void OnCircleMoved(std::function<void()> callback) {
            _onCircleMovedCallbacks.push_back(callback);
        }
        void TriggerOnCircleMoved() {
            for (auto& callback : _onCircleMovedCallbacks) callback();
        }

        void OnCubeAdded(std::function<void(const Coordinate&)> callback) {
            _onCubeAddedCallbacks.push_back(callback);
        }
        void TriggerOnCubeAdded(const Coordinate& coordinate) {
            for (auto& callback : _onCubeAddedCallbacks) callback(coordinate);
        }

        void MoveCircleTo(const Coordinate& coordinate) {
            ThrowIfOutOfBounds(coordinate);
            _circleTile = coordinate;
            TriggerOnCircleMoved();  // <-- TODO this is where we should pass the circle coordinates
        }

        void AddCubeTo(const Coordinate& coordinate) {
            ThrowIfOutOfBounds(coordinate);
            if (_cubeTiles.find(coordinate) != _cubeTiles.end())
                throw std::runtime_error(
                    string_format("({},{}) already has a cube", coordinate.x, coordinate.y)
                );
            _cubeTiles.insert(coordinate);
            TriggerOnCubeAdded(coordinate);
        }

        Coordinate&                     GetCircleTile() { return _circleTile; }
        std::unordered_set<Coordinate>& GetCubeTiles() { return _cubeTiles; }
    };
}
