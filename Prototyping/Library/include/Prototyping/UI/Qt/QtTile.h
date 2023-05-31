#pragma once

#include <Simp1e/UI/UITile.h>

#include <QGraphicsPolygonItem>
#include <cstdint>

namespace Simp1e::UI::Qt {
    class QtTile : public UITile, public QGraphicsPolygonItem {
        Tile*    _tile;
        uint32_t _layer = 0;

    public:
        QtTile(Tile* tile, QPolygonF polygon) : _tile(tile), QGraphicsPolygonItem(polygon) {}

        uint32_t GetLayer() const { return _layer; }
        void     SetLayer(uint32_t layer) { _layer = layer; }

        Tile*          GetTile() override { return _tile; }
        Tile::Position GetPosition() const override { return _tile->position(); }
    };
}
