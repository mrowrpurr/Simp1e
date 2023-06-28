#pragma once

#include <Simp1e/IECSManager.h>

namespace Simp1e {

    struct IECSManagerService {
        static const char* GetServiceName() { return "ECSManagerService"; }

        virtual ~IECSManagerService() = default;

        virtual IECSManager* GetECSManager() = 0;

        // for testing...
        virtual const char* ReturnAnything() = 0;
    };
}
