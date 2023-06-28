#pragma once

#include <Simp1e/ECSManager.h>

#include "IECSManagerService.h"

namespace Simp1e {

    template <typename EntityManagerT>
    class ECSManagerService : IECSManagerService {
        ECSManager<EntityManagerT> _ECSManager;

    public:
        static const char* GetServiceName() { return "ECSManagerService"; }

        IECSManager* GetECSManager() override { return &_ECSManager; }

        // testing...
        const char* ReturnAnything() override { return "Anything!"; }
    };
}
