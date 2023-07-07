#pragma once

#include <Simp1e/ComponentType.h>
#include <Simp1e/Entity.h>
#include <Simp1e/IEngine.h>

namespace Simp1e {

    template <typename T>
    struct QtComponentEventHandler {
        virtual ~QtComponentEventHandler() = default;

        virtual void OnComponentAdding(Entity entity, ComponentType componentType) {}
        virtual void OnComponentAdded(Entity entity, ComponentType componentType, T* component) {}
        virtual void OnComponentRemoving(Entity entity, ComponentType componentType, T* component) {}
        virtual void OnComponentRemoved(Entity entity, ComponentType componentType) {}

        // Entity entity, ComponentType componentType, void* component
        // void OnComponentAddingCallback()...
    };
}
