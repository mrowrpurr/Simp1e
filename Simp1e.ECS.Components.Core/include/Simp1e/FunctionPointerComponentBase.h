#pragma once

#include <function_pointer.h>

#include "DirtyTrackingComponentBase.h"

namespace Simp1e {

    class FunctionPointerComponentBase : public DirtyTrackingComponentBase {
        IFunctionPointer* _functionPointer = nullptr;

    public:
        FunctionPointerComponentBase() = default;
        FunctionPointerComponentBase(IFunctionPointer* functionPointer) : _functionPointer(functionPointer) {}

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