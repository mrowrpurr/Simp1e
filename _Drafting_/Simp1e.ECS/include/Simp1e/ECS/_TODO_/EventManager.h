#pragma once

#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

#include "EventPointer.h"
#include "EventType.h"


namespace Simp1e::ECS {

    class EventManager {
        std::unordered_map<EventType, std::vector<std::function<void(EventPointer&)>>> _eventHandlers;

    public:
        template <typename T>
        void AddListener(const EventType& eventType, std::function<void(T*)> handler) {
            _eventHandlers[eventType].push_back([handler](EventPointer& ptr) { handler(static_cast<T*>(ptr.get())); });
        }

        template <typename T>
        void AddListener(std::function<void(T*)> handler) {
            AddListener(T::GetEventType(), handler);
        }

        template <typename T>
        void RemoveListener(const EventType& eventType, std::function<void(T*)> handler) {
            auto& handlers = _eventHandlers[eventType];
            handlers.erase(
                std::remove_if(
                    handlers.begin(), handlers.end(),
                    [handler](auto& h) { return h.template target<void(T*)>() == handler.template target<void(T*)>(); }
                ),
                handlers.end()
            );
        }

        template <typename T>
        void RemoveListener(std::function<void(T*)> handler) {
            RemoveListener(T::GetEventType(), handler);
        }

        template <typename T>
        void Emit(T&& event) {
            auto found = _eventHandlers.find(T::GetEventType());
            if (found == _eventHandlers.end()) return;
            auto& handlers = found->second;
            auto  ptr      = MakeEventPointer(new T(std::forward<T>(event)));
            for (auto& handler : handlers) handler(ptr);
        }

        template <typename T>
        void Emit() {
            Emit(T());
        }

        void Clear() { _eventHandlers.clear(); }
    };
}
