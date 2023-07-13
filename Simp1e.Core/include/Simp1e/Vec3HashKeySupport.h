#pragma once

#include "Vec3.h"

namespace std {
    template <typename T>
    struct hash<Simp1e::Vec3<T>> {
        size_t operator()(const Simp1e::Vec3<T>& vec) const {
            return std::hash<T>()(vec.one()) ^ std::hash<T>()(vec.two()) ^ std::hash<T>()(vec.three());
        }
    };
}
