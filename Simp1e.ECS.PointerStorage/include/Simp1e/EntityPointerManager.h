#pragma once

#include <Simp1e/ComponentTypeFromHashKey.h>
#include <Simp1e/ComponentTypeHashKey.h>
#include <Simp1e/ComponentTypeToHashKey.h>
#include <Simp1e/EntityEventManager.h>
#include <_Log_.h>

#include <atomic>
#include <unordered_map>

#include "IEntityPointerManager.h"

namespace Simp1e {

    class EntityPointerManager : public IEntityPointerManager {
        std::atomic<Entity>                                                         _nextEntity = 0;
        std::unordered_map<ComponentTypeHashKey, std::unordered_map<Entity, void*>> _componentPointers;
        std::unordered_map<Entity, std::unordered_map<ComponentTypeHashKey, void*>> _entities;
        EntityEventManager                                                          _eventManager;

    public:
        IEntityEventManager* GetEventManager() override { return &_eventManager; }

        Entity CreateEntity() override {
            _Log_("[EntityPointerManager] CreateEntity");
            auto entity       = _nextEntity++;
            _entities[entity] = {};
            _eventManager.EntityCreated(entity);
            return entity;
        }

        void DestroyEntity(Entity entity) override {
            _Log_("[EntityPointerManager] DestroyEntity");
            _eventManager.EntityDestroying(entity);
            _entities.erase(entity);
            for (auto& [componentType, componentMap] : _componentPointers) {
                auto found = componentMap.find(entity);
                if (found == componentMap.end()) continue;
                _eventManager.ComponentRemoving(entity, ComponentTypeFromHashKey(componentType), found->second);
                componentMap.erase(found);
                _eventManager.ComponentRemoved(entity, ComponentTypeFromHashKey(componentType));
            }
            _eventManager.EntityDestroyed(entity);
        }

        bool EntityExists(Entity entity) override { return _entities.find(entity) != _entities.end(); }

        void RemoveComponent(Entity entity, ComponentType componentType) override {
            auto foundComponent = _entities[entity].find(componentType);
            if (foundComponent == _entities[entity].end()) return;
            _eventManager.ComponentRemoving(entity, componentType, foundComponent->second);
            _componentPointers[componentType].erase(entity);
            _entities[entity].erase(componentType);
            _eventManager.ComponentRemoved(entity, componentType);
        }

        bool HasComponent(Entity entity, ComponentType componentType) const override {
            auto entityMap = _entities.find(entity);
            if (entityMap == _entities.end()) return false;
            return entityMap->second.find(componentType) != entityMap->second.end();
        }

        void* GetComponentPointer(Entity entity, ComponentType componentType) const override {
            _Log_("GetComponentPointer of type {}", componentType);
            auto entityMap = _entities.find(entity);
            if (entityMap == _entities.end()) return nullptr;
            auto found = entityMap->second.find(componentType);
            if (found == entityMap->second.end()) return nullptr;
            return found->second;
        }

        void ForEachComponentFunction(ComponentType componentType, IFunctionPointer* functionPointer) override {
            _Log_("[EntityPointerManager] ForEachComponent of type {}", componentType);
            auto componentMap = _componentPointers.find(componentType);
            if (componentMap == _componentPointers.end()) return;
            for (auto& [entity, component] : componentMap->second)
                function_pointer::invoke(functionPointer, entity, component);
        }

        void AddComponentPointer(Entity entity, ComponentType componentType, void* component) override {
            _Log_("[EntityPointerManager] AddComponentPointer of type {}", componentType);
            _eventManager.ComponentAdding(entity, componentType);
            _componentPointers[componentType][entity] = component;
            _entities[entity][componentType]          = component;
            _eventManager.ComponentAdded(entity, componentType, component);
        }
    };
}
