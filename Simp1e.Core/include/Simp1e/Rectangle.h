#pragma once

#include <string>

#include "Point.h"
#include "Position.h"
#include "Size.h"

namespace Simp1e {

    template <typename T>
    class RectangleT {
        PointT<T> _point;
        SizeT<T>  _size;

    public:
        RectangleT() = default;
        RectangleT(const PointT<T>& point, const SizeT<T>& size) : _point(point), _size(size) {}
        RectangleT(T x, T y, T width, T height) : _point(x, y), _size(width, height) {}

        PointT<T> point() const { return _point; }
        SizeT<T>  size() const { return _size; }

        T x() const { return _point.x(); }
        T y() const { return _point.y(); }
        T width() const { return _size.width(); }
        T height() const { return _size.height(); }

        void SetPoint(const PointT<T>& point) { _point = point; }
        void SetSize(const SizeT<T>& size) { _size = size; }

        PointT<T> topLeft() const { return _point; }
        PointT<T> topRight() const { return PointT<T>(x() + width(), y()); }
        PointT<T> bottomLeft() const { return PointT<T>(x(), y() + height()); }
        PointT<T> bottomRight() const { return PointT<T>(x() + width(), y() + height()); }
        PointT<T> center() const { return PointT<T>(x() + width() / 2, y() + height() / 2); }

        bool contains(const PointT<T>& point) const {
            return point.x() >= x() && point.x() <= x() + width() && point.y() >= y() && point.y() <= y() + height();
        }
        bool contains(const Position& position) const { return contains(position.ToPoint()); }
        bool contains(const RectangleT& rectangle) const {
            return contains(rectangle.topLeft()) && contains(rectangle.topRight()) &&
                   contains(rectangle.bottomLeft()) && contains(rectangle.bottomRight());
        }

        bool containsInside(const PointT<T>& point) const {
            return point.x() > x() && point.x() < x() + width() && point.y() > y() && point.y() < y() + height();
        }
        bool containsInside(const Position& position) const { return containsInside(position.ToPoint()); }
        bool containsInside(const RectangleT& rectangle) const {
            return containsInside(rectangle.topLeft()) && containsInside(rectangle.topRight()) &&
                   containsInside(rectangle.bottomLeft()) && containsInside(rectangle.bottomRight());
        }

        bool intersects(const RectangleT& other) const {
            return !(
                other.x() > x() + width() || x() > other.x() + other.width() || other.y() > y() + height() ||
                y() > other.y() + other.height()
            );
        }

        bool intersectsInside(const RectangleT& other) const {
            return !(
                other.x() >= x() + width() || x() >= other.x() + other.width() || other.y() >= y() + height() ||
                y() >= other.y() + other.height()
            );
        }

        bool isNull() const { return width() == 0 && height() == 0; }

        std::string ToString() const {
            return string_format(
                "RectangleT({}, {}, {}, {})", topLeft().ToString(), topRight().ToString(), bottomLeft().ToString(),
                bottomRight().ToString()
            );
        }
        operator std::string() const { return ToString(); }

        bool operator==(const RectangleT& other) const { return _point == other._point && _size == other._size; }
    };

    using Rectangle  = RectangleT<int>;
    using RectangleF = RectangleT<sreal>;
}