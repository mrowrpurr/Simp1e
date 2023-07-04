#pragma once

#include "IEngine.h"

namespace Simp1e {

    struct IEngineManager {
        virtual ~IEngineManager() = default;

        virtual bool     RegisterEngine(const char* name, IEngine* environment) = 0;
        virtual IEngine* GetEngine(const char* name)                            = 0;
        virtual bool     UnregisterEngine(const char* name)                     = 0;
        virtual bool     IsEngineRegistered(const char* name)                   = 0;
    };
}
