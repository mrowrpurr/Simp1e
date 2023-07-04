#pragma once

namespace Simp1e {

    template <typename T>
    decltype(auto) JobTypeFromType() {
        return T::GetJobType();
    }
}
