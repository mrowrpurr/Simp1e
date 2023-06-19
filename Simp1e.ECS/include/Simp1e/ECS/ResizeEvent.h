#pragma once

#include <Simp1e/Size.h>

#include "EventTypeMacro.h"

namespace Simp1e::ECS {

    class ResizeEvent {
        Size _lastSize;

    public:
        SIMP1E_ECS_EVENT("Resize")

        ResizeEvent(const Size& lastSize) : _lastSize(lastSize) {}
        ResizeEvent(sreal width, sreal height) : _lastSize(width, height) {}

        virtual ~ResizeEvent() = default;

        virtual Size  GetLastSize() const { return _lastSize; }
        virtual sreal width() const { return _lastSize.width(); }
        virtual sreal height() const { return _lastSize.height(); }
    };
}
