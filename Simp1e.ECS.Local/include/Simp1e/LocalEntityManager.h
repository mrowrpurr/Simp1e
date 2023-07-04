#pragma once

#include <Simp1e/ComponentTypeFromHashKey.h>
#include <Simp1e/ComponentTypeHashKey.h>
#include <Simp1e/IEntityManager.h>
#include <_Log_.h>

#include <atomic>
#include <unordered_map>

#include "LocalEntityEventManager.h"

namespace Simp1e {

    class LocalEntityManager : public IEntityManager {
        std::atomic<Entity>                                                                _nextEntityId = 0;
        std::unordered_map<ComponentTypeHashKey, std::unordered_map<Entity, VoidPointer>>  _componentPointers;
        std::unordered_map<Entity, std::unordered_map<ComponentTypeHashKey, VoidPointer*>> _entities;
        LocalEntityEventManager                                                            _eventManager;

    public:
        bool OwnsEntityMemoryManagement() const override { return true; }

        IEntityEventManager* GetEventManager() override { return &_eventManager; }

        Entity CreateEntity() override {
            _Log_("[LocalEntityManager] CreateEntity");
            auto entity       = _nextEntityId++;
            _entities[entity] = {};
            _eventManager.EntityCreated(entity);
            return entity;
        }

        void DestroyEntity(Entity entity) override {
            _Log_("[LocalEntityManager] DestroyEntity");
            _eventManager.EntityDestroying(entity);
            _entities.erase(entity);
            for (auto& [componentType, componentMap] : _componentPointers) {
                auto found = componentMap.find(entity);
                if (found == componentMap.end()) continue;
                _eventManager.ComponentRemoving(
                    entity, ComponentTypeFromHashKey(componentType), found->second->void_ptr()
                );
                componentMap.erase(found);
                _eventManager.ComponentRemoved(entity, ComponentTypeFromHashKey(componentType));
            }
            _eventManager.EntityDestroyed(entity);
        }

        bool EntityExists(Entity entity) override { return _entities.find(entity) != _entities.end(); }

        bool RemoveComponent(Entity entity, ComponentType componentType) override {
            auto foundComponent = _entities[entity].find(componentType);
            if (foundComponent == _entities[entity].end()) return false;
            _eventManager.ComponentRemoving(entity, componentType, (*foundComponent->second)->void_ptr());
            _componentPointers[componentType].erase(entity);
            _entities[entity].erase(componentType);
            _eventManager.ComponentRemoved(entity, componentType);
            return true;
        }

        bool HasComponent(Entity entity, ComponentType componentType) const override {
            auto entityMap = _entities.find(entity);
            if (entityMap == _entities.end()) return false;
            return entityMap->second.find(componentType) != entityMap->second.end();
        }

        void* GetComponentPointer(Entity entity, ComponentType componentType) const override {
            _Log_("[LocalEntityManager] GetComponentPointer of type {}", componentType);
            auto entityMap = _entities.find(entity);
            if (entityMap == _entities.end()) return nullptr;
            auto found = entityMap->second.find(componentType);
            if (found == entityMap->second.end()) return nullptr;
            return (*found->second)->void_ptr();
        }

        void ForEachComponent(ComponentType componentType, IFunctionPointer* functionPointer) override {
            auto componentMap = _componentPointers.find(componentType);
            if (componentMap == _componentPointers.end()) return;
            for (auto& [entity, component] : componentMap->second)
                function_pointer::invoke(functionPointer, entity, component->void_ptr());
        }

        VoidPointer* AddComponentPointer(Entity entity, ComponentType componentType, VoidPointer component) override {
            _Log_("[LocalEntityManager] AddComponentPointer of type {} of entity {}", componentType, entity);
            _eventManager.ComponentAdding(entity, componentType);
            auto insertResult = _componentPointers[componentType].insert({entity, std::move(component)});
            if (insertResult.second) {
                _entities[entity][componentType] = &insertResult.first->second;
                _eventManager.ComponentAdded(entity, componentType, insertResult.first->second->void_ptr());
                return &insertResult.first->second;
            }
            return nullptr;
        }
    };
}
