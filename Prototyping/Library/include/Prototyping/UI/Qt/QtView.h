#pragma once

#include <QGraphicsView>

namespace Prototyping::UI::Qt {
    class QtView : public QGraphicsView {
    public:
        QtView(QGraphicsScene* scene) : QGraphicsView(scene) {}
    };
}
