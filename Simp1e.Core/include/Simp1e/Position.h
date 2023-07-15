#pragma once

#include <string_format.h>

#include "Point.h"
#include "Vec3.h"
#include "sreal.h"

namespace Simp1e {

    template <typename T>
    class PositionT : public Vec3<T> {
    public:
        PositionT(T x = {}, T y = {}, T z = {}) : Vec3<T>(x, y, z) {}
        PositionT(const PointT<T>& point) : Vec3<T>(point.x(), point.y(), {}) {}

        PointT<T> ToPoint() const { return Point(x(), y()); }

        T x() const { return this->one(); }
        T y() const { return this->two(); }
        T z() const { return this->three(); }

        void SetX(T x) { this->SetOne(x); }
        void SetY(T y) { this->SetTwo(y); }
        void SetZ(T z) { this->SetThree(z); }

        bool operator==(const PositionT& other) const {
            return x() == other.x() && y() == other.y() && z() == other.z();
        }

        std::string ToString() const { return string_format("Position({}, {}, {})", x(), y(), z()); }
        operator std::string() const { return ToString(); }

        PositionT operator+(const PositionT& other) const {
            return PositionT(
                one<T>() + other.template one<T>(), two<T>() + other.template two<T>(),
                three<T>() + other.template three<T>()
            );
        }
        PositionT operator-(const PositionT& other) const {
            return PositionT(
                one<T>() - other.template one<T>(), two<T>() - other.template two<T>(),
                three<T>() - other.template three<T>()
            );
        }
        PositionT operator*(const PositionT& other) const {
            return PositionT(
                one<T>() * other.template one<T>(), two<T>() * other.template two<T>(),
                three<T>() * other.template three<T>()
            );
        }
        PositionT operator*(T scalar) const {
            return PositionT(one<T>() * scalar, two<T>() * scalar, three<T>() * scalar);
        }
    };

    using Position  = PositionT<int>;
    using PositionF = PositionT<sreal>;
}
