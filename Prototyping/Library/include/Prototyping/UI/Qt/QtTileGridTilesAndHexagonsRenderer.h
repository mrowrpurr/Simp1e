#pragma once

#include <QGraphicsPolygonItem>

#include "../UITileGrid.h"
#include "QtScene.h"
#include "QtTile.h"
#include "QtTileGridRenderer.h"

namespace Prototyping::UI::Qt {

    class QtTileGridTilesAndHexagonsRenderer : public QtTileGridRenderer {
        UITileGrid::Config& _config;
        QtScene*            _scene;
        TileGrid            _hexGrid{0, 0};

        int HEX_WIDTH      = 60;
        int HEX_HEIGHT     = 60;
        int DIAMOND_WIDTH  = 120;
        int DIAMOND_HEIGHT = 60;
        int HEX_OFFSET_X   = -90;
        int HEX_OFFSET_Y   = 0;

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
            if (isGray) diamondTile->setBrush(QBrush(QColor(255, 0, 0, 75)));
            else diamondTile->setBrush(QBrush(QColor(255, 0, 0, 25)));

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

                    // put down the base tile for tracking
                    auto* tile   = _config.grid->GetTile(row, column);
                    auto  qtTile = new QtTile(tile, item->polygon());
                    _scene->addItem(qtTile);

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

            _hexGrid.Resize(hexRows, hexColumns);

            HEX_OFFSET_Y += -(DIAMOND_ROWS * DIAMOND_HEIGHT / 2);

            // Create the hex grid
            for (int row = 0; row < hexRows; ++row) {
                for (int column = 0; column < hexColumns; ++column) {
                    int  x = HEX_WIDTH * column + (row % 2 == 0 ? 0 : HEX_WIDTH / 2) + HEX_OFFSET_X;
                    int  y = HEX_HEIGHT * 3 / 4 * row + HEX_OFFSET_Y;  // changed the y calculation
                    auto item = createHexagon(x, y, HEX_WIDTH, HEX_HEIGHT);
                    _scene->addItem(item);

                    // put down the base tile for tracking
                    auto* tile   = _hexGrid.GetTile(row, column);
                    auto  qtTile = new QtTile(tile, item->polygon());
                    qtTile->SetLayer(1);
                    _scene->addItem(qtTile);

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

        UIPosition GetHexTileCenter(const Tile::Position& position) {
            int DIAMOND_ROWS     = _config.grid->GetRows();
            int DIAMOND_OFFSET_Y = -1 * DIAMOND_ROWS * DIAMOND_HEIGHT / 2.0;

            int x =
                HEX_WIDTH * position.y + (position.x % 2 == 0 ? 0 : HEX_WIDTH / 2) + HEX_OFFSET_X;
            int  y = HEX_HEIGHT * 3 / 4 * position.x + HEX_OFFSET_Y;  // changed the y calculation
            auto item = createHexagon(x, y, HEX_WIDTH, HEX_HEIGHT);

            auto center = item->boundingRect().center();
            return UIPosition{center.x(), center.y()};
        }

        UIPosition GetDiamondTileCenter(const Tile::Position& position) {
            int  DIAMOND_ROWS     = _config.grid->GetRows();
            int  DIAMOND_OFFSET_Y = -1 * DIAMOND_ROWS * DIAMOND_HEIGHT / 2.0;
            int  x    = position.y * DIAMOND_WIDTH + (position.x % 2 == 0 ? 0 : DIAMOND_WIDTH / 2);
            int  y    = position.x * DIAMOND_HEIGHT / 2 + DIAMOND_OFFSET_Y;
            auto item = createDiamondTile(x, y, DIAMOND_WIDTH, DIAMOND_HEIGHT);
            auto center = item->polygon().boundingRect().center();
            return UIPosition{center.x(), center.y()};
        }

        UIPosition GetTileCenter(const Tile::Position& position) override {
            return GetDiamondTileCenter(position);
            // return GetHexTileCenter(position);
        }

        std::unordered_map<uint32_t, Tile::Position> ScenePositionToTilePositions(
            const UIPosition& position, uint32_t layer = 0
        ) override {
            std::unordered_map<uint32_t, Tile::Position> result;
            auto items = _scene->items({position.x(), position.y()});
            for (auto* item : items)
                if (auto* tile = dynamic_cast<QtTile*>(item))
                    result[tile->GetLayer()] = tile->GetPosition();
            return result;
        }
    };
}
