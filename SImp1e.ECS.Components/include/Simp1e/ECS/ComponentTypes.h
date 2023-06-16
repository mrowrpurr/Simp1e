#pragma once

#include <cstdint>

#define SIMP1E_ECS_COMPONENT(type) \
    static ComponentType GetComponentType() { return static_cast<ComponentType>(type); }

namespace Simp1e::ECS {

    enum class ComponentTypes : uint32_t {
        Position  = 1,
        Rectangle = 2,
    };
}
