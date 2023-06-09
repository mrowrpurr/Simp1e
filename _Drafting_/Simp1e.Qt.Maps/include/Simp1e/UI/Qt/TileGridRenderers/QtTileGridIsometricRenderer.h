#pragma once

#include <Simp1e/Maps/TilePositionHash.h>
#include <Simp1e/UI/Qt/QtScene.h>
#include <Simp1e/UI/UITileGrid.h>

#include <QGraphicsPolygonItem>

#include "QtTile.h"
#include "QtTileGridRenderer.h"

namespace Simp1e::UI::Qt {

    class QtTileGridIsometricRenderer : public QtTileGridRenderer {
        UITileGrid::Config&                             _config;
        QtScene*                                        _scene;
        std::unordered_map<Maps::TilePosition, QtTile*> _tiles;

    public:
        QtTileGridIsometricRenderer(UITileGrid::Config& config, QtScene* scene)
            : _config(config), _scene(scene) {}

        UITile* GetTile(const Maps::TilePosition& position) override {
            auto it = _tiles.find(position);
            if (it != _tiles.end()) return it->second;
            return nullptr;
        }

        Maps::TileGrid* GetGrid(uint32_t layer = 0) override {
            if (layer == 0) return _config.grid;
            else return nullptr;
        }

        UISize InitializeGrid() override {
            uint32_t uiWidth    = 0;
            uint32_t uiHeight   = 0;
            qreal    tileWidth  = _config.tileWidth;
            qreal    tileHeight = _config.tileHeight;

            for (int row = 0; row < _config.grid->GetRows(); row++) {
                for (int col = 0; col < _config.grid->GetColumns(); col++) {
                    qreal x      = (col - row) * tileWidth / 2;
                    qreal y      = (row + col) * tileHeight / 2;
                    auto  top    = QPointF(x, y);
                    auto  bottom = QPointF(x, y + tileHeight);
                    auto  right  = QPointF(x + tileWidth / 2, y + tileHeight / 2);
                    auto  left   = QPointF(x - tileWidth / 2, y + tileHeight / 2);

                    QPolygonF polygon;
                    polygon << top << right << bottom << left;

                    if (uiWidth < right.x()) uiWidth = right.x();
                    if (uiHeight < bottom.y()) uiHeight = bottom.y();

                    // put down the base tile for tracking
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

        UIPosition GetTileCenter(const Maps::TilePosition& position) override {
            if (position.z != 0) return UIPosition::Invalid();

            uint32_t uiWidth    = 0;
            uint32_t uiHeight   = 0;
            qreal    tileWidth  = _config.tileWidth;
            qreal    tileHeight = _config.tileHeight;
            int      positionX  = position.x;
            int      positionY  = position.y;

            qreal x      = (positionY - positionX) * tileWidth / 2;  // <--- crazy number
            qreal y      = (positionX + positionY) * tileHeight / 2;
            auto  top    = QPointF(x, y);
            auto  bottom = QPointF(x, y + tileHeight);
            auto  right  = QPointF(x + tileWidth / 2, y + tileHeight / 2);
            auto  left   = QPointF(x - tileWidth / 2, y + tileHeight / 2);

            QPolygonF polygon;
            polygon << top << right << bottom << left;
            auto center = polygon.boundingRect().center();

            return UIPosition{center.x(), center.y()};
        }

        QPolygonF GetTileBounds(const Maps::TilePosition& position) override {
            if (position.z != 0) return QPolygonF();

            uint32_t uiWidth    = 0;
            uint32_t uiHeight   = 0;
            qreal    tileWidth  = _config.tileWidth;
            qreal    tileHeight = _config.tileHeight;
            int      positionX  = position.x;
            int      positionY  = position.y;

            qreal x      = (positionY - positionX) * tileWidth / 2;  // <--- crazy number
            qreal y      = (positionX + positionY) * tileHeight / 2;
            auto  top    = QPointF(x, y);
            auto  bottom = QPointF(x, y + tileHeight);
            auto  right  = QPointF(x + tileWidth / 2, y + tileHeight / 2);
            auto  left   = QPointF(x - tileWidth / 2, y + tileHeight / 2);

            QPolygonF polygon;
            polygon << top << right << bottom << left;

            return polygon;
        }

        std::unordered_map<uint32_t, Maps::TilePosition> ScenePositionToTilePositions(
            const UIPosition& position, uint32_t layer = 0
        ) override {
            std::unordered_map<uint32_t, Maps::TilePosition> result;
            auto items = _scene->items({position.x(), position.y()});
            for (auto* item : items)
                if (auto* tile = dynamic_cast<QtTile*>(item))
                    result[tile->GetLayer()] = tile->GetPosition();
            return result;
        }
    };
}
