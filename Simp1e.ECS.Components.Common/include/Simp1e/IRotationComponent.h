#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>
#include <Simp1e/sreal.h>

namespace Simp1e {

    struct IRotationComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("Rotation")

        enum class Fields : int {
            Rotation = 1 << 0,
        };

        virtual void  Rotate(sreal delta)         = 0;
        virtual sreal GetRotation() const         = 0;
        virtual void  SetRotation(sreal rotation) = 0;
    };
}