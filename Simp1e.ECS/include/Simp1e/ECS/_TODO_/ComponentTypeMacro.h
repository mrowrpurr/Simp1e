#pragma once

#include "ComponentType.h"

#define SIMP1E_ECS_COMPONENT(type) \
    static ComponentType GetComponentType() { return type; }
