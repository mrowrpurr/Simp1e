#pragma once

#include "ComponentType.h"
#include "ComponentTypeHashKey.h"

namespace Simp1e {
    inline ComponentType ComponentTypeFromHashKey(const ComponentTypeHashKey& hashKey) { return hashKey.c_str(); }
}
