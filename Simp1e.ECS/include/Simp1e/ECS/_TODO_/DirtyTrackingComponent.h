#pragma once

namespace Simp1e::ECS {
    class DirtyTrackingComponent {
        bool _isDirty = true;

    public:
        virtual ~DirtyTrackingComponent() = default;

        virtual bool IsDirty() { return _isDirty; }
        virtual void SetDirty(bool isDirty = true) { _isDirty = isDirty; }
    };
}
