#pragma once

#include <Simp1e/IEnvironmentManager.h>

namespace Simp1e {

    struct IEnvironmentManagerService {
        static const char* GetServiceName() { return "EnvironmentManagerService"; }

        virtual ~IEnvironmentManagerService() = default;

        virtual IEnvironmentManager* GetEnvironmentManager() = 0;
    };
}
