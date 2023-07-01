#pragma once

#include "ComponentType.h"

#define DEFINE_COMPONENT_TYPE(ComponentName) \
    static ComponentType GetComponentType() { return ComponentName; }
