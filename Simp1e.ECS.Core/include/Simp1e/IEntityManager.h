#pragma once

#include <_Log_.h>
#include <function_pointer.h>

#include <memory>

#include "ComponentType.h"
#include "IEntityEventManager.h"

namespace Simp1e {

    struct IEntityManager {
        virtual ~IEntityManager() = default;

        virtual IEntityEventManager* GetEventManager() = 0;

        virtual Entity CreateEntity()               = 0;
        virtual void   DestroyEntity(Entity entity) = 0;
        virtual bool   EntityExists(Entity entity)  = 0;

        virtual void  RemoveComponent(Entity entity, ComponentType componentType)                       = 0;
        virtual bool  HasComponent(Entity entity, ComponentType componentType) const                    = 0;
        virtual void* GetComponentPointer(Entity entity, ComponentType componentType) const             = 0;
        virtual void  ForEachComponentFunction(ComponentType componentType, IFunctionPointer* callback) = 0;

        template <typename T>
        void Remove(Entity entity) {
            RemoveComponent(entity, T::GetComponentType());
        }

        template <typename T>
        bool Has(Entity entity) const {
            return HasComponent(entity, T::GetComponentType());
        }

        void ForEachComponentOfType(ComponentType componentType, void (*callback)(Entity, void*)) {
            ForEachComponentFunction(componentType, function_pointer(callback).get());
        }

        template <typename T>
        void ForEachComponentOfType(ComponentType componentType, T* object, void (T::*callback)(Entity, void*)) {
            ForEachComponentFunction(componentType, function_pointer(object, callback).get());
        }

        template <typename ComponentT>
        void ForEach(void (*callback)(Entity, void*)) {
            _Log_("[IEntityManager] ForEach (template)");
            ForEachComponentOfType(ComponentT::GetComponentType(), callback);
        }

        template <typename ComponentT, typename T>
        void ForEach(T* object, void (T::*callback)(Entity, void*)) {
            _Log_("[IEntityManager] ForEach (template)");
            ForEachComponentOfType(ComponentT::GetComponentType(), object, callback);
        }

        template <typename T>
        T* Get(Entity entity) const {
            return static_cast<T*>(GetComponentPointer(entity, T::GetComponentType()));
        }
    };
}
