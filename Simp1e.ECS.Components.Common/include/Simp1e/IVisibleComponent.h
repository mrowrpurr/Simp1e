#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>

namespace Simp1e {

    struct IVisibleComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("Visible")

        virtual void SetVisible(bool visible) = 0;
        virtual bool IsVisible() const        = 0;
    };
}