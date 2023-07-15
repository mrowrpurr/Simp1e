#pragma once

#include "Vec2.h"
#include "sreal.h"

namespace Simp1e {

    template <typename T>
    class SizeT : public Vec2<T> {
    public:
        SizeT() = default;
        SizeT(T width, T height) : Vec2<T>(width, height) {}

        T width() const { return this->one(); }
        T height() const { return this->two(); }

        void SetWidth(T width) { this->SetOne(width); }
        void SetHeight(T height) { this->SetTwo(height); }

        bool IsNull() const { return width() == 0 && height() == 0; }

        SizeT operator+(const SizeT& other) const { return SizeT(width() + other.width(), height() + other.height()); }
        SizeT operator-(const SizeT& other) const { return SizeT(width() - other.width(), height() - other.height()); }
        SizeT operator*(const SizeT& other) const { return SizeT(width() * other.width(), height() * other.height()); }
        SizeT operator*(T scalar) const { return SizeT(width() * scalar, height() * scalar); }
    };

    using Size  = SizeT<int>;
    using SizeF = SizeT<sreal>;
}
