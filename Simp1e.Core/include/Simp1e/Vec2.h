#pragma once

#include <string_format.h>

#include <cmath>
#include <functional>
#include <string>

namespace Simp1e {

    template <typename T>
    class Vec2 {
        T _one = 0;
        T _two = 0;

    public:
        struct Params {
            T one;
            T two;
        };

        struct Hash {
            size_t operator()(const Vec2& vec) const { return std::hash<T>()(vec.one()) ^ std::hash<T>()(vec.two()); }
        };

        struct Equal {
            bool operator()(const Vec2& left, const Vec2& right) const { return left == right; }
        };

        Vec2() = default;
        Vec2(T one, T two) : _one(one), _two(two) {}
        Vec2(const Params& params) : _one(params.one), _two(params.two) {}

        virtual T one() const { return _one; }
        virtual T two() const { return _two; }

        virtual void SetOne(T one) { _one = one; }
        virtual void SetTwo(T two) { _two = two; }

        virtual bool operator==(const Vec2& other) const { return _one == other._one && _two == other._two; }

        Vec2 operator+(const Vec2& other) const { return Vec2(_one + other._one, _two + other._two); }
        Vec2 operator-(const Vec2& other) const { return Vec2(_one - other._one, _two - other._two); }
        Vec2 operator*(const Vec2& other) const { return Vec2(_one * other._one, _two * other._two); }
        Vec2 operator*(T scalar) const { return Vec2(_one * scalar, _two * scalar); }

        virtual std::string ToString() const { return string_format("Vec2({:.2f}, {:.2f})", _one, _two); }
        virtual operator std::string() const { return ToString(); }

        T distance(T otherOne, T otherTwo) const {
            T one = _one - otherOne;
            T two = _two - otherTwo;
            return std::sqrt(one * one + two * two);
        }
    };
}
