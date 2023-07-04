#pragma once

#include "sreal.h"

namespace Simp1e {
    class Vec2 {
        sreal _one = 0;
        sreal _two = 0;

    public:
        struct Params {
            sreal one;
            sreal two;
        };

        Vec2() = default;
        Vec2(sreal one, sreal two) : _one(one), _two(two) {}
        Vec2(const Params& params) : _one(params.one), _two(params.two) {}

        virtual sreal one() const { return _one; }
        virtual sreal two() const { return _two; }

        virtual void SetOne(sreal one) { _one = one; }
        virtual void SetTwo(sreal two) { _two = two; }

        bool operator==(const Vec2& other) const { return _one == other._one && _two == other._two; }
    };
}
