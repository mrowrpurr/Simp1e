#pragma once

#include <utility>

#include "IFunctionPointer.h"
#include "ValueWrapper.h"

namespace Simp1e {

    template <typename ReturnType, typename... Args>
    class FunctionPointer : public IFunctionPointer {
        ReturnType (*_func)(Args...);

        template <std::size_t... I>
        IValueWrapper* InvokeImpl(std::index_sequence<I...>, IValueWrapper** args) {
            return new ValueWrapper<ReturnType>(
                _func(static_cast<ValueWrapper<typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                          ->GetValue()...)
            );
        }

    public:
        FunctionPointer(ReturnType (*func)(Args...)) : _func(func) {}

        bool IsMemberFunction() const override { return false; }

        IValueWrapper* Invoke(IValueWrapper** args) override {
            return InvokeImpl(std::index_sequence_for<Args...>{}, args);
        }
    };
}
