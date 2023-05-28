#pragma once

#include <QGraphicsPolygonItem>

#include "../UITileGrid.h"
#include "QtScene.h"
#include "QtTileGridRenderer.h"

namespace Prototyping::UI::Qt {

    class QtTileGridTilesAndHexagonsRenderer : public QtTileGridRenderer {
        UITileGrid::Config& _config;
        QtScene*            _scene;

        int HEX_WIDTH      = 30;
        int HEX_HEIGHT     = 31;
        int DIAMOND_WIDTH  = 60;
        int DIAMOND_HEIGHT = 30;
        // int DIAMOND_ROWS     = 30;
        // int DIAMOND_COLUMNS  = 10;
        int HEX_OFFSET_X = -45;
        int HEX_OFFSET_Y = 3;

        QGraphicsPolygonItem* createDiamondTile(
            int x, int y, int width, int height, bool isGray = false
        ) {
            QPointF topPoint(x, y);
            QPointF rightPoint(x + width / 2.0, y + height / 2.0);
            QPointF bottomPoint(x, y + height);
            QPointF leftPoint(x - width / 2.0, y + height / 2.0);

            QPolygonF diamondPolygon;
            diamondPolygon << topPoint << rightPoint << bottomPoint << leftPoint;

            QGraphicsPolygonItem* diamondTile = new QGraphicsPolygonItem(diamondPolygon);
            diamondTile->setPen(::Qt::NoPen);
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
            hexagonItem->setPen(QPen(::Qt::blue, 1));
            hexagonItem->setBrush(::Qt::NoBrush);

            return hexagonItem;
        }

    public:
        QtTileGridTilesAndHexagonsRenderer(UITileGrid::Config& config, QtScene* scene)
            : _config(config), _scene(scene) {}

        UISize InitializeGrid() override {
            // Ignore the configuration ones for now ;)
            // qreal    tileWidth  = _config.tileWidth;
            // qreal    tileHeight = _config.tileHeight;

            uint32_t uiWidth  = 0;
            uint32_t uiHeight = 0;

            int DIAMOND_ROWS     = _config.grid->GetRows();
            int DIAMOND_COLUMNS  = _config.grid->GetColumns();
            int DIAMOND_OFFSET_Y = -1 * DIAMOND_ROWS * DIAMOND_HEIGHT / 2.0;

            // Create the diamond grid
            for (int row = 0; row < DIAMOND_ROWS; ++row) {
                for (int column = 0; column < DIAMOND_COLUMNS; ++column) {
                    int  x = column * DIAMOND_WIDTH + (row % 2 == 0 ? 0 : DIAMOND_WIDTH / 2);
                    int  y = row * DIAMOND_HEIGHT / 2 + DIAMOND_OFFSET_Y;
                    auto item =
                        createDiamondTile(x, y, DIAMOND_WIDTH, DIAMOND_HEIGHT, (row) % 2 == 0);
                    _scene->addItem(item);

                    auto bottomRight = item->boundingRect().bottomRight();
                    if (uiWidth < bottomRight.x()) uiWidth = bottomRight.x();
                    if (uiHeight < bottomRight.y()) uiHeight = bottomRight.y();

                    QGraphicsTextItem* textItem =
                        _scene->addText(QString::number(row) + "," + QString::number(column));
                    auto center = item->boundingRect().center();
                    textItem->setPos(
                        center.x() - textItem->boundingRect().width() / 2,
                        center.y() - textItem->boundingRect().height() / 2
                    );
                    textItem->setDefaultTextColor(::Qt::red);
                    textItem->setFont(QFont("Arial", 5));
                }
            }

            // Calculate the number of hexes to create based on the size of the diamond grid
            int hexColumns = DIAMOND_COLUMNS * DIAMOND_WIDTH / HEX_WIDTH;
            int hexRows = (DIAMOND_ROWS * DIAMOND_HEIGHT + DIAMOND_OFFSET_Y) / (HEX_HEIGHT * 3 / 4);

            HEX_OFFSET_Y += -(DIAMOND_ROWS * DIAMOND_HEIGHT / 2);

            // Create the hex grid
            for (int row = 0; row < hexRows; ++row) {
                for (int column = 0; column < hexColumns; ++column) {
                    int  x = HEX_WIDTH * column + (row % 2 == 0 ? 0 : HEX_WIDTH / 2) + HEX_OFFSET_X;
                    int  y = HEX_HEIGHT * 3 / 4 * row + HEX_OFFSET_Y;  // changed the y calculation
                    auto item = createHexagon(x, y, HEX_WIDTH, HEX_HEIGHT);
                    _scene->addItem(item);

                    auto bottomRight = item->boundingRect().bottomRight();
                    if (uiWidth < bottomRight.x()) uiWidth = bottomRight.x();
                    if (uiHeight < bottomRight.y()) uiHeight = bottomRight.y();

                    QGraphicsTextItem* textItem =
                        _scene->addText(QString::number(row) + "," + QString::number(column));
                    auto center = item->boundingRect().center();
                    textItem->setPos(
                        center.x() - textItem->boundingRect().width() / 2,
                        center.y() - textItem->boundingRect().height() / 2 + 5
                    );
                    textItem->setDefaultTextColor(::Qt::blue);
                    textItem->setFont(QFont("Arial", 5));
                }
            }

            return UISize{uiWidth, uiHeight};
            return {};
        }

        UIPosition GetDiamondTileCenter(const Tile::Position& position) {
            int  DIAMOND_ROWS     = _config.grid->GetRows();
            int  DIAMOND_OFFSET_Y = -1 * DIAMOND_ROWS * DIAMOND_HEIGHT / 2.0;
            int  x    = position.x * DIAMOND_WIDTH + (position.y % 2 == 0 ? 0 : DIAMOND_WIDTH / 2);
            int  y    = position.y * DIAMOND_HEIGHT / 2 + DIAMOND_OFFSET_Y;
            auto item = createDiamondTile(x, y, DIAMOND_WIDTH, DIAMOND_HEIGHT);
            return UIPosition{
                static_cast<uint32_t>(item->boundingRect().center().x()),
                static_cast<uint32_t>(item->boundingRect().center().y())};
        }

        UIPosition GetTileCenter(const Tile::Position& position) override {
            return GetDiamondTileCenter(position);
        }
    };
}
