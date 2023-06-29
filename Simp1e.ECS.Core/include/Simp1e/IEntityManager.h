#pragma once

#include "ComponentType.h"
#include "IEntityEventManager.h"

namespace Simp1e {

    struct IEntityManager {
        virtual ~IEntityManager() = default;

        virtual IEntityEventManager* GetEventManager() = 0;

        virtual Entity CreateEntity()               = 0;
        virtual void   DestroyEntity(Entity entity) = 0;
        virtual bool   EntityExists(Entity entity)  = 0;

        virtual void* AddComponentPointer(Entity entity, ComponentType componentType, void* component) = 0;
        virtual void  RemoveComponent(Entity entity, ComponentType componentType)                      = 0;
        virtual bool  HasComponent(Entity entity, ComponentType componentType) const                   = 0;
        virtual void* GetComponentPointer(Entity entity, ComponentType componentType) const            = 0;
        virtual void  ForEachComponent(ComponentType componentType, void (*callback)(Entity, void*))   = 0;

        // template <typename T, typename... Args>
        // T* AddComponent(Entity entity, Args&&... args) {
        //     auto* component = new T(std::forward<Args>(args)...);
        //     // _componentMap[T::GetComponentType()][entity] = MakeVoidPointer(component);
        //     return component;
        // }

        template <typename T>
        T* AddComponentPointer(Entity entity, T* component) {
            auto* componentPointer = AddComponentPointer(entity, T::GetComponentType(), component);
            if (componentPointer == nullptr) return nullptr;
            return static_cast<T*>(componentPointer);
        }

        template <typename T>
        void RemoveComponent(Entity entity) {
            RemoveComponent(entity, T::GetComponentType());
        }

        template <typename T>
        bool HasComponent(Entity entity) const {
            return HasComponent(entity, T::GetComponentType());
        }

        template <typename T>
        T* GetComponent(Entity entity) const {
            return static_cast<T*>(GetComponentPointer(entity, T::GetComponentType()));
        }

        template <typename T>
        void ForEachComponent(void (*callback)(Entity, void*)) {
            ForEachComponent(T::GetComponentType(), callback);
        }
    };
}
