#pragma once

#include <Simp1e/DefineEventType.h>
#include <Simp1e/GamepadButton.h>

namespace Simp1e {

    struct IGamepadEvent {
        DEFINE_EVENT_TYPE("GamepadEvent")

        virtual ~IGamepadEvent() = default;

        virtual GamepadButton button() const  = 0;
        virtual bool          pressed() const = 0;
        virtual float         value() const   = 0;
    };
}
