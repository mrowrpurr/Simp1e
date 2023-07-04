#pragma once

#include "IEngine.h"
#include "IEntityManager.h"
#include "IEventManager.h"
#include "ISystemManager.h"

// TODO - Add a JobManager

namespace Simp1e {

    template <typename EntityManagerT, typename SystemManagerT, typename EventManagerT, typename CommandManagerT>
    class Engine : public IEngine {
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
