#pragma once

#include <cstdint>

namespace Prototyping::UI {
    class UISize {
        uint32_t _width  = 0;
        uint32_t _height = 0;

    public:
        UISize() = default;
        UISize(uint32_t width, uint32_t height) : _width(width), _height(height) {}

        uint32_t width() const { return _width; }
        uint32_t height() const { return _height; }
    };
}
