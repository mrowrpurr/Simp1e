#pragma once

#include <string>

#include "IWindowMenuItemComponent.h"

namespace Simp1e {

    class WindowMenuItemComponent : public IWindowMenuItemComponent {
        Entity      _windowMenuEntity;
        std::string _text;
        bool        _visible = true;
        bool        _enabled = true;

    public:
        WindowMenuItemComponent(Entity windowMenuEntity, const char* text)
            : _windowMenuEntity(windowMenuEntity), _text(text) {}

        Entity      GetWindowMenuEntity() const override { return _windowMenuEntity; }
        const char* GetText() const override { return _text.c_str(); }
        void        SetText(const char* text) override {
            _text = text;
            SetDirtyFlag(Fields::Text);
        }
        bool IsVisible() const override { return _visible; }
        void SetVisible(bool visible) override {
            _visible = visible;
            SetDirtyFlag(Fields::Visible);
        }
        bool IsEnabled() const override { return _enabled; }
        void SetEnabled(bool enabled) override {
            _enabled = enabled;
            SetDirtyFlag(Fields::Enabled);
        }
    };
}
