#pragma once

#include "CommandType.h"
#include "CommandTypeHashKey.h"

namespace Simp1e {
    inline CommandTypeHashKey CommandTypeToHashKey(const CommandType& type) { return type; }
}
