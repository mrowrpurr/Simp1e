#pragma once

#include <Simp1e/SystemTypeFromType.h>
#include <function_pointer.h>
#include <void_pointer.h>

#include "IMemoryManagedEngineContainerClass.h"
#include "ISystem.h"
#include "SystemType.h"

namespace Simp1e {

    struct IEngine;

    struct ISystemGroupManager : public IMemoryManagedEngineContainerClass {
        virtual ~ISystemGroupManager() = default;

        virtual void Update(IEngine* environment, double deltaTime) = 0;

        virtual bool OwnsSystemMemoryManagement() const = 0;
        bool         ManagesEngineItemMemory() const override { return OwnsSystemMemoryManagement(); }

        virtual ISystem* AddSystemPointer(
            SystemType systemType, VoidPointer systemPointer, IFunctionPointer systemUpdateFunction
        )                                                        = 0;
        virtual ISystem* GetSystemPointer(SystemType systemType) = 0;
        virtual bool     RemoveSystem(SystemType systemType)     = 0;
        virtual bool     HasSystem(SystemType systemType)        = 0;

        virtual bool SetSystemEnabled(SystemType systemType, bool enabled) = 0;
        virtual bool EnableSystem(SystemType systemType)                   = 0;
        virtual bool DisableSystem(SystemType systemType)                  = 0;
        virtual bool IsSystemEnabled(SystemType systemType)                = 0;

        virtual void ForEachSystem(IFunctionPointer* function) = 0;

        template <typename T, typename... Args>
        T* Add(Args&&... args) {
            auto* system        = new T(std::forward<Args>(args)...);
            auto* systemPointer = AddSystemPointer(
                SystemTypeFromType<T>(), void_pointer(system),
                function_pointer([system](IEngine* engine, double deltaTime) { system->Update(engine, deltaTime); })
            );
            if (!OwnsSystemMemoryManagement()) systemPointer->GetSystemPointer()->get()->disable_destruct_on_delete();
            return static_cast<T*>(system);
        }

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
