#pragma once

#include <QGraphicsScene>

namespace Simp1e {

    class QSimp1eGraphicsScene : public QGraphicsScene {
    public:
        QSimp1eGraphicsScene(QObject* parent = nullptr) : QGraphicsScene(parent) {}
    };
}
