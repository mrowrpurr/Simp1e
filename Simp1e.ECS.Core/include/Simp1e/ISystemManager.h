#pragma once

#include <Simp1e/IExecutable.h>

#include "SystemType.h"

namespace Simp1e {

    struct IEnvironment;

    struct ISystemManager {
        virtual ~ISystemManager() = default;

        virtual void Update(IEnvironment* environment, double deltaTime) = 0;

        virtual IExecutable* GetSystemPointer(SystemType systemType) = 0;
        virtual bool         RemoveSystem(SystemType systemType)     = 0;
        virtual bool         HasSystem(SystemType systemType)        = 0;

        virtual bool EnableSystem(SystemType systemType)    = 0;
        virtual bool DisableSystem(SystemType systemType)   = 0;
        virtual bool IsSystemEnabled(SystemType systemType) = 0;

        template <typename T>
        T* Get() {
            return static_cast<T*>(GetSystemPointer(T::GetSystemType()));
        }

        template <typename T>
        bool Remove() {
            return RemoveSystem(T::GetSystemType());
        }

        template <typename T>
        bool Has() {
            return HasSystem(T::GetSystemType());
        }

        template <typename T>
        bool Enable() {
            return EnableSystem(T::GetSystemType());
        }

        template <typename T>
        bool Disable() {
            return DisableSystem(T::GetSystemType());
        }

        template <typename T>
        bool IsEnabled() {
            return IsSystemEnabled(T::GetSystemType());
        }
    };
}
