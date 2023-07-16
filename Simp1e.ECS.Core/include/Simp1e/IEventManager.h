#pragma once

#include <function_pointer.h>

#include <memory>

#include "EventPointer.h"
#include "EventResult.h"
#include "EventType.h"
#include "EventTypeFromType.h"

namespace Simp1e {

    struct IEventManager {
        virtual ~IEventManager() = default;

        virtual void
        RegisterListener(const EventType& eventType, IFunctionPointer<EventResult::Value(EventPointer)>*) = 0;
        virtual void
        UnregisterListener(const EventType& eventType, IFunctionPointer<EventResult::Value(EventPointer)>*) = 0;

        virtual void RegisterGlobalListener(IFunctionPointer<EventResult::Value(EventPointer)>*)   = 0;
        virtual void UnregisterGlobalListener(IFunctionPointer<EventResult::Value(EventPointer)>*) = 0;

        virtual void EmitEventPointer(const EventType& eventType, EventPointer event) = 0;

        template <typename T>
        void RegisterListener(IFunctionPointer<EventResult::Value(EventPointer)>* callback) {
            RegisterListener(EventTypeFromType<T>(), callback);
        }

        template <typename T>
        void RegisterListener(FunctionPointer<EventResult::Value(EventPointer)> callback) {
            callback.do_not_destroy_function_pointer();
            RegisterListener(EventTypeFromType<T>(), callback.inner_function_pointer());
        }

        template <typename T>
        void UnregisterListener(IFunctionPointer<EventResult::Value(EventPointer)>* callback) {
            UnregisterListener(EventTypeFromType<T>(), callback);
        }

        template <typename T>
        void EmitEventPointer(T* event) {
            EmitEventPointer(EventTypeFromType<T>(), event);
        }

        template <typename T, typename... Args>
        void Emit(Args&&... args) {
            EmitEventPointer<T>(std::make_unique<T>(std::forward<Args>(args)...).get());
        }
    };
}
