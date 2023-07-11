#pragma once

#include <Simp1e/Point.h>

namespace Simp1e {

    struct IInputManager {
        virtual ~IInputManager() = default;

        // virtual Point GetCurrentMousePosition() const = 0;
    };
}
