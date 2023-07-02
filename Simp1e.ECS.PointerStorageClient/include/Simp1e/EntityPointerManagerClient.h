#pragma once

#include <Simp1e/ComponentTypeHashKey.h>
#include <Simp1e/IEntityPointerManager.h>
#include <_Log_.h>
#include <void_pointer.h>

#include <memory>
#include <unordered_map>
#include <utility>

namespace Simp1e {

    class EntityPointerManagerClient : public IEntityManager {
        IEntityPointerManager*                                                            _entityManager;
        std::unordered_map<ComponentTypeHashKey, std::unordered_map<Entity, VoidPointer>> _componentMap;

    public:
        EntityPointerManagerClient(IEntityPointerManager* entityManager) : _entityManager(entityManager) {}
        EntityPointerManagerClient(IEntityManager* entityManager)
            : _entityManager(dynamic_cast<IEntityPointerManager*>(entityManager)) {}

        IEntityEventManager* GetEventManager() override { return _entityManager->GetEventManager(); }

        Entity CreateEntity() override { return _entityManager->CreateEntity(); }

        void DestroyEntity(Entity entity) override { _entityManager->DestroyEntity(entity); }

        bool EntityExists(Entity entity) override { return _entityManager->EntityExists(entity); }

        void RemoveComponent(Entity entity, ComponentType componentType) override {
            _entityManager->RemoveComponent(entity, componentType);
            _componentMap[componentType].erase(entity);
        }

        bool HasComponent(Entity entity, ComponentType componentType) const override {
            return _entityManager->HasComponent(entity, componentType);
        }

        void* GetComponentPointer(Entity entity, ComponentType componentType) const override {
            return _entityManager->GetComponentPointer(entity, componentType);
        }

        void ForEachComponentFunction(ComponentType componentType, IFunctionPointer* callback) override {
            _Log_("[EntityPointerManagerClient] ForEachComponentFunction");
            _entityManager->ForEachComponentFunction(componentType, callback);
        }

        template <typename T, typename... Args>
        T* Add(Entity entity, Args&&... args) {
            _Log_("[EntityPointerManagerClient] Add component of type {} to {}", T::GetComponentType(), entity);
            auto* component                              = new T(std::forward<Args>(args)...);
            _componentMap[T::GetComponentType()][entity] = void_pointer(component);
            _entityManager->AddComponentPointer(entity, T::GetComponentType(), component);
            return component;
        }
    };
}
