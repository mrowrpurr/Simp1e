#pragma once

#include <string>

#include "IWindowComponent.h"

namespace Simp1e {

    class WindowComponent : public IWindowComponent {
        std::string _title;

    public:
        WindowComponent() = default;
        WindowComponent(const char* title) : _title(title) {}

        const char* GetTitle() const override { return _title.c_str(); }

        void SetTitle(const char* title) override {
            _title = title;
            SetDirty(true);
        }
    };
}
