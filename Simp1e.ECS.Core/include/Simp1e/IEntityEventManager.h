#pragma once

#include <Simp1e/FunctionPointer.h>
#include <Simp1e/MemberFunctionPointer.h>

#include "ComponentType.h"
#include "Entity.h"

namespace Simp1e {

    struct IEntityEventManager {
        virtual ~IEntityEventManager() = default;

        virtual void RegisterForEntityCreated(IFunctionPointer*)    = 0;
        virtual void RegisterForEntityDestroying(IFunctionPointer*) = 0;
        virtual void RegisterForEntityDestroyed(IFunctionPointer*)  = 0;

        virtual void RegisterForComponentAdding(IFunctionPointer*)   = 0;
        virtual void RegisterForComponentAdded(IFunctionPointer*)    = 0;
        virtual void RegisterForComponentRemoving(IFunctionPointer*) = 0;
        virtual void RegisterForComponentRemoved(IFunctionPointer*)  = 0;

        virtual void RegisterForComponentAdding(ComponentType componentType, IFunctionPointer*)   = 0;
        virtual void RegisterForComponentAdded(ComponentType componentType, IFunctionPointer*)    = 0;
        virtual void RegisterForComponentRemoving(ComponentType componentType, IFunctionPointer*) = 0;
        virtual void RegisterForComponentRemoved(ComponentType componentType, IFunctionPointer*)  = 0;

        virtual void UnregisterForEntityCreated(IFunctionPointer*)    = 0;
        virtual void UnregisterForEntityDestroying(IFunctionPointer*) = 0;
        virtual void UnregisterForEntityDestroyed(IFunctionPointer*)  = 0;

        virtual void UnregisterForComponentAdding(IFunctionPointer*)   = 0;
        virtual void UnregisterForComponentAdded(IFunctionPointer*)    = 0;
        virtual void UnregisterForComponentRemoving(IFunctionPointer*) = 0;
        virtual void UnregisterForComponentRemoved(IFunctionPointer*)  = 0;

        virtual void UnregisterForComponentAdding(ComponentType componentType, IFunctionPointer*)   = 0;
        virtual void UnregisterForComponentAdded(ComponentType componentType, IFunctionPointer*)    = 0;
        virtual void UnregisterForComponentRemoving(ComponentType componentType, IFunctionPointer*) = 0;
        virtual void UnregisterForComponentRemoved(ComponentType componentType, IFunctionPointer*)  = 0;

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
