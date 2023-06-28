#pragma once

#include <Simp1e/IDirtyTrackingComponentBase.h>

namespace Simp1e {

    struct IVisibleComponent : public IDirtyTrackingComponentBase {
        virtual void SetVisible(bool visible) = 0;
        virtual bool IsVisible() const        = 0;
    };
}