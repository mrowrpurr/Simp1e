#pragma once

#include "ILineColorComponent.h"

namespace Simp1e {

    class LineColorComponent : public ILineColorComponent {
        Color _color;

    public:
        LineColorComponent() = default;
        LineColorComponent(const Color& color) : _color(color) {}
        LineColorComponent(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
            : _color(red, green, blue, alpha) {}

        Color GetColor() const override { return _color; }
        void  SetColor(const Color& color) override {
            _color = color;
            SetDirtyFlag(Fields::Color);
        }
    };
}