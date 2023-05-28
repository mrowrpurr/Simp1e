#pragma once

#include <QVBoxLayout>
#include <QWidget>

#include "../UIPosition.h"
#include "../UISize.h"
#include "../UITileGrid.h"
#include "QtCircle.h"
#include "QtScene.h"
#include "QtView.h"

namespace Prototyping::UI::Qt {

    struct QtTileGridRenderer {
        virtual UISize     InitializeGrid() { return {}; }
        virtual UIPosition GetTileCenter(const Tile::Position& position) { return {}; }
        virtual QPolygonF  GetTileBounds(const Tile::Position& position) { return {}; }
    };

    // TODO: padding support
    class QtTileGridRectangleRenderer : public QtTileGridRenderer {
        UITileGrid::Config& _config;
        QtScene*            _scene;

    public:
        QtTileGridRectangleRenderer(UITileGrid::Config& config, QtScene* scene)
            : _config(config), _scene(scene) {}

        UISize InitializeGrid() override {
            if (_config.showGrid) {
                for (uint32_t x = 0; x < _config.grid->GetColumns(); x++) {
                    for (uint32_t y = 0; y < _config.grid->GetRows(); y++) {
                        auto rect = QRectF(
                            x * _config.tileWidth + _config.padding,
                            y * _config.tileHeight + _config.padding, _config.tileWidth,
                            _config.tileHeight
                        );
                        _scene->addRect(rect, QPen(::Qt::black));
                    }
                }
            }
            return UISize{
                _config.grid->GetColumns() * _config.tileWidth + _config.padding * 2,
                _config.grid->GetRows() * _config.tileHeight + _config.padding * 2};
        }
    };

    // TODO: padding support
    class QtTileGridTrimetricRenderer : public QtTileGridRenderer {
        UITileGrid::Config& _config;
        QtScene*            _scene;

    public:
        QtTileGridTrimetricRenderer(UITileGrid::Config& config, QtScene* scene)
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
                }
            }
            return UISize{uiWidth, uiHeight};
        }
    };

    //

    class QtTileGrid : public UITileGrid {
        UITileGrid::Config _config;
        QWidget            _window;  // <-- concrete instance (* are children of this)
        QVBoxLayout        _layout;  // <-- concrete instance (* are children of this)
        QtView*            _view;
        QtScene*           _scene;
        std::unique_ptr<QtTileGridRenderer> _renderer;

    public:
        QtTileGrid(const Config& config) : _config(config) {
            _scene = new QtScene();
            _view  = new QtView(_scene);
            switch (_config.renderingStyle) {
                case RenderingStyle::Grid:
                    _renderer.reset(new QtTileGridRectangleRenderer(_config, _scene));
                    break;
                case RenderingStyle::Trimetric:
                    _renderer.reset(new QtTileGridTrimetricRenderer(_config, _scene));
                    break;
            }
            auto gridSize = _renderer->InitializeGrid();
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
    };
}
