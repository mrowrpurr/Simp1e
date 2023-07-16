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

        void AddToX(T x) { this->AddToOne(x); }
        void AddToY(T y) { this->AddToTwo(y); }
        void AddToZ(T z) { this->AddToThree(z); }

        void SubtractFromX(T x) { this->SubtractFromOne(x); }
        void SubtractFromY(T y) { this->SubtractFromTwo(y); }
        void SubtractFromZ(T z) { this->SubtractFromThree(z); }

        void MultiplyX(T x) { this->MultiplyOne(x); }
        void MultiplyY(T y) { this->MultiplyTwo(y); }
        void MultiplyZ(T z) { this->MultiplyThree(z); }

        bool operator==(const PositionT& other) const {
            return x() == other.x() && y() == other.y() && z() == other.z();
        }

        std::string ToString() const { return string_format("Position({}, {}, {})", x(), y(), z()); }
        operator std::string() const { return ToString(); }

        PositionT operator+(const PositionT& other) const {
            return PositionT(this->one() + other.one(), this->two() + other.two(), this->three() + other.three());
        }
        PositionT operator-(const PositionT& other) const {
            return PositionT(this->one() - other.one(), this->two() - other.two(), this->three() - other.three());
        }
        PositionT operator*(const PositionT& other) const {
            return PositionT(this->one() * other.one(), this->two() * other.two(), this->three() * other.three());
        }
        PositionT operator*(T scalar) const {
            return PositionT(this->one() * scalar, this->two() * scalar, this->three() * scalar);
        }
    };

    using Position  = PositionT<int>;
    using PositionF = PositionT<sreal>;
}
