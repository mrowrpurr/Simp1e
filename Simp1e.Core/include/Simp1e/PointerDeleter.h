#pragma once

namespace Simp1e {
    template <typename T>
    class PointerDeleter {
        bool _deletesPointer = true;

    public:
        void SetDeletesPointer(bool deletesPointer = true) { _deletesPointer = deletesPointer; }
        bool DeletesPointer() const { return _deletesPointer; }
        void operator()(T* ptr) const {
            if (_deletesPointer) delete ptr;
        }
    };
}
