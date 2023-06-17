#pragma once

namespace Simp1e::ECS {
    class ComponentBase {
        bool _isDirty =
            true;  // Start out dirty so that the first update will be sent to the renderer.

    public:
        bool IsDirty() { return _isDirty; }
        void SetDirty(bool isDirty = true) { _isDirty = isDirty; }
    };
}
