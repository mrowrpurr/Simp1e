#pragma once

#include <memory>

#include "VoidPointerBaseTyped.h"

namespace Simp1e {

    using VoidPointer = std::unique_ptr<void, VoidPointerBase>;

    template <typename T>
    VoidPointer MakeVoidPointer(T* ptr) {
        return VoidPointer(new VoidPointerBaseTyped<T>(ptr));
    }
}
