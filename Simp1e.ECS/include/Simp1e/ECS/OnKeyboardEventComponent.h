#pragma once

#include <functional>

#include "ComponentTypeMacro.h"
#include "KeyboardEvent.h"

namespace Simp1e::ECS {

    class OnKeyboardEventComponent {
        std::function<void(KeyboardEvent*)> _onKeyEvent;

    public:
        SIMP1E_ECS_COMPONENT("OnKeyboardEvent")

        OnKeyboardEventComponent(std::function<void(KeyboardEvent*)> onKeyEvent)
            : _onKeyEvent(onKeyEvent) {}

        virtual void TriggerEvent(KeyboardEvent* event) { _onKeyEvent(event); }
    };
}
