#include "Prototyping/Qt.h"

using namespace Prototyping;

// TODO make this a library function
#include <QMessageBox>

int main() {
    TileGrid grid{10, 20};
    auto     uiTileGrid = UI::Qt::CreateMultiTileGrid({
            .grid = &grid,
    });

    uiTileGrid->OnLeftClick([](Tile::Position position) {
        auto message = QString("Left click on tile (%1, %2)").arg(position.x).arg(position.y);
        QMessageBox::information(nullptr, "Left click", message);
    });

    uiTileGrid->AddCircle({2, 4}, UI::UIColor::Magenta(), 25);

    return UI::Qt::Run();
}