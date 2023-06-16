#pragma once

#include <Simp1e/Color.h>
#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/ComponentTypeMacro.h>
#include <Simp1e/Rectangle.h>

#include <optional>

#include "ComponentTypes.h"

namespace Simp1e::ECS {
    class RectangleComponent {
        Rectangle            _rectangle;
        std::optional<Color> _fillColor;

    public:
        SIMP1E_ECS_COMPONENT(ComponentTypes::Rectangle)

        RectangleComponent() = default;
        RectangleComponent(const Rectangle& rectangle) : _rectangle(rectangle) {}
        RectangleComponent(const Rectangle& rectangle, const Color& fillColor)
            : _rectangle(rectangle), _fillColor(fillColor) {}

        virtual Rectangle&       GetRectangle() { return _rectangle; }
        virtual const Rectangle& GetRectangle() const { return _rectangle; }
        virtual void SetRectangle(const Rectangle& rectangle) { _rectangle = rectangle; }

        virtual std::optional<Color>&       GetFillColor() { return _fillColor; }
        virtual const std::optional<Color>& GetFillColor() const { return _fillColor; }
        virtual void                        SetFillColor(const Color& color) { _fillColor = color; }
        virtual void                        ClearFillColor() { _fillColor = std::nullopt; }
    };
}
