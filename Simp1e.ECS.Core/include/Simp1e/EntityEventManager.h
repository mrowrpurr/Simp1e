#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "IEntityEventManager.h"

namespace Simp1e {

    class EntityEventManager : public IEntityEventManager {
        std::unordered_map<FunctionPointerBase*, std::unique_ptr<FunctionPointerBase>> _callbacks;
        std::unordered_set<FunctionPointerBase*>                                       _entityCreatedCallbacks;
        std::unordered_set<FunctionPointerBase*>                                       _entityDestroyingCallbacks;
        std::unordered_set<FunctionPointerBase*>                                       _entityDestroyedCallbacks;
        std::unordered_set<FunctionPointerBase*>                                       _componentAddingCallbacks;
        std::unordered_set<FunctionPointerBase*>                                       _componentAddedCallbacks;
        std::unordered_set<FunctionPointerBase*>                                       _componentRemovingCallbacks;
        std::unordered_set<FunctionPointerBase*>                                       _componentRemovedCallbacks;
        std::unordered_map<ComponentType, std::unordered_set<FunctionPointerBase*>>    _componentAddingCallbacksByType;
        std::unordered_map<ComponentType, std::unordered_set<FunctionPointerBase*>>    _componentAddedCallbacksByType;
        std::unordered_map<ComponentType, std::unordered_set<FunctionPointerBase*>> _componentRemovingCallbacksByType;
        std::unordered_map<ComponentType, std::unordered_set<FunctionPointerBase*>> _componentRemovedCallbacksByType;

    public:
        virtual ~EntityEventManager() = default;

        void RegisterForEntityCreated(FunctionPointerBase* callback) override {
            _callbacks[callback] = std::unique_ptr<FunctionPointerBase>(callback);
            _entityCreatedCallbacks.insert(callback);
        }
        void RegisterForEntityDestroying(FunctionPointerBase* callback) override {
            _callbacks[callback] = std::unique_ptr<FunctionPointerBase>(callback);
            _entityDestroyingCallbacks.insert(callback);
        }
        void RegisterForEntityDestroyed(FunctionPointerBase* callback) override {
            _callbacks[callback] = std::unique_ptr<FunctionPointerBase>(callback);
            _entityDestroyedCallbacks.insert(callback);
        }

        void RegisterForComponentAdding(FunctionPointerBase* callback) override {
            _callbacks[callback] = std::unique_ptr<FunctionPointerBase>(callback);
            _componentAddingCallbacks.insert(callback);
        }
        void RegisterForComponentAdded(FunctionPointerBase* callback) override {
            _callbacks[callback] = std::unique_ptr<FunctionPointerBase>(callback);
            _componentAddedCallbacks.insert(callback);
        }
        void RegisterForComponentRemoving(FunctionPointerBase* callback) override {
            _callbacks[callback] = std::unique_ptr<FunctionPointerBase>(callback);
            _componentRemovingCallbacks.insert(callback);
        }
        void RegisterForComponentRemoved(FunctionPointerBase* callback) override {
            _callbacks[callback] = std::unique_ptr<FunctionPointerBase>(callback);
            _componentRemovedCallbacks.insert(callback);
        }

        void RegisterForComponentAdding(FunctionPointerBase* callback, ComponentType type) override {
            _callbacks[callback] = std::unique_ptr<FunctionPointerBase>(callback);
            _componentAddingCallbacksByType[type].insert(callback);
        }
        void RegisterForComponentAdded(FunctionPointerBase* callback, ComponentType type) override {
            _callbacks[callback] = std::unique_ptr<FunctionPointerBase>(callback);
            _componentAddedCallbacksByType[type].insert(callback);
        }
        void RegisterForComponentRemoving(FunctionPointerBase* callback, ComponentType type) override {
            _callbacks[callback] = std::unique_ptr<FunctionPointerBase>(callback);
            _componentRemovingCallbacksByType[type].insert(callback);
        }
        void RegisterForComponentRemoved(FunctionPointerBase* callback, ComponentType type) override {
            _callbacks[callback] = std::unique_ptr<FunctionPointerBase>(callback);
            _componentRemovedCallbacksByType[type].insert(callback);
        }

        void UnregisterForEntityCreated(FunctionPointerBase* callback) override {
            _callbacks.erase(callback);
            _entityCreatedCallbacks.erase(callback);
        }
        void UnregisterForEntityDestroying(FunctionPointerBase* callback) override {
            _callbacks.erase(callback);
            _entityDestroyingCallbacks.erase(callback);
        }
        void UnregisterForEntityDestroyed(FunctionPointerBase* callback) override {
            _callbacks.erase(callback);
            _entityDestroyedCallbacks.erase(callback);
        }

        void UnregisterForComponentAdding(FunctionPointerBase* callback) override {
            _callbacks.erase(callback);
            _componentAddingCallbacks.erase(callback);
        }
        void UnregisterForComponentAdded(FunctionPointerBase* callback) override {
            _callbacks.erase(callback);
            _componentAddedCallbacks.erase(callback);
        }
        void UnregisterForComponentRemoving(FunctionPointerBase* callback) override {
            _callbacks.erase(callback);
            _componentRemovingCallbacks.erase(callback);
        }
        void UnregisterForComponentRemoved(FunctionPointerBase* callback) override {
            _callbacks.erase(callback);
            _componentRemovedCallbacks.erase(callback);
        }

        void UnregisterForComponentAdding(FunctionPointerBase* callback, ComponentType type) override {
            _callbacks.erase(callback);
            _componentAddingCallbacksByType[type].erase(callback);
        }
        void UnregisterForComponentAdded(FunctionPointerBase* callback, ComponentType type) override {
            _callbacks.erase(callback);
            _componentAddedCallbacksByType[type].erase(callback);
        }
        void UnregisterForComponentRemoving(FunctionPointerBase* callback, ComponentType type) override {
            _callbacks.erase(callback);
            _componentRemovingCallbacksByType[type].erase(callback);
        }
        void UnregisterForComponentRemoved(FunctionPointerBase* callback, ComponentType type) override {
            _callbacks.erase(callback);
            _componentRemovedCallbacksByType[type].erase(callback);
        }

        void EntityCreated(Entity entity) override {
            for (auto& baseCallbackPointer : _entityCreatedCallbacks) {
                if (baseCallbackPointer->IsMemberFunction()) {
                    if (auto* callbackPointer = dynamic_cast<FunctionPointer<void, Entity>*>(baseCallbackPointer))
                        callbackPointer->Invoke(entity);
                } else {
                    // // uh oh....
                    // if (auto* callbackPointer = dynamic_cast<MemberFunctionPointer<void>*>(baseCallbackPointer))
                    //     callbackPointer->Invoke();
                }
            }
        }

        // void OnEntityCreated(Entity entity) override {
        //     for (auto callback : _entityCreatedCallbacks) {
        //         callback->Invoke(entity);
        //     }
        // }
        // void OnEntityDestroying(Entity entity) override {
        //     for (auto callback : _entityDestroyingInvokebacks) {
        //         callback->Invoke(entity);
        //     }
        // }
        // void OnEntityDestroyed(Entity entity) override {
        //     for (auto callback : _entityDestroyedInvokebacks) {
        //         callback->Invoke(entity);
        //     }
        // }

        // void OnComponentAdding(Entity entity, ComponentType type) override {
        //     for (auto callback : _componentAddingInvokebacks) {
        //         callback->Invoke(entity, type);
        //     }
        //     for (auto callback : _componentAddingInvokebacksByType[type]) {
        //         callback->Invoke(entity);
        //     }
        // }
        // void OnComponentAdded(Entity entity, ComponentType type) override {
        //     for (auto callback : _componentAddedInvokebacks) {
        //         callback->Invoke(entity, type);
        //     }
        //     for (auto callback : _componentAddedInvokebacksByType[type]) {
        //         callback->Invoke(entity);
        //     }
        // }
        // void OnComponentRemoving(Entity entity, ComponentType type) override {
        //     for (auto callback : _componentRemovingInvokebacks) {
        //         callback->Invoke(entity, type);
        //     }
        //     for (auto callback : _componentRemovingInvokebacksByType[type]) {
        //         callback->Invoke(entity);
        //     }
        // }
    };
}