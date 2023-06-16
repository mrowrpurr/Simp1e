#pragma once

#include <any>
#include <memory>
#include <unordered_map>

#include "ComponentType.h"
#include "Entity.h"

namespace Simp1e::ECS {

    class EntityManager {
        std::unordered_map<ComponentType, std::unordered_map<Entity, std::any>> _components;

    public:
        Entity CreateEntity() {
            static Entity nextEntity = 0;
            return nextEntity++;
        }

        template <typename T>
        void AddComponent(Entity entity, ComponentType componentType, T&& component) {
            _components[componentType][entity] = std::forward<T>(component);
        }

        template <typename T>
        void AddComponent(Entity entity, T&& component) {
            _components[component.GetComponentType()][entity] = std::forward<T>(component);
        }

        template <typename T>
        T GetComponent(Entity entity, ComponentType componentType) {
            return std::any_cast<T>(_components[componentType][entity]);
        }

        std::unordered_map<Entity, std::any>& GetComponents(ComponentType componentType) {
            return _components[componentType];
        }

        bool HasComponent(Entity entity, ComponentType componentType) {
            return _components[componentType].find(entity) != _components[componentType].end();
        }

        void RemoveComponent(Entity entity, ComponentType componentType) {
            _components[componentType].erase(entity);
        }

        void RemoveEntity(Entity entity) {
            for (auto& component : _components) component.second.erase(entity);
        }

        void Clear() { _components.clear(); }
    };
}
