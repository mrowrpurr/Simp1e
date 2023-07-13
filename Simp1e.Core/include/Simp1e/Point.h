#pragma once

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
    };
}
