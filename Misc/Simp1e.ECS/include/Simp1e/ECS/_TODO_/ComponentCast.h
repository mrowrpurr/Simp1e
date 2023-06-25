#pragma once

#include "ComponentPointer.h"
#include "ComponentType.h"

namespace Simp1e::ECS {

    template <typename T>
    T* component_cast(ComponentPointer& componentPointer) {
        return static_cast<T*>(componentPointer.get());
    }

    template <typename T>
    T* component_cast(ComponentPointer* componentPointer) {
        return static_cast<T*>(componentPointer->get());
    }
}
