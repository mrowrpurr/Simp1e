#pragma once

#include <string>

#include "IWindowComponent.h"

namespace Simp1e {

    class WindowComponent : public IWindowComponent {
        std::string _title;
        std::string _statusBarText;

    public:
        WindowComponent() = default;
        WindowComponent(const char* title) : _title(title) {}

        const char* GetTitle() const override { return _title.c_str(); }
        const char* GetStatusBarText() const override { return _statusBarText.c_str(); }

        void SetTitle(const char* title) override {
            _title = title;
            SetDirtyFlag(Fields::Title);
        }

        void SetStatusBarText(const char* text) override {
            _statusBarText = text;
            SetDirtyFlag(Fields::StatusBarText);
        }
    };
}
