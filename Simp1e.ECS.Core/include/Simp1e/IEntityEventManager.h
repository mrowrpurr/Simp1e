#pragma once

#include <Simp1e/FunctionPointer.h>
#include <Simp1e/MemberFunctionPointer.h>

#include "ComponentType.h"
#include "Entity.h"

namespace Simp1e {

    struct IEntityEventManager {
        virtual ~IEntityEventManager() = default;

        virtual void RegisterForEntityCreated(FunctionPointerBase*)    = 0;
        virtual void RegisterForEntityDestroying(FunctionPointerBase*) = 0;
        virtual void RegisterForEntityDestroyed(FunctionPointerBase*)  = 0;

        virtual void RegisterForComponentAdding(FunctionPointerBase*)   = 0;
        virtual void RegisterForComponentAdded(FunctionPointerBase*)    = 0;
        virtual void RegisterForComponentRemoving(FunctionPointerBase*) = 0;
        virtual void RegisterForComponentRemoved(FunctionPointerBase*)  = 0;

        virtual void RegisterForComponentAdding(ComponentType componentType, FunctionPointerBase*)   = 0;
        virtual void RegisterForComponentAdded(ComponentType componentType, FunctionPointerBase*)    = 0;
        virtual void RegisterForComponentRemoving(ComponentType componentType, FunctionPointerBase*) = 0;
        virtual void RegisterForComponentRemoved(ComponentType componentType, FunctionPointerBase*)  = 0;

        virtual void UnregisterForEntityCreated(FunctionPointerBase*)    = 0;
        virtual void UnregisterForEntityDestroying(FunctionPointerBase*) = 0;
        virtual void UnregisterForEntityDestroyed(FunctionPointerBase*)  = 0;

        virtual void UnregisterForComponentAdding(FunctionPointerBase*)   = 0;
        virtual void UnregisterForComponentAdded(FunctionPointerBase*)    = 0;
        virtual void UnregisterForComponentRemoving(FunctionPointerBase*) = 0;
        virtual void UnregisterForComponentRemoved(FunctionPointerBase*)  = 0;

        virtual void UnregisterForComponentAdding(ComponentType componentType, FunctionPointerBase*)   = 0;
        virtual void UnregisterForComponentAdded(ComponentType componentType, FunctionPointerBase*)    = 0;
        virtual void UnregisterForComponentRemoving(ComponentType componentType, FunctionPointerBase*) = 0;
        virtual void UnregisterForComponentRemoved(ComponentType componentType, FunctionPointerBase*)  = 0;

        virtual void EntityCreated(Entity entity)    = 0;
        virtual void EntityDestroying(Entity entity) = 0;
        virtual void EntityDestroyed(Entity entity)  = 0;

        virtual void ComponentAdding(Entity entity, ComponentType componentType)   = 0;
        virtual void ComponentAdded(Entity entity, ComponentType componentType)    = 0;
        virtual void ComponentRemoving(Entity entity, ComponentType componentType) = 0;
        virtual void ComponentRemoved(Entity entity, ComponentType componentType)  = 0;

        template <typename T>
        void RegisterForEntityCreated(T* object, void (T::*method)(Entity)) {
            RegisterForEntityCreated(new MemberFunctionPointer<T, void, Entity>(object, method));
        }

        template <typename T>
        void RegisterForEntityCreated(void (*function)(Entity)) {
            RegisterForEntityCreated(new FunctionPointer<void, Entity>(function));
        }

        // ...

        template <typename T>
        void RegisterForComponentAdded(T* object, void (T::*method)(Entity, ComponentType)) {
            RegisterForComponentAdded(new MemberFunctionPointer<T, void, Entity, ComponentType>(object, method));
        }

        template <typename T>
        void RegisterForComponentAdded(void (*function)(Entity, ComponentType)) {
            RegisterForComponentAdded(new FunctionPointer<void, Entity, ComponentType>(function));
        }
    };
}
