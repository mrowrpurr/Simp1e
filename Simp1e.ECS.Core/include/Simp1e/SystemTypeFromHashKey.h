#pragma once

#include "SystemType.h"
#include "SystemTypeHashKey.h"

namespace Simp1e {
    inline SystemType SystemTypeFromHashKey(const SystemTypeHashKey& hashKey) { return hashKey.c_str(); }
}
