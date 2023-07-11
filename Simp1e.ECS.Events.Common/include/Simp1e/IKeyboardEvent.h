#pragma once

#include <Simp1e/DefineEventType.h>
#include <Simp1e/KeyboardKey.h>
#include <Simp1e/KeyboardModifierKey.h>
#include <function_pointer.h>

namespace Simp1e {

    struct IKeyboardEvent {
        DEFINE_EVENT_TYPE("KeyboardEvent")

        virtual ~IKeyboardEvent() = default;

        virtual KeyboardKey key() const                                                         = 0;
        virtual bool        pressed() const                                                     = 0;
        virtual bool        repeated() const                                                    = 0;
        virtual bool        isModifierPressed(KeyboardModifierKey modifierKey) const            = 0;
        virtual void        ForEachModifier(IFunctionPointer<void(KeyboardModifierKey)>*) const = 0;
    };
}
