#pragma once

#include "IPlugin.h"
#include "PluginType.h"

namespace Simp1e {

    struct IPluginManager {
        virtual ~IPluginManager() = default;

        virtual void RegisterPlugin(PluginType pluginType, IPlugin* plugin) = 0;
    };
}
