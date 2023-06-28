#pragma once

#include <Simp1e/IDirtyTrackingComponentBase.h>

namespace Simp1e {

    struct ILabelComponent : public IDirtyTrackingComponentBase {
        virtual const char* GetText() const           = 0;
        virtual void        SetText(const char* text) = 0;
    };
}