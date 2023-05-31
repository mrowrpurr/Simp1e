#pragma once

/*
    Helper for making a maps app...
*/
#include <Simp1e/UI/Qt.h>

#include <memory>

#include "QtMutlipleTileGrids.h"
#include "QtTileGrid.h"

namespace Simp1e::UI::Qt {
    std::unique_ptr<UITileGrid> CreateTileGrid(const UITileGrid::Config& config) {
        return std::make_unique<QtTileGrid>(config);
    }

    std::unique_ptr<UITileGrid> CreateMultiTileGrid(
        const UITileGrid::Config&               config,
        std::vector<UITileGrid::RenderingStyle> renderingStyles =
            {UITileGrid::RenderingStyle::IsometricWithHexagons, UITileGrid::RenderingStyle::Grid,
             UITileGrid::RenderingStyle::Hexagons, UITileGrid::RenderingStyle::Isometric}
    ) {
        return std::make_unique<QtMutlipleTileGrids>(config, renderingStyles);
    }
}