#pragma once

#include "IFillColorComponent.h"

namespace Simp1e {

    class FillColorComponent : public IFillColorComponent {
        Color _color;

    public:
        FillColorComponent() = default;
        FillColorComponent(const Color& color) : _color(color) {}
        FillColorComponent(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
            : _color(red, green, blue, alpha) {}

        Color GetColor() const override { return _color; }
        void  SetColor(const Color& color) override {
            _color = color;
            SetDirtyFlag(Fields::Color);
        }
    };
}