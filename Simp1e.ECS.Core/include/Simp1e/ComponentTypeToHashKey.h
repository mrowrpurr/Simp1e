#pragma once

#include "ComponentType.h"
#include "ComponentTypeHashKey.h"

namespace Simp1e {
    inline ComponentTypeHashKey ComponentTypeToHashKey(const ComponentType& type) { return type; }
}
