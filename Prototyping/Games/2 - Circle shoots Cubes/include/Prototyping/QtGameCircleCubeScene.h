#pragma once

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <cstdint>

#include "Prototyping/BoundingBox.h"
#include "Prototyping/GameCircleCube.h"

namespace Prototyping {

    struct QtGameCircleCubeSceneParams {
        uint32_t cellWidth       = 0;
        uint32_t cellHeight      = 0;
        uint32_t circleSize      = 0;
        bool     renderGridLines = false;
    };

    class QtGameCircleCubeScene : public QGraphicsScene {
        GameCircleCube&       _game;
        uint32_t              _cellWidth  = 0;
        uint32_t              _cellHeight = 0;
        uint32_t              _circleSize = 0;
        QGraphicsEllipseItem* _circle     = nullptr;

        void AddBackground() {
            auto* rect = new QGraphicsRectItem(
                0, 0, _cellWidth * _game.GetColumnCount(), _cellHeight * _game.GetRowCount()
            );
            rect->setBrush(QBrush(Qt::black));
            addItem(rect);
        }
        void RenderGridLines() {
            for (uint32_t x = 0; x < _game.GetColumnCount(); x++) {
                auto* line = new QGraphicsLineItem(
                    x * _cellWidth, 0, x * _cellWidth, _game.GetRowCount() * _cellHeight
                );
                line->setPen(QPen(Qt::white));
                addItem(line);
            }
            for (uint32_t y = 0; y < _game.GetRowCount(); y++) {
                auto* line = new QGraphicsLineItem(
                    0, y * _cellHeight, _game.GetColumnCount() * _cellWidth, y * _cellHeight
                );
                line->setPen(QPen(Qt::white));
                addItem(line);
            }
        }
        void UpdateCirclePosition() {
            auto newX =
                _game.GetCirclePosition().x * _cellWidth + (_cellWidth / 2) - (_circleSize / 2);
            auto newY =
                _game.GetCirclePosition().y * _cellHeight + (_cellHeight / 2) - (_circleSize / 2);
            _circle->setPos(newX, newY);
        }
        void AddCircle() {
            if (_circle) return;
            _circle = new QGraphicsEllipseItem(0, 0, _circleSize, _circleSize);
            _circle->setBrush(QBrush(Qt::magenta));
            addItem(_circle);
            _game.OnCircleMoved([this]() { UpdateCirclePosition(); });
        }

    public:
        QtGameCircleCubeScene(GameCircleCube& game, QtGameCircleCubeSceneParams params = {})
            : QGraphicsScene(),
              _game(game),
              _cellWidth(params.cellWidth),
              _cellHeight(params.cellHeight),
              _circleSize(params.circleSize) {
            AddBackground();
            if (params.renderGridLines) RenderGridLines();
            AddCircle();
            UpdateCirclePosition();
        }

        QGraphicsEllipseItem* GetCircle() const { return _circle; }
        uint32_t              GetCellWidth() const { return _cellWidth; }
        uint32_t              GetCellHeight() const { return _cellHeight; }

        Coordinate PositionToCell(QPointF position) const {
            return {
                static_cast<uint32_t>(position.x() / _cellWidth),
                static_cast<uint32_t>(position.y() / _cellHeight)};
        }

        BoundingBox CellToPosition(Coordinate cell) const {
            auto topLeft = Coordinate{cell.x * _cellWidth, cell.y * _cellHeight};
            auto bottomRight =
                Coordinate{cell.x * _cellWidth + _cellWidth, cell.y * _cellHeight + _cellHeight};
            return {topLeft, bottomRight};
        }

    protected:
        // void mousePressEvent(QGraphicsSceneMouseEvent* event) override {

        //     _game.MoveCircleTo({cellX, cellY});
        // }
    };
}