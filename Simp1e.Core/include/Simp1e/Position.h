#pragma once

#include <string_format.h>

#include "Point.h"
#include "Vec3.h"
#include "sreal.h"

namespace Simp1e {
    class Position : public Vec3<sreal> {
    public:
        struct Params {
            sreal x;
            sreal y;
            sreal z;
        };

        struct Hash {
            size_t operator()(const Position& vec) const {
                return std::hash<sreal>()(vec.x()) ^ std::hash<sreal>()(vec.y()) ^ std::hash<sreal>()(vec.z());
            }
        };

        struct Equal {
            bool operator()(const Position& left, const Position& right) const { return left == right; }
        };

        Position() = default;
        Position(sreal x, sreal y, sreal z = 0) : Vec3(x, y, z) {}
        Position(const Params& params) : Vec3(params.x, params.y, params.z) {}
        Position(const Point& point) : Vec3(point.x(), point.y(), 0) {}

        virtual Point ToPoint() const { return Point(x(), y()); }

        virtual sreal x() const { return one(); }
        virtual sreal y() const { return two(); }
        virtual sreal z() const { return three(); }

        virtual void SetX(sreal x) { SetOne(x); }
        virtual void SetY(sreal y) { SetTwo(y); }
        virtual void SetZ(sreal z) { SetThree(z); }

        virtual bool operator==(const Position& other) const {
            return x() == other.x() && y() == other.y() && z() == other.z();
        }

        std::string ToString() const { return string_format("Position({:.2f}, {:.2f}, {:.2f})", x(), y(), z()); }
        operator std::string() const { return ToString(); }

        Position operator+(const Position& other) const {
            return Position(one() + other.one(), two() + other.two(), three() + other.three());
        }
        Position operator-(const Position& other) const {
            return Position(one() - other.one(), two() - other.two(), three() - other.three());
        }
        Position operator*(const Position& other) const {
            return Position(one() * other.one(), two() * other.two(), three() * other.three());
        }
        Position operator*(sreal scalar) const { return Position(one() * scalar, two() * scalar, three() * scalar); }
    };
}
