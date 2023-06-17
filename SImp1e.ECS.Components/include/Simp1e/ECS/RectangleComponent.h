#pragma once

#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/ComponentTypeMacro.h>
#include <Simp1e/Rectangle.h>

#include <string>

#include "ComponentBase.h"

namespace Simp1e::ECS {
    class RectangleComponent : public ComponentBase {
        Rectangle _rectangle;

    public:
        SIMP1E_ECS_COMPONENT("Rectangle")

        RectangleComponent() = default;
        RectangleComponent(const Rectangle& rectangle) : _rectangle(rectangle) {}

        Rectangle GetRectangle() const { return _rectangle; }

        void SetRectangle(const Rectangle& rectangle) {
            _rectangle = rectangle;
            SetDirty();
        }
    };
}
