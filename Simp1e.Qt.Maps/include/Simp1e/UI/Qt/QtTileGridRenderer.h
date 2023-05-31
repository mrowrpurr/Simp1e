#pragma once

#include <Simp1e/Maps/TileGrid.h>
#include <Simp1e/UI/UIPosition.h>
#include <Simp1e/UI/UISize.h>
#include <Simp1e/UI/UITile.h>

#include <QPolygonF>
#include <unordered_map>

namespace Simp1e::UI::Qt {

    struct QtTileGridRenderer {
        virtual UISize          InitializeGrid() { return {}; }
        virtual UITile*         GetTile(const Maps::TilePosition& position) { return nullptr; }
        virtual Maps::TileGrid* GetGrid(uint32_t layer = 0) { return nullptr; }
        virtual UIPosition      GetTileCenter(const Maps::TilePosition& position) { return {}; }
        virtual QPolygonF       GetTileBounds(const Maps::TilePosition& position) { return {}; }
        virtual std::unordered_map<uint32_t, Maps::TilePosition> ScenePositionToTilePositions(
            const UIPosition& position, uint32_t layer = 0
        ) {
            return {};
        }
    };
}
