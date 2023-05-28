#pragma once

#include <cstdint>

namespace Prototyping::UI {
    class UIPosition {
        uint32_t _x = 0;
        uint32_t _y = 0;

    public:
        UIPosition() = default;
        UIPosition(uint32_t x, uint32_t y) : _x(x), _y(y) {}

        uint32_t x() const { return _x; }
        uint32_t y() const { return _y; }
    };
}
