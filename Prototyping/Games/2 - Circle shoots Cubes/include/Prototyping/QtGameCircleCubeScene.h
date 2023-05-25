#pragma once

#include <QGraphicsScene>
#include <cstdint>

namespace Prototyping {

    class QtGameCircleCubeScene : public QGraphicsScene {
        uint32_t _cellWidth  = 0;
        uint32_t _cellHeight = 0;

    public:
        QtGameCircleCubeScene() : QGraphicsScene() {}
        QtGameCircleCubeScene(uint32_t cellWidth, uint32_t cellHeight)
            : _cellWidth(cellWidth), _cellHeight(cellHeight), QGraphicsScene() {}

        uint32_t GetCellWidth() const { return _cellWidth; }
        uint32_t GetCellHeight() const { return _cellHeight; }
    };
}