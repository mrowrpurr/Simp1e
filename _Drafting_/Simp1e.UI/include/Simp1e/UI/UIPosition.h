#pragma once

#include <cstdint>

namespace Simp1e::UI {
    class UIPosition {
        double _x = 0;
        double _y = 0;
        double _z = 0;

    public:
        UIPosition() = default;
        UIPosition(int x, int y, int z = 0) : _x(x), _y(y), _z(z) {}
        UIPosition(uint32_t x, uint32_t y, uint32_t z = 0) : _x(x), _y(y), _z(z) {}
        UIPosition(double x, double y, double z = 0) : _x(x), _y(y), _z(z) {}

        double x() const { return _x; }
        double y() const { return _y; }
        double z() const { return _z; }

        bool operator==(const UIPosition& other) const {
            return _x == other._x && _y == other._y && _z == other._z;
        }
        bool operator!=(const UIPosition& other) const { return !(*this == other); }

        static UIPosition Invalid() { return UIPosition{UINT32_MAX, UINT32_MAX, UINT32_MAX}; }
        static bool       IsValid(const UIPosition& point) { return point != Invalid(); }
    };
}
