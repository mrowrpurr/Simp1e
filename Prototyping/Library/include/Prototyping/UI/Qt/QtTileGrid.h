#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

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
        std::unique_ptr<QtTileGridRenderer> _renderer;
        std::unordered_map<uint32_t, std::vector<std::function<void(const Tile::Position&)>>>
            _tileLeftClickHandlers;
        std::unordered_map<uint32_t, std::vector<std::function<void(const Tile::Position&)>>>
            _tileRightClickHandlers;
        std::unordered_map<uint32_t, std::vector<std::function<void(const Tile::Position&)>>>
                                               _tileMiddleClickHandlers;
        std::unordered_set<UITileGridElement*> _elements;

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
                auto tilePositions =
                    _renderer->ScenePositionToTilePositions({position.x(), position.y()});
                if (tilePositions.empty()) return;
                for (auto& clickedTilePosition : tilePositions) {
                    auto layer                      = clickedTilePosition.first;
                    auto tilePosition               = clickedTilePosition.second;
                    auto foundClickHandlersForLayer = _tileLeftClickHandlers.find(layer);
                    if (foundClickHandlersForLayer == _tileLeftClickHandlers.end()) continue;
                    for (auto& handler : foundClickHandlersForLayer->second) handler(tilePosition);
                }
            });
            _scene->OnRightClick([this](const QPointF& position) {
                auto tilePositions =
                    _renderer->ScenePositionToTilePositions({position.x(), position.y()});
                if (tilePositions.empty()) return;
                for (auto& clickedTilePosition : tilePositions) {
                    auto layer                      = clickedTilePosition.first;
                    auto tilePosition               = clickedTilePosition.second;
                    auto foundClickHandlersForLayer = _tileRightClickHandlers.find(layer);
                    if (foundClickHandlersForLayer == _tileRightClickHandlers.end()) continue;
                    for (auto& handler : foundClickHandlersForLayer->second) handler(tilePosition);
                }
            });
            _scene->OnMiddleClick([this](const QPointF& position) {
                auto tilePositions =
                    _renderer->ScenePositionToTilePositions({position.x(), position.y()});
                if (tilePositions.empty()) return;
                for (auto& clickedTilePosition : tilePositions) {
                    auto layer                      = clickedTilePosition.first;
                    auto tilePosition               = clickedTilePosition.second;
                    auto foundClickHandlersForLayer = _tileMiddleClickHandlers.find(layer);
                    if (foundClickHandlersForLayer == _tileMiddleClickHandlers.end()) continue;
                    for (auto& handler : foundClickHandlersForLayer->second) handler(tilePosition);
                }
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

        ~QtTileGrid() override {
            for (auto elementPtr : _elements) delete elementPtr;
        }

        QWidget* GetWidget() override { return &_window; }

        bool ShowAsWindow() override {
            _window.show();
            return true;
        }

        UITile* GetTile(const Tile::Position& position) override {
            qDebug() << "GetTile()" << position.x << position.y << position.z;
            qDebug() << "WARNING not recommended due to support for multiple layers";
            return _renderer->GetTile(position);
        }

        TileGrid* GetGrid(uint32_t layer = 0) override { return _renderer->GetGrid(layer); }

        UITileGrid* GetGridForRenderingStyle(RenderingStyle renderingStyle) override {
            if (_config.renderingStyle == renderingStyle) return this;
            return nullptr;
        }

        bool SetTileObstacle(const Tile::Position& position, bool isObstacle = true) override {
            auto* tile = _renderer->GetTile(position);
            if (!tile) {
                qDebug() << "SetObstacle() failed, tile not found at position" << position.x
                         << position.y << position.z;
                return false;
            }
            if (!tile->GetTile()) {
                qDebug() << "SetObstacle() failed, tile->GetTile() not found at position"
                         << position.x << position.y << position.z;
                return false;
            }
            tile->GetTile()->SetObstacle(isObstacle);
            qDebug() << "SetObstacle()" << position.x << position.y << position.z << isObstacle;
            return true;
        }

        bool OnLeftClick(std::function<void(const Tile::Position&)> handler, uint32_t layer)
            override {
            _tileLeftClickHandlers[layer].push_back(handler);
            return true;
        }
        bool OnRightClick(std::function<void(const Tile::Position&)> handler, uint32_t layer)
            override {
            _tileRightClickHandlers[layer].push_back(handler);
            return true;
        }
        bool OnMiddleClick(std::function<void(const Tile::Position&)> handler, uint32_t layer)
            override {
            _tileMiddleClickHandlers[layer].push_back(handler);
            return true;
        }

        bool MoveElement(UITileGridElement* element, const Tile::Position& position) override {
            try {
                if (!element) return false;
                auto* qtElement = std::any_cast<QtCircle*>(element->GetElement());
                if (!qtElement) return false;
                auto center = _renderer->GetTileCenter(position);
                qtElement->setPos(
                    center.x() - static_cast<uint32_t>(qtElement->boundingRect().width() / 2),
                    center.y() - static_cast<uint32_t>(qtElement->boundingRect().height() / 2)
                );
                return true;
            } catch (const std::bad_any_cast& e) {
                qDebug() << e.what();
                return false;
            }
        }

        UITileGridElement* AddCircle(
            const Tile::Position& position, const UIColor& color, uint32_t diameter
        ) override {
            auto center = _renderer->GetTileCenter(position);
            if (!UIPosition::IsValid(center)) return nullptr;

            auto circle = new QtCircle(color, diameter);
            circle->setPos(
                center.x() - static_cast<uint32_t>(diameter / 2),
                center.y() - static_cast<uint32_t>(diameter / 2)
            );
            _scene->addItem(circle);

            auto* element = new UITileGridElement(position, circle);
            _elements.insert(element);
            return element;
        }
    };
}
