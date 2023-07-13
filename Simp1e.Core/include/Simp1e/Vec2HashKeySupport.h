#pragma once

#include "Vec2.h"

namespace std {
    template <typename T>
    struct hash<Simp1e::Vec2<T>> {
        size_t operator()(const Simp1e::Vec2<T>& vec) const {
            return std::hash<T>()(vec.one()) ^ std::hash<T>()(vec.two());
        }
    };
}
