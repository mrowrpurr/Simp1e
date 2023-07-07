#pragma once

#include <Simp1e/ComponentTypeFromHashKey.h>
#include <Simp1e/ComponentTypeHashKey.h>
#include <Simp1e/IEntityManager.h>
#include <_Log_.h>

#include <atomic>
#include <memory>
#include <unordered_map>

#include "LocalEntityComponentCollection.h"
#include "LocalEntityEventManager.h"

namespace Simp1e {

    // TODO rename things. LocalEntityComponentCollection -->LocalComponentPointerCollection because it's just
    // components
    // TODO and change std::unordered_map<Entity, VoidPointer> over to something actually called a
    // LocalEntityComponentCollection

    class LocalEntityManager : public IEntityManager {
        std::atomic<Entity>     _nextEntityId = 0;
        LocalEntityEventManager _eventManager;

        // TODO: make a comtainer for these. Should accept type for VoidPointer or void* or other
        std::unordered_map<ComponentTypeHashKey, std::unordered_map<Entity, VoidPointer>> _componentPointers;
        std::unordered_map<Entity, std::unique_ptr<LocalEntityComponentCollection>>       _entityComponentsByEntity;

    public:
        bool OwnsEntityMemoryManagement() const override { return true; }

        IEntityEventManager* GetEventManager() override { return &_eventManager; }

        Entity CreateEntity() override {
            _Log_("[LocalEntityManager] CreateEntity");
            auto entity                       = _nextEntityId++;
            _entityComponentsByEntity[entity] = std::make_unique<LocalEntityComponentCollection>();
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

        IEntityComponentCollection* GetEntity(Entity entity) override {
            auto found = _entityComponentsByEntity.find(entity);
            if (found == _entityComponentsByEntity.end()) return nullptr;
            return found->second.get();
        }

        bool RemoveComponent(Entity entity, ComponentType componentType) override {
            auto foundComponent = _componentPointers.find(componentType);
            if (foundComponent == _componentPointers.end()) return false;

            auto foundEntity = _entityComponentsByEntity.find(entity);
            if (foundEntity == _entityComponentsByEntity.end()) return false;

            _eventManager.ComponentRemoving(
                entity, componentType, foundEntity->second->GetComponentPointer(componentType)
            );

            foundComponent->second.erase(entity);
            foundEntity->second->RemoveComponent(componentType);

            _eventManager.ComponentRemoved(entity, componentType);
            return true;
        }

        bool HasComponent(Entity entity, ComponentType componentType) const override {
            auto entityMap = _entityComponentsByEntity.find(entity);
            if (entityMap == _entityComponentsByEntity.end()) return false;
            return entityMap->second->HasComponent(componentType);
        }

        void* GetComponentPointer(Entity entity, ComponentType componentType) const override {
            _Log_("[LocalEntityManager] GetComponentPointer of type {}", componentType);

            auto entityComponentCollection = _entityComponentsByEntity.find(entity);
            if (entityComponentCollection == _entityComponentsByEntity.end()) return nullptr;

            return entityComponentCollection->second->GetComponentPointer(componentType);
        }

        void ForEachComponent(ComponentType componentType, IFunctionPointer* functionPointer) override {
            auto componentMap = _componentPointers.find(componentType);
            if (componentMap == _componentPointers.end()) return;
            for (auto& [entity, component] : componentMap->second)
                function_pointer::invoke(functionPointer, entity, componentType, component->void_ptr());
        }

        VoidPointer* AddComponentPointer(Entity entity, ComponentType componentType, VoidPointer component) override {
            _Log_("[LocalEntityManager] AddComponentPointer of type {} of entity {}", componentType, entity);
            _eventManager.ComponentAdding(entity, componentType);
            auto insertResult = _componentPointers[componentType].insert({entity, std::move(component)});
            if (insertResult.second) {
                _entityComponentsByEntity[entity]->AddComponentPointer(componentType, &insertResult.first->second);
                _eventManager.ComponentAdded(entity, componentType, insertResult.first->second->void_ptr());
                return &insertResult.first->second;
            }
            return nullptr;
        }
    };
}
