#pragma once

#include <functional>

#include "ComponentTypeMacro.h"
#include "MouseClickEvent.h"

namespace Simp1e::ECS {

    class OnMouseClickComponent {
        std::function<void(MouseClickEvent*)> _onMouseClickEvent;

    public:
        SIMP1E_ECS_COMPONENT("OnMouseClick")

        OnMouseClickComponent(std::function<void(MouseClickEvent*)> onMouseClickEvent)
            : _onMouseClickEvent(onMouseClickEvent) {}

        virtual void TriggerEvent(MouseClickEvent* event) { _onMouseClickEvent(event); }
    };
}
