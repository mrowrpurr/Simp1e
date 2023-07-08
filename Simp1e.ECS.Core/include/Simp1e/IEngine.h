#pragma once

#include "ICommandManager.h"
#include "IEntityManager.h"
#include "IEventManager.h"
#include "ISystemGroupManager.h"

namespace Simp1e {

    struct IEngine {
        virtual ~IEngine() = default;

        virtual void RunMainLoopCycle() = 0;

        virtual IEntityManager*      Entities()     = 0;
        virtual IEventManager*       Events()       = 0;
        virtual ICommandManager*     Commands()     = 0;
        virtual ISystemGroupManager* SystemGroups() = 0;
    };
}
