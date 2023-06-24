#pragma once

#include <Simp1e/VoidPointer.h>

namespace Simp1e::ECS {
    using ComponentPointer = VoidPointer;

    template <typename T>
    ComponentPointer MakeComponentPointer(T* ptr) {
        return MakeVoidPointer(ptr);
    }
}
