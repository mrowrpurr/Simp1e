#pragma once

#include <Simp1e/IDirtyTrackingComponentBase.h>

namespace Simp1e {

    struct INameComponent : public IDirtyTrackingComponentBase {
        virtual const char* GetName() const           = 0;
        virtual void        SetName(const char* name) = 0;
    };
}