#pragma once

#include <QVBoxLayout>
#include <QWidget>

#include "../UIPosition.h"
#include "../UISize.h"
#include "../UITileGrid.h"
#include "QtCircle.h"
#include "QtScene.h"
#include "QtTileGridHexagonRenderer.h"
#include "QtTileGridIsometricRenderer.h"
#include "QtTileGridRectangleRenderer.h"
#include "QtTileGridTilesAndHexagonsRenderer.h"
#include "QtView.h"

namespace Prototyping::UI::Qt {

    // TODO: padding support

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
                case RenderingStyle::Isometric:
                    _renderer.reset(new QtTileGridIsometricRenderer(_config, _scene));
                    break;
                case RenderingStyle::Hexagons:
                    _renderer.reset(new QtTileGridHexagonRenderer(_config, _scene));
                    break;
                case RenderingStyle::IsometricWithHexagons:
                    _renderer.reset(new QtTileGridTilesAndHexagonsRenderer(_config, _scene));
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
            auto center = _renderer->GetTileCenter(position);
            element->setPos(
                center.x() - static_cast<uint32_t>(diameter / 2),
                center.y() - static_cast<uint32_t>(diameter / 2)
            );
            return nullptr;  // TODO
        }
    };
}
