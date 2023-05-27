#pragma once

#include "../TileGrid.h"

namespace Prototyping::UI {
    struct UITileGrid {
        enum class RenderingStyle { Grid, Trimetric };
        struct Config {
            TileGrid*      grid           = nullptr;
            RenderingStyle renderingStyle = RenderingStyle::Grid;
            uint32_t       tileWidth      = 32;
            uint32_t       tileHeight     = 48;
            uint32_t       padding        = 4;
            bool           showGrid       = true;
        };
    };
}