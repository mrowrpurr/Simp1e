#pragma once

#include <Simp1e/FunctionPointer.h>
#include <Simp1e/MemberFunctionPointer.h>

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

        virtual void  RemoveComponent(Entity entity, ComponentType componentType)                          = 0;
        virtual bool  HasComponent(Entity entity, ComponentType componentType) const                       = 0;
        virtual void* GetComponentPointer(Entity entity, ComponentType componentType) const                = 0;
        virtual void  ForEachComponentFunctionPtr(ComponentType componentType, IFunctionPointer* callback) = 0;

        template <typename T>
        void Remove(Entity entity) {
            RemoveComponent(entity, T::GetComponentType());
        }

        template <typename T>
        bool Has(Entity entity) const {
            return HasComponent(entity, T::GetComponentType());
        }

        void ForEachComponentOfType(ComponentType componentType, void (*callback)(Entity, void*)) {
            auto* functionPointer = new FunctionPointer<void, Entity, void*>(callback);
            ForEachComponentFunctionPtr(componentType, functionPointer);
            delete functionPointer;
        }

        template <typename T>
        void ForEachComponentOfType(ComponentType componentType, T* object, void (T::*callback)(Entity, void*)) {
            auto* functionPointer = new MemberFunctionPointer<T, void, Entity, void*>(object, callback);
            ForEachComponentFunctionPtr(componentType, functionPointer);
            delete functionPointer;
        }

        template <typename ComponentT>
        void ForEach(void (*callback)(Entity, void*)) {
            auto* functionPointer = new FunctionPointer<void, Entity, void*>(callback);
            ForEachComponentFunctionPtr(ComponentT::GetComponentType(), functionPointer);
            delete functionPointer;
        }

        template <typename ComponentT, typename T>
        void ForEach(T* object, void (T::*callback)(Entity, void*)) {
            auto* functionPointer = new MemberFunctionPointer<T, void, Entity, void*>(object, callback);
            ForEachComponentFunctionPtr(ComponentT::GetComponentType(), functionPointer);
            delete functionPointer;
        }

        template <typename T>
        T* Get(Entity entity) const {
            return static_cast<T*>(GetComponentPointer(entity, T::GetComponentType()));
        }
    };
}
