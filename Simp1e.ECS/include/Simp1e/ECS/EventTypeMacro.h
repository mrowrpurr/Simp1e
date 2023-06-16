#pragma once

#include "EventType.h"

#define SIMP1E_ECS_EVENT(type) \
    static EventType GetEventType() { return type; }
