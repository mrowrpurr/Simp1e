#pragma once

#include <function_pointer.h>

#include "ComponentType.h"
#include "Entity.h"

namespace Simp1e {

    struct IEntityEventManager {
        virtual ~IEntityEventManager() = default;

        // TODO : update the virtual functions to be named like: RegisterForEntityCreatedFunctionPtr

        virtual IFunctionPointer* RegisterForEntityCreated(IFunctionPointer*)    = 0;
        virtual IFunctionPointer* RegisterForEntityDestroying(IFunctionPointer*) = 0;
        virtual IFunctionPointer* RegisterForEntityDestroyed(IFunctionPointer*)  = 0;

        virtual IFunctionPointer* RegisterForAllComponentAdding(IFunctionPointer*)   = 0;
        virtual IFunctionPointer* RegisterForAllComponentAdded(IFunctionPointer*)    = 0;
        virtual IFunctionPointer* RegisterForAllComponentRemoving(IFunctionPointer*) = 0;
        virtual IFunctionPointer* RegisterForAllComponentRemoved(IFunctionPointer*)  = 0;

        virtual IFunctionPointer* RegisterForComponentAdding(ComponentType componentType, IFunctionPointer*)   = 0;
        virtual IFunctionPointer* RegisterForComponentAdded(ComponentType componentType, IFunctionPointer*)    = 0;
        virtual IFunctionPointer* RegisterForComponentRemoving(ComponentType componentType, IFunctionPointer*) = 0;
        virtual IFunctionPointer* RegisterForComponentRemoved(ComponentType componentType, IFunctionPointer*)  = 0;

        virtual void UnregisterForEntityCreated(IFunctionPointer*)    = 0;
        virtual void UnregisterForEntityDestroying(IFunctionPointer*) = 0;
        virtual void UnregisterForEntityDestroyed(IFunctionPointer*)  = 0;

        virtual void UnregisterForAllComponentAdding(IFunctionPointer*)   = 0;
        virtual void UnregisterForAllComponentAdded(IFunctionPointer*)    = 0;
        virtual void UnregisterForAllComponentRemoving(IFunctionPointer*) = 0;
        virtual void UnregisterForAllComponentRemoved(IFunctionPointer*)  = 0;

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
        IFunctionPointer* RegisterForEntityCreated(T* object, void (T::*method)(Entity)) {
            // return RegisterForEntityCreated(new MemberFunctionPointer<T, void, Entity>(object, method));
            return RegisterForEntityCreated(new_function_pointer(object, method));
        }

        IFunctionPointer* RegisterForEntityCreated(void (*function)(Entity)) {
            // return RegisterForEntityCreated(new FunctionPointer<void, Entity>(function));
            return RegisterForEntityCreated(new_function_pointer(function));
        }

        template <typename T>
        IFunctionPointer* RegisterForEntityDestroying(T* object, void (T::*method)(Entity)) {
            // return RegisterForEntityDestroying(new MemberFunctionPointer<T, void, Entity>(object, method));
            return RegisterForEntityDestroying(new_function_pointer(object, method));
        }

        IFunctionPointer* RegisterForEntityDestroying(void (*function)(Entity)) {
            return RegisterForEntityDestroying(new_function_pointer(function));
        }

        template <typename T>
        IFunctionPointer* RegisterForEntityDestroyed(T* object, void (T::*method)(Entity)) {
            return RegisterForEntityDestroyed(new_function_pointer(object, method));
        }

        IFunctionPointer* RegisterForEntityDestroyed(void (*function)(Entity)) {
            return RegisterForEntityDestroyed(new_function_pointer(function));
        }

        template <typename T>
        IFunctionPointer* RegisterForAllComponentAdding(T* object, void (T::*method)(Entity, ComponentType)) {
            return RegisterForAllComponentAdding(new_function_pointer(object, method));
        }

        IFunctionPointer* RegisterForAllComponentAdding(void (*function)(Entity, ComponentType)) {
            return RegisterForAllComponentAdding(new_function_pointer(function));
        }

        template <typename T>
        IFunctionPointer* RegisterForAllComponentAdded(T* object, void (T::*method)(Entity, ComponentType)) {
            return RegisterForAllComponentAdded(new_function_pointer(object, method));
        }

        IFunctionPointer* RegisterForAllComponentAdded(void (*function)(Entity, ComponentType)) {
            return RegisterForAllComponentAdded(new_function_pointer(function));
        }

        template <typename T>
        IFunctionPointer* RegisterForAllComponentRemoving(T* object, void (T::*method)(Entity, ComponentType)) {
            return RegisterForAllComponentRemoving(new_function_pointer(object, method));
        }

        IFunctionPointer* RegisterForAllComponentRemoving(void (*function)(Entity, ComponentType)) {
            return RegisterForAllComponentRemoving(new_function_pointer(function));
        }

        template <typename T>
        IFunctionPointer* RegisterForAllComponentRemoved(T* object, void (T::*method)(Entity, ComponentType)) {
            return RegisterForAllComponentRemoved(new_function_pointer(object, method));
        }

        IFunctionPointer* RegisterForAllComponentRemoved(void (*function)(Entity, ComponentType)) {
            return RegisterForAllComponentRemoved(new_function_pointer(function));
        }

        template <typename T>
        IFunctionPointer* RegisterForComponentAdding(
            ComponentType componentType, T* object, void (T::*method)(Entity, ComponentType)
        ) {
            return RegisterForComponentAdding(componentType, new_function_pointer(object, method));
        }

        template <typename ComponentT, typename T>
        IFunctionPointer* RegisterForComponentAdding(T* object, void (T::*method)(Entity, ComponentType)) {
            return RegisterForComponentAdding(ComponentT::GetComponentType(), new_function_pointer(object, method));
        }

        IFunctionPointer* RegisterForComponentAdding(
            ComponentType componentType, void (*function)(Entity, ComponentType)
        ) {
            return RegisterForComponentAdding(componentType, new_function_pointer(function));
        }

        template <typename ComponentT>
        IFunctionPointer* RegisterForComponentAdding(void (*function)(Entity, ComponentType)) {
            return RegisterForComponentAdding(ComponentT::GetComponentType(), new_function_pointer(function));
        }

        template <typename T>
        IFunctionPointer* RegisterForComponentAdded(
            ComponentType componentType, T* object, void (T::*method)(Entity, ComponentType)
        ) {
            return RegisterForComponentAdded(componentType, new_function_pointer(object, method));
        }

        template <typename ComponentT, typename T>
        IFunctionPointer* RegisterForComponentAdded(T* object, void (T::*method)(Entity, ComponentType)) {
            return RegisterForComponentAdded(ComponentT::GetComponentType(), new_function_pointer(object, method));
        }

        IFunctionPointer* RegisterForComponentAdded(
            ComponentType componentType, void (*function)(Entity, ComponentType)
        ) {
            return RegisterForComponentAdded(componentType, new_function_pointer(function));
        }

        template <typename ComponentT>
        IFunctionPointer* RegisterForComponentAdded(void (*function)(Entity, ComponentType)) {
            return RegisterForComponentAdded(ComponentT::GetComponentType(), new_function_pointer(function));
        }

        template <typename T>
        IFunctionPointer* RegisterForComponentRemoving(
            ComponentType componentType, T* object, void (T::*method)(Entity, ComponentType)
        ) {
            return RegisterForComponentRemoving(componentType, new_function_pointer(object, method));
        }

        template <typename ComponentT, typename T>
        IFunctionPointer* RegisterForComponentRemoving(T* object, void (T::*method)(Entity, ComponentType)) {
            return RegisterForComponentRemoving(ComponentT::GetComponentType(), new_function_pointer(object, method));
        }

        IFunctionPointer* RegisterForComponentRemoving(
            ComponentType componentType, void (*function)(Entity, ComponentType)
        ) {
            return RegisterForComponentRemoving(componentType, new_function_pointer(function));
        }

        template <typename ComponentT>
        IFunctionPointer* RegisterForComponentRemoving(void (*function)(Entity, ComponentType)) {
            return RegisterForComponentRemoving(ComponentT::GetComponentType(), new_function_pointer(function));
        }

        template <typename T>
        IFunctionPointer* RegisterForComponentRemoved(
            ComponentType componentType, T* object, void (T::*method)(Entity, ComponentType)
        ) {
            return RegisterForComponentRemoved(componentType, new_function_pointer(object, method));
        }

        template <typename ComponentT, typename T>
        IFunctionPointer* RegisterForComponentRemoved(T* object, void (T::*method)(Entity, ComponentType)) {
            return RegisterForComponentRemoved(ComponentT::GetComponentType(), new_function_pointer(object, method));
        }

        IFunctionPointer* RegisterForComponentRemoved(
            ComponentType componentType, void (*function)(Entity, ComponentType)
        ) {
            return RegisterForComponentRemoved(componentType, new_function_pointer(function));
        }

        template <typename ComponentT>
        IFunctionPointer* RegisterForComponentRemoved(void (*function)(Entity, ComponentType)) {
            return RegisterForComponentRemoved(ComponentT::GetComponentType(), new_function_pointer(function));
        }
    };
}
