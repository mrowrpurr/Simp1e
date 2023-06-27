#pragma once

#include <memory>

#include "ComponentCast.h"
#include "EntityManager.h"
#include "PositionComponent.h"
#include "SystemTypeMacro.h"

namespace Simp1e::ECS {

    class StoreLastPositionSystem {
        EntityManager& _entityManager;

    public:
        SIMP1E_ECS_SYSTEM("StoreLastPosition")

        StoreLastPositionSystem(EntityManager& entityManager) : _entityManager(entityManager) {}

        virtual void Update() {
            for (auto& [entity, component] : _entityManager.GetComponents<PositionComponent>()) {
                auto* positionComponent = component_cast<PositionComponent>(component);
                if (!positionComponent) continue;
                positionComponent->StoreLastPosition();
            }
        }
    };
}
