#pragma once

#include <memory>

#include "PointerDeleter.h"

namespace Simp1e {
    using VoidPointer = std::unique_ptr<void, PointerDeleter>;

    template <typename T>
    VoidPointer MakeVoidPointer(T* ptr) {
        return VoidPointer(ptr, PointerDeleter());
    }
}
