#pragma once

#include <Simp1e/DirtyTrackingComponentBase.h>

namespace Simp1e {

    struct IVisibleComponent : public DirtyTrackingComponentBase {
        virtual void SetVisible(bool visible) = 0;
        virtual bool IsVisible() const        = 0;
    };
}