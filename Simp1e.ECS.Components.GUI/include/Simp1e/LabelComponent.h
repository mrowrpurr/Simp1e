#pragma once

#include <string>

#include "ILabelComponent.h"

namespace Simp1e {

    class LabelComponent : public ILabelComponent {
        std::string _text;

    public:
        LabelComponent() = default;
        LabelComponent(const char* text) : _text(text) {}

        const char* GetText() const override { return _text.c_str(); }

        void SetText(const char* text) override {
            _text = text;
            SetDirty(true);
        }
    };
}