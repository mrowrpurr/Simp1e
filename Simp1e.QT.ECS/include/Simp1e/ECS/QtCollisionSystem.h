#pragma once

#include <Simp1e/ECS/CollisionComponent.h>
#include <Simp1e/ECS/ComponentCast.h>
#include <Simp1e/ECS/EntityManager.h>
#include <Simp1e/ECS/EventManager.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/QTGraphicsItemComponent.h>
#include <Simp1e/ECS/SizeComponent.h>
#include <Simp1e/ECS/SystemTypeMacro.h>

#include <QGraphicsScene>

#include "EntityQTGraphicsItem.h"
#include "QTGraphicsItemComponent.h"

namespace Simp1e::ECS {

    class QtCollisionSystem {
        EntityManager&  _entityManager;
        EventManager&   _eventManager;
        QGraphicsScene& _scene;

    public:
        SIMP1E_ECS_SYSTEM("QtCollision")

        QtCollisionSystem(EntityManager& entityManager, EventManager& eventManager, QGraphicsScene& scene)
            : _entityManager(entityManager), _eventManager(eventManager), _scene(scene) {}

        void Update() {
            for (auto& [entity, component] : _entityManager.GetComponents<CollisionComponent>()) {
                auto collisionComponent = component_cast<CollisionComponent>(component);
                if (!collisionComponent) continue;
                if (!collisionComponent->RaisesEvents()) continue;

                // Get its Graphics Item on the scene
                auto* graphicsItemComponent = _entityManager.GetComponent<QTGraphicsItemComponent>(entity);
                if (!graphicsItemComponent) continue;

                // Get all colliding items (using QGraphicsScene logic from the boundingBox())
                auto collidingItems =
                    _scene.collidingItems(graphicsItemComponent->GetGraphicsItem(), Qt::IntersectsItemShape);

                for (auto* sceneItem : collidingItems) {
                    if (auto* otherGraphicsItem = dynamic_cast<EntityQTGraphicsItem*>(sceneItem)) {
                        // Get the other entity
                        auto otherEntity = otherGraphicsItem->GetEntity();

                        // Get its CollisionComponent
                        auto* otherCollisionComponent = _entityManager.GetComponent<CollisionComponent>(otherEntity);
                        if (!otherCollisionComponent) continue;

                        // // Raise the collision event
                        // _eventManager.RaiseEvent<CollisionEvent>(entity, otherEntity);

                        // For now, just set its position back to what it was before this frame
                        auto* positionComponent = _entityManager.GetComponent<PositionComponent>(entity);
                        if (!positionComponent) continue;
                        positionComponent->RestoreLastPosition();
                    }
                }
            }
        }
    };
}
