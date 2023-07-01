#pragma once

#include <utility>

#include "FunctionPointerBase.h"

namespace Simp1e {

    template <typename ReturnType, typename... Args>
    class FunctionPointer : public FunctionPointerBase {
        ReturnType (*_func)(Args...);

    public:
        FunctionPointer(ReturnType (*func)(Args...)) : _func(func) {}

        bool IsMemberFunction() const override { return false; }

        ReturnType Invoke(Args... args) { return func(std::forward<Args>(args)...); }
        ReturnType operator()(Args... args) { return Invoke(std::forward<Args>(args)...); }
    };
}
