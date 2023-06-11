#pragma once

#include <QGraphicsView>

namespace SideScroller {

    class IUIViewport : public QGraphicsView {
    public:
        IUIViewport(QWidget* parent = nullptr) : QGraphicsView(parent) {}
    };
}
