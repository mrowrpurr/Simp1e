#pragma once

#include <Simp1e/ComponentType.h>
#include <Simp1e/ComponentTypeFromType.h>
#include <Simp1e/Entity.h>
#include <Simp1e/IEntityManager.h>
#include <void_pointer.h>

namespace Simp1e {

    struct ILocalEntityManager : public IEntityManager {
        virtual void AddComponentPointer(Entity entity, ComponentType componentType, VoidPointer component) = 0;

        template <typename T, typename... Args>
        T* AddComponent(Entity entity, Args&&... args) {
            auto* component = new T(std::forward<Args>(args)...);
            AddComponentPointer(entity, ComponentTypeFromType<T>(), void_pointer(component));
            return component;
        }
    };
}
