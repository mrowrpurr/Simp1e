#pragma once

#include <QPolygonF>

#include "../../Tile.h"
#include "../UIPosition.h"
#include "../UISize.h"

namespace Prototyping::UI::Qt {

    struct QtTileGridRenderer {
        virtual UISize     InitializeGrid() { return {}; }
        virtual UIPosition GetTileCenter(const Tile::Position& position) { return {}; }
        virtual QPolygonF  GetTileBounds(const Tile::Position& position) { return {}; }
    };
}
