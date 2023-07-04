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
        std::unordered_map<IFunctionPointer*, std::unique_ptr<IFunctionPointer>>        _callbacks;
        std::unordered_set<IFunctionPointer*>                                           _entityCreatedCallbacks;
        std::unordered_set<IFunctionPointer*>                                           _entityDestroyingCallbacks;
        std::unordered_set<IFunctionPointer*>                                           _entityDestroyedCallbacks;
        std::unordered_set<IFunctionPointer*>                                           _componentAddingCallbacks;
        std::unordered_set<IFunctionPointer*>                                           _componentAddedCallbacks;
        std::unordered_set<IFunctionPointer*>                                           _componentRemovingCallbacks;
        std::unordered_set<IFunctionPointer*>                                           _componentRemovedCallbacks;
        std::unordered_map<ComponentTypeHashKey, std::unordered_set<IFunctionPointer*>> _componentAddingCallbacksByType;
        std::unordered_map<ComponentTypeHashKey, std::unordered_set<IFunctionPointer*>> _componentAddedCallbacksByType;
        std::unordered_map<ComponentTypeHashKey, std::unordered_set<IFunctionPointer*>>
            _componentRemovingCallbacksByType;
        std::unordered_map<ComponentTypeHashKey, std::unordered_set<IFunctionPointer*>>
            _componentRemovedCallbacksByType;

    public:
        virtual ~LocalEntityEventManager() = default;

        IFunctionPointer* RegisterForEntityCreated(IFunctionPointer* callback) override {
            _callbacks[callback] = std::unique_ptr<IFunctionPointer>(callback);
            _entityCreatedCallbacks.insert(callback);
            return callback;
        }
        IFunctionPointer* RegisterForEntityDestroying(IFunctionPointer* callback) override {
            _callbacks[callback] = std::unique_ptr<IFunctionPointer>(callback);
            _entityDestroyingCallbacks.insert(callback);
            return callback;
        }
        IFunctionPointer* RegisterForEntityDestroyed(IFunctionPointer* callback) override {
            _callbacks[callback] = std::unique_ptr<IFunctionPointer>(callback);
            _entityDestroyedCallbacks.insert(callback);
            return callback;
        }

        IFunctionPointer* RegisterForAllComponentAdding(IFunctionPointer* callback) override {
            _callbacks[callback] = std::unique_ptr<IFunctionPointer>(callback);
            _componentAddingCallbacks.insert(callback);
            return callback;
        }
        IFunctionPointer* RegisterForAllComponentAdded(IFunctionPointer* callback) override {
            _callbacks[callback] = std::unique_ptr<IFunctionPointer>(callback);
            _componentAddedCallbacks.insert(callback);
            return callback;
        }
        IFunctionPointer* RegisterForAllComponentRemoving(IFunctionPointer* callback) override {
            _callbacks[callback] = std::unique_ptr<IFunctionPointer>(callback);
            _componentRemovingCallbacks.insert(callback);
            return callback;
        }
        IFunctionPointer* RegisterForAllComponentRemoved(IFunctionPointer* callback) override {
            _callbacks[callback] = std::unique_ptr<IFunctionPointer>(callback);
            _componentRemovedCallbacks.insert(callback);
            return callback;
        }

        IFunctionPointer* RegisterForComponentAdding(ComponentType type, IFunctionPointer* callback) override {
            _callbacks[callback] = std::unique_ptr<IFunctionPointer>(callback);
            _componentAddingCallbacksByType[type].insert(callback);
            return callback;
        }
        IFunctionPointer* RegisterForComponentAdded(ComponentType type, IFunctionPointer* callback) override {
            _callbacks[callback] = std::unique_ptr<IFunctionPointer>(callback);
            _componentAddedCallbacksByType[type].insert(callback);
            return callback;
        }
        IFunctionPointer* RegisterForComponentRemoving(ComponentType type, IFunctionPointer* callback) override {
            _callbacks[callback] = std::unique_ptr<IFunctionPointer>(callback);
            _componentRemovingCallbacksByType[type].insert(callback);
            return callback;
        }
        IFunctionPointer* RegisterForComponentRemoved(ComponentType type, IFunctionPointer* callback) override {
            _callbacks[callback] = std::unique_ptr<IFunctionPointer>(callback);
            _componentRemovedCallbacksByType[type].insert(callback);
            return callback;
        }

        void UnregisterForEntityCreated(IFunctionPointer* callback) override {
            _callbacks.erase(callback);
            _entityCreatedCallbacks.erase(callback);
        }
        void UnregisterForEntityDestroying(IFunctionPointer* callback) override {
            _callbacks.erase(callback);
            _entityDestroyingCallbacks.erase(callback);
        }
        void UnregisterForEntityDestroyed(IFunctionPointer* callback) override {
            _callbacks.erase(callback);
            _entityDestroyedCallbacks.erase(callback);
        }

        void UnregisterForAllComponentAdding(IFunctionPointer* callback) override {
            _callbacks.erase(callback);
            _componentAddingCallbacks.erase(callback);
        }
        void UnregisterForAllComponentAdded(IFunctionPointer* callback) override {
            _callbacks.erase(callback);
            _componentAddedCallbacks.erase(callback);
        }
        void UnregisterForAllComponentRemoving(IFunctionPointer* callback) override {
            _callbacks.erase(callback);
            _componentRemovingCallbacks.erase(callback);
        }
        void UnregisterForAllComponentRemoved(IFunctionPointer* callback) override {
            _callbacks.erase(callback);
            _componentRemovedCallbacks.erase(callback);
        }

        void UnregisterForComponentAdding(ComponentType type, IFunctionPointer* callback) override {
            _callbacks.erase(callback);
            _componentAddingCallbacksByType[type].erase(callback);
        }
        void UnregisterForComponentAdded(ComponentType type, IFunctionPointer* callback) override {
            _callbacks.erase(callback);
            _componentAddedCallbacksByType[type].erase(callback);
        }
        void UnregisterForComponentRemoving(ComponentType type, IFunctionPointer* callback) override {
            _callbacks.erase(callback);
            _componentRemovingCallbacksByType[type].erase(callback);
        }
        void UnregisterForComponentRemoved(ComponentType type, IFunctionPointer* callback) override {
            _callbacks.erase(callback);
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
