#include <Simp1e/UI/Qt/Maps.h>

using namespace Simp1e::Maps;
using namespace Simp1e::UI;

bool               playerBorderVisible = false;
UITileGridElement* player              = nullptr;
UITileGridElement* inn                 = nullptr;

int main() {
    std::filesystem::path imagesFolder{std::getenv("Openclipart")};
    std::filesystem::path happyMonster{imagesFolder / "201476.png"};
    std::filesystem::path knight{imagesFolder / "293917.png"};
    std::filesystem::path innBuilding{imagesFolder / "11470.png"};
    std::filesystem::path tree{imagesFolder / "218087.png"};
    std::filesystem::path road{imagesFolder / "264733.png"};

    TileGrid grid{20, 10};

    auto uiGrid = Simp1e::UI::Qt::CreateMultiTileGrid({
        .grid = &grid,
    });

    uiGrid->OnLeftClick([&](TilePosition position) {
        if (!player) player = uiGrid->AddImage(position, knight);
    });
    uiGrid->OnRightClick([&](TilePosition position) {
        if (player) {
            playerBorderVisible = !playerBorderVisible;
            uiGrid->SetBorder(player, playerBorderVisible, {255, 0, 255}, UILineStyle::Dotted);
        }
    });

    return Simp1e::UI::Qt::Run();
}