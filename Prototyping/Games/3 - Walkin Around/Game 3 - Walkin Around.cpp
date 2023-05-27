#include "Prototyping/Qt.h"

using namespace Prototyping;

int main() {
    TileGrid tileGrid{10, 20};
    auto     uiTileGrid = UI::Qt::CreateTileGrid(&tileGrid);
    UI::Qt::Run();
}