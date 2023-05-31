#pragma once

#include <QGraphicsPolygonItem>

#include "../UITileGrid.h"
#include "QtScene.h"
#include "QtTile.h"
#include "QtTileGridRenderer.h"

namespace Simp1e::UI::Qt {

    // TODO padding support
    class QtTileGridHexagonRenderer : public QtTileGridRenderer {
        UITileGrid::Config& _config;
        QtScene*            _scene;
        // sizes
        float hexagonSide = 40.0f;  // Length of one side of a hexagon
        float aspectRatio = 0.6f;   // Adjust this to make hexagons taller or shorter - was 0.8f
        float height      = sqrt(3.0f) * hexagonSide * aspectRatio;  // Height of a hexagon
        float width       = 2.0f * hexagonSide;                      // Width of a hexagon
        std::unordered_map<Tile::Position, QtTile*> _tiles;

    public:
        QtTileGridHexagonRenderer(UITileGrid::Config& config, QtScene* scene)
            : _config(config), _scene(scene) {}

        UITile* GetTile(const Tile::Position& position) override {
            auto it = _tiles.find(position);
            if (it != _tiles.end()) return it->second;
            return nullptr;
        }

        TileGrid* GetGrid(uint32_t layer = 0) override {
            if (layer == 0) return _config.grid;
            else return nullptr;
        }

        UISize InitializeGrid() override {
            uint32_t uiWidth    = 0;
            uint32_t uiHeight   = 0;
            qreal    tileWidth  = _config.tileWidth;   // or use width
            qreal    tileHeight = _config.tileHeight;  // or use height

            for (uint32_t row = 0; row < _config.grid->GetRows(); row++) {
                for (uint32_t col = 0; col < _config.grid->GetColumns(); col++) {
                    // Calculate the top-left point of the hexagon
                    float x = col * width + ((row % 2 == 0) ? 0.0f : width / 2.0f);
                    float y = row * height * 3.0f / 4.0f;

                    // Create a hexagon
                    QPointF topLeft(x, y);
                    QPointF topRight(x + width / 2.0f, y + height / 4.0f);
                    QPointF middleRight(x + width / 2.0f, y + height * 3.0f / 4.0f);
                    QPointF bottom(x, y + height);
                    QPointF middleLeft(x - width / 2.0f, y + height * 3.0f / 4.0f);
                    QPointF topLeftExtended(x - width / 2.0f, y + height / 4.0f);

                    QPolygonF polygon;
                    polygon << topLeft << topRight << middleRight << bottom << middleLeft
                            << topLeftExtended;

                    if (uiWidth < middleRight.x()) uiWidth = middleRight.x();
                    if (uiHeight < bottom.y()) uiHeight = bottom.y();

                    // Put down the base tile for tracking
                    auto* tile   = _config.grid->GetTile(row, col);
                    auto  qtTile = new QtTile(tile, polygon);
                    _scene->addItem(qtTile);
                    _tiles[tile->GetPosition()] = qtTile;

                    if (_config.showGrid) {
                        QGraphicsPolygonItem* item = new QGraphicsPolygonItem(polygon);
                        item->setPen(QPen(::Qt::black, 1));
                        item->setBrush(::Qt::white);
                        _scene->addItem(item);
                    }
                    if (_config.displayCoordinates) {
                        auto text = _scene->addText(
                            QString("(%1, %2)").arg(row).arg(col), QFont("Arial", 5)
                        );
                        text->setDefaultTextColor(::Qt::lightGray);
                        auto center = polygon.boundingRect().center();
                        text->setPos(
                            center.x() - text->boundingRect().width() / 2,
                            center.y() - text->boundingRect().height() / 2
                        );
                    }
                }
            }
            return UISize{uiWidth, uiHeight};
        }

        UIPosition GetTileCenter(const Tile::Position& position) override {
            if (position.z != 0) return UIPosition::Invalid();

            qreal tileWidth  = _config.tileWidth;
            qreal tileHeight = _config.tileHeight;

            // Calculate the top-left point of the hexagon
            float x = position.y * width + ((position.x % 2 == 0) ? 0.0f : width / 2.0f);
            float y = position.x * height * 3.0f / 4.0f;

            // Create a hexagon
            QPointF topLeft(x, y);
            QPointF topRight(x + width / 2.0f, y + height / 4.0f);
            QPointF middleRight(x + width / 2.0f, y + height * 3.0f / 4.0f);
            QPointF bottom(x, y + height);
            QPointF middleLeft(x - width / 2.0f, y + height * 3.0f / 4.0f);
            QPointF topLeftExtended(x - width / 2.0f, y + height / 4.0f);

            QPolygonF polygon;
            polygon << topLeft << topRight << middleRight << bottom << middleLeft
                    << topLeftExtended;

            auto center = polygon.boundingRect().center();

            return UIPosition{center.x(), center.y()};
        }

        QPolygonF GetTileBounds(const Tile::Position& position) override {
            if (position.z != 0) return QPolygonF();

            qreal tileWidth  = _config.tileWidth;
            qreal tileHeight = _config.tileHeight;

            // Calculate the top-left point of the hexagon
            float x = position.y * width + ((position.x % 2 == 0) ? 0.0f : width / 2.0f);
            float y = position.x * height * 3.0f / 4.0f;

            // Create a hexagon
            QPointF topLeft(x, y);
            QPointF topRight(x + width / 2.0f, y + height / 4.0f);
            QPointF middleRight(x + width / 2.0f, y + height * 3.0f / 4.0f);
            QPointF bottom(x, y + height);
            QPointF middleLeft(x - width / 2.0f, y + height * 3.0f / 4.0f);
            QPointF topLeftExtended(x - width / 2.0f, y + height / 4.0f);

            QPolygonF polygon;
            polygon << topLeft << topRight << middleRight << bottom << middleLeft
                    << topLeftExtended;

            return polygon;
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
