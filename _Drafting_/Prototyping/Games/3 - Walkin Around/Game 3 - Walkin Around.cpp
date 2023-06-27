#include <Simp1e/UI/Qt/Maps.h>

using namespace Simp1e::Maps;
using namespace Simp1e::UI;

UITileGridElement* circle       = nullptr;
UITileGridElement* circleLayer2 = nullptr;

int main() {
    TileGrid grid{20, 10};

    auto uiGrid = Simp1e::UI::Qt::CreateMultiTileGrid({
        .grid = &grid,
    });

    uiGrid->OnLeftClick([&](TilePosition position) {
        if (!circle) circle = uiGrid->AddCircle(position, {255, 0, 0}, 20);
        else uiGrid->AnimatedMoveElement(circle, position);
    });
    uiGrid->OnMiddleClick(
        [&](TilePosition position) {
            if (!circleLayer2) circleLayer2 = uiGrid->AddCircle(position, {0, 255, 0}, 30);
            else uiGrid->AnimatedMoveElement(circleLayer2, position);
        },
        1
    );
    uiGrid->OnRightClick([&](TilePosition position) {
        auto* element = uiGrid->AddCircle(position, {0, 0, 0}, 20);
        uiGrid->SetTileObstacle(position);
    });

    return Simp1e::UI::Qt::Run();
}