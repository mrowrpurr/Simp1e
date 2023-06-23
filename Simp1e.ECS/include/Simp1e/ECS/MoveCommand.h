#pragma once

#include <Simp1e/Direction.h>
#include <Simp1e/sreal.h>

#include "CommandTypeMacro.h"
#include "Entity.h"
#include "Game.h"
#include "GravityComponent.h"
#include "PositionComponent.h"

namespace Simp1e::ECS {

    class MoveCommand {
        Entity    _entity;
        Direction _direction;
        sreal     _distance;
        bool      _respectCollision;

    public:
        SIMP1E_ECS_COMMAND("Move");

        MoveCommand(Entity entity, Direction direction, sreal distance)
            : _entity(entity), _direction(direction), _distance(distance) {}

        Entity    Entity() const { return _entity; }
        Direction Direction() const { return _direction; }
        sreal     Distance() const { return _distance; }
        bool      RespectCollision() const { return _respectCollision; }

        void Execute(Game& game) {
            if (auto* position = game.Entities().GetComponent<PositionComponent>(_entity)) {
                if (_direction == Direction::North) position->SetY(position->y() - _distance);
                else if (_direction == Direction::South) position->SetY(position->y() + _distance);
                else if (_direction == Direction::West) position->SetX(position->x() - _distance);
                else if (_direction == Direction::East) position->SetX(position->x() + _distance);
            }
        }

        static MoveCommand North(::Simp1e::ECS::Entity entity, sreal distance) {
            return MoveCommand(entity, Direction::North, distance);
        }
        static MoveCommand Up(::Simp1e::ECS::Entity entity, sreal distance) {
            return MoveCommand(entity, Direction::North, distance);
        }

        static MoveCommand South(::Simp1e::ECS::Entity entity, sreal distance) {
            return MoveCommand(entity, Direction::South, distance);
        }
        static MoveCommand Down(::Simp1e::ECS::Entity entity, sreal distance) {
            return MoveCommand(entity, Direction::South, distance);
        }

        static MoveCommand West(::Simp1e::ECS::Entity entity, sreal distance) {
            return MoveCommand(entity, Direction::West, distance);
        }
        static MoveCommand Left(::Simp1e::ECS::Entity entity, sreal distance) {
            return MoveCommand(entity, Direction::West, distance);
        }

        static MoveCommand East(::Simp1e::ECS::Entity entity, sreal distance) {
            return MoveCommand(entity, Direction::East, distance);
        }
        static MoveCommand Right(::Simp1e::ECS::Entity entity, sreal distance) {
            return MoveCommand(entity, Direction::East, distance);
        }
    };
}