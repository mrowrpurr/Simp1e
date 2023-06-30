#pragma once

#include "IEnvironment.h"

namespace Simp1e {

    struct IEnvironmentManager {
        virtual ~IEnvironmentManager() = default;

        virtual bool          RegisterEnvironment(const char* name, IEnvironment* environment) = 0;
        virtual IEnvironment* GetEnvironment(const char* name)                                 = 0;
        virtual bool          UnregisterEnvironment(const char* name)                          = 0;
        virtual bool          IsEnvironmentRegistered(const char* name)                        = 0;
    };
}
