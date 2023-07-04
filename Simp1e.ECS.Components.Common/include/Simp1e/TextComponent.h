#pragma once

#include <string>

#include "ITextComponent.h"

namespace Simp1e {

    class TextComponent : public ITextComponent {
        std::string _text;

    public:
        TextComponent() = default;
        TextComponent(const char* text) : _text(text) {}

        const char* GetText() const override { return _text.c_str(); }

        void SetText(const char* text) override {
            _text = text;
            SetDirtyFlag(Fields::Text);
        }
    };
}