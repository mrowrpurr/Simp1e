#pragma once

#include <memory>

#include "ComponentDeleter.h"

namespace Simp1e::ECS {
    using ComponentPtr = std::unique_ptr<void, ComponentDeleter>;

    template <typename T>
    ComponentPtr MakeComponentPtr(T* ptr) {
        return ComponentPtr(ptr, ComponentDeleter());
    }
}
