#pragma once

#include "IECS.h"

namespace Simp1e {

    struct IECSManager {
        virtual ~IECSManager() = default;

        virtual IECS* CreateECS(const char* environmentName)  = 0;
        virtual IECS* GetECS(const char* environmentName)     = 0;
        virtual bool  HasECS(const char* environmentName)     = 0;
        virtual void  DestroyECS(const char* environmentName) = 0;
    };
}
