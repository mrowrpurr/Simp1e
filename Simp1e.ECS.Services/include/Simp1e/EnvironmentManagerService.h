#pragma once

#include <Simp1e/EnvironmentManager.h>

#include "IEnvironmentManagerService.h"

namespace Simp1e {

    class EnvironmentManagerService : public IEnvironmentManagerService {
        EnvironmentManager _environmentManager;

    public:
        IEnvironmentManager* GetEnvironmentManager() override { return &_environmentManager; }
    };
}
