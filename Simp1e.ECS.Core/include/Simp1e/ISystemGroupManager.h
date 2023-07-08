#pragma once

#include <function_pointer.h>

#include "ISystemGroup.h"
#include "SystemGroupType.h"

namespace Simp1e {

    struct IEngine;

    struct ISystemGroupManager {
        virtual ~ISystemGroupManager() = default;

        virtual void Update(IEngine* environment, double deltaTime) = 0;

        virtual ISystemGroup* GetGroup(SystemGroupType systemGroupType)    = 0;
        virtual bool          RemoveGroup(SystemGroupType systemGroupType) = 0;
        virtual bool          HasGroup(SystemGroupType systemGroupType)    = 0;

        virtual bool MoveGroupAfterGroup(SystemGroupType systemGroupType, SystemGroupType afterSystemGroupType)   = 0;
        virtual bool MoveGroupBeforeGroup(SystemGroupType systemGroupType, SystemGroupType beforeSystemGroupType) = 0;

        virtual bool SetGroupEnabled(SystemGroupType systemGroupType, bool enabled) = 0;
        virtual bool EnableGroup(SystemGroupType systemGroupType)                   = 0;
        virtual bool DisableGroup(SystemGroupType systemGroupType)                  = 0;
        virtual bool IsSystemGrouEnabled(SystemGroupType systemGroupType)           = 0;

        virtual void ForEachGroup(IFunctionPointer<void(ISystemGroup*)>* function) = 0;
        virtual void ForEachGroup(FunctionPointer<void(ISystemGroup*)>* function) { ForEachGroup(function); }
    };
}
