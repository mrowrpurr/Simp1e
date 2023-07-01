#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "IFunctionPointer.h"
#include "ValueWrapper.h"

namespace Simp1e {

    template <typename ReturnType, typename... Args>
    class FunctionPointer : public IFunctionPointer {
        ReturnType (*_func)(Args...);

        template <std::size_t... I>
        IValueWrapper* InvokeAndReturnImpl(std::index_sequence<I...>, IValueWrapper** args) {
            return new ValueWrapper<ReturnType>(
                _func(static_cast<ValueWrapper<typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                          ->GetValue()...)
            );
        }

        template <std::size_t... I>
        void InvokeImpl(std::index_sequence<I...>, IValueWrapper** args) {
            _func(static_cast<ValueWrapper<typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                      ->GetValue()...);
        }

    public:
        FunctionPointer(ReturnType (*func)(Args...)) : _func(func) {}

        IValueWrapper* InvokeAndReturn(IValueWrapper** args) override {
            return InvokeAndReturnImpl(std::index_sequence_for<Args...>{}, args);
        }
        void Invoke(IValueWrapper** args) override { InvokeImpl(std::index_sequence_for<Args...>{}, args); }

        static void InvokeFunc(IFunctionPointer* callback, Args... args) {
            std::vector<std::unique_ptr<IValueWrapper>> wrappers;

            // Populate vector using variadic template expansion
            int dummy[] = {0, ((void)wrappers.push_back(std::make_unique<ValueWrapper<Args>>(args)), 0)...};
            (void)dummy;  // to avoid unused variable warning

            std::vector<IValueWrapper*> rawPointers;
            for (auto& wrapper : wrappers) rawPointers.push_back(wrapper.get());
            callback->Invoke(rawPointers.data());
        }

        static ReturnType InvokeAndReturnFunc(IFunctionPointer* callback, Args... args) {
            std::vector<std::unique_ptr<IValueWrapper>> wrappers;

            // Populate vector using variadic template expansion
            int dummy[] = {0, ((void)wrappers.push_back(std::make_unique<ValueWrapper<Args>>(args)), 0)...};
            (void)dummy;  // to avoid unused variable warning

            std::vector<IValueWrapper*> rawPointers;
            for (auto& wrapper : wrappers) rawPointers.push_back(wrapper.get());
            auto result      = static_cast<ValueWrapper<ReturnType>*>(callback->InvokeAndReturn(rawPointers.data()));
            auto resultValue = result->GetValue();
            delete result;
            return resultValue;
        }
    };

    template <typename... Args>
    class FunctionPointer<void, Args...> : public IFunctionPointer {
        void (*_func)(Args...);

        template <std::size_t... I>
        IValueWrapper* InvokeAndReturnImpl(std::index_sequence<I...>, IValueWrapper** args) {
            _func(static_cast<ValueWrapper<typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                      ->GetValue()...);
            return nullptr;
        }

        template <std::size_t... I>
        void InvokeImpl(std::index_sequence<I...>, IValueWrapper** args) {
            _func(static_cast<ValueWrapper<typename std::tuple_element<I, std::tuple<Args...>>::type>*>(args[I])
                      ->GetValue()...);
        }

    public:
        FunctionPointer(void (*func)(Args...)) : _func(func) {}

        IValueWrapper* InvokeAndReturn(IValueWrapper** args) override {
            return InvokeAndReturnImpl(std::index_sequence_for<Args...>{}, args);
        }
        void Invoke(IValueWrapper** args) override { InvokeImpl(std::index_sequence_for<Args...>{}, args); }

        static void InvokeFunc(IFunctionPointer* callback, Args... args) {
            std::vector<std::unique_ptr<IValueWrapper>> wrappers;

            // Populate vector using variadic template expansion
            int dummy[] = {0, ((void)wrappers.push_back(std::make_unique<ValueWrapper<Args>>(args)), 0)...};
            (void)dummy;  // to avoid unused variable warning

            std::vector<IValueWrapper*> rawPointers;
            for (auto& wrapper : wrappers) rawPointers.push_back(wrapper.get());
            callback->Invoke(rawPointers.data());
        }
    };
}
