#pragma once

namespace Simp1e::ECS {
    class ComponentBase {
        bool _isDirty = true;

    public:
        virtual ~ComponentBase() = default;

        virtual bool IsDirty() { return _isDirty; }
        virtual void SetDirty(bool isDirty = true) { _isDirty = isDirty; }
    };
}
