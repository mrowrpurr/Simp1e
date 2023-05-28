#include "Prototyping/Qt.h"

using namespace Prototyping;

int main() {
    TileGrid grid{10, 20};
    auto     uiTileGrid = UI::Qt::CreateMultiTileGrid({
            .grid = &grid,
    });


    uiTileGrid->AddCircle({2, 4}, UI::UIColor::Magenta(), 25);

    return UI::Qt::Run();
}