#pragma once

#include <QGraphicsTextItem>
#include <QMessageBox>

#include "../UIPosition.h"
#include "../UISize.h"
#include "../UITileGrid.h"
#include "QtScene.h"
#include "QtTile.h"
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
                    auto rect = QRectF(
                        col * _config.tileWidth, row * _config.tileHeight, _config.tileWidth,
                        _config.tileHeight
                    );
                    // put down the base tile for tracking
                    auto* tile   = _config.grid->GetTile(row, col);
                    auto  qtTile = new QtTile(tile, rect);
                    _scene->addItem(qtTile);
                    if (_config.showGrid) {
                        _scene->addRect(rect, QPen(::Qt::black));
                    }
                    if (_config.displayCoordinates) {
                        auto text = _scene->addText(
                            QString("(%1, %2)").arg(row).arg(col), QFont("Arial", 5)
                        );
                        text->setDefaultTextColor(::Qt::lightGray);
                        text->setPos(col * _config.tileWidth, row * _config.tileHeight);
                    }
                }
            }
            return UISize{
                _config.grid->GetColumns() * _config.tileWidth,
                _config.grid->GetRows() * _config.tileHeight};
        }

        UIPosition GetTileCenter(const Tile::Position& position) override {
            return UIPosition{
                static_cast<double>(position.y * _config.tileWidth + _config.tileWidth / 2 / 2),
                static_cast<double>(position.x * _config.tileHeight + _config.tileHeight / 2 / 2)};
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
