#pragma once

#include <string>

#include "IDisplayTextComponent.h"

namespace Simp1e {

    class DisplayTextComponent : public IDisplayTextComponent {
        std::string _displayText;

    public:
        DisplayTextComponent() = default;
        DisplayTextComponent(const char* displayText) : _displayText(displayText) {}

        const char* GetDisplayText() const override { return _displayText.c_str(); }

        void SetDisplayText(const char* displayText) override {
            _displayText = displayText;
            SetDirty(true);
        }
    };
}