#pragma once

#include <Simp1e/Vec3.h>
#include <Simp1e/sreal.h>

namespace Simp1e {

    struct ISensorInputManager {
        virtual ~ISensorInputManager() = default;

        virtual Vec3<sreal> ReadAccelerometer() const = 0;
    };
}
