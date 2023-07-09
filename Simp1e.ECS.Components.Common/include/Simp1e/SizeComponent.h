#pragma once

#include <Simp1e/Size.h>

#include "ISizeComponent.h"

namespace Simp1e {

    class SizeComponent : public ISizeComponent {
        Size _size;

    public:
        SizeComponent() = default;
        SizeComponent(sreal width, sreal height) : _size(width, height) {}
        SizeComponent(const Size& size) : _size(size) {}

        Size GetSize() const override { return _size; }

        sreal GetWidth() const override { return _size.width(); }
        sreal GetHeight() const override { return _size.height(); }

        void SetWidth(sreal width) override {
            _size.SetWidth(width);
            SetDirtyFlag(Fields::Width);
        }
        void SetHeight(sreal height) override {
            _size.SetHeight(height);
            SetDirtyFlag(Fields::Height);
        }
    };
}