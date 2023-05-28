#include "Prototyping/Qt.h"

using namespace Prototyping;

int main() {
    TileGrid tileGrid{10, 20};

    /////////////////////////////////////////////////////
    auto uiTileGrid = UI::Qt::CreateTileGrid({
        .grid           = &tileGrid,
        .renderingStyle = UI::UITileGrid::RenderingStyle::Grid,
    });
    uiTileGrid->AddCircle({2, 4}, UI::UIColor::Magenta(), 25);

    // ///////////////////////////////////////////////////////
    auto uiIsometricGrid = UI::Qt::CreateTileGrid({
        .grid           = &tileGrid,
        .renderingStyle = UI::UITileGrid::RenderingStyle::Isometric,
    });
    uiIsometricGrid->AddCircle({2, 4}, UI::UIColor::Magenta(), 25);

    // ///////////////////////////////////////////////////////
    auto uiHexGrid = UI::Qt::CreateTileGrid({
        .grid           = &tileGrid,
        .renderingStyle = UI::UITileGrid::RenderingStyle::Hexagons,
    });
    uiHexGrid->AddCircle({2, 4}, UI::UIColor::Magenta(), 25);

    // ///////////////////////////////////////////////////////
    auto uiIsoHexGrid = UI::Qt::CreateTileGrid({
        .grid           = &tileGrid,
        .renderingStyle = UI::UITileGrid::RenderingStyle::IsometricWithHexagons,
    });
    uiIsoHexGrid->AddCircle({2, 4}, UI::UIColor::Magenta(), 25);

    return UI::Qt::Run();
}