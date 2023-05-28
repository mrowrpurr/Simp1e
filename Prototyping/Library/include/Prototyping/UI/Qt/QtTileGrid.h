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

            // add some padding to the scene
            // auto padding   = 50;
            // auto sceneRect = _scene->sceneRect();
            // _scene->setSceneRect(
            //     sceneRect.x() - padding, sceneRect.y() - padding, sceneRect.width() + padding *
            //     2, sceneRect.height() + padding * 2
            // );
            _view->setHorizontalScrollBarPolicy(::Qt::ScrollBarAlwaysOff);
            _view->setVerticalScrollBarPolicy(::Qt::ScrollBarAlwaysOff);

            _layout.addWidget(_view);
            _window.setLayout(&_layout);
            _window.show();
        }

    protected:
        UITileGridElement* AddCircle(
            const Tile::Position& position, const UIColor& color, uint32_t diameter
        ) override {
            if (QtTileGridTilesAndHexagonsRenderer* renderer =
                    dynamic_cast<QtTileGridTilesAndHexagonsRenderer*>(_renderer.get())) {
                auto element = new QtCircle(color, diameter);
                _scene->addItem(element);
                auto center = renderer->GetDiamondTileCenter(position);
                element->setPos(
                    center.x() - static_cast<uint32_t>(diameter / 2),
                    center.y() - static_cast<uint32_t>(diameter / 2)
                );

                auto element2 = new QtCircle({0, 255, 0}, diameter);
                _scene->addItem(element2);
                auto center2 = renderer->GetHexTileCenter(position);
                element2->setPos(
                    center2.x() - static_cast<uint32_t>(diameter / 2),
                    center2.y() - static_cast<uint32_t>(diameter / 2)
                );
            } else {
                auto element = new QtCircle(color, diameter);
                _scene->addItem(element);
                auto center = _renderer->GetTileCenter(position);
                element->setPos(
                    center.x() - static_cast<uint32_t>(diameter / 2),
                    center.y() - static_cast<uint32_t>(diameter / 2)
                );
            }

            return nullptr;  // TODO
        }
    };
}
