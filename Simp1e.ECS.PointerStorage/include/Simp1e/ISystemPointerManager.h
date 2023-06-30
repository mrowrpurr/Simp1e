#pragma once

#include <Simp1e/ISystemManager.h>

namespace Simp1e {

    struct ISystemPointerManager : public ISystemManager {
        virtual bool AddSystemPointer(SystemType systemType, IExecutable* system) = 0;
    };
}
