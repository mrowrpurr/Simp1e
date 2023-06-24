#pragma once

#include <Simp1e/VoidPointer.h>

namespace Simp1e::ECS {
    using EventPointer = VoidPointer;

    template <typename T>
    EventPointer MakeEventPointer(T* ptr) {
        return MakeVoidPointer(ptr);
    }
}
