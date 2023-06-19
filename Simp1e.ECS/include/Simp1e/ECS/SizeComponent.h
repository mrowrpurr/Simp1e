#pragma once

#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/ComponentTypeMacro.h>
#include <Simp1e/Size.h>

#include "DirtyTrackingComponent.h"

namespace Simp1e::ECS {
    class SizeComponent : public DirtyTrackingComponent {
        Size _size;

    public:
        SIMP1E_ECS_COMPONENT("Size")

        SizeComponent() = default;
        SizeComponent(const Size& size) : _size(size) {}
        SizeComponent(sreal width, sreal height) : _size(width, height) {}

        virtual Size GetSize() const { return _size; }

        virtual void SetSize(const Size& size) {
            _size = size;
            SetDirty();
        }
        virtual void SetSize(sreal width, sreal height) {
            _size = Size(width, height);
            SetDirty();
        }
        virtual void SetWidth(sreal width) {
            _size.SetWidth(width);
            SetDirty();
        }
        virtual void SetHeight(sreal height) {
            _size.SetHeight(height);
            SetDirty();
        }
    };
}
