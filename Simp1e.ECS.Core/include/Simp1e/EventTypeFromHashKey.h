#pragma once

#include "EventType.h"
#include "EventTypeHashKey.h"

namespace Simp1e {
    inline EventType EventTypeFromHashKey(const EventTypeHashKey& hashKey) { return hashKey.c_str(); }
}
