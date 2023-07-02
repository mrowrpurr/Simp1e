#pragma once

namespace Simp1e {

    class DirtyTrackingComponentBase {
        int _dirtyFlags = 0;

    public:
        void AddDirtyFlag(int flag) { _dirtyFlags |= flag; }
        void RemoveDirtyFlag(int flag) { _dirtyFlags &= ~flag; }
        bool IsDirtyFlagSet(int flag) const { return (_dirtyFlags & flag) != 0; }
        bool IsDirty() const { return _dirtyFlags != 0; }
        void SetDirty(bool dirty) { _dirtyFlags = dirty ? 1 : 0; }
        void ClearDirty() { _dirtyFlags = 0; }

        template <typename T>
        void AddDirtyFlag(T flag) {
            AddDirtyFlag((int)flag);
        }

        template <typename T>
        void RemoveDirtyFlag(T flag) {
            RemoveDirtyFlag((int)flag);
        }

        template <typename T>
        bool IsDirtyFlagSet(T flag) const {
            return IsDirtyFlagSet((int)flag);
        }
    };
}