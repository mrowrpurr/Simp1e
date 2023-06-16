#pragma once

namespace Simp1e::ECS {
    struct ComponentDeleter {
        template <typename T>
        void operator()(T* ptr) const {
            delete ptr;
        }
    };
}
