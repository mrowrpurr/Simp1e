#pragma once

namespace Simp1e {

    template <typename T>
    T* component_cast(void* component) {
        return static_cast<T*>(component);
    }
}
