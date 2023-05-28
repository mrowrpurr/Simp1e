#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Tile.h"

namespace Prototyping {
    class TileGrid {
        uint32_t                                        _rows    = 0;
        uint32_t                                        _columns = 0;
        std::vector<std::vector<std::unique_ptr<Tile>>> _tiles;

    public:
        TileGrid(uint32_t columns, uint32_t rows) : _rows(rows), _columns(columns) {
            _tiles.resize(_rows);
            for (auto& row : _tiles) row.resize(_columns);
        }

        uint32_t GetRows() const { return _rows; }
        uint32_t GetColumns() const { return _columns; }

        Tile* GetTile(uint32_t row, uint32_t column) { return _tiles.at(row).at(column).get(); }
    };
}
