#pragma once

#include <Simp1e/VoidPointer.h>

namespace Simp1e::ECS {
    using CommandPointer = VoidPointer;

    template <typename T>
    CommandPointer MakeCommandPointer(T* ptr) {
        return MakeVoidPointer(ptr);
    }
}
