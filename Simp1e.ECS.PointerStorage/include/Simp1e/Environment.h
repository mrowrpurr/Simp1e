#pragma once

// #include "CommandManager.h"
#include "EntityManager.h"
// #include "EventManager.h"
#include "IEnvironment.h"
// #include "SystemManager.h"

namespace Simp1e {

    class Environment : public IEnvironment {
        EntityManager _entityManager;
        // EventManager   _eventManager;
        // CommandManager _commandManager;
        // SystemManager  _systemManager;

    public:
        IEntityManager* GetEntityManager() override { return &_entityManager; }
        // IEventManager*   GetEventManager() override { return &_eventManager; }
        // ICommandManager* GetCommandManager() override { return &_commandManager; }
        // ISystemManager*  GetSystemManager() override { return &_systemManager; }
    };
}
