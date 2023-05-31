#pragma once

#include <QGraphicsView>

namespace Simp1e::UI::Qt {
    class QtView : public QGraphicsView {
    public:
        QtView(QGraphicsScene* scene) : QGraphicsView(scene) {}
    };
}
