#pragma once

#include <Simp1e/Direction.h>
#include <Simp1e/ECS/CommandTypeMacro.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/sreal.h>


namespace Simp1e {

    class MovePositionCommand {
        PositionComponent* position;
        Direction          direction;
        sreal              distance;

    public:
        SIMP1E_ECS_COMMAND("MovePosition")

        MovePositionCommand(PositionComponent* position, Direction direction, sreal distance)
            : position(position), direction(direction), distance(distance) {}

        void Execute() {
            if (direction == Direction::North) position->SetY(position->y() - distance);
            else if (direction == Direction::South) position->SetY(position->y() + distance);
            else if (direction == Direction::West) position->SetX(position->x() - distance);
            else if (direction == Direction::East) position->SetX(position->x() + distance);
        }
    };
}