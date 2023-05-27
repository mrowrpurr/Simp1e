#pragma once

#include <QWidget>

#include "../UITileGrid.h"


namespace Prototyping::UI::Qt {
    class QtTileGrid : public UITileGrid {
        UITileGrid::RenderingStyle _renderingStyle;
        TileGrid*                  _tileGrid;

        // testing
        QWidget _widget;

    public:
        QtTileGrid(TileGrid* tileGrid, UITileGrid::RenderingStyle renderingStyle)
            : _tileGrid(tileGrid), _renderingStyle(renderingStyle) {
            _widget.show();
        }
    };
}
