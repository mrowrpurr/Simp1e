#pragma once

#include "SystemType.h"

#define DEFINE_SYSTEM_TYPE(SystemName) \
    static SystemType GetSystemType() { return SystemName; }
