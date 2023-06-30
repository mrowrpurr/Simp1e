#pragma once

#include "SystemType.h"
#include "SystemTypeHashKey.h"

namespace Simp1e {
    inline SystemTypeHashKey SystemTypeToHashKey(const SystemType& type) { return type; }
}
