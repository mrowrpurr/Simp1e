#pragma once

#include "ComponentPointer.h"

namespace Simp1e {

    template <typename T>
    T* component_cast(ComponentPointer component) {
        return static_cast<T*>(component);
    }
}
