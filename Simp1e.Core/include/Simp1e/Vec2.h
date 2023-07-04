#pragma once

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

        Vec2() = default;
        Vec2(T one, T two) : _one(one), _two(two) {}
        Vec2(const Params& params) : _one(params.one), _two(params.two) {}

        virtual T one() const { return _one; }
        virtual T two() const { return _two; }

        virtual void SetOne(T one) { _one = one; }
        virtual void SetTwo(T two) { _two = two; }

        bool operator==(const Vec2& other) const { return _one == other._one && _two == other._two; }
    };
}
