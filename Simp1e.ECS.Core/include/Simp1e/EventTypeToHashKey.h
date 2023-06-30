#pragma once

#include "EventType.h"
#include "EventTypeHashKey.h"

namespace Simp1e {
    inline EventTypeHashKey EventTypeToHashKey(const EventType& type) { return type; }
}
