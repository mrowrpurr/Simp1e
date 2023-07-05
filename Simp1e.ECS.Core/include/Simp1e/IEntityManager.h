#pragma once

#include <_Log_.h>
#include <function_pointer.h>
#include <void_pointer.h>

#include <memory>

#include "ComponentType.h"
#include "ComponentTypeFromType.h"
#include "IEntityComponentCollection.h"
#include "IEntityEventManager.h"
#include "IMemoryManagedEngineContainerClass.h"

namespace Simp1e {

    struct IEntityManager : public IMemoryManagedEngineContainerClass {
        virtual IEntityEventManager* GetEventManager() = 0;

        virtual Entity                      CreateEntity()               = 0;
        virtual void                        DestroyEntity(Entity entity) = 0;
        virtual bool                        EntityExists(Entity entity)  = 0;
        virtual IEntityComponentCollection* GetEntity(Entity entity)     = 0;

        virtual bool OwnsEntityMemoryManagement() const = 0;
        bool         ManagesEngineItemMemory() const override { return OwnsEntityMemoryManagement(); }

        virtual VoidPointer* AddComponentPointer(
            Entity entity, ComponentType componentType, VoidPointer componentPointer
        )                                                                                   = 0;
        virtual bool  RemoveComponent(Entity entity, ComponentType componentType)           = 0;
        virtual bool  HasComponent(Entity entity, ComponentType componentType) const        = 0;
        virtual void* GetComponentPointer(Entity entity, ComponentType componentType) const = 0;

        virtual void ForEachComponent(ComponentType componentType, IFunctionPointer* callback) = 0;

        template <typename T, typename... Args>
        T* Add(Entity entity, Args&&... args) {
            _Log_("[IEntityManager] Add component of type {} to {}", ComponentTypeFromType<T>(), entity);
            auto* component   = new T(std::forward<Args>(args)...);
            auto* voidPointer = AddComponentPointer(entity, ComponentTypeFromType<T>(), void_pointer(component));
            if (!ManagesEngineItemMemory()) voidPointer->get()->disable_destruct_on_delete();
            return component;
        }

        template <typename T>
        bool Remove(Entity entity) {
            RemoveComponent(entity, ComponentTypeFromType<T>());
        }

        template <typename T>
        bool Has(Entity entity) const {
            return HasComponent(entity, ComponentTypeFromType<T>());
        }

        template <typename T>
        void ForEach(IFunctionPointer* callback) {
            ForEachComponent(ComponentTypeFromType<T>(), callback);
        }

        template <typename T>
        T* Get(Entity entity) const {
            return static_cast<T*>(GetComponentPointer(entity, ComponentTypeFromType<T>()));
        }
    };
}
