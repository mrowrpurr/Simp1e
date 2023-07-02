#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>

namespace Simp1e {

    struct IWindowComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("GUI Window")

        virtual ~IWindowComponent() = default;

        virtual void        SetTitle(const char* title) = 0;
        virtual const char* GetTitle() const            = 0;
    };
}
