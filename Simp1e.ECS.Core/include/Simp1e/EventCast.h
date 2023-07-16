#pragma once

#include "EventPointer.h"

namespace Simp1e {

    template <typename T>
    T* event_cast(EventPointer event) {
        return static_cast<T*>(event);
    }
}
