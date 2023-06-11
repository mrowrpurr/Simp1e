#pragma once

#include <cstdint>

namespace Simp1e::UI {
    class UISize {
        double _width  = 0;
        double _height = 0;

    public:
        UISize() = default;
        // UISize(int width, int height) : _width(width), _height(height) {}
        // UISize(uint32_t width, uint32_t height) : _width(width), _height(height) {}
        UISize(double width, double height) : _width(width), _height(height) {}

        double width() const { return _width; }
        double height() const { return _height; }
    };
}
