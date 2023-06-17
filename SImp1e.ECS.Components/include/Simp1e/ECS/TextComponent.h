#pragma once

#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/ComponentTypeMacro.h>

#include <string>

#include "ComponentBase.h"

namespace Simp1e::ECS {
    class TextComponent : public ComponentBase {
        std::string _text;

    public:
        SIMP1E_ECS_COMPONENT("Text")

        TextComponent() = default;
        TextComponent(const std::string& text) : _text(text) {}

        virtual std::string GetText() const { return _text; }

        virtual void SetText(const std::string& text) {
            _text = text;
            SetDirty();
        }
    };
}
