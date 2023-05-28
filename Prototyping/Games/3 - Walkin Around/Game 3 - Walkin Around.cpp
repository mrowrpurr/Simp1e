#include "Prototyping/Qt.h"

using namespace Prototyping;

int main() {
    TileGrid tileGrid{15, 10};
    auto     uiTileGrid = UI::Qt::CreateTileGrid({
            .grid               = &tileGrid,
            .renderingStyle     = UI::UITileGrid::RenderingStyle::Grid,
            .tileWidth          = 48,
            .tileHeight         = 36,
            .padding            = 4,
            .showGrid           = true,
            .displayCoordinates = true,
    });
    // TODO: move the AddCircle to the TileGrid itself and hookup events or something
    uiTileGrid->AddCircle({2, 4}, UI::UIColor::Magenta(), 25);

    auto uiIsometricGrid = UI::Qt::CreateTileGrid({
        .grid               = &tileGrid,
        .renderingStyle     = UI::UITileGrid::RenderingStyle::Isometric,
        .tileWidth          = 48,
        .tileHeight         = 36,
        .padding            = 4,
        .showGrid           = true,
        .displayCoordinates = true,
    });
    uiIsometricGrid->AddCircle({2, 4}, UI::UIColor::Magenta(), 25);

    UI::Qt::Run();
}