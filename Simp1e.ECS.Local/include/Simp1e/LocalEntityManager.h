#pragma once

#include <Simp1e/ComponentTypeFromHashKey.h>
#include <Simp1e/ComponentTypeFromType.h>
#include <Simp1e/ComponentTypeHashKey.h>
#include <Simp1e/IEntityManager.h>
#include <_Log_.h>

#include <atomic>
#include <memory>
#include <unordered_map>

#include "LocalEntityEventManager.h"

namespace Simp1e {

    class LocalEntityManager : public IEntityManager {
        std::atomic<Entity>     _nextEntityId = 0;
        LocalEntityEventManager _eventManager;

        std::unordered_map<ComponentTypeHashKey, std::unordered_map<Entity, std::unique_ptr<IVoidPointer>>>
                                                                                            _componentPointers;
        std::unordered_map<Entity, std::unordered_map<ComponentTypeHashKey, IVoidPointer*>> _entityComponentsByEntity;

    public:
        IEntityEventManager* GetEventManager() override { return &_eventManager; }

        ComponentPointer AddComponentPointer(Entity entity, ComponentType componentType, IVoidPointer* pointer)
            override {
            _Log_("[LocalEntityManager] AddComponentPointer {} to {}", componentType, entity);
            _eventManager.ComponentAdding(entity, componentType);
            _componentPointers[componentType][entity]        = std::unique_ptr<IVoidPointer>(pointer);
            _entityComponentsByEntity[entity][componentType] = pointer;
            _eventManager.ComponentAdded(entity, componentType, pointer->void_ptr());
            return pointer->void_ptr();
        }

        Entity CreateEntity() override {
            _Log_("[LocalEntityManager] CreateEntity");
            auto entity                       = _nextEntityId++;
            _entityComponentsByEntity[entity] = {};
            _eventManager.EntityCreated(entity);
            return entity;
        }

        void DestroyEntity(Entity entity) override {
            _Log_("[LocalEntityManager] DestroyEntity");
            _eventManager.EntityDestroying(entity);
            _entityComponentsByEntity.erase(entity);
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

        bool EntityExists(Entity entity) override {
            return _entityComponentsByEntity.find(entity) != _entityComponentsByEntity.end();
        }

        bool RemoveComponent(Entity entity, ComponentType componentType) override {
            auto foundEntity = _entityComponentsByEntity.find(entity);
            if (foundEntity == _entityComponentsByEntity.end()) return false;

            auto foundComponent = _componentPointers.find(componentType);
            if (foundComponent == _componentPointers.end()) return false;

            // Find and remove this entity/component from the Map<Entity, Map<ComponentType, Component>>
            auto foundEntityComponent = foundEntity->second.find(componentType);
            if (foundEntityComponent != foundEntity->second.end()) {
                _eventManager.ComponentRemoving(entity, componentType, foundEntityComponent->second->void_ptr());
                foundEntity->second.erase(foundEntityComponent);
            }

            // Find and remove this entity/component from the Map<ComponentType, Map<Entity, Component>>
            for (auto& [componentEntity, component] : foundComponent->second) {
                if (componentEntity != entity) continue;
                foundComponent->second.erase(entity);
                break;
            }

            _eventManager.ComponentRemoved(entity, componentType);

            return true;
        }

        bool HasComponent(Entity entity, ComponentType componentType) const override {
            auto entityMap = _entityComponentsByEntity.find(entity);
            if (entityMap == _entityComponentsByEntity.end()) return false;

            auto foundComponent = entityMap->second.find(componentType);
            return foundComponent != entityMap->second.end();
        }

        ComponentPointer GetComponentPointer(Entity entity, ComponentType componentType) const override {
            // _Log_("[LocalEntityManager] GetComponentPointer of type {}", componentType);

            auto entityMap = _entityComponentsByEntity.find(entity);
            if (entityMap == _entityComponentsByEntity.end()) return nullptr;

            auto foundComponent = entityMap->second.find(componentType);
            if (foundComponent == entityMap->second.end()) return nullptr;
            return foundComponent->second->void_ptr();
        }

        void ForEachComponent(
            ComponentType                                                    componentType,
            IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>* functionPointer
        ) override {
            auto componentMap = _componentPointers.find(componentType);
            if (componentMap == _componentPointers.end()) return;
            for (auto& [entity, component] : componentMap->second)
                functionPointer->invoke(entity, componentType, component->void_ptr());
        }
    };
}
