#pragma once

#include <Simp1e/sreal.h>

#include <string>

#include "CommandTypeMacro.h"
#include "Entity.h"
#include "Game.h"
#include "GravityComponent.h"
#include "JumpingComponent.h"

namespace Simp1e::ECS {

    class JumpCommand {
        Entity _entity;
        sreal  _jumpHeight;
        sreal  _jumpSpeed;

    public:
        SIMP1E_ECS_COMMAND("Jump")

        JumpCommand() = default;
        JumpCommand(const Entity& entity, sreal jumpHeight, sreal jumpSpeed)
            : _entity(entity), _jumpHeight(jumpHeight), _jumpSpeed(jumpSpeed) {}

        void Execute(Game& game) {
            auto* gravityComponent = game.Entities().GetComponent<GravityComponent>(_entity);
            if (!gravityComponent) return;
            if (!gravityComponent->IsGrounded()) return;

            game.Entities().AddComponent(_entity, JumpingComponent{_jumpHeight, _jumpSpeed});
        }
    };
}
