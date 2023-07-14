#pragma once

#include <Simp1e/DefineComponentType.h>

namespace Simp1e {

    struct ICameraComponent {
        DEFINE_COMPONENT_TYPE("Camera")

        virtual ~ICameraComponent() = default;
    };
}