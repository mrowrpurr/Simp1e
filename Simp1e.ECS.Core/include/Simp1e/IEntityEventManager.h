#pragma once

#include "ComponentType.h"
#include "Entity.h"

namespace Simp1e {

    struct IEntityEventManager {
        virtual ~IEntityEventManager() = default;

        virtual void OnEntityCreated(void (*callback)(Entity))    = 0;
        virtual void OnEntityDestroying(void (*callback)(Entity)) = 0;
        virtual void OnEntityDestroyed(void (*callback)(Entity))  = 0;

        virtual void OnComponentAdding(void (*callback)(Entity, ComponentType))   = 0;
        virtual void OnComponentAdded(void (*callback)(Entity, ComponentType))    = 0;
        virtual void OnComponentRemoving(void (*callback)(Entity, ComponentType)) = 0;
        virtual void OnComponentRemoved(void (*callback)(Entity, ComponentType))  = 0;

        virtual void EntityCreated(Entity entity)    = 0;
        virtual void EntityDestroying(Entity entity) = 0;
        virtual void EntityDestroyed(Entity entity)  = 0;

        virtual void ComponentAdding(Entity entity, ComponentType componentType)   = 0;
        virtual void ComponentAdded(Entity entity, ComponentType componentType)    = 0;
        virtual void ComponentRemoving(Entity entity, ComponentType componentType) = 0;
        virtual void ComponentRemoved(Entity entity, ComponentType componentType)  = 0;
    };
}
