#pragma once

#include "INativePlugin.h"

namespace Simp1e {

    struct INativePluginManager {
        virtual ~INativePluginManager() = default;

        virtual void RegisterPlugin(INativePlugin* plugin) = 0;
    };
}
