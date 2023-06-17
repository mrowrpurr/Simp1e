#pragma once

#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/ComponentTypeMacro.h>

namespace Simp1e::ECS {
    class VisibleComponent {
        bool _isVisible = true;

    public:
        SIMP1E_ECS_COMPONENT("Visible")

        VisibleComponent() = default;
        VisibleComponent(bool isVisible) : _isVisible(isVisible) {}

        bool IsVisible() { return _isVisible; }

        void SetVisible(bool isVisible) { _isVisible = isVisible; }

        void Show() { _isVisible = true; }
        void Hide() { _isVisible = false; }

        void ToggleVisible() { _isVisible = !_isVisible; }
    };
}
