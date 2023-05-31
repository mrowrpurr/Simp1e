#pragma once

#include "QtGraphicsItemWithBorder.h"
#include "QtResizeRotateHandleCollection.h"

namespace Simp1e::UI::Qt {
    class QtMoveableResizableGraphicsItem : public QtGraphicsItemWithBorder {
        bool                            _resizingEnabled              = false;
        bool                            _resizingMaintainsAspectRatio = false;
        bool                            _movingEnabled                = false;
        bool                            _rotatingEnabled              = false;
        QtResizeRotateHandleCollection* _resizeRotateHandleCollection = nullptr;

        void ShowHandles() {
            if (!_resizeRotateHandleCollection)
                _resizeRotateHandleCollection = new QtResizeRotateHandleCollection(this);
            if (_resizingEnabled) _resizeRotateHandleCollection->ActivateResizing();
            if (_rotatingEnabled) _resizeRotateHandleCollection->ActivateRotation();
        }

        void DestroyHandles() {
            if (_resizeRotateHandleCollection) {
                delete _resizeRotateHandleCollection;
                _resizeRotateHandleCollection = nullptr;
            }
        }

    public:
        QtMoveableResizableGraphicsItem(QGraphicsItem* parent = nullptr)
            : QtGraphicsItemWithBorder(parent) {}

        virtual bool SetResizingMode(bool enabled, bool maintainAspectRatio = false) {
            if (enabled == _resizingEnabled) return false;
            _resizingEnabled              = enabled;
            _resizingMaintainsAspectRatio = maintainAspectRatio;
            if (enabled) {
                ShowHandles();
                SetBorder(enabled, {100, 100, 100}, UILineStyle::Dotted);
            } else {
                DestroyHandles();
                SetBorder(false);
            }
            return true;
        }
        virtual bool SetMovingMode(bool enabled) {
            if (enabled == _movingEnabled) return false;
            _movingEnabled = enabled;
            setFlag(QGraphicsItem::ItemIsMovable, enabled);
            SetBorder(enabled, {100, 100, 100}, UILineStyle::Dashed);
            if (_movingEnabled) setCursor(QCursor(::Qt::CursorShape::DragMoveCursor));
            else unsetCursor();
            return true;
        }
        virtual bool SetRotatingMode(bool enabled) {
            if (enabled == _rotatingEnabled) return false;
            _rotatingEnabled = enabled;
            if (enabled) {
                ShowHandles();
                SetBorder(enabled, {100, 100, 100}, UILineStyle::Dotted);
            } else {
                DestroyHandles();
                SetBorder(false);
            }
            return true;
        }

        virtual void Resize(QRectF) {}
        virtual void Rotate(int) {}
        virtual void Move(QPointF) {}
    };
}
