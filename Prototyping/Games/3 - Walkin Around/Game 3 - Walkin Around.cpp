#include "Prototyping/Qt.h"

using namespace Prototyping;

// TODO make this a library function
#include <QMessageBox>

int main() {
    TileGrid grid{20, 10};
    auto     uiTileGrid = UI::Qt::CreateMultiTileGrid({
            .grid = &grid,
    });

    uiTileGrid->OnLeftClick([](Tile::Position position) {
        auto message =
            QString("[Default Layer] Left click on tile (%1, %2)").arg(position.x).arg(position.y);
        QMessageBox::information(nullptr, "Left click", message);
    });
    uiTileGrid->OnRightClick(
        [](Tile::Position position) {
            auto message =
                QString("[Layer 1] Left click on tile (%1, %2)").arg(position.x).arg(position.y);
            QMessageBox::information(nullptr, "Left click", message);
        },
        1
    );

    uiTileGrid->AddCircle({2, 4}, UI::UIColor::Magenta(), 25);

    return UI::Qt::Run();
}