#pragma once

#include <function_pointer.h>

namespace Simp1e {

    class FunctionPointerComponentHelper {
        FunctionPointer _functionPointer = nullptr;

    protected:
        FunctionPointerComponentHelper() = default;
        FunctionPointerComponentHelper(FunctionPointer functionPointer)
            : _functionPointer(std::move(functionPointer)) {}

        virtual ~FunctionPointerComponentHelper() = default;

        virtual IFunctionPointer* GetUniqueFunctionPointer() const { return _functionPointer.get(); }
        virtual void              SetUniqueFunctionPointer(FunctionPointer functionPointer) {
            _functionPointer = std::move(functionPointer);
        }
    };
}