#pragma once

#include <memory>

#include "PointerDeleter.h"
#include "VoidPointerBase.h"

namespace Simp1e {

    template <typename T>
    class VoidPointerBaseTyped : public VoidPointerBase {
        std::unique_ptr<void, PointerDeleter<T>> _pointer;

        // Disable copy constructor and copy assignment operator
        VoidPointerBaseTyped(const VoidPointerBaseTyped&)            = delete;
        VoidPointerBaseTyped& operator=(const VoidPointerBaseTyped&) = delete;

    public:
        VoidPointerBaseTyped(T* pointer) : _pointer(pointer) {}
        ~VoidPointerBaseTyped() override = default;
    };
}
