#pragma once

#include <Simp1e/IEngineManager.h>

namespace Simp1e {

    struct IEngineManagerService {
        static const char* GetServiceName() { return "EngineManagerService"; }

        virtual ~IEngineManagerService() = default;

        virtual IEngineManager* GetEngineManager() = 0;
    };
}
