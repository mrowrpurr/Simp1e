#pragma once

#include <Simp1e/Direction.h>

#include <memory>

#include "CommandSystem.h"
#include "ComponentCast.h"
#include "EntityManager.h"
#include "GravityComponent.h"
#include "JumpingComponent.h"
#include "MoveCommand.h"
#include "PositionComponent.h"
#include "SizeComponent.h"
#include "SystemTypeMacro.h"

namespace Simp1e::ECS {

    class JumpSystem {
        EntityManager& _entityManager;
        CommandSystem& _commandSystem;

    public:
        JumpSystem(EntityManager& entityManager, CommandSystem& commandSystem)
            : _entityManager(entityManager), _commandSystem(commandSystem) {}

        SIMP1E_ECS_SYSTEM("Gravity")

        void Update() {
            for (auto& [entity, component] : _entityManager.GetComponents<JumpingComponent>()) {
                if (auto* jumpingComponent = component_cast<JumpingComponent>(component)) {
                    qDebug() << "Jump System";

                    auto* positionComponent = _entityManager.GetComponent<PositionComponent>(entity);
                    if (!positionComponent) continue;

                    if (jumpingComponent->CurrentJumpHeight() >= jumpingComponent->JumpHeight()) {
                        qDebug() << "JumpingComponent::Update() - Jump height reached, removing component";
                        _entityManager.RemoveComponent<JumpingComponent>(entity);
                        continue;
                    }

                    auto* gravityComponent = _entityManager.GetComponent<GravityComponent>(entity);
                    if (gravityComponent) gravityComponent->SetIsGrounded(false);

                    qDebug() << "JumpingComponent::Update() - CurrentJumpHeight: "
                             << jumpingComponent->CurrentJumpHeight();
                    jumpingComponent->SetCurrentJumpHeight(
                        jumpingComponent->CurrentJumpHeight() + jumpingComponent->JumpSpeed()
                    );
                    _commandSystem.RunCommand(MoveCommand::Up(entity, jumpingComponent->JumpSpeed()));
                }
            }
        }
    };
}
