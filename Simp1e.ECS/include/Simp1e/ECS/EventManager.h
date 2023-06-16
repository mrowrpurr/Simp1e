#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include "EventPointer.h"
#include "EventType.h"

namespace Simp1e::ECS {

    class EventManager {
        std::unordered_map<EventType, std::vector<std::function<void(EventPointer)>>>
            _eventHandlers;

    public:
        template <typename T>
        void AddListener(const EventType& eventType, std::function<void(T*)> handler) {
            _eventHandlers[eventType].push_back([handler](EventPointer ptr) {
                handler(static_cast<T*>(ptr));
            });
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
                    [handler](auto& h) {
                        return h.template target<void(T*)>() == handler.template target<void(T*)>();
                    }
                ),
                handlers.end()
            );
        }

        template <typename T>
        void RemoveListener(std::function<void(T*)> handler) {
            RemoveListener(T::GetEventType(), handler);
        }

        template <typename T>
        void SendEvent(T* event) {
            auto& handlers = _eventHandlers[T::GetEventType()];
            for (auto& handler : handlers) {
                handler(MakeEventPointer(event));
            }
        }

        template <typename T>
        void SendEvent(T&& event) {
            SendEvent(&event);
        }

        template <typename T>
        void SendEvent() {
            SendEvent(T());
        }

        void Clear() { _eventHandlers.clear(); }
    };
}
