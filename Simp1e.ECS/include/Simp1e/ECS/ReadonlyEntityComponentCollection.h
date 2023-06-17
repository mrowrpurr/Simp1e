#pragma once

#include <unordered_map>

#include "ComponentPointer.h"
#include "ComponentType.h"

namespace Simp1e::ECS {

    class ReadonlyEntityComponentCollection {
        std::unordered_map<ComponentType, ComponentPointer*>& _components;

    public:
        ReadonlyEntityComponentCollection(
            std::unordered_map<ComponentType, ComponentPointer*>& components
        )
            : _components(components) {}

        template <typename T>
        T* GetComponent(const ComponentType& componentType) {
            auto found = _components.find(componentType);
            if (found != _components.end()) return static_cast<T*>(found->second->get());
            return nullptr;
        }

        template <typename T>
        T* GetComponent() {
            return GetComponent<T>(T::GetComponentType());
        }

        bool HasComponent(const ComponentType& componentType) {
            return _components.find(componentType) != _components.end();
        }

        template <typename T>
        bool HasComponent() {
            return HasComponent(T::GetComponentType());
        }

        size_t size() { return _components.size(); }

        auto begin() { return _components.begin(); }
        auto end() { return _components.end(); }
    };
}
