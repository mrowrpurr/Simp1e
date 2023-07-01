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
        IValueWrapper* InvokeAndReturnImpl(std::index_sequence<I...>, IValueWrapper** args) {
            return new ValueWrapper<ReturnType>((_instance->*_func)(
                static_cast<ValueWrapper<typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                    ->GetValue()...
            ));
        }

        template <std::size_t... I>
        void InvokeImpl(std::index_sequence<I...>, IValueWrapper** args) {
            (_instance->*_func)(
                static_cast<ValueWrapper<typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                    ->GetValue()...
            );
        }

    public:
        MemberFunctionPointer(T* instance, ReturnType (T::*func)(Args...)) : _func(func), _instance(instance) {}

        IValueWrapper* InvokeAndReturn(IValueWrapper** args) override {
            return InvokeAndReturnImpl(std::index_sequence_for<Args...>{}, args);
        }
        void Invoke(IValueWrapper** args) override { InvokeImpl(std::index_sequence_for<Args...>{}, args); }
    };

    template <typename T, typename... Args>
    class MemberFunctionPointer<T, void, Args...> : public IFunctionPointer {
        void (T::*_func)(Args...);
        T* _instance;

        template <std::size_t... I>
        IValueWrapper* InvokeAndReturnImpl(std::index_sequence<I...>, IValueWrapper** args) {
            (_instance->*_func)(
                static_cast<ValueWrapper<typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                    ->GetValue()...
            );
            return nullptr;
        }

        template <std::size_t... I>
        void InvokeImpl(std::index_sequence<I...>, IValueWrapper** args) {
            (_instance->*_func)(
                static_cast<ValueWrapper<typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                    ->GetValue()...
            );
        }

    public:
        MemberFunctionPointer(T* instance, void (T::*func)(Args...)) : _func(func), _instance(instance) {}

        IValueWrapper* InvokeAndReturn(IValueWrapper** args) override {
            return InvokeAndReturnImpl(std::index_sequence_for<Args...>{}, args);
        }
        void Invoke(IValueWrapper** args) override { InvokeImpl(std::index_sequence_for<Args...>{}, args); }
    };
}
