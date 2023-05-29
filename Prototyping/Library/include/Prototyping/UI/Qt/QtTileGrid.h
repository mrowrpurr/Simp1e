#pragma once

#include <QPropertyAnimation>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "../../AStar/AStar.h"
#include "../UIPosition.h"
#include "../UISize.h"
#include "../UITileGrid.h"
#include "QtCircle.h"
#include "QtImage.h"
#include "QtScene.h"
#include "QtTileGridHexagonRenderer.h"
#include "QtTileGridIsometricRenderer.h"
#include "QtTileGridRectangleRenderer.h"
#include "QtTileGridTilesAndHexagonsRenderer.h"
#include "QtView.h"

namespace Prototyping::UI::Qt {

    class QtTileGrid : public UITileGrid {
        bool               _isAnimating = false;
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

        RenderingStyle GetRenderingStyle() override { return _config.renderingStyle; }

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

        std::vector<Tile::Position> GetPath(
            const Tile::Position& startPosition, const Tile::Position& endPosition,
            bool hexgrid = false, bool allowDiagonalMovement = true
        ) override {
            qDebug() << "GetPath()" << startPosition.x << startPosition.y << startPosition.z
                     << endPosition.x << endPosition.y << endPosition.z << hexgrid
                     << allowDiagonalMovement;
            bool isHexGrid = _config.renderingStyle == RenderingStyle::Hexagons ||
                             (_config.renderingStyle == RenderingStyle::IsometricWithHexagons &&
                              startPosition.z == 1);
            qDebug() << "isHexGrid" << isHexGrid;
            auto* grid = GetGrid(startPosition.z);
            if (!grid) {
                qDebug() << "GetPath() failed, grid not found at layer" << startPosition.z;
                return {};
            }
            auto* start = grid->GetTile(startPosition.x, startPosition.y);
            if (!start) {
                qDebug() << "GetPath() failed, start tile not found at position" << startPosition.x
                         << startPosition.y << startPosition.z;
                return {};
            }
            auto* end = grid->GetTile(endPosition.x, endPosition.y);
            if (!end) {
                qDebug() << "GetPath() failed, end tile not found at position" << endPosition.x
                         << endPosition.y << endPosition.z;
                return {};
            }
            auto tiles = AStar::GetShortestPath(
                *grid, start, end, isHexGrid, true
            );  // TODO Read diagonal movement from _config
            if (tiles.empty()) return {};
            qDebug() << "GetPath() found path with" << tiles.size() << "tiles";
            for (auto& tile : tiles)
                qDebug() << tile.tile->GetPosition().x << tile.tile->GetPosition().y
                         << tile.tile->GetPosition().z;
            std::vector<Tile::Position> positions;
            for (auto& tile : tiles) positions.push_back(tile.tile->GetPosition());
            return positions;
            return {};
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
                qDebug() << "MoveElement()" << position.x << position.y << position.z;
                auto* qtElement = std::any_cast<QGraphicsItem*>(element->GetElement());
                if (!qtElement) return false;
                auto center = _renderer->GetTileCenter(position);
                qtElement->setPos(
                    center.x() - static_cast<uint32_t>(qtElement->boundingRect().width() / 2),
                    center.y() - static_cast<uint32_t>(qtElement->boundingRect().height() / 2)
                );
                element->SetPosition(position);
                return true;
            } catch (const std::bad_any_cast& e) {
                qDebug() << e.what();
                return false;
            }
        }

        bool AnimatedMoveElement(
            UITileGridElement* element, const Tile::Position& position, double duration = 500,
            double delay = 0
        ) override {
            if (!element) return false;
            qDebug() << "AnimatedMoveElement()" << position.x << position.y << position.z;
            if (_isAnimating) {
                qDebug() << "AnimatedMoveElement() failed, already animating";
                return false;
            }
            auto path = GetPath(element->GetPosition(), position);
            if (path.empty()) return false;
            if (delay) {
                QTimer::singleShot(delay, [this, element, path, duration]() {
                    double d = duration;
                    for (auto& position : path) {
                        qDebug() << "AnimatedMoveElement() QTimer::singleShot()" << position.x
                                 << position.y << position.z << d;
                        QTimer::singleShot(d, [this, element, position]() {
                            MoveElement(element, position);
                        });
                        d += duration;
                    }
                });
            } else {
                auto d = duration;
                for (auto& position : path) {
                    qDebug() << "AnimatedMoveElement() QTimer::singleShot()" << position.x
                             << position.y << position.z << duration;
                    QTimer::singleShot(d, [this, element, position]() {
                        MoveElement(element, position);
                    });
                    d += duration;
                }
            }
            return true;
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

            QGraphicsItem* elementPtr = circle;
            auto*          element    = new UITileGridElement(position, elementPtr);
            _elements.insert(element);
            return element;
        }

        UITileGridElement* AddImage(
            const Tile::Position& position, const std::filesystem::path& imagePath, uint32_t width,
            uint32_t height
        ) override {
            auto center = _renderer->GetTileCenter(position);
            if (!UIPosition::IsValid(center)) return nullptr;
            auto bounds = _renderer->GetTileBounds(position);
            auto image  = new QtImage(imagePath.string().c_str());
            image->SetPolygon(bounds);
            image->SetResize(true);
            image->setPos(
                center.x() - static_cast<uint32_t>(image->GetWidth() / 2),
                center.y() - static_cast<uint32_t>(image->GetHeight() / 2)
            );
            _scene->addItem(image);
            _scene->update();

            QGraphicsItem* elementPtr = image;
            auto*          element    = new UITileGridElement(position, elementPtr);
            _elements.insert(element);
            return element;
        }
    };
}
