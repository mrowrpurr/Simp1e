#pragma once

#include <Simp1e/DefineEventType.h>
#include <Simp1e/MouseButton.h>

namespace Simp1e {

    struct IMouseEvent {
        DEFINE_EVENT_TYPE("MouseEvent")

        virtual ~IMouseEvent() = default;

        virtual MouseButton button() const  = 0;
        virtual bool        pressed() const = 0;
    };
}
