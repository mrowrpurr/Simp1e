#pragma once

namespace Simp1e {

    template <typename T>
    decltype(auto) EventTypeFromType() {
        return T::GetEventType();
    }
}
