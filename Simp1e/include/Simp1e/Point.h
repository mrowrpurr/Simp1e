#pragma once

#include "sreal.h"

namespace Simp1e {
    class Point {
        sreal _x = 0;
        sreal _y = 0;

    public:
        struct Params {
            sreal x;
            sreal y;
        };

        Point() = default;
        Point(sreal x, sreal y) : _x(x), _y(y) {}
        Point(const Params& params) : _x(params.x), _y(params.y) {}

        sreal& x() { return _x; }
        sreal& y() { return _y; }

        const sreal& x() const { return _x; }
        const sreal& y() const { return _y; }
    };
}
