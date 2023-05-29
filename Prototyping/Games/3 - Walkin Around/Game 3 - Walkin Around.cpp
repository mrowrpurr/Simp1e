#include "Prototyping/Qt.h"

using namespace Prototyping;

// TODO make this a library function
#include <QMessageBox>

int main() {
    TileGrid grid{20, 10};

    auto uiGrid = UI::Qt::CreateMultiTileGrid({
        .grid = &grid,
    });

    auto* circle = uiGrid->AddCircle({2, 4}, UI::UIColor::Magenta(), 25);

    uiGrid->OnLeftClick([&](Tile::Position position) {
        uiGrid->AddCircle(position, {0, 255, 0}, 25);
    });
    uiGrid->OnMiddleClick(
        [&](Tile::Position position) {
            uiGrid->AddCircle({position.x, position.y, 1}, {0, 0, 255}, 35);
        },
        1
    );
    uiGrid->OnRightClick([&](Tile::Position position) {
        auto* element = uiGrid->AddCircle(position, {0, 0, 0}, 20);
        // element->GetPosition().
    });

    return UI::Qt::Run();
}