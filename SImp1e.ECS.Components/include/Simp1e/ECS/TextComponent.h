#pragma once

#include <Simp1e/Color.h>
#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/ComponentTypeMacro.h>

#include <optional>
#include <string>

#include "ComponentBase.h"

namespace Simp1e::ECS {
    class TextComponent : public ComponentBase {
        std::string          _text;
        std::optional<Color> _color;

    public:
        SIMP1E_ECS_COMPONENT("Text")

        TextComponent() = default;
        TextComponent(const std::string& text, std::optional<Color> color = std::nullopt)
            : _text(text), _color(color) {}

        virtual std::string          GetText() const { return _text; }
        virtual std::optional<Color> GetColor() const { return _color; }

        virtual void SetText(const std::string& text) {
            _text = text;
            SetDirty();
        }
        virtual void SetColor(Color color) {
            _color = color;
            SetDirty();
        }
        virtual void UnsetColor() {
            _color = std::nullopt;
            SetDirty();
        }
    };
}
