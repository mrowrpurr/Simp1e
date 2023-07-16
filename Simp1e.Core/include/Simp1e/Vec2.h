#pragma once

#include <string_format.h>

#include <cmath>
#include <functional>
#include <string>
#include <vector>

namespace Simp1e {

    template <typename T>
    class Vec2 {
        T _one = 0;
        T _two = 0;

    public:
        Vec2() = default;
        Vec2(T one, T two) : _one(one), _two(two) {}
        Vec2(const std::vector<T>& values) {
            if (values.size() > 0) _one = values[0];
            if (values.size() > 1) _two = values[1];
        }

        T one() const { return _one; }
        T two() const { return _two; }

        void SetOne(T one) { _one = one; }
        void SetTwo(T two) { _two = two; }

        void AddToOne(T one) { _one += one; }
        void AddToTwo(T two) { _two += two; }

        void SubtractFromOne(T one) { _one -= one; }
        void SubtractFromTwo(T two) { _two -= two; }

        void MultiplyOne(T one) { _one *= one; }
        void MultiplyTwo(T two) { _two *= two; }

        bool operator==(const Vec2& other) const { return _one == other._one && _two == other._two; }

        Vec2 operator+(const Vec2& other) const { return Vec2(_one + other._one, _two + other._two); }
        Vec2 operator-(const Vec2& other) const { return Vec2(_one - other._one, _two - other._two); }
        Vec2 operator*(const Vec2& other) const { return Vec2(_one * other._one, _two * other._two); }
        Vec2 operator*(T scalar) const { return Vec2(_one * scalar, _two * scalar); }

        bool IsNull() const { return _one == T{} && _two == T{}; }

        std::string ToString() const { return string_format("Vec2({}, {})", _one, _two); }
        operator std::string() const { return ToString(); }

        std::vector<T> ToVector() const { return {_one, _two}; }
        operator std::vector<T>() const { return ToVector(); }

        T distance(T otherOne, T otherTwo) const {
            T one = _one - otherOne;
            T two = _two - otherTwo;
            return std::sqrt(one * one + two * two);
        }
    };
}
