#pragma once

#include <function_pointer.h>

#include "ComponentPointer.h"
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
        virtual IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>*
        RegisterForAllComponentAdded(IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>*) = 0;
        virtual IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>*
        RegisterForAllComponentRemoving(IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>*) = 0;
        virtual IFunctionPointer<void(Entity, ComponentType)>*
        RegisterForAllComponentRemoved(IFunctionPointer<void(Entity, ComponentType)>*) = 0;

        virtual IFunctionPointer<void(Entity, ComponentType)>*
        RegisterForComponentAdding(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType)>*) = 0;
        virtual IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>*
        RegisterForComponentAdded(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>*) = 0;
        virtual IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>*
        RegisterForComponentRemoving(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>*) = 0;
        virtual IFunctionPointer<void(Entity, ComponentType)>*
        RegisterForComponentRemoved(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType)>*) = 0;

        template <typename TComponent>
        IFunctionPointer<void(Entity, ComponentType)>* RegisterForComponentAdding(
            FunctionPointer<void(Entity, ComponentType)> callback
        ) {
            callback.do_not_destroy_function_pointer();
            return RegisterForComponentAdding(ComponentTypeFromType<TComponent>(), callback.inner_function_pointer());
        }

        template <typename TComponent>
        IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>* RegisterForComponentAdded(
            FunctionPointer<void(Entity, ComponentType, ComponentPointer)> callback
        ) {
            callback.do_not_destroy_function_pointer();
            return RegisterForComponentAdded(ComponentTypeFromType<TComponent>(), callback.inner_function_pointer());
        }

        template <typename TComponent>
        IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>* RegisterForComponentRemoving(
            FunctionPointer<void(Entity, ComponentType, ComponentPointer)> callback
        ) {
            callback.do_not_destroy_function_pointer();
            return RegisterForComponentRemoving(ComponentTypeFromType<TComponent>(), callback.inner_function_pointer());
        }

        template <typename TComponent>
        IFunctionPointer<void(Entity, ComponentType)>* RegisterForComponentRemoved(
            FunctionPointer<void(Entity, ComponentType)> callback
        ) {
            callback.do_not_destroy_function_pointer();
            return RegisterForComponentRemoved(ComponentTypeFromType<TComponent>(), callback.inner_function_pointer());
        }

        virtual void UnregisterForEntityCreated(IFunctionPointer<void(Entity)>*)    = 0;
        virtual void UnregisterForEntityDestroying(IFunctionPointer<void(Entity)>*) = 0;
        virtual void UnregisterForEntityDestroyed(IFunctionPointer<void(Entity)>*)  = 0;

        virtual void UnregisterForAllComponentAdding(IFunctionPointer<void(Entity, ComponentType)>*) = 0;
        virtual void
        UnregisterForAllComponentAdded(IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>*) = 0;
        virtual void
        UnregisterForAllComponentRemoving(IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>*) = 0;
        virtual void UnregisterForAllComponentRemoved(IFunctionPointer<void(Entity, ComponentType)>*)       = 0;

        virtual void
        UnregisterForComponentAdding(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType)>*) = 0;
        virtual void
        UnregisterForComponentAdded(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>*) = 0;
        virtual void
        UnregisterForComponentRemoving(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>*) = 0;
        virtual void
        UnregisterForComponentRemoved(ComponentType componentType, IFunctionPointer<void(Entity, ComponentType)>*) = 0;

        virtual void EntityCreated(Entity entity)    = 0;
        virtual void EntityDestroying(Entity entity) = 0;
        virtual void EntityDestroyed(Entity entity)  = 0;

        virtual void ComponentAdding(Entity entity, ComponentType componentType)                               = 0;
        virtual void ComponentAdded(Entity entity, ComponentType componentType, ComponentPointer component)    = 0;
        virtual void ComponentRemoving(Entity entity, ComponentType componentType, ComponentPointer component) = 0;
        virtual void ComponentRemoved(Entity entity, ComponentType componentType)                              = 0;
    };
}
