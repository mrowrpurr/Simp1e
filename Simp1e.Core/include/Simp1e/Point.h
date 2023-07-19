#pragma once

#include <string_format.h>

#include <string>

#include "Vec2.h"
#include "sreal.h"

namespace Simp1e {

    template <typename T>
    class PointT : public Vec2<T> {
    public:
        PointT(T x = {}, T y = {}) : Vec2<T>(x, y) {}

        T x() const { return this->one(); }
        T y() const { return this->two(); }

        void SetX(T x) { this->SetOne(x); }
        void SetY(T y) { this->SetTwo(y); }

        void AddToX(T x) { this->AddToOne(x); }
        void AddToY(T y) { this->AddToTwo(y); }

        void SubtractFromX(T x) { this->SubtractFromOne(x); }
        void SubtractFromY(T y) { this->SubtractFromTwo(y); }

        void MultiplyX(T x) { this->MultiplyOne(x); }
        void MultiplyY(T y) { this->MultiplyTwo(y); }

        bool operator==(const PointT& other) const { return this->one() == other.one() && this->two() == other.two(); }

        PointT operator+(const PointT& other) const {
            return PointT(this->one() + other.one(), this->two() + other.two());
        }
        PointT operator-(const PointT& other) const {
            return PointT(this->one() - other.one(), this->two() - other.two());
        }
        PointT operator*(const PointT& other) const {
            return PointT(this->one() * other.one(), this->two() * other.two());
        }
        PointT operator/(const PointT& other) const {
            return PointT(this->one() / other.one(), this->two() / other.two());
        }

        std::string ToString() const { return string_format("Point({}, {})", x(), y()); }
        operator std::string() const { return ToString(); }

        T distance(const PointT& other) const {
            return std::sqrt(std::pow(x() - other.x(), 2) + std::pow(y() - other.y(), 2));
        }
    };

    using Point  = PointT<int>;
    using PointF = PointT<sreal>;
}
