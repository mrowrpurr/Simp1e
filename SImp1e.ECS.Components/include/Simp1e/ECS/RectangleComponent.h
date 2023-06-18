#pragma once

#include <Simp1e/Color.h>
#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/ComponentTypeMacro.h>
#include <Simp1e/Rectangle.h>

#include <optional>
#include <string>

#include "ComponentBase.h"

namespace Simp1e::ECS {
    class RectangleComponent : public ComponentBase {
        Rectangle            _rectangle;
        std::optional<Color> _backgroundColor;
        std::optional<Color> _borderColor;

    public:
        SIMP1E_ECS_COMPONENT("Rectangle")

        RectangleComponent() = default;
        RectangleComponent(
            const Rectangle& rectangle, std::optional<Color> backgroundColor = std::nullopt,
            std::optional<Color> borderColor = std::nullopt
        )
            : _rectangle(rectangle), _backgroundColor(backgroundColor), _borderColor(borderColor) {}

        virtual Rectangle            GetRectangle() const { return _rectangle; }
        virtual std::optional<Color> GetBackgroundColor() const { return _backgroundColor; }
        virtual std::optional<Color> GetBorderColor() const { return _borderColor; }

        virtual void SetRectangle(const Rectangle& rectangle) {
            _rectangle = rectangle;
            SetDirty();
        }
        virtual void SetBackgroundColor(Color backgroundColor) {
            _backgroundColor = backgroundColor;
            SetDirty();
        }
        virtual void UnsetBackgroundColor() {
            _backgroundColor = std::nullopt;
            SetDirty();
        }
        virtual void SetBorderColor(Color borderColor) {
            _borderColor = borderColor;
            SetDirty();
        }
        virtual void UnsetBorderColor() {
            _borderColor = std::nullopt;
            SetDirty();
        }
    };
}
