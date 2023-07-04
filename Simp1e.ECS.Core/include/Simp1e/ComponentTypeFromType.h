#pragma once

namespace Simp1e {

    template <typename T>
    decltype(auto) ComponentTypeFromType() {
        return T::GetComponentType();
    }
}
