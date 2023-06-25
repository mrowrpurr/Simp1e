#pragma once

#include <Simp1e/ECS/CollisionComponent.h>
#include <Simp1e/ECS/CommandSystem.h>
#include <Simp1e/ECS/ComponentCast.h>
#include <Simp1e/ECS/EntityManager.h>
#include <Simp1e/ECS/EventManager.h>
#include <Simp1e/ECS/GravityComponent.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/ResolveCollisionCommand.h>
#include <Simp1e/ECS/Simp1eQGraphicsItemComponent.h>
#include <Simp1e/ECS/SizeComponent.h>
#include <Simp1e/ECS/SystemTypeMacro.h>

#include <QGraphicsScene>
#include <vector>

#include "EntitySimp1eQGraphicsItem.h"
#include "Simp1eQGraphicsItemComponent.h"


namespace Simp1e::ECS {

    class QtCollisionSystem {
        EntityManager&  _entityManager;
        EventManager&   _eventManager;
        CommandSystem&  _commandSystem;
        QGraphicsScene& _scene;

    public:
        SIMP1E_ECS_SYSTEM("QtCollision")

        QtCollisionSystem(
            EntityManager& entityManager, EventManager& eventManager, CommandSystem& commandSystem,
            QGraphicsScene& scene
        )
            : _entityManager(entityManager),
              _eventManager(eventManager),
              _commandSystem(commandSystem),
              _scene(scene) {}

        void Update() {
            for (auto& [entity, component] : _entityManager.GetComponents<CollisionComponent>()) {
                auto collisionComponent = component_cast<CollisionComponent>(component);
                if (!collisionComponent) continue;
                if (!collisionComponent->RaisesEvents()) continue;

                auto* graphicsItemComponent = _entityManager.GetComponent<Simp1eQGraphicsItemComponent>(entity);
                if (!graphicsItemComponent) continue;

                auto collidingItems =
                    _scene.collidingItems(graphicsItemComponent->GetGraphicsItem(), Qt::IntersectsItemShape);

                std::vector<Entity> collidingEntities;
                for (auto* sceneItem : collidingItems) {
                    if (auto* otherGraphicsItem = dynamic_cast<EntitySimp1eQGraphicsItem*>(sceneItem)) {
                        auto  otherEntity             = otherGraphicsItem->GetEntity();
                        auto* otherCollisionComponent = _entityManager.GetComponent<CollisionComponent>(otherEntity);
                        if (!otherCollisionComponent) continue;
                        collidingEntities.push_back(otherEntity);
                    }
                }
                if (collidingEntities.empty()) continue;
                _commandSystem.AddCommand<ResolveCollisionCommand>(entity, std::move(collidingEntities));
            }
        }
    };
}

// // Raise the collision event
// _eventManager.RaiseEvent<CollisionEvent>(entity, otherEntity);

//     // For now, just set its position back to what it was before this frame
//     auto* positionComponent = _entityManager.GetComponent<PositionComponent>(entity);
//     if (!positionComponent) continue;
//     positionComponent->RestoreLastPosition();
// }