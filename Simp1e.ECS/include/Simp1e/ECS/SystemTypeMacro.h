#pragma once

#include "SystemType.h"

#define SIMP1E_ECS_SYSTEM(type) \
    static SystemType GetSystemType() { return type; }
