#include "Prototyping/Qt.h"

using namespace Prototyping;

UI::UITileGridElement* player = nullptr;
UI::UITileGridElement* inn    = nullptr;

int main() {
    std::filesystem::path imagesFolder{std::getenv("Openclipart")};
    std::filesystem::path happyMonster{imagesFolder / "201476.png"};
    std::filesystem::path knight{imagesFolder / "293917.png"};
    std::filesystem::path innBuilding{imagesFolder / "11470.png"};
    std::filesystem::path tree{imagesFolder / "218087.png"};
    std::filesystem::path road{imagesFolder / "264733.png"};

    TileGrid grid{20, 10};

    auto uiGrid = UI::Qt::CreateMultiTileGrid({
        .grid = &grid,
    });

    uiGrid->OnLeftClick([&](Tile::Position position) {
        if (!player) player = uiGrid->AddImage(position, knight);
        else uiGrid->AnimatedMoveElement(player, position);
    });
    uiGrid->OnRightClick([&](Tile::Position position) { uiGrid->AddImage(position, road, true); });
    uiGrid->OnMiddleClick([&](Tile::Position position) { uiGrid->AddImage(position, tree); });

    return UI::Qt::Run();
}