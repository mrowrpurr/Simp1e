#pragma once

#include "SystemGroupType.h"
#include "SystemGroupTypeHashKey.h"

namespace Simp1e {
    inline SystemGroupTypeHashKey SystemGroupTypeToHashKey(const SystemGroupType& type) { return type; }
}
