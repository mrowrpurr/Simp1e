#pragma once

#include <Simp1e/EngineManager.h>

#include "IEngineManagerService.h"

namespace Simp1e {

    class EngineManagerService : public IEngineManagerService {
        EngineManager _environmentManager;

    public:
        IEngineManager* GetEngineManager() override { return &_environmentManager; }
    };
}
