#pragma once

#include <Simp1e/Direction.h>
#include <Simp1e/ECS/CommandSystem.h>
#include <Simp1e/ECS/CommandTypeMacro.h>
#include <Simp1e/ECS/Entity.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/MoveCommand.h>
#include <Simp1e/ECS/SetImageCommand.h>
#include <Simp1e/sreal.h>

#include "Images.h"

namespace Simp1e::ECS {

    class MovePlayerCommand {
        Direction _direction;
        Entity    _playerEntity;
        sreal     _distance;

    public:
        SIMP1E_ECS_COMMAND("MovePlayer")

        MovePlayerCommand() = default;
        MovePlayerCommand(const Entity& playerEntity, const Direction& direction, sreal distance)
            : _playerEntity(playerEntity), _direction(direction), _distance(distance) {}

        void Execute(Game& game) {
            auto* commandSystem = game.Systems().GetSystem<CommandSystem>();
            if (!commandSystem) return;

            if (_direction == Direction::West) {
                commandSystem->AddCommand(MoveCommand::Left(_playerEntity, _distance));
                commandSystem->AddCommand<SetImageCommand>({_playerEntity, Images::Player::Look::Left});
            } else if (_direction == Direction::East) {
                commandSystem->AddCommand(MoveCommand::Right(_playerEntity, _distance));
                commandSystem->AddCommand<SetImageCommand>({_playerEntity, Images::Player::Look::Right});
            }
        }
    };
}
