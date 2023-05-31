#pragma once

#include <Simp1e/Maps/Tile.h>
#include <Simp1e/Maps/TileGrid.h>

#include <QPolygonF>
#include <unordered_map>

#include "../UIPosition.h"
#include "../UISize.h"
#include "../UITile.h"

namespace Prototyping::UI::Qt {

    struct QtTileGridRenderer {
        virtual UISize     InitializeGrid() { return {}; }
        virtual UITile*    GetTile(const Tile::Position& position) { return nullptr; }
        virtual TileGrid*  GetGrid(uint32_t layer = 0) { return nullptr; }
        virtual UIPosition GetTileCenter(const Tile::Position& position) { return {}; }
        virtual QPolygonF  GetTileBounds(const Tile::Position& position) { return {}; }
        virtual std::unordered_map<uint32_t, Tile::Position> ScenePositionToTilePositions(
            const UIPosition& position, uint32_t layer = 0
        ) {
            return {};
        }
    };
}
