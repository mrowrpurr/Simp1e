#pragma once

#include <QVBoxLayout>
#include <QWidget>

#include "../UITileGrid.h"
#include "QtCircle.h"
#include "QtScene.h"
#include "QtView.h"

namespace Prototyping::UI::Qt {
    class QtTileGrid : public UITileGrid {
        UITileGrid::Config _config;
        QWidget            _window;  // <-- concrete instance (* are children of this)
        QVBoxLayout        _layout;  // <-- concrete instance (* are children of this)
        QtView*            _view;
        QtScene*           _scene;

    public:
        QtTileGrid(const Config& config) : _config(config) {
            _scene = new QtScene();
            _view  = new QtView(_scene);
            _view->setFixedSize(
                _config.grid->GetColumns() * _config.tileWidth + _config.padding * 2,
                _config.grid->GetRows() * _config.tileHeight + _config.padding * 2
            );
            if (_config.showGrid) drawGridLines();
            _layout.addWidget(_view);
            _window.setLayout(&_layout);
            _window.show();
        }

    protected:
        UITileGridElement* AddCircle(
            const Tile::Position& position, const UIColor& color, uint32_t diameter
        ) override {
            auto element = new QtCircle(color, diameter);
            _scene->addItem(element);
            element->setPos(
                position.x * _config.tileWidth + _config.padding,
                position.y * _config.tileHeight + _config.padding
            );
            return nullptr;  // TODO
        }

    private:
        void drawGridLines() {
            if (!_config.showGrid) return;
            for (uint32_t x = 0; x < _config.grid->GetColumns(); x++) {
                for (uint32_t y = 0; y < _config.grid->GetRows(); y++) {
                    auto& tile = _config.grid->GetTile(x, y);
                    auto  rect = QRectF(
                        x * _config.tileWidth + _config.padding,
                        y * _config.tileHeight + _config.padding, _config.tileWidth,
                        _config.tileHeight
                    );
                    _scene->addRect(rect, QPen(::Qt::black));
                }
            }
        }
    };
}
