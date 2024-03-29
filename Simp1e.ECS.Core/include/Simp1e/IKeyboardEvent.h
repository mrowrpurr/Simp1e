#pragma once

#include <Simp1e/DefineEventType.h>

namespace Simp1e {

    struct IKeyboardEvent {
        DEFINE_EVENT_TYPE("KeyboardEvent")

        virtual ~IKeyboardEvent() = default;

        virtual int  key() const      = 0;
        virtual bool pressed() const  = 0;
        virtual bool repeated() const = 0;
    };
}
