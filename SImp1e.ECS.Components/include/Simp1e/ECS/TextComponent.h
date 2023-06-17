#pragma once

#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/ComponentTypeMacro.h>

#include <string>

namespace Simp1e::ECS {
    class TextComponent {
        std::string _text;

    public:
        SIMP1E_ECS_COMPONENT("Text")

        TextComponent() = default;
        TextComponent(const std::string& text) : _text(text) {}

        std::string& GetText() { return _text; }

        void SetText(const std::string& text) { _text = text; }
    };
}
