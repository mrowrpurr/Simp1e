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
        Rectangle(const Point& origin, const Size& size) : _origin(origin), _size(size) {}
        Rectangle(sreal x, sreal y, sreal width, sreal height) : _origin(x, y), _size(width, height) {}
        Rectangle(const Params& params) : _origin(params.origin), _size(params.size) {}
        Rectangle(const Size& size) : _origin(0, 0), _size(size) {}
        Rectangle(sreal width, sreal height) : _origin(0, 0), _size(width, height) {}

        virtual Point origin() const { return _origin; }
        virtual Size  size() const { return _size; }

        virtual sreal x() const { return _origin.x(); }
        virtual sreal y() const { return _origin.y(); }
        virtual sreal width() const { return _size.width(); }
        virtual sreal height() const { return _size.height(); }
    };
}