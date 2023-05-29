#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Tile.h"

namespace Prototyping {
    class TileGrid {
        uint32_t                                        _layer   = 0;
        uint32_t                                        _rows    = 0;
        uint32_t                                        _columns = 0;
        std::vector<std::vector<std::unique_ptr<Tile>>> _tiles;

        void SetupTiles() {
            _tiles.resize(_rows);
            for (auto& row : _tiles) row.resize(_columns);
            for (uint32_t row = 0; row < _rows; ++row)
                for (uint32_t column = 0; column < _columns; ++column)
                    _tiles.at(row).at(column) =
                        std::make_unique<Tile>(Tile::Position{row, column, _layer});
        }

    public:
        TileGrid(uint32_t rows, uint32_t columns, uint32_t layer = 0)
            : _rows(rows), _columns(columns), _layer(layer) {
            SetupTiles();
        }

        void Resize(uint32_t rows, uint32_t columns) {
            _rows    = rows;
            _columns = columns;
            SetupTiles();
        }

        uint32_t GetLayer() const { return _layer; }
        uint32_t GetRows() const { return _rows; }
        uint32_t GetColumns() const { return _columns; }

        Tile* GetTile(uint32_t row, uint32_t column) { return _tiles.at(row).at(column).get(); }
    };
}
