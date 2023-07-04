#pragma once

namespace Simp1e {

    template <typename T>
    decltype(auto) CommandTypeFromType() {
        return T::GetCommandType();
    }
}
