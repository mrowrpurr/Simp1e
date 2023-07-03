#pragma once

#include <function_pointer.h>

namespace Simp1e {

    class FunctionPointerComponentHelper {
    protected:
        FunctionPointer _uniqueFunctionPointer = nullptr;

    protected:
        FunctionPointerComponentHelper() = default;
        FunctionPointerComponentHelper(FunctionPointer functionPointer)
            : _uniqueFunctionPointer(std::move(functionPointer)) {}

        virtual ~FunctionPointerComponentHelper() = default;

        virtual IFunctionPointer* GetUniqueFunctionPointer() const { return _uniqueFunctionPointer.get(); }
        virtual void              SetUniqueFunctionPointer(FunctionPointer functionPointer) {
            _uniqueFunctionPointer = std::move(functionPointer);
        }
    };
}