#pragma once

#include <cstdint>

namespace Simp1e::UI {
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

        bool operator==(const UIPoint& other) const { return _x == other._x && _y == other._y; }
        bool operator!=(const UIPoint& other) const { return !(*this == other); }

        static UIPoint Invalid() { return UIPoint{UINT32_MAX, UINT32_MAX}; }
        static bool    IsValid(const UIPoint& point) { return point != Invalid(); }
    };
}
