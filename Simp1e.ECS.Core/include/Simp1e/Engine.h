#pragma once

#include <_Log_.h>

#include <chrono>

#include "IEngine.h"
#include "IEntityManager.h"
#include "IEventManager.h"
#include "ISystemGroupManager.h"

// TODO - Add a JobManager

namespace Simp1e {

    template <typename EntityManagerT, typename SystemGroupManagerT, typename EventManagerT, typename CommandManagerT>
    class Engine : public IEngine {
        EntityManagerT      _entityManager;
        SystemGroupManagerT _systemGroupManager;
        EventManagerT       _eventManager;
        CommandManagerT     _commandManager;

        std::chrono::high_resolution_clock::time_point _mainLoopLastTime = std::chrono::high_resolution_clock::now();

    public:
        void RunMainLoopCycle() override {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto deltaTime =
                std::chrono::duration_cast<std::chrono::microseconds>(currentTime - _mainLoopLastTime).count();
            _mainLoopLastTime = currentTime;
            auto seconds      = deltaTime / 1000000.0;
            _systemGroupManager.Update(this, seconds);
        }

        IEntityManager*      GetEntities() override { return &_entityManager; }
        ISystemGroupManager* GetSystemGroups() override { return &_systemGroupManager; }
        IEventManager*       GetEvents() override { return &_eventManager; }
        ICommandManager*     GetCommands() override { return &_commandManager; }

        EntityManagerT&      Entities() { return _entityManager; }
        SystemGroupManagerT& SystemGroups() { return _systemGroupManager; }
        EventManagerT&       Events() { return _eventManager; }
        CommandManagerT&     Commands() { return _commandManager; }
    };
}
