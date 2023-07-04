#pragma once

#include "sreal.h"

namespace Simp1e {
    class Vec3 {
        sreal _one   = 0;
        sreal _two   = 0;
        sreal _three = 0;

    public:
        struct Params {
            sreal one;
            sreal two;
            sreal three;
        };

        Vec3() = default;
        Vec3(sreal one, sreal two, sreal three) : _one(one), _two(two), _three(three) {}
        Vec3(const Params& params) : _one(params.one), _two(params.two), _three(params.three) {}

        virtual sreal one() const { return _one; }
        virtual sreal two() const { return _two; }
        virtual sreal three() const { return _three; }

        virtual void SetOne(sreal one) { _one = one; }
        virtual void SetTwo(sreal two) { _two = two; }
        virtual void SetThree(sreal three) { _three = three; }

        bool operator==(const Vec3& other) const {
            return _one == other._one && _two == other._two && _three == other._three;
        }
    };
}
