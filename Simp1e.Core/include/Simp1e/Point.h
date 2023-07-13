#pragma once

#include <string_format.h>

#include <string>

#include "Vec2.h"
#include "sreal.h"

namespace Simp1e {
    class Point : public Vec2<sreal> {
    public:
        struct Params {
            sreal x;
            sreal y;
        };

        Point() = default;
        Point(sreal x, sreal y) : Vec2(x, y) {}
        Point(const Params& params) : Vec2(params.x, params.y) {}

        virtual sreal x() const { return one(); }
        virtual sreal y() const { return two(); }

        virtual void SetX(sreal x) { SetOne(x); }
        virtual void SetY(sreal y) { SetTwo(y); }

        virtual bool operator==(const Point& other) const { return one() == other.one() && two() == other.two(); }

        virtual Point operator+(const Point& other) const { return Point(one() + other.one(), two() + other.two()); }
        virtual Point operator-(const Point& other) const { return Point(one() - other.one(), two() - other.two()); }
        virtual Point operator*(const Point& other) const { return Point(one() * other.one(), two() * other.two()); }
        virtual Point operator/(const Point& other) const { return Point(one() / other.one(), two() / other.two()); }

        virtual std::string ToString() const { return string_format("Point({}, {})", x(), y()); }
        operator std::string() const { return ToString(); }

        virtual Point ToPoint() const { return *this; }

        virtual sreal distance(const Point& other) const {
            return std::sqrt(std::pow(x() - other.x(), 2) + std::pow(y() - other.y(), 2));
        }
    };
}
