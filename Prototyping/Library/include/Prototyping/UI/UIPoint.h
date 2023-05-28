#pragma once

#include <cstdint>

namespace Prototyping::UI {
    class UIPoint {
        double _x = 0;
        double _y = 0;

    public:
        UIPoint() = default;
        UIPoint(int x, int y) : _x(x), _y(y) {}
        UIPoint(uint32_t x, uint32_t y) : _x(x), _y(y) {}
        UIPoint(double x, double y) : _x(x), _y(y) {}

        double x() const { return _x; }
        double y() const { return _y; }
    };
}
