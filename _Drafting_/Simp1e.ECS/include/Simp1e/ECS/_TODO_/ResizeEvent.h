#pragma once

#include <Simp1e/Point.h>
#include <Simp1e/Size.h>

#include "EventTypeMacro.h"

namespace Simp1e::ECS {

    class ResizeEvent {
        Point _lastSizeViewPositionOffset;
        Size  _lastSize;

    public:
        SIMP1E_ECS_EVENT("Resize")

        ResizeEvent(const Point& lastSizeViewPositionOffset, const Size& lastSize)
            : _lastSizeViewPositionOffset(lastSizeViewPositionOffset), _lastSize(lastSize) {}

        virtual ~ResizeEvent() = default;

        virtual Size  GetLastSize() const { return _lastSize; }
        virtual sreal width() const { return _lastSize.width(); }
        virtual sreal height() const { return _lastSize.height(); }

        virtual Point GetLastSizeViewPositionOffset() const { return _lastSizeViewPositionOffset; }
        virtual sreal x() const { return _lastSizeViewPositionOffset.x(); }
        virtual sreal y() const { return _lastSizeViewPositionOffset.y(); }
    };
}
