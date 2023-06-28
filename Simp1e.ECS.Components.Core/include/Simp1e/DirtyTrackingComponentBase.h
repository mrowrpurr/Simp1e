#pragma once

#include "IDirtyTrackingComponentBase.h"

namespace Simp1e {

    class DirtyTrackingComponentBase : public IDirtyTrackingComponentBase {
        bool _dirty = false;

    public:
        void SetDirty(bool dirty) override { _dirty = dirty; }
        bool IsDirty() const override { return _dirty; }
    };
}