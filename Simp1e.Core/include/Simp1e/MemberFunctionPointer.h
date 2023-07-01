#pragma once

#include <utility>

#include "IFunctionPointer.h"
#include "ValueWrapper.h"

namespace Simp1e {

    template <typename T, typename ReturnType, typename... Args>
    class MemberFunctionPointer : public IFunctionPointer {
        ReturnType (T::*_func)(Args...);
        T* _instance;

        template <std::size_t... I>
        IValueWrapper* InvokeImpl(std::index_sequence<I...>, IValueWrapper** args) {
            return new ValueWrapper<ReturnType>((_instance->*_func)(
                static_cast<ValueWrapper<typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                    ->GetValue()...
            ));
        }

    public:
        MemberFunctionPointer(ReturnType (T::*func)(Args...), T* instance) : _func(func), _instance(instance) {}

        bool IsMemberFunction() const override { return true; }

        IValueWrapper* Invoke(IValueWrapper** args) override {
            return InvokeImpl(std::index_sequence_for<Args...>{}, args);
        }
    };
}
