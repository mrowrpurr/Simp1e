#pragma once

#include <functional>
#include <vector>

#include "ComponentType.h"
#include "Entity.h"

namespace Simp1e::ECS {

    class EntityManagerEvents {
        std::vector<std::function<void(Entity)>>                _onEntityCreated;
        std::vector<std::function<void(Entity)>>                _onEntityRemoving;
        std::vector<std::function<void(Entity)>>                _onEntityRemoved;
        std::vector<std::function<void(Entity, ComponentType)>> _onComponentAdding;
        std::vector<std::function<void(Entity, ComponentType)>> _onComponentAdded;
        std::vector<std::function<void(Entity, ComponentType)>> _onComponentRemoving;
        std::vector<std::function<void(Entity, ComponentType)>> _onComponentRemoved;
        std::unordered_map<ComponentType, std::vector<std::function<void(Entity)>>>
            _onComponentAddingByType;
        std::unordered_map<ComponentType, std::vector<std::function<void(Entity)>>>
            _onComponentAddedByType;
        std::unordered_map<ComponentType, std::vector<std::function<void(Entity)>>>
            _onComponentRemovingByType;
        std::unordered_map<ComponentType, std::vector<std::function<void(Entity)>>>
            _onComponentRemovedByType;

    public:
        void OnEntityCreated(std::function<void(Entity)> callback) {
            _onEntityCreated.push_back(callback);
        }

        void OnEntityRemoving(std::function<void(Entity)> callback) {
            _onEntityRemoving.push_back(callback);
        }

        void OnEntityRemoved(std::function<void(Entity)> callback) {
            _onEntityRemoved.push_back(callback);
        }

        void OnComponentAdding(std::function<void(Entity, ComponentType)> callback) {
            _onComponentAdding.push_back(callback);
        }

        void OnComponentAdding(
            const ComponentType& componentType, std::function<void(Entity)> callback
        ) {
            _onComponentAddingByType[componentType].push_back(callback);
        }

        template <typename T>
        void OnComponentAdding(std::function<void(Entity)> callback) {
            OnComponentAdding(T::GetComponentType(), callback);
        }

        void OnComponentAdded(std::function<void(Entity, ComponentType)> callback) {
            _onComponentAdded.push_back(callback);
        }

        void OnComponentAdded(
            const ComponentType& componentType, std::function<void(Entity)> callback
        ) {
            _onComponentAddedByType[componentType].push_back(callback);
        }

        template <typename T>
        void OnComponentAdded(std::function<void(Entity)> callback) {
            OnComponentAdded(T::GetComponentType(), callback);
        }

        void OnComponentRemoving(std::function<void(Entity, ComponentType)> callback) {
            _onComponentRemoving.push_back(callback);
        }

        void OnComponentRemoving(
            const ComponentType& componentType, std::function<void(Entity)> callback
        ) {
            _onComponentRemovingByType[componentType].push_back(callback);
        }

        template <typename T>
        void OnComponentRemoving(std::function<void(Entity)> callback) {
            OnComponentRemoving(T::GetComponentType(), callback);
        }

        void OnComponentRemoved(std::function<void(Entity, ComponentType)> callback) {
            _onComponentRemoved.push_back(callback);
        }

        void OnComponentRemoved(
            const ComponentType& componentType, std::function<void(Entity)> callback
        ) {
            _onComponentRemovedByType[componentType].push_back(callback);
        }

        template <typename T>
        void OnComponentRemoved(std::function<void(Entity)> callback) {
            OnComponentRemoved(T::GetComponentType(), callback);
        }

        void CreatedEntity(Entity entity) {
            for (auto& callback : _onEntityCreated) callback(entity);
        }

        void RemovingEntity(Entity entity) {
            for (auto& callback : _onEntityRemoving) callback(entity);
        }

        void RemovedEntity(Entity entity) {
            for (auto& callback : _onEntityRemoved) callback(entity);
        }

        void AddingComponent(Entity entity, ComponentType componentType) {
            for (auto& callback : _onComponentAdding) callback(entity, componentType);
            auto found = _onComponentAddingByType.find(componentType);
            if (found != _onComponentAddingByType.end())
                for (auto& callback : found->second) callback(entity);
        }

        template <typename T>
        void AddingComponent(Entity entity) {
            AddingComponent(entity, T::GetComponentType());
        }

        void AddedComponent(Entity entity, ComponentType componentType) {
            for (auto& callback : _onComponentAdded) callback(entity, componentType);
            auto found = _onComponentAddedByType.find(componentType);
            if (found != _onComponentAddedByType.end())
                for (auto& callback : found->second) callback(entity);
        }

        template <typename T>
        void AddedComponent(Entity entity) {
            AddedComponent(entity, T::GetComponentType());
        }

        void RemovingComponent(Entity entity, ComponentType componentType) {
            for (auto& callback : _onComponentRemoving) callback(entity, componentType);
            auto found = _onComponentRemovingByType.find(componentType);
            if (found != _onComponentRemovingByType.end())
                for (auto& callback : found->second) callback(entity);
        }

        template <typename T>
        void RemovingComponent(Entity entity) {
            RemovingComponent(entity, T::GetComponentType());
        }

        void RemovedComponent(Entity entity, ComponentType componentType) {
            for (auto& callback : _onComponentRemoved) callback(entity, componentType);
            auto found = _onComponentRemovedByType.find(componentType);
            if (found != _onComponentRemovedByType.end())
                for (auto& callback : found->second) callback(entity);
        }

        template <typename T>
        void RemovedComponent(Entity entity) {
            RemovedComponent(entity, T::GetComponentType());
        }
    };

}
