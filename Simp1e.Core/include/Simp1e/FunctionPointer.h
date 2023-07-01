#pragma once

#include <utility>

#include "IFunctionPointer.h"

namespace Simp1e {

    template <typename ReturnType, typename... Args>
    class FunctionPointer : public IFunctionPointer {
        ReturnType (*_func)(Args...);

    public:
        FunctionPointer(ReturnType (*func)(Args...)) : _func(func) {}

        bool IsMemberFunction() const override { return false; }

        ReturnType Invoke(Args... args) { return _func(std::forward<Args>(args)...); }
        ReturnType operator()(Args... args) { return Invoke(std::forward<Args>(args)...); }
    };
}
