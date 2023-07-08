#pragma once

#include <function_pointer.h>

#include "ISystemGroupManager.h"
#include "SystemGroupType.h"

namespace Simp1e {

    struct IEngine;

    struct ISystemManager {
        virtual ~ISystemManager() = default;

        virtual void Update(IEngine* environment, double deltaTime) = 0;

        virtual ISystemGroupManager* GetGroup(SystemGroupType systemGroupType)    = 0;
        virtual bool                 RemoveGroup(SystemGroupType systemGroupType) = 0;
        virtual bool                 HasGroup(SystemGroupType systemGroupType)    = 0;

        virtual bool MoveGroupAfterGroup(SystemGroupType systemGroupType, SystemGroupType afterSystemGroupType)   = 0;
        virtual bool MoveGroupBeforeGroup(SystemGroupType systemGroupType, SystemGroupType beforeSystemGroupType) = 0;

        virtual bool SetGroupEnabled(SystemGroupType systemGroupType, bool enabled) = 0;
        virtual bool EnableGroup(SystemGroupType systemGroupType)                   = 0;
        virtual bool DisableGroup(SystemGroupType systemGroupType)                  = 0;
        virtual bool IsSystemGrouEnabled(SystemGroupType systemGroupType)           = 0;

        virtual void ForEachSystemGroupManager(IFunctionPointer<ISystemGroupManager*>* function) = 0;
    };
}
