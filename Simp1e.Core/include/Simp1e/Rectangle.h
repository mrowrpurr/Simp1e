#pragma once

#include <string>

#include "Point.h"
#include "Position.h"
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
        Rectangle(const Position& origin, const Size& size) : _origin(origin.ToPoint()), _size(size) {}
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

        virtual Point topLeft() const { return _origin; }
        virtual Point topRight() const { return Point(x() + width(), y()); }
        virtual Point bottomLeft() const { return Point(x(), y() + height()); }
        virtual Point bottomRight() const { return Point(x() + width(), y() + height()); }
        virtual Point center() const { return Point(x() + width() / 2, y() + height() / 2); }

        virtual bool constains(const Point& point) const {
            return point.x() >= x() && point.x() <= x() + width() && point.y() >= y() && point.y() <= y() + height();
        }
        virtual bool constains(const Position& position) const { return constains(position.ToPoint()); }

        virtual bool intersects(const Rectangle& other) const {
            return constains(other.topLeft()) || constains(other.topRight()) || constains(other.bottomLeft()) ||
                   constains(other.bottomRight());
        }

        std::string ToString() const {
            return string_format("Rectangle(({}, {}), ({}, {}))", x(), y(), width(), height());
        }
        operator std::string() const { return ToString(); }
    };
}