#pragma once

namespace Prototyping::UI {
    class UIPosition {
        double _x = 0;
        double _y = 0;

    public:
        UIPosition() = default;
        UIPosition(double x, double y) : _x(x), _y(y) {}

        double x() const { return _x; }
        double y() const { return _y; }
    };
}
