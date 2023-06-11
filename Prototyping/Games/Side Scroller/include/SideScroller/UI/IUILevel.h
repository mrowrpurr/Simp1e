#pragma once

#include <QGraphicsScene>

#include "../Game/Level.h"

namespace SideScroller {

    class IUILevel : public QGraphicsScene {
    public:
        IUILevel(QObject* parent = nullptr) : QGraphicsScene(parent) {}
        virtual ~IUILevel()                  = default;
        virtual void LoadLevel(const Level&) = 0;
    };
}
