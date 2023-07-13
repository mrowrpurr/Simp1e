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

        virtual void SetOrigin(const Point& origin) { _origin = origin; }
        virtual void SetTopLeft(const Point& topLeft) { SetOrigin(topLeft); }

        virtual Point topLeft() const { return _origin; }
        virtual Point topRight() const { return Point(x() + width(), y()); }
        virtual Point bottomLeft() const { return Point(x(), y() + height()); }
        virtual Point bottomRight() const { return Point(x() + width(), y() + height()); }
        virtual Point center() const { return Point(x() + width() / 2, y() + height() / 2); }

        virtual bool contains(const Point& point) const {
            return point.x() >= x() && point.x() <= x() + width() && point.y() >= y() && point.y() <= y() + height();
        }
        virtual bool contains(const Position& position) const { return contains(position.ToPoint()); }
        virtual bool contains(const Rectangle& rectangle) const {
            return contains(rectangle.topLeft()) && contains(rectangle.topRight()) &&
                   contains(rectangle.bottomLeft()) && contains(rectangle.bottomRight());
        }

        virtual bool containsInside(const Point& point) const {
            return point.x() > x() && point.x() < x() + width() && point.y() > y() && point.y() < y() + height();
        }
        virtual bool containsInside(const Position& position) const { return containsInside(position.ToPoint()); }
        virtual bool containsInside(const Rectangle& rectangle) const {
            return containsInside(rectangle.topLeft()) && containsInside(rectangle.topRight()) &&
                   containsInside(rectangle.bottomLeft()) && containsInside(rectangle.bottomRight());
        }

        virtual bool intersects(const Rectangle& other) const {
            return !(
                other.x() > x() + width() || x() > other.x() + other.width() || other.y() > y() + height() ||
                y() > other.y() + other.height()
            );
        }

        virtual bool intersectsInside(const Rectangle& other) const {
            return !(
                other.x() >= x() + width() || x() >= other.x() + other.width() || other.y() >= y() + height() ||
                y() >= other.y() + other.height()
            );
        }

        virtual bool isNull() const { return width() == 0 && height() == 0; }

        std::string ToString() const {
            return string_format(
                "Rectangle({}, {}, {}, {})", topLeft().ToString(), topRight().ToString(), bottomLeft().ToString(),
                bottomRight().ToString()
            );
        }
        operator std::string() const { return ToString(); }

        bool operator==(const Rectangle& other) const { return _origin == other._origin && _size == other._size; }
    };
}