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
        void* void_pointer() const override { return _pointer.get(); }
        void  set_destruct_on_delete(bool destruct_on_delete) override {
            _pointer.get_deleter().SetDeletesPointer(destruct_on_delete);
        }
        bool does_destruct_on_delete() const override { return _pointer.get_deleter().DeletesPointer(); }

        VoidPointerBaseTyped(T* pointer) : _pointer(pointer) {}
        ~VoidPointerBaseTyped() override = default;
    };
}
