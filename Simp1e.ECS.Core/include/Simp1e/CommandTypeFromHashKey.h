#pragma once

#include "CommandType.h"
#include "CommandTypeHashKey.h"

namespace Simp1e {
    inline CommandType CommandTypeFromHashKey(const CommandTypeHashKey& hashKey) { return hashKey.c_str(); }
}
