#pragma once

#include <Simp1e/IDirtyTrackingComponentBase.h>

namespace Simp1e {

    struct IDisplayTextComponent : public IDirtyTrackingComponentBase {
        virtual const char* GetDisplayText() const           = 0;
        virtual void        SetDisplayText(const char* name) = 0;
    };
}