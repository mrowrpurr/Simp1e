#pragma once

#include <Simp1e/SystemTypeFromType.h>

#include "ISystem.h"
#include "SystemType.h"

namespace Simp1e {

    struct IEngine;

    struct ISystemManager {
        virtual ~ISystemManager() = default;

        virtual void Update(IEngine* environment, double deltaTime) = 0;

        virtual ISystem* GetSystemPointer(SystemType systemType) = 0;
        virtual bool     RemoveSystem(SystemType systemType)     = 0;
        virtual bool     HasSystem(SystemType systemType)        = 0;

        virtual bool EnableSystem(SystemType systemType)    = 0;
        virtual bool DisableSystem(SystemType systemType)   = 0;
        virtual bool IsSystemEnabled(SystemType systemType) = 0;

        template <typename T>
        T* Get() {
            return static_cast<T*>(GetSystemPointer(SystemTypeFromType<T>()));
        }

        template <typename T>
        bool Remove() {
            return RemoveSystem(SystemTypeFromType<T>());
        }

        template <typename T>
        bool Has() {
            return HasSystem(SystemTypeFromType<T>());
        }

        template <typename T>
        bool Enable() {
            return EnableSystem(SystemTypeFromType<T>());
        }

        template <typename T>
        bool Disable() {
            return DisableSystem(SystemTypeFromType<T>());
        }

        template <typename T>
        bool IsEnabled() {
            return IsSystemEnabled(SystemTypeFromType<T>());
        }
    };
}
