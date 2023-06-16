#pragma once

namespace Simp1e {
    struct PointerDeleter {
        template <typename T>
        void operator()(T* ptr) const {
            delete ptr;
        }
    };
}
