#pragma once

#include <Simp1e/UI/UITile.h>

#include <QGraphicsPolygonItem>
#include <cstdint>

namespace Simp1e::UI::Qt {
    class QtTile : public UITile, public QGraphicsPolygonItem {
        Maps::Tile* _tile;
        uint32_t    _layer = 0;

    public:
        QtTile(Maps::Tile* tile, QPolygonF polygon) : _tile(tile), QGraphicsPolygonItem(polygon) {}

        uint32_t GetLayer() const { return _layer; }
        void     SetLayer(uint32_t layer) { _layer = layer; }

        Maps::Tile*        GetTile() override { return _tile; }
        Maps::TilePosition GetPosition() const override { return _tile->GetPosition(); }
    };
}
