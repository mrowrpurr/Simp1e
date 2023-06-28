#pragma once

#include <vector>

#include "IEntityEventManager.h"

namespace Simp1e {

    class EntityEventManager : public IEntityEventManager {
        std::vector<void (*)(Entity)>                _entityCreatedCallbacks;
        std::vector<void (*)(Entity)>                _entityDestroyingCallbacks;
        std::vector<void (*)(Entity)>                _entityDestroyedCallbacks;
        std::vector<void (*)(Entity, ComponentType)> _componentAddingCallbacks;
        std::vector<void (*)(Entity, ComponentType)> _componentAddedCallbacks;
        std::vector<void (*)(Entity, ComponentType)> _componentRemovingCallbacks;
        std::vector<void (*)(Entity, ComponentType)> _componentRemovedCallbacks;

    public:
        virtual ~EntityEventManager() = default;

        void OnEntityCreated(void (*callback)(Entity)) override { _entityCreatedCallbacks.push_back(callback); }
        void OnEntityDestroying(void (*callback)(Entity)) override { _entityDestroyingCallbacks.push_back(callback); }
        void OnEntityDestroyed(void (*callback)(Entity)) override { _entityDestroyedCallbacks.push_back(callback); }

        void OnComponentAdding(void (*callback)(Entity, ComponentType)) override {
            _componentAddingCallbacks.push_back(callback);
        }
        void OnComponentAdded(void (*callback)(Entity, ComponentType)) override {
            _componentAddedCallbacks.push_back(callback);
        }
        void OnComponentRemoving(void (*callback)(Entity, ComponentType)) override {
            _componentRemovingCallbacks.push_back(callback);
        }
        void OnComponentRemoved(void (*callback)(Entity, ComponentType)) override {
            _componentRemovedCallbacks.push_back(callback);
        }

        void EntityCreated(Entity entity) override {
            for (auto& callback : _entityCreatedCallbacks) callback(entity);
        }
        void EntityDestroying(Entity entity) override {
            for (auto& callback : _entityDestroyingCallbacks) callback(entity);
        }
        void EntityDestroyed(Entity entity) override {
            for (auto& callback : _entityDestroyedCallbacks) callback(entity);
        }

        void ComponentAdding(Entity entity, ComponentType componentType) override {
            for (auto& callback : _componentAddingCallbacks) callback(entity, componentType);
        }
        void ComponentAdded(Entity entity, ComponentType componentType) override {
            for (auto& callback : _componentAddedCallbacks) callback(entity, componentType);
        }
        void ComponentRemoving(Entity entity, ComponentType componentType) override {
            for (auto& callback : _componentRemovingCallbacks) callback(entity, componentType);
        }
        void ComponentRemoved(Entity entity, ComponentType componentType) override {
            for (auto& callback : _componentRemovedCallbacks) callback(entity, componentType);
        }
    };
}
