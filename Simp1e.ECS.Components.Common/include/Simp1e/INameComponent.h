#pragma once

#include <Simp1e/DirtyTrackingComponentBase.h>

namespace Simp1e {

    struct INameComponent : public DirtyTrackingComponentBase {
        virtual const char* GetName() const           = 0;
        virtual void        SetName(const char* name) = 0;
    };
}