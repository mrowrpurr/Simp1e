#pragma once

#include <Simp1e/IExecutable.h>

#include "SystemType.h"

namespace Simp1e {

    struct ISystemManager {
        virtual ~ISystemManager() = default;

        virtual void Update() = 0;

        virtual IExecutable* GetSystemPointer(SystemType systemType) = 0;
        virtual bool         RemoveSystem(SystemType systemType)     = 0;
        virtual bool         HasSystem(SystemType systemType)        = 0;

        virtual bool EnableSystem(SystemType systemType)    = 0;
        virtual bool DisableSystem(SystemType systemType)   = 0;
        virtual bool IsSystemEnabled(SystemType systemType) = 0;
    };
}
