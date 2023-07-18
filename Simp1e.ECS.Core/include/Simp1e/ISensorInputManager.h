#pragma once

#include <Simp1e/Position.h>

namespace Simp1e {

    struct ISensorInputManager {
        virtual ~ISensorInputManager() = default;

        virtual PositionF ReadAccelerometer() const = 0;
    };
}
