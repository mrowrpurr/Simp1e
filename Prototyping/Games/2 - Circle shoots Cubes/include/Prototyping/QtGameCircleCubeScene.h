#pragma once

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <cstdint>
#include <iostream>
#include <vector>

#include "Prototyping/BoundingBox.h"
#include "Prototyping/GameCircleCube.h"
#include "Prototyping/GameCircleCubeInputHandler.h"

namespace Prototyping {

    struct QtGameCircleCubeSceneParams {
        uint32_t tileWidth       = 0;
        uint32_t tileHeight      = 0;
        uint32_t circleSize      = 0;
        uint32_t cubeSize        = 0;
        bool     renderGridLines = false;
    };

    class QtGameCircleCubeScene : public QGraphicsScene {
        GameCircleCube&                 _game;
        GameCircleCubeInputHandler      _inputHandler{_game};
        uint32_t                        _tileWidth  = 0;
        uint32_t                        _tileHeight = 0;
        uint32_t                        _circleSize = 0;
        uint32_t                        _cubeSize   = 0;
        QGraphicsEllipseItem*           _circle     = nullptr;
        std::vector<QGraphicsRectItem*> _cubes;

        void AddBackground() {
            auto* rect = new QGraphicsRectItem(
                0, 0, _tileWidth * _game.GetColumnCount(), _tileHeight * _game.GetRowCount()
            );
            rect->setBrush(QBrush(Qt::black));
            addItem(rect);
        }
        void RenderGridLines() {
            for (uint32_t x = 0; x < _game.GetColumnCount(); x++) {
                auto* line = new QGraphicsLineItem(
                    x * _tileWidth, 0, x * _tileWidth, _game.GetRowCount() * _tileHeight
                );
                line->setPen(QPen(Qt::white));
                addItem(line);
            }
            for (uint32_t y = 0; y < _game.GetRowCount(); y++) {
                auto* line = new QGraphicsLineItem(
                    0, y * _tileHeight, _game.GetColumnCount() * _tileWidth, y * _tileHeight
                );
                line->setPen(QPen(Qt::white));
                addItem(line);
            }
        }
        void UpdateCirclePosition() {
            auto newX = _game.GetCircleTile().x * _tileWidth + (_tileWidth / 2) - (_circleSize / 2);
            auto newY =
                _game.GetCircleTile().y * _tileHeight + (_tileHeight / 2) - (_circleSize / 2);
            _circle->setPos(newX, newY);
        }
        void AddCircle() {
            if (_circle) return;
            _circle = new QGraphicsEllipseItem(0, 0, _circleSize, _circleSize);
            _circle->setBrush(QBrush(Qt::magenta));
            addItem(_circle);
        }
        void AddCube(const Coordinate& coordinate) {
            auto* cube = new QGraphicsRectItem(
                coordinate.x * _tileWidth, coordinate.y * _tileHeight, _tileWidth, _tileHeight
            );
            cube->setBrush(QBrush(Qt::white));
            addItem(cube);
            _cubes.push_back(cube);
        }
        void RegisterGameCallbacks() {
            _game.OnCircleMoved([this]() { UpdateCirclePosition(); });
            _game.OnCubeAdded([this](const Coordinate& coordinate) { AddCube(coordinate); });
        }

    public:
        QtGameCircleCubeScene(GameCircleCube& game, QtGameCircleCubeSceneParams params = {})
            : QGraphicsScene(),
              _game(game),
              _tileWidth(params.tileWidth),
              _tileHeight(params.tileHeight),
              _circleSize(params.circleSize) {
            AddBackground();
            if (params.renderGridLines) RenderGridLines();
            AddCircle();
            UpdateCirclePosition();
            RegisterGameCallbacks();
        }

        QGraphicsEllipseItem*            GetCircle() const { return _circle; }
        std::vector<QGraphicsRectItem*>& GetCubes() { return _cubes; }
        uint32_t                         GetCellWidth() const { return _tileWidth; }
        uint32_t                         GetCellHeight() const { return _tileHeight; }

        Coordinate PositionToTile(QPointF position) const {
            return {
                static_cast<uint32_t>(position.x() / _tileWidth),
                static_cast<uint32_t>(position.y() / _tileHeight)};
        }
        BoundingBox TileToPosition(Coordinate tile) const {
            auto topLeft = Coordinate{tile.x * _tileWidth, tile.y * _tileHeight};
            auto bottomRight =
                Coordinate{tile.x * _tileWidth + _tileWidth, tile.y * _tileHeight + _tileHeight};
            return {topLeft, bottomRight};
        }

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
            if (event->button() == Qt::MouseButton::LeftButton)
                _inputHandler.OnLeftClick(PositionToTile(event->scenePos()));
            QGraphicsScene::mousePressEvent(event);
        }
    };
}