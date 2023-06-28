#pragma once

#include <Simp1e/IEnvironmentManager.h>

namespace Simp1e {

    struct IEnvironmentManagerService {
        static const char* GetServiceName() { return "EnvironmentManager"; }

        virtual ~IEnvironmentManagerService() = default;

        virtual IEnvironmentManager* GetEnvironmentManager() = 0;
    };
}
