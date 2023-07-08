#pragma once

#include <function_pointer.h>

#include "ComponentType.h"
#include "Entity.h"

namespace Simp1e {

    struct IEntityEventManager {
        virtual ~IEntityEventManager() = default;

        virtual IFunctionPointer<void(Entity)>* RegisterForEntityCreated(IFunctionPointer<void(Entity)>*)    = 0;
        virtual IFunctionPointer<void(Entity)>* RegisterForEntityDestroying(IFunctionPointer<void(Entity)>*) = 0;
        virtual IFunctionPointer<void(Entity)>* RegisterForEntityDestroyed(IFunctionPointer<void(Entity)>*)  = 0;

        virtual IFunctionPointer<void(Entity, ComponentType)>*
        RegisterForAllComponentAdding(IFunctionPointer<void(Entity, ComponentType)>*) = 0;
        virtual IFunctionPointer<void(Entity, ComponentType, void*)>*
        RegisterForAllComponentAdded(IFunctionPointer<void(Entity, ComponentType, void*)>*) = 0;
        virtual IFunctionPointer<void(Entity, ComponentType, void*)>*
        RegisterForAllComponentRemoving(IFunctionPointer<void(Entity, ComponentType, void*)>*) = 0;
        virtual IFunctionPointer<void(Entity, ComponentType)>*
        RegisterForAllComponentRemoved(IFunctionPointer<void(Entity, ComponentType)>*) = 0;

        virtual IFunctionPointer<void(Entity, ComponentType)>*
        RegisterForComponentAdding(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType)>*) = 0;
        virtual IFunctionPointer<void(Entity, ComponentType, void*)>*
        RegisterForComponentAdded(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType, void*)>*) = 0;
        virtual IFunctionPointer<void(Entity, ComponentType, void*)>*
        RegisterForComponentRemoving(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType, void*)>*) = 0;
        virtual IFunctionPointer<void(Entity, ComponentType)>*
        RegisterForComponentRemoved(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType)>*) = 0;

        virtual void UnregisterForEntityCreated(IFunctionPointer<void(Entity)>*)    = 0;
        virtual void UnregisterForEntityDestroying(IFunctionPointer<void(Entity)>*) = 0;
        virtual void UnregisterForEntityDestroyed(IFunctionPointer<void(Entity)>*)  = 0;

        virtual void UnregisterForAllComponentAdding(IFunctionPointer<void(Entity, ComponentType)>*)          = 0;
        virtual void UnregisterForAllComponentAdded(IFunctionPointer<void(Entity, ComponentType, void*)>*)    = 0;
        virtual void UnregisterForAllComponentRemoving(IFunctionPointer<void(Entity, ComponentType, void*)>*) = 0;
        virtual void UnregisterForAllComponentRemoved(IFunctionPointer<void(Entity, ComponentType)>*)         = 0;

        virtual void
        UnregisterForComponentAdding(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType)>*) = 0;
        virtual void
        UnregisterForComponentAdded(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType, void*)>*) = 0;
        virtual void
        UnregisterForComponentRemoving(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType, void*)>*) = 0;
        virtual void
        UnregisterForComponentRemoved(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType)>*) = 0;

        virtual void EntityCreated(Entity entity)    = 0;
        virtual void EntityDestroying(Entity entity) = 0;
        virtual void EntityDestroyed(Entity entity)  = 0;

        virtual void ComponentAdding(Entity entity, ComponentType componentType)                    = 0;
        virtual void ComponentAdded(Entity entity, ComponentType componentType, void* component)    = 0;
        virtual void ComponentRemoving(Entity entity, ComponentType componentType, void* component) = 0;
        virtual void ComponentRemoved(Entity entity, ComponentType componentType)                   = 0;
    };
}
