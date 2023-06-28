#pragma once

namespace Simp1e {
    template <typename T>
    struct PointerDeleter {
        void operator()(T* ptr) const { delete ptr; }
    };
}
