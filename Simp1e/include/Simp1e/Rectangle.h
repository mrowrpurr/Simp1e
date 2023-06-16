#pragma once

#include "Point.h"
#include "Size.h"

namespace Simp1e {

    class Rectangle {
        Point _origin;
        Size  _size;

    public:
        struct Params {
            Point::Params origin;
            Size::Params  size;
        };

        Rectangle() = default;
        Rectangle(sreal x, sreal y, sreal width, sreal height)
            : _origin(x, y), _size(width, height) {}
        Rectangle(const Params& params) : _origin(params.origin), _size(params.size) {}

        Point& origin() { return _origin; }
        Size&  size() { return _size; }

        const Point& origin() const { return _origin; }
        const Size&  size() const { return _size; }

        sreal& x() { return _origin.x(); }
        sreal& y() { return _origin.y(); }
        sreal& width() { return _size.width(); }
        sreal& height() { return _size.height(); }

        const sreal& x() const { return _origin.x(); }
        const sreal& y() const { return _origin.y(); }
        const sreal& width() const { return _size.width(); }
        const sreal& height() const { return _size.height(); }
    };
}