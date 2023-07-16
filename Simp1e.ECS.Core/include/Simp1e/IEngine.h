#pragma once

#include "ICommandManager.h"
#include "IEntityManager.h"
#include "IEventManager.h"
#include "IInputManager.h"
#include "ISystemGroupManager.h"

namespace Simp1e {

    struct IEngine {
        virtual ~IEngine() = default;

        virtual void RunMainLoopCycle() = 0;

        virtual IEntityManager*      GetEntities()     = 0;
        virtual IEventManager*       GetEvents()       = 0;
        virtual ICommandManager*     GetCommands()     = 0;
        virtual ISystemGroupManager* GetSystemGroups() = 0;
        virtual IInputManager*       GetInput()        = 0;
    };
}
