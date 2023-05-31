#pragma once

#include <QGraphicsTextItem>
#include <QMessageBox>

#include "../UIPosition.h"
#include "../UISize.h"
#include "../UITileGrid.h"
#include "QtScene.h"
#include "QtTile.h"
#include "QtTileGridRenderer.h"

namespace Simp1e::UI::Qt {

    // TODO: padding support
    class QtTileGridRectangleRenderer : public QtTileGridRenderer {
        UITileGrid::Config&                         _config;
        QtScene*                                    _scene;
        std::unordered_map<Tile::Position, QtTile*> _tiles;

    public:
        QtTileGridRectangleRenderer(UITileGrid::Config& config, QtScene* scene)
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
                    _tiles[tile->GetPosition()] = qtTile;
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
            if (position.z != 0) return UIPosition::Invalid();
            return UIPosition{
                static_cast<double>(position.y * _config.tileWidth + _config.tileWidth / 2),
                static_cast<double>(position.x * _config.tileHeight + _config.tileHeight / 2)};
        }

        QPolygonF GetTileBounds(const Tile::Position& position) override {
            auto uiPosition = UIPosition{
                static_cast<double>(position.y * _config.tileWidth + _config.tileWidth / 2),
                static_cast<double>(position.x * _config.tileHeight + _config.tileHeight / 2)};
            auto rect = QRectF(
                uiPosition.x() - _config.tileWidth / 2, uiPosition.y() - _config.tileHeight / 2,
                _config.tileWidth, _config.tileHeight
            );
            return QPolygonF(rect);
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
