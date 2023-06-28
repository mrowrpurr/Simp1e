#pragma once

#include <Simp1e/ComponentTypeFromHashKey.h>
#include <Simp1e/ComponentTypeHashKey.h>
#include <Simp1e/EntityEventManager.h>
#include <Simp1e/IEntityManager.h>

#include <atomic>
#include <unordered_map>

namespace Simp1e {

    class EntityPointerManager : public IEntityManager {
        std::atomic<Entity>                                                         _nextEntity = 0;
        std::unordered_map<ComponentTypeHashKey, std::unordered_map<Entity, void*>> _componentPointers;
        std::unordered_map<Entity, std::unordered_map<ComponentTypeHashKey, void*>> _entities;
        EntityEventManager                                                          _eventManager;

    public:
        IEntityEventManager* GetEventManager() override { return &_eventManager; }

        Entity CreateEntity() override {
            auto entity       = _nextEntity++;
            _entities[entity] = {};
            _eventManager.EntityCreated(entity);
            return entity;
        }
        void DestroyEntity(Entity entity) override {
            _eventManager.EntityDestroying(entity);
            _entities.erase(entity);
            for (auto& [componentType, componentMap] : _componentPointers) {
                auto found = componentMap.find(entity);
                if (found == componentMap.end()) continue;
                _eventManager.ComponentRemoving(entity, ComponentTypeFromHashKey(componentType));
                componentMap.erase(found);
                _eventManager.ComponentRemoved(entity, ComponentTypeFromHashKey(componentType));
            }
            _eventManager.EntityDestroyed(entity);
        }
        bool EntityExists(Entity entity) override { return _entities.find(entity) != _entities.end(); }

        void* AddComponentPointer(Entity entity, ComponentType componentType, void* component) override {
            _eventManager.ComponentAdding(entity, componentType);
            _componentPointers[componentType][entity] = component;
            _entities[entity][componentType]          = component;
            _eventManager.ComponentAdded(entity, componentType);
            return component;
        }
        void RemoveComponent(Entity entity, ComponentType componentType) override {
            _eventManager.ComponentRemoving(entity, componentType);
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
            auto entityMap = _entities.find(entity);
            if (entityMap == _entities.end()) return nullptr;
            auto found = entityMap->second.find(componentType);
            if (found == entityMap->second.end()) return nullptr;
            return found->second;
        }
        void ForEachComponent(ComponentType componentType, void (*callback)(Entity, void*)) override {
            auto componentMap = _componentPointers.find(componentType);
            if (componentMap == _componentPointers.end()) return;
            for (auto& [entity, component] : componentMap->second) callback(entity, component);
        }
    };
}
