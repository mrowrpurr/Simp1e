#pragma once

#include <memory>
#include <unordered_map>

#include "ComponentPointer.h"
#include "ComponentType.h"
#include "Entity.h"

namespace Simp1e::ECS {

    class EntityManager {
        std::unordered_map<ComponentType, std::unordered_map<Entity, ComponentPointer>> _components;

    public:
        Entity CreateEntity() {
            static Entity nextEntity = 0;
            return nextEntity++;
        }

        template <typename T>
        void AddComponent(Entity entity, const ComponentType& componentType) {
            _components[componentType][entity] = MakeComponentPointer(new T());
        }

        template <typename T>
        void AddComponent(Entity entity, const ComponentType& componentType, T&& component) {
            _components[componentType][entity] =
                MakeComponentPointer(new T(std::forward<T>(component)));
        }

        template <typename T>
        void AddComponent(Entity entity, T&& component) {
            AddComponent(entity, T::GetComponentType(), std::forward<T>(component));
        }

        template <typename T>
        void AddComponent(Entity entity, const ComponentType& componentType, T* component) {
            _components[componentType][entity] = MakeComponentPointer(component);
        }

        template <typename T>
        void AddComponent(Entity entity, T* component) {
            AddComponent(entity, T::GetComponentType(), component);
        }

        template <typename T>
        T* GetComponent(Entity entity, const ComponentType& componentType) {
            return static_cast<T*>(_components[componentType][entity].get());
        }

        std::unordered_map<Entity, ComponentPointer>& GetComponents(
            const ComponentType& componentType
        ) {
            return _components[componentType];
        }

        template <typename T>
        std::unordered_map<Entity, ComponentPointer>& GetComponents() {
            return _components[T::GetComponentType()];
        }

        bool HasComponent(Entity entity, const ComponentType& componentType) {
            return _components[componentType].find(entity) != _components[componentType].end();
        }

        template <typename T>
        bool HasComponent(Entity entity) {
            return HasComponent(entity, T::GetComponentType());
        }

        void RemoveComponent(Entity entity, const ComponentType& componentType) {
            _components[componentType].erase(entity);
        }

        template <typename T>
        void RemoveComponent(Entity entity) {
            RemoveComponent(entity, T::GetComponentType());
        }

        void RemoveEntity(Entity entity) {
            for (auto& component : _components) component.second.erase(entity);
        }

        void Clear() { _components.clear(); }
    };
}
