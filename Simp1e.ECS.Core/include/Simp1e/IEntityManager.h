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

        virtual void  RemoveComponent(Entity entity, ComponentType componentType)                      = 0;
        virtual bool  HasComponent(Entity entity, ComponentType componentType) const                   = 0;
        virtual void* GetComponentPointer(Entity entity, ComponentType componentType) const            = 0;
        virtual void  ForEachComponent(ComponentType componentType, void (*callback)(Entity, void*))   = 0;
        virtual void  AddComponentPointer(Entity entity, ComponentType componentType, void* component) = 0;
    };
}
