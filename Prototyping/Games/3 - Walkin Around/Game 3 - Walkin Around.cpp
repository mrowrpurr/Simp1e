#include "Prototyping/Qt.h"

using namespace Prototyping;

UI::UITileGridElement* circle       = nullptr;
UI::UITileGridElement* circleLayer2 = nullptr;

int main() {
    TileGrid grid{20, 10};

    auto uiGrid = UI::Qt::CreateMultiTileGrid({
        .grid = &grid,
    });

    uiGrid->OnLeftClick([&](Tile::Position position) {
        if (!circle) circle = uiGrid->AddCircle(position, {255, 0, 0}, 20);
        else {
            auto* startTile = grid.GetTile(circle->GetPosition().x, circle->GetPosition().y);
            auto* endTile   = grid.GetTile(position.x, position.y);
            if (!startTile || !endTile) {
                qDebug() << "Invalid start or end tile";
                return;
            }
            uiGrid->AnimatedMoveElement(circle, position);
        }
    });
    uiGrid->OnMiddleClick(
        [&](Tile::Position position) {
            auto* isoHexUIGrid = uiGrid->GetGridForRenderingStyle(
                UI::UITileGrid::RenderingStyle::IsometricWithHexagons
            );
            if (isoHexUIGrid) {
                auto* layer2Grid = isoHexUIGrid->GetGrid(1);
                if (layer2Grid) {
                    if (!circleLayer2)
                        circleLayer2 =
                            isoHexUIGrid->AddCircle({position.x, position.y, 1}, {0, 0, 255}, 35);
                    else {
                        auto* startTile =
                            layer2Grid->GetTile(circle->GetPosition().x, circle->GetPosition().y);
                        auto* endTile = layer2Grid->GetTile(position.x, position.y);
                        if (!startTile || !endTile) {
                            qDebug() << "Invalid start or end tile";
                            return;
                        }
                        qDebug() << "A* Search - Start tile:" << startTile->GetPosition().x
                                 << startTile->GetPosition().y;
                        auto path =
                            uiGrid->GetPath(startTile->GetPosition(), endTile->GetPosition());
                        qDebug() << "Path length:" << path.size();
                        for (auto& tile : path) {
                            qDebug() << "Tile:" << tile.x << tile.y;
                        }
                    }
                }
            }
        },
        1
    );
    uiGrid->OnRightClick([&](Tile::Position position) {
        auto* element = uiGrid->AddCircle(position, {0, 0, 0}, 20);
        uiGrid->SetTileObstacle(position);
    });

    return UI::Qt::Run();
}