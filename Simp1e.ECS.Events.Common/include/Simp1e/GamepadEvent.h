#pragma once

#include "IGamepadEvent.h"

namespace Simp1e {

    class GamepadEvent {
        GamepadButton _button;
        bool          _pressed;
        float         _value;

    public:
        GamepadEvent(GamepadButton button, bool pressed = true, float value = 0.0f)
            : _button(button), _pressed(pressed), _value(value) {}

        GamepadButton button() const { return _button; }
        bool          pressed() const { return _pressed; }
        float         value() const { return _value; }
    };
}
