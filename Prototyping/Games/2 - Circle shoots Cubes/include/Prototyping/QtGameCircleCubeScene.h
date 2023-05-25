#pragma once

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <cstdint>

#include "Prototyping/GameCircleCube.h"

namespace Prototyping {

    struct QtGameCircleCubeSceneParams {
        uint32_t cellWidth  = 0;
        uint32_t cellHeight = 0;
        uint32_t circleSize = 0;
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
        void AddCircle() {
            _circle = new QGraphicsEllipseItem(0, 0, _circleSize, _circleSize);
            _circle->setBrush(QBrush(Qt::magenta));
            addItem(_circle);
        }
        void UpdateCirclePosition() {
            _circle->setPos(
                _game.GetCirclePosition().x * _cellWidth, _game.GetCirclePosition().y * _cellHeight
            );
        }

    public:
        QtGameCircleCubeScene(GameCircleCube& game, QtGameCircleCubeSceneParams params = {})
            : QGraphicsScene(),
              _game(game),
              _cellWidth(params.cellWidth),
              _cellHeight(params.cellHeight),
              _circleSize(params.circleSize) {
            AddBackground();

            // // Add a blue rectangle to the scene
            // auto* rect = new QGraphicsRectItem(0, 0, 100, 100);
            // rect->setBrush(QBrush(Qt::blue));
            // addItem(rect);

            AddCircle();
            UpdateCirclePosition();
        }

        QGraphicsEllipseItem* GetCircle() const { return _circle; }
        uint32_t              GetCellWidth() const { return _cellWidth; }
        uint32_t              GetCellHeight() const { return _cellHeight; }
    };
}