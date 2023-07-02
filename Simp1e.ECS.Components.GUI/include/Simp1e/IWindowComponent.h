#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>

namespace Simp1e {

    struct IWindowComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("Window");

        enum class Fields : int {
            Title         = 1 << 0,
            StatusBarText = 1 << 1,
        };

        virtual const char* GetTitle() const         = 0;
        virtual const char* GetStatusBarText() const = 0;

        virtual void SetTitle(const char* title)        = 0;
        virtual void SetStatusBarText(const char* text) = 0;
    };
}
