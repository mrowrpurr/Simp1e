#include "Prototyping/Qt.h"

using namespace Prototyping;

int main() {
    TileGrid tileGrid{15, 10};
    auto     uiTileGrid = UI::Qt::CreateTileGrid({
            .grid           = &tileGrid,
            .renderingStyle = UI::UITileGrid::RenderingStyle::Trimetric,
            .tileWidth      = 96,
            .tileHeight     = 48,
            .padding        = 8,
            .showGrid       = true,
    });
    uiTileGrid->AddCircle({2, 4}, UI::UIColor::Magenta(), 25);
    UI::Qt::Run();
}