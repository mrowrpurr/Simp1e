#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Tile.h"

namespace Prototyping {
    class TileGrid {
        uint32_t                                        _columns = 0;
        uint32_t                                        _rows    = 0;
        std::vector<std::vector<std::unique_ptr<Tile>>> _tiles;

    public:
        TileGrid(uint32_t columns, uint32_t rows) : _columns(columns), _rows(rows) {
            _tiles.resize(_columns);
            for (auto& column : _tiles) column.resize(_rows);
        }

        uint32_t GetColumns() const { return _columns; }
        uint32_t GetRows() const { return _rows; }

        std::unique_ptr<Tile>& GetTile(uint32_t column, uint32_t row) {
            return _tiles[column][row];
        }
    };
}
