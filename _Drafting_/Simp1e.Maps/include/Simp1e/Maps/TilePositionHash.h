#pragma once

#include <cstdint>
#include <functional>

#include "TilePosition.h"

namespace std {
    template <>
    struct hash<Simp1e::Maps::TilePosition> {
        std::size_t operator()(const Simp1e::Maps::TilePosition& pos) const {
            std::size_t h1 = std::hash<uint32_t>{}(pos.x);
            std::size_t h2 = std::hash<uint32_t>{}(pos.y);
            std::size_t h3 = std::hash<uint32_t>{}(pos.z);
            return h1 ^ (h2 << 1) ^ (h3 << 2);  // Or any other kind of combination
        }
    };
}
