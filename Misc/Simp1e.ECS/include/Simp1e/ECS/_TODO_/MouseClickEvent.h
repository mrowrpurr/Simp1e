#pragma once

#include <Simp1e/sreal.h>

#include "EventTypeMacro.h"

namespace Simp1e::ECS {

    class MouseClickEvent {
    public:
        enum class Button { Left, Right, Middle };

    private:
        sreal  _x;
        sreal  _y;
        Button _button;
        bool   _pressed;

    public:
        SIMP1E_ECS_EVENT("Mouse")

        MouseClickEvent(sreal x, sreal y, Button button = Button::Left, bool pressed = true)
            : _x(x), _y(y), _button(button), _pressed(pressed) {}

        virtual ~MouseClickEvent() = default;

        virtual sreal  x() const { return _x; }
        virtual sreal  y() const { return _y; }
        virtual Button button() const { return _button; }
        virtual bool   leftButton() const { return _button == Button::Left; }
        virtual bool   rightButton() const { return _button == Button::Right; }
        virtual bool   middleButton() const { return _button == Button::Middle; }
        virtual bool   pressed() const { return _pressed; }
    };
}
