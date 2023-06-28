#pragma once

#include "IEnvironment.h"

namespace Simp1e {

    struct IEnvironmentManager {
        virtual ~IEnvironmentManager() = default;

        virtual IEnvironment* CreateEnvironment(const char* name)  = 0;
        virtual IEnvironment* GetEnvironment(const char* name)     = 0;
        virtual void          DestroyEnvironment(const char* name) = 0;
        virtual bool          EnvironmentExists(const char* name)  = 0;
    };
}
