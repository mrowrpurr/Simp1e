#pragma once

#include "SystemPointer.h"

namespace Simp1e {

    template <typename T>
    T* system_cast(SystemPointer system) {
        return static_cast<T*>(system);
    }
}
