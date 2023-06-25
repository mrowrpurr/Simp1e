#pragma once

#include <string>
#include <vector>

#include "CommandTypeMacro.h"
#include "Entity.h"
#include "Game.h"
#include "GravityComponent.h"
#include "PositionComponent.h"
#include "SizeComponent.h"

namespace Simp1e::ECS {

    class ResolveCollisionCommand {
        Entity              _entity;
        std::vector<Entity> _collidedEntities;

    public:
        SIMP1E_ECS_COMMAND("ResolveCollision")

        ResolveCollisionCommand() = default;
        ResolveCollisionCommand(const Entity& entity, std::vector<Entity> collidedEntities)
            : _entity(entity), _collidedEntities(collidedEntities) {}

        void Execute(Game& game) {
            auto* positionComponent = game.Entities().GetComponent<PositionComponent>(_entity);
            auto* sizeComponent     = game.Entities().GetComponent<SizeComponent>(_entity);
            if (!positionComponent || !sizeComponent) return;

            bool bottomCollision = false;
            bool topCollision    = false;
            bool leftCollision   = false;
            bool rightCollision  = false;

            for (auto& collidedEntity : _collidedEntities) {
                auto* collidedPositionComponent = game.Entities().GetComponent<PositionComponent>(collidedEntity);
                auto* collidedSizeComponent     = game.Entities().GetComponent<SizeComponent>(collidedEntity);
                if (!collidedPositionComponent || !collidedSizeComponent) continue;

                auto position = positionComponent->position();
                auto size     = sizeComponent->size();

                auto collidedPosition = collidedPositionComponent->position();
                auto collidedSize     = collidedSizeComponent->size();

                auto collidedBottom = collidedPosition.y() + collidedSize.height();
                auto collidedTop    = collidedPosition.y();
                auto collidedLeft   = collidedPosition.x();
                auto collidedRight  = collidedPosition.x() + collidedSize.width();

                auto bottom = position.y() + size.height();
                auto top    = position.y();
                auto left   = position.x();
                auto right  = position.x() + size.width();

                if (bottom > collidedTop && bottom < collidedBottom) bottomCollision = true;
                if (top > collidedTop && top < collidedBottom) topCollision = true;
                if (left > collidedLeft && left < collidedRight) leftCollision = true;
                if (right > collidedLeft && right < collidedRight) rightCollision = true;
            }

            auto* gravityComponent = game.Entities().GetComponent<GravityComponent>(_entity);
            gravityComponent->SetIsGrounded(false);

            if (bottomCollision) {
                auto highestGround = -std::numeric_limits<sreal>::infinity();
                for (auto& collidedEntity : _collidedEntities) {
                    auto* collidedPositionComponent = game.Entities().GetComponent<PositionComponent>(collidedEntity);
                    auto* collidedSizeComponent     = game.Entities().GetComponent<SizeComponent>(collidedEntity);
                    if (!collidedPositionComponent || !collidedSizeComponent) continue;

                    auto collidedPosition = collidedPositionComponent->position();
                    auto collidedTop      = collidedPosition.y();
                    if (collidedTop > highestGround) highestGround = collidedTop;
                }
                positionComponent->SetY(highestGround - sizeComponent->size().height() + 1);
                if (gravityComponent) {
                    gravityComponent->SetIsGrounded();
                }
            } else if (topCollision || leftCollision || rightCollision) {
                // Just set it back for now...
                positionComponent->RestoreLastPosition();
            }
        }
    };
}
