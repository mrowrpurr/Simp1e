// #include "LetsTryThis.h"

// int main(int argc, char* argv[]) {
//     QApplication a(argc, argv);

//     GameBoard     board(20, 20, 50, 50, 30, 30);
//     QGraphicsView view(board.getScene());
//     view.show();

//     return a.exec();
// }

#include <QApplication>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPen>

const int HEX_WIDTH        = 30;
const int HEX_HEIGHT       = 31;
const int DIAMOND_WIDTH    = 60;
const int DIAMOND_HEIGHT   = 30;
const int DIAMOND_ROWS     = 30;
const int DIAMOND_COLUMNS  = 10;
const int HEX_OFFSET_X     = -45;
int       HEX_OFFSET_Y     = 3;
const int DIAMOND_OFFSET_Y = -1 * DIAMOND_ROWS * DIAMOND_HEIGHT / 2.0;

QGraphicsPolygonItem* createDiamondTile(int x, int y, int width, int height, bool isGray) {
    QPointF topPoint(x, y);
    QPointF rightPoint(x + width / 2.0, y + height / 2.0);
    QPointF bottomPoint(x, y + height);
    QPointF leftPoint(x - width / 2.0, y + height / 2.0);

    QPolygonF diamondPolygon;
    diamondPolygon << topPoint << rightPoint << bottomPoint << leftPoint;

    QGraphicsPolygonItem* diamondTile = new QGraphicsPolygonItem(diamondPolygon);
    diamondTile->setPen(Qt::NoPen);
    if (isGray) diamondTile->setBrush(QBrush(QColor(255, 0, 0, 50)));
    else diamondTile->setBrush(QBrush(QColor(255, 0, 0, 100)));

    return diamondTile;
}

QGraphicsPolygonItem* createHexagon(int x, int y, int width, int height) {
    qreal sideLength = width / 2;  // The horizontal side length of the hexagon

    QPointF top(x + sideLength, y);
    QPointF topRight(x + width, y + height / 4);
    QPointF bottomRight(x + width, y + height * 3 / 4);
    QPointF bottom(x + sideLength, y + height);
    QPointF bottomLeft(x, y + height * 3 / 4);
    QPointF topLeft(x, y + height / 4);

    QPolygonF hexagonPolygon;
    hexagonPolygon << top << topRight << bottomRight << bottom << bottomLeft << topLeft;

    QGraphicsPolygonItem* hexagonItem = new QGraphicsPolygonItem(hexagonPolygon);
    hexagonItem->setPen(QPen(Qt::blue, 1));
    hexagonItem->setBrush(Qt::NoBrush);

    return hexagonItem;
}

int main(int argc, char** argv) {
    QApplication   app(argc, argv);
    QGraphicsScene scene;

    // Create the diamond grid
    for (int row = 0; row < DIAMOND_ROWS; ++row) {
        for (int column = 0; column < DIAMOND_COLUMNS; ++column) {
            int  x    = column * DIAMOND_WIDTH + (row % 2 == 0 ? 0 : DIAMOND_WIDTH / 2);
            int  y    = row * DIAMOND_HEIGHT / 2 + DIAMOND_OFFSET_Y;
            auto item = createDiamondTile(x, y, DIAMOND_WIDTH, DIAMOND_HEIGHT, (row) % 2 == 0);
            scene.addItem(item);

            QGraphicsTextItem* textItem =
                scene.addText(QString::number(row) + "," + QString::number(column));
            auto center = item->boundingRect().center();
            textItem->setPos(
                center.x() - textItem->boundingRect().width() / 2,
                center.y() - textItem->boundingRect().height() / 2
            );
            textItem->setDefaultTextColor(Qt::red);
            textItem->setFont(QFont("Arial", 5));
        }
    }

    // Calculate the number of hexes to create based on the size of the diamond grid
    int hexColumns = DIAMOND_COLUMNS * DIAMOND_WIDTH / HEX_WIDTH;
    int hexRows    = (DIAMOND_ROWS * DIAMOND_HEIGHT + DIAMOND_OFFSET_Y) / (HEX_HEIGHT * 3 / 4);

    HEX_OFFSET_Y += -(DIAMOND_ROWS * DIAMOND_HEIGHT / 2);

    // Create the hex grid
    for (int row = 0; row < hexRows; ++row) {
        for (int column = 0; column < hexColumns; ++column) {
            int  x    = HEX_WIDTH * column + (row % 2 == 0 ? 0 : HEX_WIDTH / 2) + HEX_OFFSET_X;
            int  y    = HEX_HEIGHT * 3 / 4 * row + HEX_OFFSET_Y;  // changed the y calculation
            auto item = createHexagon(x, y, HEX_WIDTH, HEX_HEIGHT);
            scene.addItem(item);

            QGraphicsTextItem* textItem =
                scene.addText(QString::number(row) + "," + QString::number(column));
            auto center = item->boundingRect().center();
            textItem->setPos(
                center.x() - textItem->boundingRect().width() / 2,
                center.y() - textItem->boundingRect().height() / 2 + 5
            );
            textItem->setDefaultTextColor(Qt::blue);
            textItem->setFont(QFont("Arial", 5));
        }
    }

    QGraphicsView view(&scene);
    view.show();

    return app.exec();
}

// #include "Prototyping/Qt.h"

// using namespace Prototyping;

// int main2() {
//     TileGrid tileGrid{20, 50};

//     // ///////////////////////////////////////////////////////
//     // auto uiTileGrid = UI::Qt::CreateTileGrid({
//     //     .grid               = &tileGrid,
//     //     .renderingStyle     = UI::UITileGrid::RenderingStyle::Grid,
//     //     .tileWidth          = 48,
//     //     .tileHeight         = 36,
//     //     .padding            = 4,
//     //     .showGrid           = true,
//     //     .displayCoordinates = true,
//     // });
//     // uiTileGrid->AddCircle({2, 4}, UI::UIColor::Magenta(), 25);

//     // // ///////////////////////////////////////////////////////
//     // auto uiIsometricGrid = UI::Qt::CreateTileGrid({
//     //     .grid               = &tileGrid,
//     //     .renderingStyle     = UI::UITileGrid::RenderingStyle::Isometric,
//     //     .tileWidth          = 48,
//     //     .tileHeight         = 36,
//     //     .padding            = 4,
//     //     .showGrid           = true,
//     //     .displayCoordinates = true,
//     // });
//     // uiIsometricGrid->AddCircle({2, 4}, UI::UIColor::Magenta(), 25);

//     // ///////////////////////////////////////////////////////
//     auto uiHexGrid = UI::Qt::CreateTileGrid({
//         .grid               = &tileGrid,
//         .renderingStyle     = UI::UITileGrid::RenderingStyle::Hexagons,
//         .tileWidth          = 48,
//         .tileHeight         = 36,
//         .padding            = 4,
//         .showGrid           = true,
//         .displayCoordinates = false,
//     });
//     // uiHexGrid->AddCircle({2, 4}, UI::UIColor::Magenta(), 25);

//     return UI::Qt::Run();
// }