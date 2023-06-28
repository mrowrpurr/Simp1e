#pragma once

namespace Simp1e {

    struct IDirtyTrackingComponentBase {
        virtual ~IDirtyTrackingComponentBase() = default;

        virtual void SetDirty(bool dirty) = 0;
        virtual bool IsDirty() const      = 0;
    };
}