#pragma once

#include <utility>

#include "IFunctionPointer.h"

namespace Simp1e {

    template <typename T, typename ReturnType, typename... Args>
    class MemberFunctionPointer : public IFunctionPointer {
        ReturnType (T::*_func)(Args...);
        T* _instance;

    public:
        MemberFunctionPointer(ReturnType (T::*func)(Args...), T* instance) : _func(func), _instance(instance) {}

        bool IsMemberFunction() const override { return true; }

        ReturnType Invoke(Args... args) { return (_instance->*_func)(std::forward<Args>(args)...); }
        ReturnType operator()(Args... args) { return Invoke(std::forward<Args>(args)...); }
    };
}
