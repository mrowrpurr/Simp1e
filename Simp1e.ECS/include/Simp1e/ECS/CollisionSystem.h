#pragma once

#include "CollisionComponent.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "SystemTypeMacro.h"

namespace Simp1e::ECS {

    class CollisionSystem {
        EntityManager& _entityManager;
        EventManager&  _eventManager;

    public:
        SIMP1E_ECS_SYSTEM("Collision")

        CollisionSystem(EntityManager& entityManager, EventManager& eventManager)
            : _entityManager(entityManager), _eventManager(eventManager) {}

        void Update() {
            for (auto& [entity, component] : _entityManager.GetComponents<CollisionComponent>()) {
                // TODO
            }
        }
    };
}
