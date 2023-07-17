#pragma once

#include "Vec2.h"

namespace Simp1e {

    template <typename T>
    class Vec3 : public Vec2<T> {
        T _three = 0;

    public:
        Vec3(T one = {}, T two = {}, T three = {}) : Vec2<T>(one, two), _three(three) {}
        Vec3(const std::vector<T>& values) : Vec2<T>(values) {
            if (values.size() > 2) _three = values[2];
        }

        T one() const { return Vec2<T>::one(); }
        T two() const { return Vec2<T>::two(); }
        T three() const { return _three; }

        void SetOne(T one) { Vec2<T>::SetOne(one); }
        void SetTwo(T two) { Vec2<T>::SetTwo(two); }
        void SetThree(T three) { _three = three; }

        void AddToOne(T one) { Vec2<T>::AddToOne(one); }
        void AddToTwo(T two) { Vec2<T>::AddToTwo(two); }
        void AddToThree(T three) { _three += three; }

        void SubtractFromOne(T one) { Vec2<T>::SubtractFromOne(one); }
        void SubtractFromTwo(T two) { Vec2<T>::SubtractFromTwo(two); }
        void SubtractFromThree(T three) { _three -= three; }

        void MultiplyOne(T one) { Vec2<T>::MultiplyOne(one); }
        void MultiplyTwo(T two) { Vec2<T>::MultiplyTwo(two); }
        void MultiplyThree(T three) { _three *= three; }

        bool operator==(const Vec3& other) const { return Vec2<T>::operator==(other) && _three == other._three; }

        Vec3 operator+(const Vec3& other) const {
            return Vec3(one() + other.one(), two() + other.two(), three() + other.three());
        }
        Vec3 operator-(const Vec3& other) const {
            return Vec3(one() - other.one(), two() - other.two(), three() - other.three());
        }
        Vec3 operator*(const Vec3& other) const {
            return Vec3(one() * other.one(), two() * other.two(), three() * other.three());
        }
        Vec3 operator*(T scalar) const { return Vec3(one() * scalar, two() * scalar, three() * scalar); }

        bool IsNull() const { return Vec2<T>::IsNull() && _three == T{}; }

        std::vector<T> ToVector() const { return {one(), two(), three()}; }
        operator std::vector<T>() const { return ToVector(); }

        std::string ToString() const { return string_format("Vec3({}, {}, {})", one(), two(), three()); }
        operator std::string() const { return ToString(); }
    };
}
