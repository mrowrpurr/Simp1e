#pragma once

#include <Simp1e/VoidPointer.h>

namespace Simp1e::ECS {
    using SystemPointer = VoidPointer;

    template <typename T>
    SystemPointer MakeSystemPointer(T* ptr) {
        return MakeVoidPointer(ptr);
    }
}
