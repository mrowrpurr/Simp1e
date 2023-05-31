#pragma once

#include "QtGraphicsItemWithBorder.h"

namespace Simp1e::UI::Qt {
    class QtMoveableResizableGraphicsItem : public QtGraphicsItemWithBorder {
        bool _resizingEnabled              = false;
        bool _resizingMaintainsAspectRatio = false;
        bool _movingEnabled                = false;
        bool _rotatingEnabled              = false;

        // setFlags(QGraphicsItem::ItemIsMovable);
    public:
        QtMoveableResizableGraphicsItem(QGraphicsItem* parent = nullptr)
            : QtGraphicsItemWithBorder(parent) {}

        virtual bool SetResizingMode(bool enabled, bool maintainAspectRatio = false) {
            return false;
        }
        virtual bool SetMovingMode(bool enabled) {
            if (enabled == _movingEnabled) return false;
            _movingEnabled = enabled;
            setFlag(QGraphicsItem::ItemIsMovable, enabled);
            SetBorder(enabled, UIColor{255, 0, 255}, UILineStyle::Dotted);
        }
        virtual bool SetRotatingMode(bool enabled) { return false; }

        virtual void Resize(QRectF) {}
        virtual void Rotate(int) {}
        virtual void Move(QPointF) {}
    };
}
