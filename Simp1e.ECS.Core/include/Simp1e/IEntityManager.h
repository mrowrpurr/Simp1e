#pragma once

#include <_Log_.h>
#include <function_pointer.h>
#include <void_pointer.h>

#include <memory>

#include "ComponentCast.h"
#include "ComponentPointer.h"
#include "ComponentType.h"
#include "ComponentTypeFromType.h"
#include "IEntityEventManager.h"

namespace Simp1e {

    struct IEntityManager {
        virtual IEntityEventManager* GetEventManager() = 0;

        virtual Entity CreateEntity()               = 0;
        virtual void   DestroyEntity(Entity entity) = 0;
        virtual bool   EntityExists(Entity entity)  = 0;

        virtual ComponentPointer AddComponentPointer(
            Entity entity, ComponentType componentType, IVoidPointer* pointer
        )                                                                                              = 0;
        virtual bool             RemoveComponent(Entity entity, ComponentType componentType)           = 0;
        virtual bool             HasComponent(Entity entity, ComponentType componentType) const        = 0;
        virtual ComponentPointer GetComponentPointer(Entity entity, ComponentType componentType) const = 0;

        virtual void ForEachComponent(
            ComponentType componentType, IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>* callback
        ) = 0;

        virtual void ForEachComponent(
            ComponentType componentType, FunctionPointer<void(Entity, ComponentType, ComponentPointer)> callback
        ) {
            ForEachComponent(componentType, &callback);
        }

        template <typename TComponent, typename... TArgs>
        TComponent* AddComponent(Entity entity, TArgs&&... args) {
            auto  componentType     = ComponentTypeFromType<TComponent>();
            auto* existingComponent = GetComponentPointer(entity, componentType);
            if (existingComponent) {
                _Log_("[LocalEntityManager] Entity {} already has component {}", entity, componentType);
                return component_cast<TComponent>(existingComponent);
            }

            _Log_("[LocalEntityManager] AddComponent {} to {}", componentType, entity);

            auto component        = new TComponent(std::forward<TArgs>(args)...);
            auto componentPointer = new VoidPointer<TComponent>(component);

            AddComponentPointer(entity, componentType, componentPointer);

            return component;
        }

        template <typename T>
        bool Remove(Entity entity) {
            return RemoveComponent(entity, ComponentTypeFromType<T>());
        }

        template <typename T>
        bool HasComponent(Entity entity) const {
            return HasComponent(entity, ComponentTypeFromType<T>());
        }

        template <typename T>
        T* GetComponent(Entity entity) const {
            return component_cast<T>(GetComponentPointer(entity, ComponentTypeFromType<T>()));
        }

        template <typename T>
        void ForEach(IFunctionPointer<void(Entity, ComponentType, ComponentPointer)>* callback) {
            ForEachComponent(ComponentTypeFromType<T>(), callback);
        }

        template <typename T>
        void ForEach(FunctionPointer<void(Entity, ComponentType, ComponentPointer)> callback) {
            ForEachComponent(ComponentTypeFromType<T>(), &callback);
        }
    };
}
