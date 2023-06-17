#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "ComponentPointer.h"
#include "ComponentType.h"
#include "Entity.h"
#include "EntityManagerEvents.h"
#include "ReadonlyEntityComponentCollection.h"

namespace Simp1e::ECS {

    class EntityManager {
        std::unordered_map<ComponentType, std::unordered_map<Entity, ComponentPointer>> _components;
        std::unordered_map<Entity, std::unordered_map<ComponentType, ComponentPointer*>> _entities;
        EntityManagerEvents                                                              _events;

    public:
        Entity CreateEntity() {
            static Entity nextEntity = 0;
            return nextEntity++;
        }

        bool HasEntity(Entity entity) { return _entities.find(entity) != _entities.end(); }

        EntityManagerEvents& Events() { return _events; }

        ReadonlyEntityComponentCollection GetEntityComponents(Entity entity) {
            return ReadonlyEntityComponentCollection(_entities[entity]);
        }

        template <typename T>
        void AddComponent(Entity entity, const ComponentType& componentType, T* component) {
            Events().AddingComponent(entity, componentType);
            _components[componentType][entity] = MakeComponentPointer(component);
            _entities[entity][componentType]   = &_components[componentType][entity];
            Events().AddedComponent(entity, componentType);
        }

        template <typename T>
        void AddComponent(Entity entity, const ComponentType& componentType) {
            Events().AddingComponent(entity, componentType);
            _components[componentType][entity] = MakeComponentPointer(new T());
            _entities[entity][componentType]   = &_components[componentType][entity];
            Events().AddedComponent(entity, componentType);
        }

        template <typename T>
        void AddComponent(Entity entity, const ComponentType& componentType, T&& component) {
            Events().AddingComponent(entity, componentType);
            _components[componentType][entity] =
                MakeComponentPointer(new T(std::forward<T>(component)));
            _entities[entity][componentType] = &_components[componentType][entity];
            Events().AddedComponent(entity, componentType);
        }

        template <typename T>
        void AddComponent(Entity entity, T&& component) {
            AddComponent(entity, T::GetComponentType(), std::forward<T>(component));
        }

        template <typename T>
        void AddComponent(Entity entity, T* component) {
            AddComponent(entity, T::GetComponentType(), component);
        }

        template <typename T>
        void AddComponent(Entity entity) {
            AddComponent<T>(entity, T::GetComponentType(), new T());
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
            Events().RemovingComponent(entity, componentType);
            _components[componentType].erase(entity);
            for (auto& entityComponent : _entities[entity]) {
                if (entityComponent.first == componentType) {
                    _entities[entity].erase(entityComponent.first);
                    break;
                }
            }
            Events().RemovedComponent(entity, componentType);
        }

        template <typename T>
        void RemoveComponent(Entity entity) {
            RemoveComponent(entity, T::GetComponentType());
        }

        void RemoveEntity(Entity entity) {
            Events().RemovingEntity(entity);
            for (auto& component : _components) component.second.erase(entity);
            _entities.erase(entity);
            Events().RemovedEntity(entity);
        }

        void Clear() {
            _components.clear();
            _entities.clear();
        }
    };
}
