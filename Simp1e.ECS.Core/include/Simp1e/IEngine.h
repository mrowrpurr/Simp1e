#pragma once

#include "ICommandManager.h"
#include "IEntityManager.h"
#include "IEventManager.h"
#include "ISystemManager.h"

namespace Simp1e {

    struct IEngine {
        virtual ~IEngine() = default;

        virtual IEntityManager*  GetEntityManager()  = 0;
        virtual IEventManager*   GetEventManager()   = 0;
        virtual ICommandManager* GetCommandManager() = 0;
        virtual ISystemManager*  GetSystemManager()  = 0;
    };
}
