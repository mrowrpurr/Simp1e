#pragma once

#include <Simp1e/SystemTypeFromType.h>
#include <function_pointer.h>
#include <void_pointer.h>

#include "SystemCast.h"
#include "SystemType.h"

namespace Simp1e {

    struct IEngine;

    struct ISystemGroup {
        virtual ~ISystemGroup() = default;

        virtual void Update(IEngine* environment, double deltaTime) = 0;

        virtual SystemPointer AddSystemPointer(
            SystemType systemType, IVoidPointer* system, IFunctionPointer<void(IEngine*, double)>* systemUpdateFunction
        )                                                             = 0;
        virtual SystemPointer GetSystemPointer(SystemType systemType) = 0;
        virtual bool          RemoveSystem(SystemType systemType)     = 0;
        virtual bool          HasSystem(SystemType systemType)        = 0;

        template <typename T>
        SystemPointer AddSystemPointer(T* system) {
            return AddSystemPointer(
                SystemTypeFromType<T>(), new VoidPointer(system),
                new_function_pointer([system](IEngine* engine, double timeDelta) { system->Update(engine, timeDelta); })
            );
        }

        template <typename T, typename... Args>
        T* AddSystem(Args&&... args) {
            auto* system               = new T(std::forward<Args>(args)...);
            auto* systemPointer        = new VoidPointer(system);
            auto* systemUpdateFunction = new_function_pointer([system](IEngine* engine, double timeDelta) {
                system->Update(engine, timeDelta);
            });
            AddSystemPointer(SystemTypeFromType<T>(), systemPointer, systemUpdateFunction);
            return system;
        }

        virtual bool SetSystemEnabled(SystemType systemType, bool enabled) = 0;
        virtual bool EnableSystem(SystemType systemType)                   = 0;
        virtual bool DisableSystem(SystemType systemType)                  = 0;
        virtual bool IsSystemEnabled(SystemType systemType)                = 0;

        virtual void ForEachSystem(
            IFunctionPointer<void(SystemType, SystemPointer, IFunctionPointer<void(IEngine*, double)>*)>* callback
        ) = 0;
        virtual void ForEachSystem(
            FunctionPointer<void(SystemType, SystemPointer, IFunctionPointer<void(IEngine*, double)>*)>* callback
        ) {
            ForEachSystem(callback);
        }

        template <typename T>
        T* Get() {
            return system_cast<T>(GetSystemPointer(SystemTypeFromType<T>()));
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
