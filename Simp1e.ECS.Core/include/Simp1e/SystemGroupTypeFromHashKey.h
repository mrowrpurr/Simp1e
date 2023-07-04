#pragma once

#include "SystemGroupType.h"
#include "SystemGroupTypeHashKey.h"

namespace Simp1e {
    inline SystemGroupType SystemGroupTypeFromHashKey(const SystemGroupTypeHashKey& hashKey) { return hashKey.c_str(); }
}
