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

    // uiGrid->OnLeftClick([&](Tile::Position position) { uiGrid->MoveElement(circle, position); });
    uiGrid->OnLeftClick([&](Tile::Position position) {
        qDebug() << "Left click at" << position.x << position.y;
        uiGrid->AddCircle(position, {0, 255, 0}, 25);
    });

    return UI::Qt::Run();
}