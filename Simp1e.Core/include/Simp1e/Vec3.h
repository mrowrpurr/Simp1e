#pragma once

#include <string>

#include "Vec2.h"

namespace Simp1e {

    template <typename T>
    class Vec3 : public Vec2<T> {
        T _three = 0;

    public:
        struct Params {
            T one;
            T two;
            T three;
        };

        Vec3() = default;
        Vec3(T one, T two, T three) : Vec2<T>(one, two), _three(three) {}
        Vec3(const Params& params) : Vec2<T>(params.one, params.two), _three(params.three) {}

        virtual T one() const { return Vec2<T>::one(); }
        virtual T two() const { return Vec2<T>::two(); }
        virtual T three() const { return _three; }

        virtual void SetOne(T one) { Vec2<T>::SetOne(one); }
        virtual void SetTwo(T two) { Vec2<T>::SetTwo(two); }
        virtual void SetThree(T three) { _three = three; }

        virtual bool operator==(const Vec3& other) const {
            return Vec2<T>::operator==(other) && _three == other._three;
        }
    };
}
