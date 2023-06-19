#pragma once

#include <functional>

#include "ComponentTypeMacro.h"
#include "ResizeEvent.h"

namespace Simp1e::ECS {

    class OnResizeComponent {
        std::function<void(ResizeEvent*)> _onResizeEvent;

    public:
        SIMP1E_ECS_COMPONENT("OnResize")

        OnResizeComponent(std::function<void(ResizeEvent*)> onResizeEvent)
            : _onResizeEvent(onResizeEvent) {}

        virtual void TriggerEvent(ResizeEvent* event) { _onResizeEvent(event); }
    };
}
