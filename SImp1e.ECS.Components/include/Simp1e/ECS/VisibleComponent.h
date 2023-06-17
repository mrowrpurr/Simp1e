#pragma once

#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/ComponentTypeMacro.h>

#include "ComponentBase.h"

namespace Simp1e::ECS {
    class VisibleComponent : public ComponentBase {
        bool _isVisible = true;

    public:
        SIMP1E_ECS_COMPONENT("Visible")

        VisibleComponent() = default;
        VisibleComponent(bool isVisible) : _isVisible(isVisible) {}

        virtual bool IsVisible() { return _isVisible; }

        virtual void SetVisible(bool isVisible) {
            _isVisible = isVisible;
            SetDirty();
        }

        virtual void Show() {
            _isVisible = true;
            SetDirty();
        }

        virtual void Hide() {
            _isVisible = false;
            SetDirty();
        }

        virtual void ToggleVisible() {
            _isVisible = !_isVisible;
            SetDirty();
        }
    };
}
