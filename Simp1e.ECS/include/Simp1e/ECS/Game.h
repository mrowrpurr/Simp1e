#pragma once

#include <functional>
#include <unordered_map>

#include "EntityManager.h"
#include "EventManager.h"
#include "SystemManager.h"

namespace Simp1e::ECS {

    class Game {
        EntityManager _entityManager;
        EventManager  _eventManager;
        SystemManager _systemManager;

    public:
        EntityManager& Entities() { return _entityManager; }
        EventManager&  Events() { return _eventManager; }
        SystemManager& Systems() { return _systemManager; }

        void Update() { _systemManager.Update(); }

        void Clear() {
            _entityManager.Clear();
            _eventManager.Clear();
            _systemManager.Clear();
        }
    };
}
