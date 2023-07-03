#pragma once

#include <_Log_.h>
#include <function_pointer.h>

#include "DirtyTrackingComponentBase.h"

namespace Simp1e {

    class FunctionPointerComponentBase : public DirtyTrackingComponentBase {
    protected:
        IFunctionPointer* _functionPointer = nullptr;

    public:
        FunctionPointerComponentBase() = default;
        FunctionPointerComponentBase(IFunctionPointer* functionPointer) : _functionPointer(functionPointer) {
            _Log_(
                "FunctionPointerComponentBase constructor here with ptr {:x}",
                reinterpret_cast<uintptr_t>(_functionPointer)
            );
        }

        enum class Fields {
            FunctionPointer = 1 << 0,
        };

        virtual IFunctionPointer* GetFunctionPointer() const { return _functionPointer; }
        virtual void              SetFunctionPointer(IFunctionPointer* functionPointer) {
            _functionPointer = functionPointer;
            SetDirtyFlag(Fields::FunctionPointer);
        }
    };
}