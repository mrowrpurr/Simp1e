#pragma once

#include <memory>

#include "PointerDeleter.h"
#include "VoidPointerBase.h"

namespace Simp1e {

    template <typename T>
    class VoidPointerBaseTyped : public VoidPointerBase {
        std::unique_ptr<void, PointerDeleter<T>> _pointer;

    public:
        VoidPointerBaseTyped(T* pointer) : _pointer(pointer) {}
        ~VoidPointerBaseTyped() override = default;
    };
}
