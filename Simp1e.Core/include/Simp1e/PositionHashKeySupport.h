#pragma once

#include "Position.h"

namespace std {
    template <>
    struct hash<Simp1e::Position> {
        size_t operator()(const Simp1e::Position& pos) const {
            return std::hash<int>()(pos.x()) ^ std::hash<int>()(pos.y());
        }
    };
}
