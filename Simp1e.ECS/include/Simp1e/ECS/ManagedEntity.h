#pragma once

#include "EntityManager.h"

namespace Simp1e::ECS {

    class ManagedEntity {
        EntityManager& _entityManager;
        Entity         _entity;

    public:
        ManagedEntity(EntityManager& entityManager, Entity entity)
            : _entityManager(entityManager), _entity(entity) {}

        template <typename T>
        void AddComponent(ComponentType componentType, T&& component) {
            _entityManager.AddComponent(_entity, componentType, std::forward<T>(component));
        }

        template <typename T>
        void AddComponent(T&& component) {
            _entityManager.AddComponent(_entity, std::forward<T>(component));
        }

        template <typename T>
        T GetComponent(ComponentType componentType) {
            return _entityManager.GetComponent<T>(_entity, componentType);
        }

        template <typename T>
        T GetComponent() {
            return _entityManager.GetComponent<T>(_entity, T::GetComponentType());
        }

        template <typename T>
        bool HasComponent(ComponentType componentType) {
            return _entityManager.HasComponent(_entity, componentType);
        }

        template <typename T>
        bool HasComponent() {
            return _entityManager.HasComponent(_entity, T::GetComponentType());
        }

        template <typename T>
        void RemoveComponent(ComponentType componentType) {
            _entityManager.RemoveComponent(_entity, componentType);
        }

        template <typename T>
        void RemoveComponent() {
            _entityManager.RemoveComponent(_entity, T::GetComponentType());
        }

        void RemoveEntity() { _entityManager.RemoveEntity(_entity); }
    };
}
