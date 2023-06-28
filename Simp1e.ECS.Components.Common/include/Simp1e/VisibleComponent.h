#pragma once

#include "IVisibleComponent.h"

namespace Simp1e {

    class VisibleComponent : public IVisibleComponent {
        bool _visible = true;

    public:
        VisibleComponent() = default;
        VisibleComponent(bool visible) : _visible(visible) {}

        void SetVisible(bool visible = true) override {
            if (_visible == visible) return;
            _visible = visible;
            SetDirty(true);
        }

        bool IsVisible() const override { return _visible; }
    };
}