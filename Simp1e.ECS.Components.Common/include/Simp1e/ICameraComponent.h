#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>

namespace Simp1e {

    struct ICameraComponent : DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("Camera")

        virtual ~ICameraComponent() = default;
    };
}