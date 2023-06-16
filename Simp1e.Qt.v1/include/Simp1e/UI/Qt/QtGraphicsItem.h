#pragma once

#include "QtMoveableResizableGraphicsItem.h"

namespace Simp1e::UI::Qt {

    class QtGraphicsItem : public QtMoveableResizableGraphicsItem {
    public:
        QtGraphicsItem(QGraphicsItem* parent = nullptr) : QtMoveableResizableGraphicsItem(parent) {}
    };
}
