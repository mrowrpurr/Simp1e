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
        std::unordered_map<ComponentType, std::unordered_map<Entity, ComponentPointer>>  _components;
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
        T* AddComponent(Entity entity, const ComponentType& componentType, T* component) {
            if (HasComponent<T>(entity)) {
                qDebug() << "Entity " << entity << " already has component " << componentType;
                return GetComponent<T>(entity, componentType);
            }

            Events().AddingComponent(entity, componentType);
            qDebug() << "Adding component " << componentType << " to entity " << entity;
            _components[componentType][entity] = MakeComponentPointer(component);
            _entities[entity][componentType]   = &_components[componentType][entity];
            Events().AddedComponent(entity, componentType);
            return component;
        }

        template <typename T>
        T* AddComponent(Entity entity, const ComponentType& componentType) {
            return AddComponent<T>(entity, componentType, new T());
        }

        template <typename T>
        T* AddComponent(Entity entity, const ComponentType& componentType, T&& componentValue) {
            return AddComponent<T>(entity, componentType, new T(std::forward<T>(componentValue)));
        }

        template <typename T>
        T* AddComponent(Entity entity, T&& component) {
            return AddComponent(entity, T::GetComponentType(), std::forward<T>(component));
        }

        template <typename T>
        T* AddComponent(Entity entity, T* component) {
            return AddComponent(entity, T::GetComponentType(), component);
        }

        template <typename T>
        T* AddComponent(Entity entity) {
            return AddComponent<T>(entity, T::GetComponentType(), new T());
        }

        // add a variadic one...

        template <typename T>
        T* GetComponent(Entity entity, const ComponentType& componentType) {
            auto foundEntity = _entities.find(entity);
            if (foundEntity == _entities.end()) return nullptr;

            auto foundComponent = foundEntity->second.find(componentType);
            if (foundComponent == foundEntity->second.end()) return nullptr;

            return static_cast<T*>(foundComponent->second->get());
        }

        template <typename T>
        T* GetComponent(Entity entity) {
            return GetComponent<T>(entity, T::GetComponentType());
        }

        std::unordered_map<Entity, ComponentPointer>& GetComponents(const ComponentType& componentType) {
            if (_components.find(componentType) == _components.end())
                _components[componentType] = std::unordered_map<Entity, ComponentPointer>();
            return _components[componentType];
        }

        template <typename T>
        std::unordered_map<Entity, ComponentPointer>& GetComponents() {
            return GetComponents(T::GetComponentType());
        }

        bool HasComponent(Entity entity, const ComponentType& componentType) {
            auto foundEntity = _entities.find(entity);
            if (foundEntity == _entities.end()) return false;

            auto foundComponent = foundEntity->second.find(componentType);
            return foundComponent != foundEntity->second.end();
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
