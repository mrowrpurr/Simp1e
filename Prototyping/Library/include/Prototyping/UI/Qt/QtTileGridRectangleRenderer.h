#pragma once

#include <QGraphicsTextItem>

#include "../UIPosition.h"
#include "../UISize.h"
#include "../UITileGrid.h"
#include "QtScene.h"
#include "QtTileGridRenderer.h"

namespace Prototyping::UI::Qt {

    // TODO: padding support
    class QtTileGridRectangleRenderer : public QtTileGridRenderer {
        UITileGrid::Config& _config;
        QtScene*            _scene;

    public:
        QtTileGridRectangleRenderer(UITileGrid::Config& config, QtScene* scene)
            : _config(config), _scene(scene) {}

        UISize InitializeGrid() override {
            for (uint32_t row = 0; row < _config.grid->GetRows(); row++) {
                for (uint32_t col = 0; col < _config.grid->GetColumns(); col++) {
                    if (_config.showGrid) {
                        auto rect = QRectF(
                            col * _config.tileWidth + _config.padding,
                            row * _config.tileHeight + _config.padding, _config.tileWidth,
                            _config.tileHeight
                        );
                        _scene->addRect(rect, QPen(::Qt::black));
                    }
                    if (_config.displayCoordinates) {
                        auto text = _scene->addText(
                            QString("(%1, %2)").arg(row).arg(col), QFont("Arial", 5)
                        );
                        text->setDefaultTextColor(::Qt::lightGray);
                        text->setPos(
                            col * _config.tileWidth + _config.padding,
                            row * _config.tileHeight + _config.padding
                        );
                    }
                }
            }
            return UISize{
                _config.grid->GetColumns() * _config.tileWidth + _config.padding * 2,
                _config.grid->GetRows() * _config.tileHeight + _config.padding * 2};
        }

        UIPosition GetTileCenter(const Tile::Position& position) override {
            return UIPosition{
                static_cast<double>(
                    position.y * _config.tileWidth + _config.tileWidth / 2 + _config.padding / 2
                ),
                static_cast<double>(
                    position.x * _config.tileHeight + _config.tileHeight / 2 + _config.padding / 2
                )};
        }
    };
}
