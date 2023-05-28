#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <functional>

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

    class QtTileGrid : public UITileGrid {
        UITileGrid::Config _config;
        QWidget            _window;  // <-- concrete instance (* are children of this)
        QVBoxLayout        _layout;  // <-- concrete instance (* are children of this)
        QtView*            _view;
        QtScene*           _scene;
        std::unique_ptr<QtTileGridRenderer>                     _renderer;
        std::vector<std::function<void(const Tile::Position&)>> _tileLeftClickHandlers;
        std::vector<std::function<void(const Tile::Position&)>> _tileRightClickHandlers;
        std::vector<std::function<void(const Tile::Position&)>> _tileMiddleClickHandlers;

        void SetupRenderer() {
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
        }

        void ListenForSceneEvents() {
            _scene->OnLeftClick([this](const QPointF& position) {
                auto tilePosition =
                    _renderer->ScenePositionToTilePosition({position.x(), position.y()});
                for (auto& handler : _tileLeftClickHandlers) handler(tilePosition);
            });
            _scene->OnRightClick([this](const QPointF& position) {
                auto tilePosition =
                    _renderer->ScenePositionToTilePosition({position.x(), position.y()});
                for (auto& handler : _tileRightClickHandlers) handler(tilePosition);
            });
            _scene->OnMiddleClick([this](const QPointF& position) {
                auto tilePosition =
                    _renderer->ScenePositionToTilePosition({position.x(), position.y()});
                for (auto& handler : _tileMiddleClickHandlers) handler(tilePosition);
            });
        }

    public:
        QtTileGrid(const Config& config) : _config(config) {
            _scene = new QtScene();
            SetupRenderer();
            auto gridSize = _renderer->InitializeGrid();
            ListenForSceneEvents();

            // auto padding   = 50;
            // auto sceneRect = _scene->sceneRect();
            // _scene->setSceneRect(
            //     sceneRect.x() - padding, sceneRect.y() - padding, sceneRect.width() + padding *
            //     2, sceneRect.height() + padding * 2
            // );

            _view = new QtView(_scene);
            _view->setHorizontalScrollBarPolicy(::Qt::ScrollBarAlwaysOff);
            _view->setVerticalScrollBarPolicy(::Qt::ScrollBarAlwaysOff);
            _layout.addWidget(_view);
            _window.setLayout(&_layout);
        }

        QWidget* GetWidget() override { return &_window; }

        bool ShowAsWindow() override {
            _window.show();
            return true;
        }

        bool OnLeftClick(std::function<void(const Tile::Position&)> handler) override {
            _tileLeftClickHandlers.push_back(handler);
            return true;
        }
        bool OnRightClick(std::function<void(const Tile::Position&)> handler) override {
            _tileRightClickHandlers.push_back(handler);
            return true;
        }
        bool OnMiddleClick(std::function<void(const Tile::Position&)> handler) override {
            _tileMiddleClickHandlers.push_back(handler);
            return true;
        }

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
