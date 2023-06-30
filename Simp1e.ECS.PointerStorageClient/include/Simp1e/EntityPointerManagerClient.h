#pragma once

#include <Simp1e/ComponentTypeHashKey.h>
#include <Simp1e/IEntityPointerManager.h>
#include <Simp1e/VoidPointer.h>
#include <_Log_.h>

#include <memory>
#include <unordered_map>

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

        void ForEachComponent(ComponentType componentType, void (*callback)(Entity, void*)) override {
            _entityManager->ForEachComponent(componentType, callback);
        }

        template <typename T, typename... Args>
        T* AddComponent(Entity entity, Args&&... args) {
            auto* component                              = new T(std::forward<Args>(args)...);
            _componentMap[T::GetComponentType()][entity] = MakeVoidPointer(component);
            _entityManager->AddComponentPointer(entity, T::GetComponentType(), component);
            return component;
        }

        template <typename T>
        void ForEachComponent(void (*callback)(Entity, void*)) {
            ForEachComponent(T::GetComponentType(), callback);
        }
    };
}
