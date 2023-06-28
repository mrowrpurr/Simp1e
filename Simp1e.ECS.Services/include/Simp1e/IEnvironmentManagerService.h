#pragma once

#include <Simp1e/IEnvironmentManager.h>
#include <Simp1e/ServiceDefinition.h>

namespace Simp1e {

    struct IEnvironmentManagerService {
        SIMP1E_SERVICE("EnvironmentManager")

        virtual ~IEnvironmentManagerService() = default;

        virtual IEnvironmentManager* GetEnvironmentManager() = 0;
    };
}
