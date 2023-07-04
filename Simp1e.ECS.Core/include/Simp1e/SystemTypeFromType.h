#pragma once

namespace Simp1e {

    template <typename T>
    decltype(auto) SystemTypeFromType() {
        return T::GetSystemType();
    }
}
