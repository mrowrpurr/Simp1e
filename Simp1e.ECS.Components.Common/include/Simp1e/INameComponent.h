#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>

namespace Simp1e {

    struct INameComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("Name")

        enum class Fields : int {
            Name = 1 << 0,
        };

        virtual const char* GetName() const           = 0;
        virtual void        SetName(const char* name) = 0;
    };
}