#pragma once

#include <chrono>

#include "DefaultSystemGroupTypes.h"
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

        std::chrono::high_resolution_clock::time_point _mainLoopLastTime = std::chrono::high_resolution_clock::now();

    public:
        void RunMainLoopCycle() override {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto deltaTime =
                std::chrono::duration_cast<std::chrono::microseconds>(currentTime - _mainLoopLastTime).count();
            _mainLoopLastTime = currentTime;
            auto seconds      = deltaTime / 1000000.0;
            _systemManager.Update(this, seconds);
        }

        IEntityManager*  GetEntityManager() override { return &_entityManager; }
        ISystemManager*  GetSystemManager() override { return &_systemManager; }
        IEventManager*   GetEventManager() override { return &_eventManager; }
        ICommandManager* GetCommandManager() override { return &_commandManager; }

        EntityManagerT&  GetEntityManagerInstance() { return _entityManager; }
        SystemManagerT&  GetSystemManagerInstance() { return _systemManager; }
        EventManagerT&   GetEventManagerInstance() { return _eventManager; }
        CommandManagerT& GetCommandManagerInstance() { return _commandManager; }
    };
}