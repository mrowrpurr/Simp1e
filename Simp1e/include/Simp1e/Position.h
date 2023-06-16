#pragma once

#include "Point.h"

namespace Simp1e {
    class Position {
        Point _point;
        sreal _z     = 0;
        sreal _angle = 0;

    public:
        struct Params {
            Point::Params point;
            sreal         z;
            sreal         angle;
        };

        Position() = default;
        Position(sreal x, sreal y, sreal z = 0, sreal angle = 0)
            : _point(x, y), _z(z), _angle(angle) {}
        Position(const Params& params) : _point(params.point), _z(params.z), _angle(params.angle) {}

        Point& point() { return _point; }
        sreal& z() { return _z; }
        sreal& angle() { return _angle; }

        const Point& point() const { return _point; }
        const sreal& z() const { return _z; }
        const sreal& angle() const { return _angle; }
    };
}
