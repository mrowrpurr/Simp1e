#pragma once

#include <QGraphicsItem>

namespace Prototyping::UI::Qt {
    class QtMoveableResizableGraphicsItem : public QGraphicsItem {
        bool _supportsResize;
        bool _supportsRotation;
        bool _supportsMovement;
        bool _resizeMaintainsAspectRatio;

    public:
        QtMoveableResizableGraphicsItem() {
            _supportsResize             = false;
            _supportsRotation           = false;
            _supportsMovement           = false;
            _resizeMaintainsAspectRatio = false;
        }
        QtMoveableResizableGraphicsItem(
            bool supportsResize, bool supportsRotation, bool supportsMovement,
            bool resizeMaintainsAspectRatio
        ) {
            _supportsResize             = supportsResize;
            _supportsRotation           = supportsRotation;
            _supportsMovement           = supportsMovement;
            _resizeMaintainsAspectRatio = resizeMaintainsAspectRatio;
        }

        void SetSupportsResize(bool supportsResize) { _supportsResize = supportsResize; }
        void SetSupportsRotation(bool supportsRotation) { _supportsRotation = supportsRotation; }
        void SetSupportsMovement(bool supportsMovement) { _supportsMovement = supportsMovement; }
        void SetResizeMaintainsAspectRatio(bool resizeMaintainsAspectRatio) {
            _resizeMaintainsAspectRatio = resizeMaintainsAspectRatio;
        }

        virtual bool GetSupportsResize() { return _supportsResize; }
        virtual bool GetSupportsRotation() { return _supportsRotation; }
        virtual bool GetSupportsMovement() { return _supportsMovement; }
        virtual bool GetResizeMaintainsAspectRatio() { return _resizeMaintainsAspectRatio; }
        virtual void Resize(QRectF) {}
        virtual void Rotate(int) {}
        virtual void Move(QPointF) {}
    };
}
