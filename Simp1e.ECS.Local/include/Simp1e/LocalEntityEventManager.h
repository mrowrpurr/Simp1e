#pragma once

#include <Simp1e/ComponentTypeHashKey.h>
#include <Simp1e/IEntityEventManager.h>
#include <_Log_.h>

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Simp1e {

    class LocalEntityEventManager : public IEntityEventManager {
        std::unordered_map<IFunctionPointerBase*, std::unique_ptr<IFunctionPointerBase>> _callbackInstances;

        std::unordered_set<IFunctionPointer<void(Entity)>*>                       _entityCreatedCallbacks;
        std::unordered_set<IFunctionPointer<void(Entity)>*>                       _entityDestroyingCallbacks;
        std::unordered_set<IFunctionPointer<void(Entity)>*>                       _entityDestroyedCallbacks;
        std::unordered_set<IFunctionPointer<void(Entity, ComponentType)>*>        _componentAddingCallbacks;
        std::unordered_set<IFunctionPointer<void(Entity, ComponentType, void*)>*> _componentAddedCallbacks;
        std::unordered_set<IFunctionPointer<void(Entity, ComponentType, void*)>*> _componentRemovingCallbacks;
        std::unordered_set<IFunctionPointer<void(Entity, ComponentType)>*>        _componentRemovedCallbacks;

        std::unordered_map<ComponentTypeHashKey, std::unordered_set<IFunctionPointer<void(Entity, ComponentType)>*>>
            _componentAddingCallbacksByType;
        std::unordered_map<
            ComponentTypeHashKey, std::unordered_set<IFunctionPointer<void(Entity, ComponentType, void*)>*>>
            _componentAddedCallbacksByType;
        std::unordered_map<
            ComponentTypeHashKey, std::unordered_set<IFunctionPointer<void(Entity, ComponentType, void*)>*>>
            _componentRemovingCallbacksByType;
        std::unordered_map<ComponentTypeHashKey, std::unordered_set<IFunctionPointer<void(Entity, ComponentType)>*>>
            _componentRemovedCallbacksByType;

    public:
        virtual ~LocalEntityEventManager() = default;

        IFunctionPointer<void(Entity)>* RegisterForEntityCreated(IFunctionPointer<void(Entity)>* callback) override {
            _callbackInstances[callback] = std::unique_ptr<IFunctionPointer<void(Entity)>>(callback);
            _entityCreatedCallbacks.insert(callback);
            return callback;
        }
        IFunctionPointer<void(Entity)>* RegisterForEntityDestroying(IFunctionPointer<void(Entity)>* callback) override {
            _callbackInstances[callback] = std::unique_ptr<IFunctionPointer<void(Entity)>>(callback);
            _entityDestroyingCallbacks.insert(callback);
            return callback;
        }
        IFunctionPointer<void(Entity)>* RegisterForEntityDestroyed(IFunctionPointer<void(Entity)>* callback) override {
            _callbackInstances[callback] = std::unique_ptr<IFunctionPointer<void(Entity)>>(callback);
            _entityDestroyedCallbacks.insert(callback);
            return callback;
        }

        IFunctionPointer<void(Entity, ComponentType)>* RegisterForAllComponentAdding(
            IFunctionPointer<void(Entity, ComponentType)>* callback
        ) override {
            _callbackInstances[callback] = std::unique_ptr<IFunctionPointer<void(Entity, ComponentType)>>(callback);
            _componentAddingCallbacks.insert(callback);
            return callback;
        }
        IFunctionPointer<void(Entity, ComponentType, void*)>* RegisterForAllComponentAdded(
            IFunctionPointer<void(Entity, ComponentType, void*)>* callback
        ) override {
            _callbackInstances[callback] =
                std::unique_ptr<IFunctionPointer<void(Entity, ComponentType, void*)>>(callback);
            _componentAddedCallbacks.insert(callback);
            return callback;
        }
        IFunctionPointer<void(Entity, ComponentType, void*)>* RegisterForAllComponentRemoving(
            IFunctionPointer<void(Entity, ComponentType, void*)>* callback
        ) override {
            _callbackInstances[callback] =
                std::unique_ptr<IFunctionPointer<void(Entity, ComponentType, void*)>>(callback);
            _componentRemovingCallbacks.insert(callback);
            return callback;
        }
        IFunctionPointer<void(Entity, ComponentType)>* RegisterForAllComponentRemoved(
            IFunctionPointer<void(Entity, ComponentType)>* callback
        ) override {
            _callbackInstances[callback] = std::unique_ptr<IFunctionPointer<void(Entity, ComponentType)>>(callback);
            _componentRemovedCallbacks.insert(callback);
            return callback;
        }

        IFunctionPointer<void(Entity, ComponentType)>* RegisterForComponentAdding(
            ComponentType type, IFunctionPointer<void(Entity, ComponentType)>* callback
        ) override {
            _callbackInstances[callback] = std::unique_ptr<IFunctionPointer<void(Entity, ComponentType)>>(callback);
            _componentAddingCallbacksByType[type].insert(callback);
            return callback;
        }
        IFunctionPointer<void(Entity, ComponentType, void*)>* RegisterForComponentAdded(
            ComponentType type, IFunctionPointer<void(Entity, ComponentType, void*)>* callback
        ) override {
            _callbackInstances[callback] =
                std::unique_ptr<IFunctionPointer<void(Entity, ComponentType, void*)>>(callback);
            _componentAddedCallbacksByType[type].insert(callback);
            return callback;
        }
        IFunctionPointer<void(Entity, ComponentType, void*)>* RegisterForComponentRemoving(
            ComponentType type, IFunctionPointer<void(Entity, ComponentType, void*)>* callback
        ) override {
            _callbackInstances[callback] =
                std::unique_ptr<IFunctionPointer<void(Entity, ComponentType, void*)>>(callback);
            _componentRemovingCallbacksByType[type].insert(callback);
            return callback;
        }
        IFunctionPointer<void(Entity, ComponentType)>* RegisterForComponentRemoved(
            ComponentType type, IFunctionPointer<void(Entity, ComponentType)>* callback
        ) override {
            _callbackInstances[callback] = std::unique_ptr<IFunctionPointer<void(Entity, ComponentType)>>(callback);
            _componentRemovedCallbacksByType[type].insert(callback);
            return callback;
        }

        void UnregisterForEntityCreated(IFunctionPointer<void(Entity)>* callback) override {
            _callbackInstances.erase(callback);
            _entityCreatedCallbacks.erase(callback);
        }
        void UnregisterForEntityDestroying(IFunctionPointer<void(Entity)>* callback) override {
            _callbackInstances.erase(callback);
            _entityDestroyingCallbacks.erase(callback);
        }
        void UnregisterForEntityDestroyed(IFunctionPointer<void(Entity)>* callback) override {
            _callbackInstances.erase(callback);
            _entityDestroyedCallbacks.erase(callback);
        }

        void UnregisterForAllComponentAdding(IFunctionPointer<void(Entity, ComponentType)>* callback) override {
            _callbackInstances.erase(callback);
            _componentAddingCallbacks.erase(callback);
        }
        void UnregisterForAllComponentAdded(IFunctionPointer<void(Entity, ComponentType, void*)>* callback) override {
            _callbackInstances.erase(callback);
            _componentAddedCallbacks.erase(callback);
        }
        void UnregisterForAllComponentRemoving(IFunctionPointer<void(Entity, ComponentType, void*)>* callback
        ) override {
            _callbackInstances.erase(callback);
            _componentRemovingCallbacks.erase(callback);
        }
        void UnregisterForAllComponentRemoved(IFunctionPointer<void(Entity, ComponentType)>* callback) override {
            _callbackInstances.erase(callback);
            _componentRemovedCallbacks.erase(callback);
        }

        void UnregisterForComponentAdding(ComponentType type, IFunctionPointer<void(Entity, ComponentType)>* callback)
            override {
            _callbackInstances.erase(callback);
            _componentAddingCallbacksByType[type].erase(callback);
        }
        void UnregisterForComponentAdded(
            ComponentType type, IFunctionPointer<void(Entity, ComponentType, void*)>* callback
        ) override {
            _callbackInstances.erase(callback);
            _componentAddedCallbacksByType[type].erase(callback);
        }
        void UnregisterForComponentRemoving(
            ComponentType type, IFunctionPointer<void(Entity, ComponentType, void*)>* callback
        ) override {
            _callbackInstances.erase(callback);
            _componentRemovingCallbacksByType[type].erase(callback);
        }
        void UnregisterForComponentRemoved(ComponentType type, IFunctionPointer<void(Entity, ComponentType)>* callback)
            override {
            _callbackInstances.erase(callback);
            _componentRemovedCallbacksByType[type].erase(callback);
        }

        void EntityCreated(Entity entity) override {
            for (auto& callback : _entityCreatedCallbacks) function_pointer::invoke(callback, entity);
        }
        void EntityDestroying(Entity entity) override {
            for (auto& callback : _entityDestroyingCallbacks) function_pointer::invoke(callback, entity);
        }
        void EntityDestroyed(Entity entity) override {
            for (auto& callback : _entityDestroyedCallbacks) function_pointer::invoke(callback, entity);
        }

        void ComponentAdding(Entity entity, ComponentType type) override {
            for (auto& callback : _componentAddingCallbacks) function_pointer::invoke(callback, entity, type);
            auto foundCallbacksByType = _componentAddingCallbacksByType.find(type);
            if (foundCallbacksByType != _componentAddingCallbacksByType.end())
                for (auto& callback : foundCallbacksByType->second) function_pointer::invoke(callback, entity, type);
        }
        void ComponentAdded(Entity entity, ComponentType type, void* component) override {
            for (auto& callback : _componentAddedCallbacks) function_pointer::invoke(callback, entity, type, component);
            auto foundCallbacksByType = _componentAddedCallbacksByType.find(type);
            if (foundCallbacksByType != _componentAddedCallbacksByType.end())
                for (auto& callback : foundCallbacksByType->second)
                    function_pointer::invoke(callback, entity, type, component);
        }
        void ComponentRemoving(Entity entity, ComponentType type, void* component) override {
            for (auto& callback : _componentRemovingCallbacks)
                function_pointer::invoke(callback, entity, type, component);
            auto foundCallbacksByType = _componentRemovingCallbacksByType.find(type);
            if (foundCallbacksByType != _componentRemovingCallbacksByType.end())
                for (auto& callback : foundCallbacksByType->second)
                    function_pointer::invoke(callback, entity, type, component);
        }
        void ComponentRemoved(Entity entity, ComponentType type) override {
            for (auto& callback : _componentRemovedCallbacks) function_pointer::invoke(callback, entity, type);
            auto foundCallbacksByType = _componentRemovedCallbacksByType.find(type);
            if (foundCallbacksByType != _componentRemovedCallbacksByType.end())
                for (auto& callback : foundCallbacksByType->second) function_pointer::invoke(callback, entity, type);
        }
    };
}
