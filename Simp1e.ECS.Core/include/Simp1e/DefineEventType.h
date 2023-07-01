#pragma once

#include "EventType.h"

#define DEFINE_EVENT_TYPE(EventName) \
    static EventType GetEventType() { return EventName; }
