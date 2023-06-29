#pragma once

#include <Simp1e/ComponentTypeHashKey.h>
#include <Simp1e/EntityPointerManager.h>
#include <Simp1e/VoidPointer.h>
#include <_Log_.h>

#include <memory>
#include <unordered_map>

namespace Simp1e {

    class EntityPointerManagerClient : public IEntityManager {
        EntityPointerManager*                                                             _entityManager;
        std::unordered_map<ComponentTypeHashKey, std::unordered_map<Entity, VoidPointer>> _componentMap;

    public:
        EntityPointerManagerClient(EntityPointerManager* entityManager) : _entityManager(entityManager) {}

        IEntityEventManager* GetEventManager() override { return _entityManager->GetEventManager(); }

        Entity CreateEntity() override {
            if (!_entityManager) _Log_("[EntityPointerManagerClient] CreateEntity - entity manager is null");
            _Log_("[EntityPointerManagerClient] CreateEntity - calling entity manager create entity...");
            return _entityManager->CreateEntity();
        }

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
            _Log_("!! [EntityPointerManagerClient] AddComponent");
            auto* component                              = new T(std::forward<Args>(args)...);
            _componentMap[T::GetComponentType()][entity] = MakeVoidPointer(component);
            _Log_("!! Calling entity manager AddComponentPointer() hmm");
            _entityManager->AddComponentPointer(entity, T::GetComponentType(), component);
            return component;
        }

        void AddComponentPointer(Entity entity, ComponentType componentType, void* component) override {}
    };
}
