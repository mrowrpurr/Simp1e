#pragma once

#include "IEntityManager.h"
#include "IEnvironment.h"
#include "IEventManager.h"
#include "ISystemManager.h"

namespace Simp1e {

    template <typename EntityManagerT, typename SystemManagerT, typename EventManagerT, typename CommandManagerT>
    class Environment : public IEnvironment {
        EntityManagerT  _entityManager;
        SystemManagerT  _systemManager;
        EventManagerT   _eventManager;
        CommandManagerT _commandManager;

    public:
        IEntityManager*  GetEntityManager() override { return &_entityManager; }
        ISystemManager*  GetSystemManager() override { return &_systemManager; }
        IEventManager*   GetEventManager() override { return &_eventManager; }
        ICommandManager* GetCommandManager() override { return &_commandManager; }
    };
}
