#pragma once

#include <QGraphicsPolygonItem>

#include "../UITileGrid.h"
#include "QtScene.h"
#include "QtTileGridRenderer.h"

namespace Prototyping::UI::Qt {

    class QtTileGridIsometricRenderer : public QtTileGridRenderer {
        UITileGrid::Config& _config;
        QtScene*            _scene;

    public:
        QtTileGridIsometricRenderer(UITileGrid::Config& config, QtScene* scene)
            : _config(config), _scene(scene) {}

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
            qreal tileWidth  = _config.tileWidth;
            qreal tileHeight = _config.tileHeight;
            qreal x          = (position.y - position.x) * tileWidth / 2;
            qreal y          = (position.x + position.y) * tileHeight / 2;
            auto  top        = QPointF(x, y);
            auto  bottom     = QPointF(x, y + tileHeight);
            auto  right      = QPointF(x + tileWidth / 2, y + tileHeight / 2);
            auto  left       = QPointF(x - tileWidth / 2, y + tileHeight / 2);

            QPolygonF polygon;
            polygon << top << right << bottom << left;
            auto center = polygon.boundingRect().center();

            return UIPosition{center.x(), center.y()};
        }

        Tile::Position ScenePositionToTilePosition(const UIPosition& position) override {
            qreal tileWidth  = _config.tileWidth;
            qreal tileHeight = _config.tileHeight;
            qreal x          = position.x();
            qreal y          = position.y();

            for (int row = 0; row < _config.grid->GetRows(); row++) {
                for (int col = 0; col < _config.grid->GetColumns(); col++) {
                    qreal tileX  = (col - row) * tileWidth / 2;
                    qreal tileY  = (row + col) * tileHeight / 2;
                    auto  top    = QPointF(tileX, tileY);
                    auto  bottom = QPointF(tileX, tileY + tileHeight);
                    auto  right  = QPointF(tileX + tileWidth / 2, tileY + tileHeight / 2);
                    auto  left   = QPointF(tileX - tileWidth / 2, tileY + tileHeight / 2);

                    QPolygonF polygon;
                    polygon << top << right << bottom << left;

                    if (polygon.containsPoint(QPointF(x, y), ::Qt::OddEvenFill)) {
                        return Tile::Position{
                            static_cast<uint32_t>(row), static_cast<uint32_t>(col)};
                    }
                }
            }

            // Point does not belong to any tile.
            // Return an invalid position.
            return Tile::Position{UINT32_MAX, UINT32_MAX};
        }
    };
}
