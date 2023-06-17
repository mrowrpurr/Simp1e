#pragma once

#include "Point.h"

namespace Simp1e {
    class Position : public Point {
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

        virtual Point& point() { return _point; }
        virtual sreal& z() { return _z; }
        virtual sreal& angle() { return _angle; }

        virtual const Point& point() const { return _point; }
        virtual const sreal& z() const { return _z; }
        virtual const sreal& angle() const { return _angle; }

        sreal&       x() override { return _point.x(); }
        sreal&       y() override { return _point.y(); }
        const sreal& x() const override { return _point.x(); }
        const sreal& y() const override { return _point.y(); }
    };
}
