#pragma once

#include <function_pointer.h>

#include <memory>

#include "EventPointer.h"
#include "EventType.h"
#include "EventTypeFromType.h"

namespace Simp1e {

    struct IEventManager {
        virtual ~IEventManager() = default;

        virtual void RegisterListener(const EventType& eventType, IFunctionPointer<void(EventPointer)>*) = 0;
        virtual void UnregisterListener(IFunctionPointer<void(EventPointer)>*)                           = 0;

        virtual void RegisterGlobalListener(IFunctionPointer<void(EventPointer)>*)   = 0;
        virtual void UnregisterGlobalListener(IFunctionPointer<void(EventPointer)>*) = 0;

        virtual void EmitEventPointer(const EventType& eventType, EventPointer* event) = 0;

        template <typename T>
        void RegisterListener(IFunctionPointer<void(EventPointer)>* callback) {
            RegisterListener(EventTypeFromType<T>(), callback);
        }

        template <typename T>
        void UnregisterListener(IFunctionPointer<void(EventPointer)>* callback) {
            UnregisterListener(EventTypeFromType<T>(), callback);
        }

        template <typename T, typename... Args>
        void EmitEventPointer(T* event) {
            EmitEventPointer(EventTypeFromType<T>(), event);
        }

        template <typename T, typename... Args>
        void Emit(Args&&... args) {
            EmitEventPointer<T>(std::make_unique<T>(std::forward<Args>(args)...).get());
        }
    };
}
