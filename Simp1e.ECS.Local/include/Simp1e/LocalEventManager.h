#pragma once

#include <Simp1e/IEventManager.h>

#include <unordered_map>
#include <unordered_set>

namespace Simp1e {

    class LocalEventManager : public IEventManager {
        std::unordered_map<EventType, std::unordered_set<IFunctionPointer<void(EventPointer)>*>> _listeners;
        std::unordered_set<IFunctionPointer<void(EventPointer)>*>                                _globalListeners;

    public:
        void RegisterListener(const EventType& eventType, IFunctionPointer<void(EventPointer)>* callback) override {
            _listeners[eventType].insert(callback);
        }

        void UnregisterListener(const EventType& eventType, IFunctionPointer<void(EventPointer)>* callback) override {
            _listeners[eventType].erase(callback);
        }

        void RegisterGlobalListener(IFunctionPointer<void(EventPointer)>* callback) override {
            _globalListeners.insert(callback);
        }

        void UnregisterGlobalListener(IFunctionPointer<void(EventPointer)>* callback) override {
            _globalListeners.erase(callback);
        }

        void EmitEventPointer(const EventType& eventType, EventPointer* event) override {
            for (auto& listener : _listeners[eventType]) listener->invoke(event);
            for (auto& listener : _globalListeners) listener->invoke(event);
        }
    };
}
