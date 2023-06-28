#pragma once

#include <Simp1e/IDirtyTrackingComponentBase.h>

namespace Simp1e {

    struct IWindowComponent {
        virtual ~IWindowComponent() = default;

        virtual void        SetTitle(const char* title) = 0;
        virtual const char* GetTitle() const            = 0;
    };
}
