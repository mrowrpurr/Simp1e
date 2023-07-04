#pragma once

namespace Simp1e {

    class DirtyTrackingComponentBase {
        int _dirtyFlags = 0;

    public:
        virtual ~DirtyTrackingComponentBase() = default;

        void SetDirtyFlag(int flag) { _dirtyFlags |= flag; }
        void UnsetDirtyFlag(int flag) { _dirtyFlags &= ~flag; }
        bool IsDirtyFlagSet(int flag) const { return (_dirtyFlags & flag) != 0; }
        bool IsDirty() const { return _dirtyFlags != 0; }
        void SetDirty(bool dirty) { _dirtyFlags = dirty ? 1 : 0; }
        void ClearDirty() { _dirtyFlags = 0; }

        template <typename T>
        void SetDirtyFlag(T flag) {
            SetDirtyFlag((int)flag);
        }

        template <typename... Args>
        void SetDirtyFlags(Args... args) {
            int flags[] = {static_cast<int>(args)...};
            for (auto flag : flags) SetDirtyFlag(flag);
        }

        template <typename T>
        void UnsetDirtyFlag(T flag) {
            UnsetDirtyFlag((int)flag);
        }

        template <typename T>
        bool IsDirtyFlagSet(T flag) const {
            return IsDirtyFlagSet((int)flag);
        }
    };
}