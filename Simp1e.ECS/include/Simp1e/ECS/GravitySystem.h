#pragma once

#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/SizeComponent.h>

#include <memory>

#include "CommandSystem.h"
#include "ComponentCast.h"
#include "EntityManager.h"
#include "GravityComponent.h"
#include "MoveCommand.h"
#include "SystemTypeMacro.h"

namespace Simp1e::ECS {

    class GravitySystem {
        EntityManager& _entityManager;
        CommandSystem& _commandSystem;

    public:
        GravitySystem(EntityManager& entityManager, CommandSystem& commandSystem)
            : _entityManager(entityManager), _commandSystem(commandSystem) {}

        SIMP1E_ECS_SYSTEM("Gravity")

        void Update() {
            for (auto& [entity, component] : _entityManager.GetComponents<GravityComponent>()) {
                if (auto* gravityComponent = component_cast<GravityComponent>(component)) {
                    if (!gravityComponent->IsGrounded()) {
                    }
                }
            }
        }
    };
}
