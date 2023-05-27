#include "Prototyping/Qt.h"

using namespace Prototyping;

int main() {
    TileGrid tileGrid{20, 10};
    auto     uiTileGrid = UI::Qt::CreateTileGrid({
            .grid           = &tileGrid,
            .renderingStyle = UI::UITileGrid::RenderingStyle::Trimetric,
            .tileWidth      = 38,
            .tileHeight     = 64,
            .padding        = 8,
    });
    UI::Qt::Run();
}