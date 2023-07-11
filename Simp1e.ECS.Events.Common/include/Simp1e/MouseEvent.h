#pragma once

#include "IMouseEvent.h"

namespace Simp1e {

    class MouseEvent : public IMouseEvent {
        MouseButton _button;
        bool        _pressed;

    public:
        MouseEvent(MouseButton button, bool pressed) : _button(button), _pressed(pressed) {}

        MouseButton button() const override { return _button; }
        bool        pressed() const override { return _pressed; }
    };
}
