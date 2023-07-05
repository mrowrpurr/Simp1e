#pragma once

#include "ComponentType.h"
#include "ComponentTypeFromType.h"
#include "Entity.h"

namespace Simp1e {

    // TODO - use this :)

    // Intended to wrap a map<ComponentTypeHashKey, VoidPointer*>.
    // std::unordered_map<Entity, std::unordered_map<ComponentTypeHashKey, VoidPointer*>> _entities;

    struct IEntityComponentCollection {
        virtual ~IEntityComponentCollection() = default;

        virtual bool  HasComponent(ComponentType componentType) const        = 0;
        virtual bool  RemoveComponent(ComponentType componentType)           = 0;
        virtual void* GetComponentPointer(ComponentType componentType) const = 0;

        template <typename T>
        bool Has() const {
            return HasComponent(ComponentTypeFromType<T>());
        }

        template <typename T>
        T* Get() const {
            return static_cast<T*>(GetComponentPointer(ComponentTypeFromType<T>())->get());
        }
    };
}
